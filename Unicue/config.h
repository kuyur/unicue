/************************************************************************/
/*                                                                      */
/* Unicue 1.2                                                           */
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

#include <WinDef.h>
#include <atlmisc.h>

typedef struct CConfig_tag
{
    WTL::CString TemplateStr;              // 命名模板
    BOOL AutoFixCue;                       // 自动修正cue中的音频文件扩展名
    BOOL AutoFixTTA;                       // 自动修正旧式TTA标签
    BOOL AcceptDragAudioFile;              // 接受拖曳音频文档提取内嵌cue
    BOOL AutoCheckCode;                    // 是否自动检查编码
    BOOL AlwaysOnTop;                      // 是否总在最前
    BOOL CloseCuePrompt;                   // 是否关闭cue文件有错误的提示
    BOOL RegNewUniFile;                    // 注册新建uni文件
    WTL::CString MapConfName;              // 字符映射表配置文件路径
}CConfig;

#endif // CONFIG_H_