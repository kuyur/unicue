#pragma once

// cmdline.h
#ifndef CMDLINE_H_
#define CMDLINE_H_

enum APPMODE {
    MODE_SETTING,
    MODE_PROCESS
};

typedef struct CParameters_tag
{
    APPMODE               appMode;
    std::vector<wchar_t*> folders;
}CParameters;

void freeCParameters(CParameters &param);
CParameters parseCmdLine(wchar_t *lpstrCmdLine);

#endif  // CMDLINE_H_
