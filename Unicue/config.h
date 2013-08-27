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