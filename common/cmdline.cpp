/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* Utility for command line parsing                                     */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/win32-common                   */
/*                                                                      */
/************************************************************************/

#include <deque>
#include "cmdline.h"
#include "utils.h"

CommandLine::CommandLine(const wchar_t *lpstrCmdLine): m_cmdline(NULL), m_szArglist(NULL),
    m_nArgs(0), m_processFolder(NULL), m_paramValues(NULL)
{
    if (NULL != lpstrCmdLine)
    {
        int length = wcslen(lpstrCmdLine);
        m_cmdline = new wchar_t[length + 1];
        wmemcpy(m_cmdline, lpstrCmdLine, length);
        m_cmdline[length] = L'\0';
        m_szArglist = CommandLineToArgvW(m_cmdline, &m_nArgs);
    }
}

CommandLine::~CommandLine()
{
    if (NULL != m_paramValues)
    {
        std::unordered_map<std::wstring, std::pair<wchar_t**, int>>::iterator iter = m_paramValues->begin();
        for (; iter != m_paramValues->end(); ++iter)
        {
            std::pair<wchar_t**, int> p = iter->second;
            if (NULL != p.first)
                delete [](p.first);
        }
        m_paramValues->clear();
        delete m_paramValues;
        m_paramValues = NULL;
    }
    if (NULL != m_szArglist)
    {
        LocalFree(m_szArglist);
        m_szArglist = NULL;
    }
    if (NULL != m_cmdline)
    {
        delete []m_cmdline;
        m_cmdline = NULL;
    }
    if (NULL != m_processFolder)
    {
        delete []m_processFolder;
        m_processFolder = NULL;
    }
}

CommandLine CommandLine::getCurrentProcessCmdLine()
{
    return CommandLine(GetCommandLineW());
}

CommandLine CommandLine::parseFromString(const wchar_t *lpstrCmdLine)
{
    return CommandLine(lpstrCmdLine);
}

const wchar_t* CommandLine::getFullString()
{
    return m_cmdline;
}

const wchar_t* CommandLine::getProcessPath()
{
    if (NULL != m_szArglist)
        return m_szArglist[0];

    return NULL;
}

const wchar_t* CommandLine::getProcessFolder()
{
    if (NULL != m_processFolder)
        return m_processFolder;

    if (NULL == m_szArglist)
        return NULL;

    const wchar_t* p = wcsrchr(m_szArglist[0], L'\\');
    if (NULL == p)
        return NULL;

    int len = p - m_szArglist[0] + 1;
    m_processFolder = new wchar_t[len+1];
    wmemcpy(m_processFolder, m_szArglist[0], len);
    m_processFolder[len] = L'\0';
    return m_processFolder;
}

const wchar_t* CommandLine::getProcessName()
{
    if (NULL == m_szArglist)
        return NULL;
    const wchar_t* p = wcsrchr(m_szArglist[0], L'\\');
    if (NULL == p)
        return NULL;
    return p + 1;
}

const wchar_t* CommandLine::getParamValue(const wchar_t* paramName)
{
    if (NULL == m_szArglist)
        return NULL;

    if (NULL == m_paramValues)
    {
        parseCommandLine();
    }
    std::unordered_map<std::wstring, std::pair<wchar_t**, int>>::iterator iter = m_paramValues->find(std::wstring(paramName));
    if (iter != m_paramValues->end())
    {
         std::pair<wchar_t**, int> &p = iter->second;
         if (p.second > 0)
             return p.first[0];
    }

    return NULL;
}

const LPCWSTR* CommandLine::getParamValues(const wchar_t* paramName, int &paramValueCount)
{
    paramValueCount = 0;
    if (NULL == m_szArglist)
        return NULL;

    if (NULL == m_paramValues)
    {
        parseCommandLine();
    }
    std::unordered_map<std::wstring, std::pair<wchar_t**, int>>::iterator iter = m_paramValues->find(std::wstring(paramName));
    if (iter != m_paramValues->end())
    {
         std::pair<wchar_t**, int> &p = iter->second;
         if ((paramValueCount = p.second) > 0)
             return p.first;
    }

    return NULL;
}

void CommandLine::parseCommandLine()
{
    if (NULL != m_paramValues) return;

    m_paramValues = new std::unordered_map<std::wstring, std::pair<wchar_t**, int>>();
    std::unordered_map<std::wstring, std::deque<int>> positions;
    // ingore first token (it is process path)
    for (int i = 1; i < m_nArgs; ++i)
    {
        wchar_t *token = m_szArglist[i];
        if (beginWith(L"-", token))
        {
            std::wstring s(token);
            if (m_paramValues->find(s) == m_paramValues->end())
            {
                (*m_paramValues)[s] = std::pair<wchar_t**, int>();
                (*m_paramValues)[s].first = NULL;
                (*m_paramValues)[s].second = 0;
                positions[s] = std::deque<int>();
            }
            // read next token
            if ((i + 1 < m_nArgs) && !beginWith(L"-", m_szArglist[i + 1]))
            {
                (*m_paramValues)[s].second++;
                positions[s].push_back(i + 1);
                ++i;
            }
        }
    }
    std::unordered_map<std::wstring, std::pair<wchar_t**, int>>::iterator iter = m_paramValues->begin();
    for (;iter != m_paramValues->end(); ++iter)
    {
        std::pair<wchar_t**, int> &p = iter->second;
        if (p.second > 0)
        {
            std::wstring s(iter->first);
            p.first = new wchar_t*[p.second];
            for (int j = 0; j < p.second; ++j)
            {
                p.first[j] = m_szArglist[positions[s][j]];
            }
        }
    }
}

bool CommandLine::hasToken(const wchar_t* token)
{
    // ingore first token (it is process path)
    for (int i = 1; i < m_nArgs; ++i)
    {
        if (wcscmp(token, m_szArglist[i]) == 0)
            return true;
    }
    return false;
}

int CommandLine::getTokenCount()
{
    return m_nArgs > 0 ? m_nArgs - 1 : 0;
}

const wchar_t* CommandLine::getTokenAt(int index)
{
    if (m_nArgs - 1 > 0 && index >= 0 && index < m_nArgs - 1)
    {
        return m_szArglist[index + 1];
    }
    return NULL;
}

const wchar_t* CommandLine::getTokenBeginWith(const wchar_t* prefix)
{
    // ingore first token (it is process path)
    for (int i = 1; i < m_nArgs; ++i)
    {
        // only return the first matched one
        if (beginWith(prefix, m_szArglist[i]) == 0)
            return m_szArglist[i];
    }
    return NULL;
}
