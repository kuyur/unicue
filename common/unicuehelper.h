/************************************************************************/
/*                                                                      */
/* Unicue Common Modules Library                                        */
/*                                                                      */
/* Common helper utility for Unicue Project.                            */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/unicue-common                  */
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