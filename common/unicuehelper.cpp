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

#include "unicuehelper.h"
#include "../c4-lib/c4context.h"
#include "winfile.h"

namespace Unicue {

bool FixTTAOutdatedTag(ATL::CString &cue_string)
{
    ATL::CString low_cue_string(cue_string);
    low_cue_string.MakeLower();

    int pos = low_cue_string.Find(L"the true audio");
    if (pos <= 0) return false;

    cue_string.Replace(cue_string.Mid(pos, 14), L"WAVE");
    return true;
}

bool FixTTAOutdatedTag(const ATL::CString &cue_string, ATL::CString &fixed_string)
{
    fixed_string.Empty();
    fixed_string += cue_string;
    return FixTTAOutdatedTag(fixed_string);
}

bool FixAudioFilePath(const ATL::CString &cue_file_path, ATL::CString &cue_string, bool &cue_file_error)
{
    cue_file_error = false;
    int begin_pos = cue_string.Find(L"FILE \"");
    if (begin_pos == -1)
    {
        cue_file_error = true;
        return false;
    }
    int end_pos = cue_string.Find(L"\" WAVE");
    if (end_pos == -1)
    {
        cue_file_error = true;
        return false;
    }
    begin_pos += 6;
    if (begin_pos >= end_pos)
    {
        cue_file_error = true;
        return false;
    }

    ATL::CString &audio_file_name = cue_string.Mid(begin_pos, end_pos - begin_pos); // origin audio file name
    ATL::CString &audio_file_path = cue_file_path.Left(cue_file_path.ReverseFind(L'\\'));
    audio_file_path += L"\\";
    audio_file_path += audio_file_name;

    if (PathFileExists(audio_file_path)) return false; // no need to fix

    // 替换扩展名查找
    int pos = audio_file_name.ReverseFind(L'.');
    int extension_length = 0;
    ATL::CString audio_file_name_found(L"");
    if (-1 != pos)
    {
        extension_length += audio_file_name.GetLength() - pos; // contain .
        audio_file_name_found += audio_file_name.Left(pos);
    }
    else
        audio_file_name_found += audio_file_name;

    const static wchar_t* FORMAT[12] =
    {
        L".ape",
        L".flac",
        L".tta",
        L".tak",
        L".wv",
        L".m4a",
        L".wma",
        L".wav",
        L".mac",
        L".fla",
        L".wave",
        L".mp3"
    };

    for (int i = 0; i < 12; ++i)
    {
        RemoveFromEnd(audio_file_path, extension_length);
        const wchar_t *format = FORMAT[i];
        audio_file_path += format;
        if (PathFileExists(audio_file_path))
        {
            audio_file_name_found += format;
            cue_string.Replace(audio_file_name, audio_file_name_found);
            return true;
        }
        extension_length = wcslen(format);
    }

    // also guess from cue file name
    ATL::CString audio_file_path_implicit(cue_file_path);
    ATL::CString &audio_file_name_implicit = cue_file_path.Right(cue_file_path.GetLength() - cue_file_path.ReverseFind(L'\\') - 1);
    // For first time, length is 4 (.cue)
    extension_length = 4;
    RemoveFromEnd(audio_file_name_implicit, extension_length);

    for (int i = 0; i < 12; ++i)
    {
        RemoveFromEnd(audio_file_path_implicit, extension_length);
        const wchar_t *format = FORMAT[i];
        audio_file_path_implicit += format;
        if (PathFileExists(audio_file_path_implicit))
        {
            audio_file_name_implicit += format;
            cue_string.Replace(audio_file_name, audio_file_name_implicit);
            return true;
        }
        extension_length = wcslen(format);
    }

    // no audio file found
    return false;
}

bool FixAudioFilePath(const ATL::CString &cue_file_path, const ATL::CString &cue_string, ATL::CString &fixed_string, bool &cue_file_error)
{
    fixed_string.Empty();
    fixed_string += cue_string;
    return FixAudioFilePath(cue_file_path, fixed_string, cue_file_error);
}

bool FixInternalCue(const ATL::CString &audio_file_name, ATL::CString &cue_string, bool &cue_file_error)
{
    cue_file_error = false;
    int begin_pos = cue_string.Find(_T("FILE \""));
    if (begin_pos == -1)
    {
        cue_file_error = true;
        return false;
    }
    int end_pos = cue_string.Find(_T("\" WAVE"));
    if (end_pos == -1)
    {
        cue_file_error = true;
        return false;
    }

    begin_pos += 6;
    if (begin_pos >= end_pos)
    {
        cue_file_error = false;
        return false;
    }

    ATL::CString &old_file_name = cue_string.Mid(begin_pos, end_pos - begin_pos); // 音频文件名
    if (old_file_name != audio_file_name)
    {
        cue_string.Replace(old_file_name, audio_file_name);
        return true;
    }
 
    return false;
}

bool ExtractTakInternalCue(const ATL::CString &audio_file_path, ATL::CString &cue_string, int &cue_rawstring_length)
{
    cue_string.Empty();
    cue_rawstring_length = 0;

    if (audio_file_path.IsEmpty())
        return false;

    CWinFile file(audio_file_path, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
        return false;

    if (file.length() < 20480) 
    {
        // 小于20K，文档太小了
        file.close();
        return false;
    }
    // 20k的缓冲区
    unsigned char buffer[20480] = {0}; 
    file.seek(-20480, CWinFile::end);
    file.read((char*)buffer, 20480);
    file.close();

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state = 0, begin_pos = 0, end_pos = 0, length = 0;
    for (int i = 0; i < 20480; ++i)
    {
        if ((buffer[i] >= 0x41) && (buffer[i] <= 0x5A))
            buffer[i] = buffer[i] + 0x20;

        switch (buffer[i])
        {
        case 'c':
            state = 1;      // C
            break;
        case 'u':
            if (state == 1)
                state = 2;  // Cu
            else
                state = 0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state = 3;  // Cue
                break;
            case 5:
                state = 6;  // Cueshe
                break;
            case 6:
                state = 7;  // Cueshee
                break;
            default:
                state = 0;
            }
            break;
        case 's':
            if (state == 3)
                state = 4;  // Cues
            else
                state = 0;
            break;
        case 'h':
            if (state == 4)
                state = 5;  // Cuesh
            else
                state = 0;
            break;
        case 't':
            if (state == 7)
            {
                state = 8;  // Cuesheet
            }
            else
                state = 0;
            break;
        default:
            state = 0;
        }
        if (state == 8)
        {
            begin_pos = i + 2;
            break;
        }
    }

    if (begin_pos == 0)
        return FALSE;

    // 查找终止符 0D 0A ? 00 00 00 00 00 00 （连续六个终止符以上）
    state = 0;
    for (int i = begin_pos; i < 20480; ++i)
    {
        switch (buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state = 0;
        }
        if (state == 6)
        {
            end_pos = i - 6; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (end_pos <= 1)
        return false;

    if ((buffer[end_pos - 2] == '\x0D') && (buffer[end_pos - 1] == '\x0A'))
        end_pos--;

    length = end_pos - begin_pos + 1;
    if (length <= 10) //too short
        return false;

    cue_rawstring_length = length;
    cue_string += CC4EncodeUTF8::convert2unicode((char*)(buffer + begin_pos), length).c_str();

    return true;
}

bool ExtractFlacInternalCue(const ATL::CString &audio_file_path, ATL::CString &cue_string, int &cue_rawstring_length)
{
    cue_string.Empty();
    cue_rawstring_length = 0;

    if (audio_file_path.IsEmpty())
        return false;

    CWinFile file(audio_file_path, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
        return false;

    if (file.length() < 1048576) // 小于1M，文档太小了
    {
        file.close();
        return false;
    }

    unsigned char header[5];
    memset(header,0,5);
    UINT64 position=0;
    // 4个字节的头部
    file.seek(0, CWinFile::begin);
    file.read((char*)header, 4);
    if (strcmp((char*)header, "fLaC")!=0)
        return false; // Not real flac file

    unsigned char chr;
    unsigned char *buffer=NULL;
    UINT length;
    // 4个字节的METADATA_BLOCK_HEADER
    do 
    {
        file.read((char*)header, 4);
        // 解析
        memcpy(&chr, header, 1);
        // 检查最高位是否为1
        if ((chr&0x80) == 0x80)
        {
            // 最后一个METADATA_BLOCK
            if ((chr&0x7F) == 0x04) // 是VORBIS_COMMENT
            {
                // 读取BLOCK长度
                length = header[1] * 0x10000 + header[2] * 0x100 + header[3];
                // 申请空间
                buffer = new unsigned char[length+1];
                // 读取BLOCK DATA
                file.read((char*)buffer, length);
                buffer[length]='\0';
            }
            break;
        }
        else
        {
            // 不是最后一个METADATA_BLOCK
            if ((chr&0x7F) == 0x04)// 是VORBIS_COMMENT
            {
                // 读取BLOCK长度
                length = header[1] * 0x10000 + header[2] * 0x100 + header[3];
                // 申请空间
                buffer = new unsigned char[length+1];
                // 读取BLOCK DATA
                file.read((char*)buffer, length);
                buffer[length]='\0';
                break;
            }
            else // 不是VORBIS_COMMENT
            {
                // 读取BLOCK长度
                length = header[1] * 0x10000 + header[2] * 0x100 + header[3];
                //移动文件指针
                file.seek(length, CWinFile::current);
                position = file.getPosition();
            }
        }
    } while (position <= 1048576);

    file.close();
    if (!buffer)
        return false;

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state = 0, begin_pos = 0, end_pos = 0;
    for (UINT i = 0; i < length; ++i)
    {
        if ((buffer[i] >= 0x41) && (buffer[i] <= 0x5A))
            buffer[i] = buffer[i] + 0x20;

        switch (buffer[i])
        {
        case 'c':
            state = 1;      // C
            break;
        case 'u':
            if (state == 1)
                state = 2;  // Cu
            else
                state = 0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state = 3;  // Cue
                break;
            case 5:
                state = 6;  // Cueshe
                break;
            case 6:
                state = 7;  // Cueshee
                break;
            default:
                state = 0;
            }
            break;
        case 's':
            if (state == 3)
                state = 4;  // Cues
            else
                state = 0;
            break;
        case 'h':
            if (state == 4)
                state = 5;  // Cuesh
            else
                state = 0;
            break;
        case 't':
            if (state == 7)
            {
                state = 8;  // Cuesheet
            }
            else
                state = 0;
            break;
        default:
            state = 0;
        }
        if (state == 8)
        {
            begin_pos = i + 2;
            break;
        }
    }
    if (begin_pos == 0)
    {
        delete []buffer;
        return false;
    }
    // 查找终止符 0D 0A ? 00 00 00（连续3个终止符以上）
    state = 0;
    for (int i = begin_pos; i < 20480; ++i)
    {
        switch (buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state = 0;
        }
        if (state == 3)
        {
            end_pos = i-3; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (end_pos <= 1)
    {
        delete []buffer;
        return false;
    }

    if ((buffer[end_pos - 2] == '\x0D') && (buffer[end_pos - 1] == '\x0A'))
        end_pos--;

    int cue_length = end_pos - begin_pos + 1;
    if (cue_length <= 10) // too short
    {
        delete []buffer;
        return false;
    }

    cue_rawstring_length = cue_length;
    cue_string += CC4EncodeUTF8::convert2unicode((char*)(buffer + begin_pos), cue_length).c_str();
    delete []buffer;

    return true;
}

};