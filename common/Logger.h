/************************************************************************/
/*                                                                      */
/* Common helper functions for Unicue Project.                          */
/* The file is a part of Unicue Project.                                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
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
