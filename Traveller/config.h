
#pragma once

#ifndef TRAVELLER_CONFIG_H
#define TRAVELLER_CONFIG_H

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
#include "..\c4-lib\tinyxml.h"

typedef struct CConfig_tag
{
    BOOL isIgnoreUtf8;
    BOOL isIgnoreUtf8WithoutBom;
    BOOL isIgnoreHidden;
    BOOL isOverride;
    BOOL isBackupOrig;
    BOOL isAutoFixAudioExt;
    BOOL isAutoFixCueError;
    WTL::CString extensions;
    WTL::CString templateString;
    WTL::CString charmapConfig;
}CConfig;

BOOL LoadConfigFile(TiXmlDocument *xmlfile, CConfig &config);
BOOL SaveConfigFile(LPCTSTR configPath, const CConfig &config);
void SetDefault(CConfig &config);

#endif // TRAVELLER_CONFIG_H