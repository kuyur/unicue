/************************************************************************/
/*                                                                      */
/* Traveller 1.4                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

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
CParameters parseCmdLine(const wchar_t *lpstrCmdLine);

#endif  // CMDLINE_H_
