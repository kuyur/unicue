/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/win32-common                   */
/*                                                                      */
/************************************************************************/

#include "Logger.h"

std::unordered_map<std::wstring, CLogger*>* CLogger::s_instances = NULL;
CLogger::CGarbo CLogger::garbo;

CLogger::CLogger(const wchar_t* logFilePath)
{
}

CLogger::~CLogger(void)
{
}

CLogger* CLogger::getInstance(const wchar_t* logFilePath)
{
    // check file path
    if (NULL == logFilePath)
        return NULL;
    // is a valid file path?
    // TODO

    if (NULL == s_instances)
        s_instances = new std::unordered_map<std::wstring, CLogger*>();
    std::unordered_map<std::wstring, CLogger*>::iterator iter = s_instances->find(logFilePath);
    if (iter == s_instances->end())
    {
        CLogger *logger = new CLogger(logFilePath);
        (*s_instances)[logFilePath] = logger;
        return logger;
    }
    else
    {
        return iter->second;
    }
}
