/************************************************************************/
/*                                                                      */
/* Common helper utility for Unicue Project.                            */
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

// unicuehelper.h
#ifndef UNICUE_HELPER_H
#define UNICUE_HELPER_H

#include "wtlhelper.h"

namespace Unicue {
bool FixTTAOutdatedTag(WTL::CString &cue_string);
bool FixTTAOutdatedTag(const WTL::CString &cue_string, WTL::CString &fixed_string);
bool FixAudioFilePath(const WTL::CString &cue_file_path, WTL::CString &cue_string, bool &cue_file_error);
bool FixAudioFilePath(const WTL::CString &cue_file_path, const WTL::CString &cue_string, WTL::CString &fixed_string, bool &cue_file_error);
bool FixInternalCue(const WTL::CString &audio_file_path, WTL::CString &cue_string, bool &cue_file_error);
bool ExtractTakInternalCue(const WTL::CString &audio_file_path, WTL::CString &cue_string, int &cue_rawstring_length);
bool ExtractFlacInternalCue(const WTL::CString &audio_file_path, WTL::CString &cue_string, int &cue_rawstring_length);
};

#endif // UNICUE_HELPER_H