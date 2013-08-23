// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "file.h"
#include "utils.h"
#include "resource.h"
#include "aboutdlg.h"
#include "SettingDlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

CMainDlg::CMainDlg()
    :m_bNeedConvert(TRUE), m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0),
    m_StringCodeType(L"Local Codepage"),m_bCueFile(FALSE),m_bTransferString(FALSE)
{
    m_RawString = NULL;
    m_String = NULL;
    m_UnicodeString = NULL;
    m_FilePathName = L"";
    m_CodeStatus = L"";
    m_Config.RegNewUniFile = FALSE;

    // Load config file...
    TCHAR processPath[_MAX_PATH];
    GetModuleFileName(NULL, processPath, _MAX_PATH);
    wchar_t *pos = wcsrchr(processPath, L'\\');
    if (pos) *(pos+1) = L'\0';

    m_ConfigPath.append(processPath);
    m_ConfigPath.append(L"Config.xml");

    // Because TiXml does not support wchar_t file name,
    // use stream to load xml file.
    CFile file(m_ConfigPath, CFile::READ);
    if (!file.open())
    {
        CreateConfigFile();
        m_Config.TemplateStr = _T(".utf-8");
        m_Config.AutoFixCue = TRUE;
        m_Config.AutoFixTTA = FALSE;
        m_Config.AcceptDragAudioFile = TRUE;
        m_Config.AutoCheckCode = TRUE;
        m_Config.AlwaysOnTop = TRUE;
        m_Config.CloseCuePrompt = FALSE;
        m_Config.MapConfName = _T("charmap-anisong.xml");
    }
    else
    {
        unsigned int fileLength = file.length();
        char *fileBuffer = new char[fileLength+1];
        memset((void*)fileBuffer, 0, fileLength+1);
        file.read(fileBuffer, fileLength);
        file.close();

        TiXmlDocument *doc = new TiXmlDocument;
        doc->Parse(fileBuffer, NULL, TIXML_ENCODING_UTF8);
        if (doc->Error() || !LoadConfigFile(doc))
        {
            ::DeleteFile(m_ConfigPath.c_str());
            CreateConfigFile();
            m_Config.TemplateStr = _T(".utf-8");
            m_Config.AutoFixCue = TRUE;
            m_Config.AutoFixTTA = FALSE;
            m_Config.AcceptDragAudioFile = TRUE;
            m_Config.AutoCheckCode = TRUE;
            m_Config.AlwaysOnTop = TRUE;
            m_Config.CloseCuePrompt = FALSE;
            m_Config.MapConfName = _T("charmap-anisong.xml");
        }

        delete []fileBuffer;
        fileBuffer = NULL;
        delete doc;
    }
    m_context = new CC4Context(m_Config.MapConfName, processPath);
    if (!m_context->init())
        MessageBox(_T("Failed to load charmaps!"), _T("Unicue"));
    // set local here
    //SetThreadLocale(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
    SetThreadLocale(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
    //SetThreadLocale(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL));
}

CMainDlg::~CMainDlg()
{
    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString=NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString=NULL;
    }
    if (m_context)
    {
        m_context->finalize();
        delete m_context;
        m_context = NULL;
    }
}


BOOL CMainDlg::LoadConfigFile(TiXmlDocument *xmlfile)
{
    TiXmlHandle hRoot(xmlfile);
    TiXmlElement *pElem;
    TiXmlHandle hXmlHandle(0);

    //config节点
    pElem=hRoot.FirstChildElement().Element();
    if (!pElem) return FALSE;
    if (strcmp(pElem->Value(),"config")!=0)
        return FALSE;

    //TemplateStr节点
    hXmlHandle=TiXmlHandle(pElem);
    pElem=hXmlHandle.FirstChild("TemplateStr").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    m_Config.TemplateStr=CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    //AutoFixCue节点
    pElem=hXmlHandle.FirstChild("AutoFixCue").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.AutoFixCue=TRUE;
    else
        m_Config.AutoFixCue=FALSE;

    //AutoFixTTA节点
    pElem=hXmlHandle.FirstChild("AutoFixTTA").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.AutoFixTTA=TRUE;
    else
        m_Config.AutoFixTTA=FALSE;

    //AcceptDragAudioFile节点
    pElem=hXmlHandle.FirstChild("AcceptDragAudioFile").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.AcceptDragAudioFile=TRUE;
    else
        m_Config.AcceptDragAudioFile=FALSE;

    //CloseCuePrompt节点
    pElem=hXmlHandle.FirstChild("CloseCuePrompt").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.CloseCuePrompt=TRUE;
    else
        m_Config.CloseCuePrompt=FALSE;

    //AutoCheckCode节点
    pElem=hXmlHandle.FirstChild("AutoCheckCode").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.AutoCheckCode=TRUE;
    else
        m_Config.AutoCheckCode=FALSE;

    //AlwaysOnTop节点
    pElem=hXmlHandle.FirstChild("AlwaysOnTop").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        m_Config.AlwaysOnTop=TRUE;
    else
        m_Config.AlwaysOnTop=FALSE;

    //CharmapConfPath节点
    pElem=hXmlHandle.FirstChild("CharmapConfPath").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    m_Config.MapConfName = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText()));

    return TRUE;
}

BOOL CMainDlg::CreateConfigFile()
{
    TiXmlDocument configdoc;
    TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
    TiXmlElement *configure=new TiXmlElement("config");

    TiXmlElement *TemplateStr=new TiXmlElement("TemplateStr");
    TiXmlText *TemplateStrValue=new TiXmlText(".utf-8");
    TemplateStr->LinkEndChild(TemplateStrValue);
    configure->LinkEndChild(TemplateStr);

    TiXmlElement *AutoFixCue=new TiXmlElement("AutoFixCue");
    TiXmlText *AutoFixCueValue=new TiXmlText("true");
    AutoFixCue->LinkEndChild(AutoFixCueValue);
    configure->LinkEndChild(AutoFixCue);

    TiXmlElement *AutoFixTTA=new TiXmlElement("AutoFixTTA");
    TiXmlText *AutoFixTTAValue=new TiXmlText("false");
    AutoFixTTA->LinkEndChild(AutoFixTTAValue);
    configure->LinkEndChild(AutoFixTTA);

    TiXmlElement *AcceptDragAudioFile=new TiXmlElement("AcceptDragAudioFile");
    TiXmlText *AcceptDragAudioFileValue=new TiXmlText("true");
    AcceptDragAudioFile->LinkEndChild(AcceptDragAudioFileValue);
    configure->LinkEndChild(AcceptDragAudioFile);

    TiXmlElement *CloseCuePrompt=new TiXmlElement("CloseCuePrompt");
    TiXmlText *CloseCuePromptValue=new TiXmlText("false");
    CloseCuePrompt->LinkEndChild(CloseCuePromptValue);
    configure->LinkEndChild(CloseCuePrompt);

    TiXmlElement *AutoCheckCode=new TiXmlElement("AutoCheckCode");
    TiXmlText *AutoCheckCodeValue=new TiXmlText("true");
    AutoCheckCode->LinkEndChild(AutoCheckCodeValue);
    configure->LinkEndChild(AutoCheckCode);

    TiXmlElement *AlwaysOnTop=new TiXmlElement("AlwaysOnTop");
    TiXmlText *AlwaysOnTopValue=new TiXmlText("true");
    AlwaysOnTop->LinkEndChild(AlwaysOnTopValue);
    configure->LinkEndChild(AlwaysOnTop);

    TiXmlElement *CharmapConfPath = new TiXmlElement("CharmapConfPath");
    TiXmlText *CharmapConfPathValue = new TiXmlText("charmap-anisong.xml");
    CharmapConfPath->LinkEndChild(CharmapConfPathValue);
    configure->LinkEndChild(CharmapConfPath);

    configdoc.LinkEndChild(dec);
    configdoc.LinkEndChild(configure);

    TiXmlPrinter printer;
    configdoc.Accept(&printer);

    CFile file(m_ConfigPath, CFile::WRITE);
    if (file.open())
    {
        file.write(CC4Encode::UTF_8_BOM, 3);
        file.write(printer.CStr(), strlen(printer.CStr()));
        file.close();
    }

    return TRUE;
}

BOOL CMainDlg::SaveConfigFile()
{
    TiXmlDocument configdoc;
    TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
    TiXmlElement *configure=new TiXmlElement("config");

    TiXmlElement *TemplateStr=new TiXmlElement("TemplateStr");
    std::string &UTF8Str = CC4EncodeUTF16::getInstance()->convertWideString(m_Config.TemplateStr.c_str());
    TiXmlText *TemplateStrValue=new TiXmlText(UTF8Str.c_str());
    TemplateStr->LinkEndChild(TemplateStrValue);
    configure->LinkEndChild(TemplateStr);

    TiXmlElement *AutoFixCue=new TiXmlElement("AutoFixCue");
    TiXmlText *AutoFixCueValue;
    if (m_Config.AutoFixCue)
        AutoFixCueValue=new TiXmlText("true");
    else
        AutoFixCueValue=new TiXmlText("false");
    AutoFixCue->LinkEndChild(AutoFixCueValue);
    configure->LinkEndChild(AutoFixCue);

    TiXmlElement *AutoFixTTA=new TiXmlElement("AutoFixTTA");
    TiXmlText *AutoFixTTAValue;
    if (m_Config.AutoFixTTA)
        AutoFixTTAValue=new TiXmlText("true");
    else
        AutoFixTTAValue=new TiXmlText("false");
    AutoFixTTA->LinkEndChild(AutoFixTTAValue);
    configure->LinkEndChild(AutoFixTTA);

    TiXmlElement *AcceptDragAudioFile=new TiXmlElement("AcceptDragAudioFile");
    TiXmlText *AcceptDragAudioFileValue;
    if (m_Config.AcceptDragAudioFile)
        AcceptDragAudioFileValue=new TiXmlText("true");
    else
        AcceptDragAudioFileValue=new TiXmlText("false");
    AcceptDragAudioFile->LinkEndChild(AcceptDragAudioFileValue);
    configure->LinkEndChild(AcceptDragAudioFile);

    TiXmlElement *CloseCuePrompt=new TiXmlElement("CloseCuePrompt");
    TiXmlText *CloseCuePromptValue;
    if (m_Config.CloseCuePrompt)
        CloseCuePromptValue=new TiXmlText("true");
    else
        CloseCuePromptValue=new TiXmlText("false");
    CloseCuePrompt->LinkEndChild(CloseCuePromptValue);
    configure->LinkEndChild(CloseCuePrompt);

    TiXmlElement *AutoCheckCode=new TiXmlElement("AutoCheckCode");
    TiXmlText *AutoCheckCodeValue;
    if (m_Config.AutoCheckCode)
        AutoCheckCodeValue=new TiXmlText("true");
    else
        AutoCheckCodeValue=new TiXmlText("false");
    AutoCheckCode->LinkEndChild(AutoCheckCodeValue);
    configure->LinkEndChild(AutoCheckCode);

    TiXmlElement *AlwaysOnTop=new TiXmlElement("AlwaysOnTop");
    TiXmlText *AlwaysOnTopValue;
    if (m_Config.AlwaysOnTop)
        AlwaysOnTopValue=new TiXmlText("true");
    else
        AlwaysOnTopValue=new TiXmlText("false");
    AlwaysOnTop->LinkEndChild(AlwaysOnTopValue);
    configure->LinkEndChild(AlwaysOnTop);

    TiXmlElement *CharmapConfPath = new TiXmlElement("CharmapConfPath");
    TiXmlText *CharmapConfPathValue;
    CharmapConfPathValue = new TiXmlText(CC4EncodeUTF16::getInstance()->convertWideString(m_Config.MapConfName.c_str()).c_str());
    CharmapConfPath->LinkEndChild(CharmapConfPathValue);
    configure->LinkEndChild(CharmapConfPath);

    configdoc.LinkEndChild(dec);
    configdoc.LinkEndChild(configure);

    TiXmlPrinter printer;
    configdoc.Accept(&printer);
    CFile file(m_ConfigPath, CFile::WRITE);
    if (file.open())
    {
        file.write(CC4Encode::UTF_8_BOM, 3);
        file.write(printer.CStr(), strlen(printer.CStr()));
        file.close();
    }

    return TRUE;
}

BOOL CMainDlg::ExtractTakInternalCue(std::wstring AudioFileName)
{
    m_CodeStatus = _T("UTF-8 (Internal Cue File)");
    m_bNeedConvert = FALSE;
    m_StringCodeType = CC4EncodeUTF8::_getName();
    int nIndex = ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).FindStringExact(0, m_StringCodeType.c_str());
    ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).SetCurSel(nIndex);

    std::wstring statusText = _T("文档编码检测结果：");
    statusText.append(m_CodeStatus).append(_T("\n\n文档路径：")).append(m_FilePathName);
    GetDlgItem(IDC_STATIC_STAT).SetWindowText(statusText.c_str());
    GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));

    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString = NULL;
        m_String = NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString = NULL;
    }

    if (m_FilePathName.length() <= 0)
        return FALSE;

    CFile OpenFile(m_FilePathName, CFile::READ);
    if (!OpenFile.open(/*CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary*/))
    {
        MessageBox(_T("打开失败！"), _T("Unicue"), MB_OK);
        return FALSE;
    }

    m_FilePathName.append(_T(".cue"));
    statusText.append(_T(".cue"));
    GetDlgItem(IDC_STATIC_STAT).SetWindowText(statusText.c_str());

    if (OpenFile.length() < 20480) // 小于20K，文档太小了
    {
        OpenFile.close();
        return FALSE;
    }
    unsigned char Buffer[20480]; //20k的缓冲区
    memset(Buffer,0,20480);
    OpenFile.seek(-20480, std::ios::end);
    OpenFile.read((char*)Buffer, 20480);
    OpenFile.close();

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state=0,BeginPos=0,EndPos=0,Length=0;
    for (int i=0;i<20480;++i)
    {
        if ((Buffer[i]>=0x41)&&(Buffer[i]<=0x5A))
            Buffer[i]=Buffer[i]+0x20;

        switch (Buffer[i])
        {
        case 'c':
            state=1;      //C
            break;
        case 'u':
            if (state==1)
                state=2;  //Cu
            else
                state=0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state=3;  //Cue
                break;
            case 5:
                state=6;  //Cueshe
                break;
            case 6:
                state=7;  //Cueshee
                break;
            default:
                state=0;
            }
            break;
        case 's':
            if (state==3)
                state=4;  //Cues
            else
                state=0;
            break;
        case 'h':
            if (state==4)
                state=5;  //Cuesh
            else
                state=0;
            break;
        case 't':
            if (state==7)
            {
                state=8;  //Cuesheet
            }
            else
                state=0;
            break;
        default:
            state=0;
        }
        if (state==8)
        {
            BeginPos=i+2;
            break;
        }
    }

    if (BeginPos==0)
        return FALSE;

    // 查找终止符 0D 0A ? 00 00 00 00 00 00 （连续六个终止符以上）
    state=0;
    for (int i=BeginPos;i<20480;++i)
    {
        switch (Buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state=0;
        }
        if (state==6)
        {
            EndPos=i-6; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (EndPos<=1)
        return FALSE;

    if ((Buffer[EndPos-2]=='\x0D')&&(Buffer[EndPos-1]=='\x0A'))
        EndPos--;

    Length=EndPos-BeginPos+1;
    if (Length<=10) //too short
        return FALSE;

    m_RawStringLength=Length;
    m_RawString=new char[m_RawStringLength+1];
    memcpy(m_RawString,Buffer+BeginPos,m_RawStringLength);
    m_RawString[m_RawStringLength]='\0';
    m_String=m_RawString;
    m_StringLength=m_RawStringLength;

    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());

    FixInternalCue(AudioFileName);

    return TRUE;
}

// flac文件结构
// http://flac.sourceforge.net/format.html
BOOL CMainDlg::ExtractFlacInternalCue(std::wstring AudioFileName)
{
    m_CodeStatus=_T("UTF-8 (Internal Cue File)");
    m_bNeedConvert=FALSE;
    m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
    int nIndex = ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).FindStringExact(0, m_StringCodeType.c_str());
    ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).SetCurSel(nIndex);
    std::wstring statusText = _T("文档编码检测结果：");
    statusText.append(m_CodeStatus).append(_T("\n\n文档路径：")).append(m_FilePathName);
    GetDlgItem(IDC_STATIC_STAT).SetWindowText(statusText.c_str());
    GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));

    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString=NULL;
        m_String=NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString=NULL;
    }

    if (m_FilePathName.length() <= 0)
        return FALSE;

    CFile OpenFile(m_FilePathName, CFile::READ);
    if (!OpenFile.open(/*CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary*/))
    {
        MessageBox(_T("打开失败！"), _T("Unicue"), MB_OK);
        return FALSE;
    }

    m_FilePathName.append(_T(".cue"));
    statusText.append(_T(".cue"));
    GetDlgItem(IDC_STATIC_STAT).SetWindowText(statusText.c_str());

    if (OpenFile.length() < 1048576) // 小于1M，文档太小了
    {
        OpenFile.close();
        return FALSE;
    }

    unsigned char Header[5];
    memset(Header,0,5);
    ULONGLONG position=0;
    //4个字节的头部
    OpenFile.read((char*)Header,4);
    if (strcmp((char*)Header,"fLaC")!=0)
    {
        //AfxMessageBox(_T("Not real flac file!"));
        return FALSE;
    }

    unsigned char chr;
    unsigned char *Buffer=NULL;
    UINT Length;
    //4个字节的METADATA_BLOCK_HEADER
    do 
    {
        OpenFile.read((char*)Header,4);
        //解析
        memcpy(&chr,Header,1);
        //检查最高位是否为1
        if ((chr&0x80)==0x80)
        {
            //最后一个METADATA_BLOCK
            if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //申请空间
                Buffer=new unsigned char[Length+1];
                //读取BLOCK DATA
                OpenFile.read((char*)Buffer,Length);
                Buffer[Length]='\0';
            }
            break;
        }
        else
        {
            //不是最后一个METADATA_BLOCK
            if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //申请空间
                Buffer=new unsigned char[Length+1];
                //读取BLOCK DATA
                OpenFile.read((char*)Buffer,Length);
                Buffer[Length]='\0';
                break;
            }
            else //不是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //移动文件指针
                OpenFile.seek(Length,std::ios::cur);
                position=OpenFile.getPosition();
            }
        }
    } while(position<=1048576);

    OpenFile.close();
    if (!Buffer)
        return FALSE;

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state=0,BeginPos=0,EndPos=0;
    for (UINT i=0;i<Length;++i)
    {
        if ((Buffer[i]>=0x41)&&(Buffer[i]<=0x5A))
            Buffer[i]=Buffer[i]+0x20;

        switch (Buffer[i])
        {
        case 'c':
            state=1;      //C
            break;
        case 'u':
            if (state==1)
                state=2;  //Cu
            else
                state=0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state=3;  //Cue
                break;
            case 5:
                state=6;  //Cueshe
                break;
            case 6:
                state=7;  //Cueshee
                break;
            default:
                state=0;
            }
            break;
        case 's':
            if (state==3)
                state=4;  //Cues
            else
                state=0;
            break;
        case 'h':
            if (state==4)
                state=5;  //Cuesh
            else
                state=0;
            break;
        case 't':
            if (state==7)
            {
                state=8;  //Cuesheet
            }
            else
                state=0;
            break;
        default:
            state=0;
        }
        if (state==8)
        {
            BeginPos=i+2;
            break;
        }
    }
    if (BeginPos==0)
    {
        delete []Buffer;
        return FALSE;
    }
    //查找终止符 0D 0A ? 00 00 00（连续3个终止符以上）
    state=0;
    for (int i=BeginPos;i<20480;++i)
    {
        switch (Buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state=0;
        }
        if (state==3)
        {
            EndPos=i-3; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (EndPos<=1)
    {
        delete []Buffer;
        return FALSE;
    }

    if ((Buffer[EndPos-2]=='\x0D')&&(Buffer[EndPos-1]=='\x0A'))
        EndPos--;

    int CueLength=EndPos-BeginPos+1;
    if (CueLength<=10) //too short
    {
        delete []Buffer;
        return FALSE;
    }

    m_RawStringLength=CueLength;
    m_RawString=new char[m_RawStringLength+1];
    memcpy(m_RawString,Buffer+BeginPos,m_RawStringLength);
    m_RawString[m_RawStringLength]='\0';
    m_String=m_RawString;
    m_StringLength=m_RawStringLength;
    delete []Buffer;

    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
    FixInternalCue(AudioFileName);

    return TRUE;
}

void CMainDlg::FixCue()
{
    if (!m_bCueFile)
        return;

    FixTTACue();

    int len = GetDlgItem(IDC_EDIT_UNICODE).GetWindowTextLengthW();
    wchar_t * cueStringBuffer = new wchar_t[len+1];
    cueStringBuffer[len] = L'\0';
    GetDlgItem(IDC_EDIT_UNICODE).GetWindowText(cueStringBuffer, len);
    std::wstring CueString(cueStringBuffer);
    delete []cueStringBuffer;

    std::wstring::size_type BeginPos = CueString.find(_T("FILE \""));
    if (BeginPos == std::wstring::npos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }
    std::wstring::size_type EndPos = CueString.find(_T("\" WAVE"));
    if (EndPos == std::wstring::npos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }
    BeginPos += 6;
    if (BeginPos >= EndPos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }

    std::wstring MusicFileName(CueString.substr(BeginPos,EndPos - BeginPos)), MusicFilePath; //音频文件名，路径

    //依据文档路径：m_FilePathName查找音频文件
    std::wstring::size_type pos=m_FilePathName.rfind('\\');
    MusicFilePath.append(m_FilePathName.substr(0, pos));
    MusicFilePath.append(_T("\\"));
    MusicFilePath.append(MusicFileName);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    hFind=FindFirstFile(MusicFilePath.c_str(), &FindFileData);

    if (hFind==INVALID_HANDLE_VALUE) //没找到cue中音频文件
    {
        pos=MusicFilePath.rfind('.');
        MusicFilePath=MusicFilePath.substr(pos);
        //替换扩展名查找
        std::wstring FindFilePath;

        FindFilePath.append(MusicFilePath).append(_T(".ape")); //ape
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".mac")); //mac
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".flac")); //flac
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".fla")); //flac
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".tta")); //tta
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".tak")); //tak
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".wv")); //wv
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".m4a")); //apple lossless
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".wma")); //wma
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".wav")); //wav
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".wave")); //wav
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        FindFilePath.clear();
        FindFilePath.append(MusicFilePath).append(_T(".mp3")); //mp3
        hFind=FindFirstFile(FindFilePath.c_str(), &FindFileData);
        if (hFind!=INVALID_HANDLE_VALUE)
        {
            replace(CueString, MusicFileName, FindFileData.cFileName);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
            FindClose(hFind);
            return;
        }

        //最后还是没找到
        FindClose(hFind);
        return;
    }
    else
    {
        FindClose(hFind);
        return;
    }
}

void CMainDlg::FixInternalCue(std::wstring AudioFileName)
{
    int len = GetDlgItem(IDC_EDIT_UNICODE).GetWindowTextLengthW();
    wchar_t * cueStringBuffer = new wchar_t[len+1];
    cueStringBuffer[len] = L'\0';
    GetDlgItem(IDC_EDIT_UNICODE).GetWindowText(cueStringBuffer, len);
    std::wstring CueString(cueStringBuffer);
    delete []cueStringBuffer;

    std::wstring::size_type BeginPos=CueString.find(_T("FILE \""));
    if (BeginPos == std::wstring::npos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }
    std::wstring::size_type EndPos = CueString.find(_T("\" WAVE"));
    if (EndPos == std::wstring::npos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }
    BeginPos+=6;
    if (BeginPos >= EndPos)
    {
        if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
        return;
    }

    std::wstring OldFileName(CueString.substr(BeginPos, EndPos - BeginPos)); //音频文件名
    replace(CueString, OldFileName, AudioFileName);
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString.c_str());
}

void CMainDlg::FixTTACue()
{
    if (!m_bCueFile)
        return;

    int len = GetDlgItem(IDC_EDIT_UNICODE).GetWindowTextLengthW();
    wchar_t * cueStringBuffer = new wchar_t[len+1];
    cueStringBuffer[len] = L'\0';
    GetDlgItem(IDC_EDIT_UNICODE).GetWindowText(cueStringBuffer, len);

    _wcslwr(cueStringBuffer);
    wchar_t *pos = wcsstr(cueStringBuffer, L"the true audio");
    if (pos)
    {
        GetDlgItem(IDC_EDIT_UNICODE).GetWindowText(cueStringBuffer, len);
        std::wstring newCueString(cueStringBuffer);
        newCueString.replace(pos - cueStringBuffer, 14, L"WAVE");
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(newCueString.c_str());
    }

    delete []cueStringBuffer;
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    // init menu
    m_menu.LoadMenu(IDR_MENU1);
    CWindow::SetMenu(m_menu);
    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME_BIG, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME_LITTLE, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    // add encode items
    CComboBox theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
    std::list<std::wstring>::iterator iter;
    theCombo.InsertString(-1, _T("Local Codepage"));
    for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
        theCombo.InsertString(-1, iter->c_str());
    theCombo.SetCurSel(0);

    // when called from command line
    LPWSTR *szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if(NULL != szArglist )
    {
        for (int i=0; i < nArgs; i++)
        {
            // TODO
            //MessageBox(szArglist[i]); // 0 is execution path
        }
    }
    LocalFree(szArglist);

    return TRUE;
}

BOOL CMainDlg::SetDialogPos()
{
    RECT rc;
    GetWindowRect(&rc);

    if (m_Config.AlwaysOnTop)
        return SetWindowPos(HWND_TOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);
    else
        return SetWindowPos(HWND_NOTOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);
}


BOOL CMainDlg::DealFile()
{
    if (m_FilePathName.length() <= 0) return FALSE;

    // TODO
    return true;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CMainDlg::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnFileExit(WORD, WORD wID, HWND, BOOL&)
{
    CloseDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnFileOpen(WORD, WORD, HWND, BOOL&)
{
    CFileDialog openFile(TRUE, _T("*.txt"), NULL, OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("文本文件(*.txt;*.cue;*.log)\0*.txt;*.cue;*.log\0txt文本文件(*.txt)\0*.txt\0cue文件(*.cue)\0*.cue\0log文件(*.log)\0*.log\0All Files (*.*)\0*.*\0\0"));
    if (openFile.DoModal() == IDOK)
    {
        // TODO
    }
    return 0;
}

LRESULT CMainDlg::OnFileSave(WORD, WORD, HWND, BOOL&)
{
    // TODO
    return 0;
}

LRESULT CMainDlg::OnFileOption(WORD, WORD, HWND, BOOL&)
{
    CSettingDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnDropFiles(UINT, WPARAM, LPARAM, BOOL&)
{
    // TODO
    MessageBox(_T("File Dropped"));
    return 0;
}