/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.3                                                 */
/* A tool to convert Simplified Chinese into Traditional Chinese and    */
/* convert back. It is a part of Unicue Project.                        */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// config.h
#ifndef CHINESECONVERTER_CONFIG_H_
#define CHINESECONVERTER_CONFIG_H_

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
#include "..\c4-lib\tinyxml.h"

enum LANG {
    EN  = (int)0x00,
    CHN = (int)0x01,
    CHT = (int)0x02,
    JPN = (int)0x03
};

enum OUTPUT_ENCODING {
    O_UTF_8       = (int)0x00,
    O_UTF_8_NOBOM = (int)0x01,
    O_UTF_16_LE   = (int)0x02,
    O_UTF_16_BE   = (int)0x03
};

typedef struct CConfig_tag
{
    ATL::CString TemplateStr;              // Template string used at saving as
    ATL::CString MapConfName;              // Config file path for charmaps
    LANG Lang;                             // Language for GUI
    LONG CurrentChoice;                    // Current encoding choice
    OUTPUT_ENCODING OutputEncoding;        // Output encoding
    LONG WindowWidth;                      // width of main window
    LONG WindowHeight;                     // height of main window
}CConfig;

BOOL LoadConfigFile(TiXmlDocument *xmlfile, CConfig &config);
BOOL SaveConfigFile(LPCTSTR configPath, const CConfig &config);
void SetDefault(CConfig &config);

#endif // CHINESECONVERTER_CONFIG_H_