/************************************************************************/
/*                                                                      */
/* Unicue 1.3                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
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
#ifndef CONFIG_H_
#define CONFIG_H_

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
    WTL::CString TemplateStr;              // 命名模板
    BOOL AutoFixCue;                       // 自动修正cue中的音频文件扩展名
    BOOL AutoFixTTA;                       // 自动修正旧式TTA标签
    BOOL AcceptDragAudioFile;              // 接受拖曳音频文档提取内嵌cue
    BOOL AutoCheckCode;                    // 是否自动检查编码
    BOOL AlwaysOnTop;                      // 是否总在最前
    BOOL CloseCuePrompt;                   // 是否关闭cue文件有错误的提示
    //BOOL RegNewUniFile;                    // 注册新建uni文件
    WTL::CString MapConfName;              // 字符映射表配置文件路径
    LANG Lang;                             // Language for GUI
    OUTPUT_ENCODING OutputEncoding;        // Output encoding
    LONG WindowWidth;                      // width of main window
    LONG WindowHeight;                     // height of main window
}CConfig;

BOOL LoadConfigFile(TiXmlDocument *xmlfile, CConfig &config);
BOOL SaveConfigFile(LPCTSTR configPath, const CConfig &config);
void SetDefault(CConfig &config);

// The global instance for config
extern CConfig _Config;

#endif // CONFIG_H_