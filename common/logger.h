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

#pragma once
// logger.h
#ifndef LOGGER_H_
#define LOGGER_H_

#include <unordered_map>
#include "winfile.h"

class CLogger
{
public:
    static CLogger* getInstance(const wchar_t* logFilePath);
private:
    static std::unordered_map<std::wstring, CLogger*> *s_instances;
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (s_instances)
            {
                std::unordered_map<std::wstring, CLogger*>::iterator logger_iter;
                for (; logger_iter != s_instances->end(); ++logger_iter)
                {
                    delete logger_iter->second;
                }
                s_instances->clear();
                delete s_instances;
                s_instances = NULL;
            }
                
        }
    };
    static CGarbo garbo;

private:
    CLogger(const wchar_t* logFilePath);
    ~CLogger(void);
    std::wstring m_filePath;

public:
    BOOL log(const wchar_t* str, ...);
};

#endif // LOGGER_H_
