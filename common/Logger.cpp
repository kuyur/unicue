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
