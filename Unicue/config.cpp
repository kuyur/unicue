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

#include "config.h"
#include "../c4-lib/c4encode.h"
#include "../common/utils.h"
#include "../common/winfile.h"

BOOL LoadConfigFile(TiXmlDocument *xmlfile, CConfig &config)
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
    config.TemplateStr = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    //AutoFixCue节点
    pElem=hXmlHandle.FirstChild("AutoFixCue").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.AutoFixCue = TRUE;
    else
        config.AutoFixCue = FALSE;

    //AutoFixTTA节点
    pElem=hXmlHandle.FirstChild("AutoFixTTA").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        config.AutoFixTTA=TRUE;
    else
        config.AutoFixTTA=FALSE;

    //AcceptDragAudioFile节点
    pElem=hXmlHandle.FirstChild("AcceptDragAudioFile").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        config.AcceptDragAudioFile=TRUE;
    else
        config.AcceptDragAudioFile=FALSE;

    //CloseCuePrompt节点
    pElem=hXmlHandle.FirstChild("CloseCuePrompt").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        config.CloseCuePrompt=TRUE;
    else
        config.CloseCuePrompt=FALSE;

    //AutoCheckCode节点
    pElem=hXmlHandle.FirstChild("AutoCheckCode").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        config.AutoCheckCode=TRUE;
    else
        config.AutoCheckCode=FALSE;

    //AlwaysOnTop节点
    pElem=hXmlHandle.FirstChild("AlwaysOnTop").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true")==0)
        config.AlwaysOnTop=TRUE;
    else
        config.AlwaysOnTop=FALSE;

    //CharmapConfPath节点
    pElem=hXmlHandle.FirstChild("CharmapConfPath").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.MapConfName = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    // Lang node
    pElem = hXmlHandle.FirstChild("Language").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"en") == 0)
        config.Lang = EN;
    else if (_stricmp(pElem->GetText(),"chn") == 0)
        config.Lang = CHN;
    else if (_stricmp(pElem->GetText(),"cht") == 0)
        config.Lang = CHT;
    else if (_stricmp(pElem->GetText(),"jpn") == 0)
        config.Lang = JPN;
    else
        config.Lang = CHN;

    // Output node
    pElem = hXmlHandle.FirstChild("Output").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"UTF_8_BOM") == 0)
        config.OutputEncoding = O_UTF_8;
    else if (_stricmp(pElem->GetText(),"UTF_8_NOBOM") == 0)
        config.OutputEncoding = O_UTF_8_NOBOM;
    else if (_stricmp(pElem->GetText(),"UTF_16_LE") == 0)
        config.OutputEncoding = O_UTF_16_LE;
    else if (_stricmp(pElem->GetText(),"UTF_16_BE") == 0)
        config.OutputEncoding = O_UTF_16_BE;
    else
        config.OutputEncoding = O_UTF_8;

    // WindowWidth node
    pElem = hXmlHandle.FirstChild("WindowWidth").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.WindowWidth = (LONG)strtol(pElem->GetText(), (char **)NULL, 10);

    // WindowHeight node
    pElem = hXmlHandle.FirstChild("WindowHeight").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.WindowHeight = (LONG)strtol(pElem->GetText(), (char **)NULL, 10);

    return TRUE;
}

BOOL SaveConfigFile(LPCTSTR configPath, const CConfig &config)
{
    TiXmlDocument configdoc;
    TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
    TiXmlElement *configure=new TiXmlElement("config");

    TiXmlElement *TemplateStr=new TiXmlElement("TemplateStr");
    std::string &UTF8Str = CC4EncodeUTF16::getInstance()->convertWideString(config.TemplateStr);
    TiXmlText *TemplateStrValue=new TiXmlText(UTF8Str.c_str());
    TemplateStr->LinkEndChild(TemplateStrValue);
    configure->LinkEndChild(TemplateStr);

    TiXmlElement *AutoFixCue=new TiXmlElement("AutoFixCue");
    TiXmlText *AutoFixCueValue;
    if (config.AutoFixCue)
        AutoFixCueValue=new TiXmlText("true");
    else
        AutoFixCueValue=new TiXmlText("false");
    AutoFixCue->LinkEndChild(AutoFixCueValue);
    configure->LinkEndChild(AutoFixCue);

    TiXmlElement *AutoFixTTA=new TiXmlElement("AutoFixTTA");
    TiXmlText *AutoFixTTAValue;
    if (config.AutoFixTTA)
        AutoFixTTAValue=new TiXmlText("true");
    else
        AutoFixTTAValue=new TiXmlText("false");
    AutoFixTTA->LinkEndChild(AutoFixTTAValue);
    configure->LinkEndChild(AutoFixTTA);

    TiXmlElement *AcceptDragAudioFile=new TiXmlElement("AcceptDragAudioFile");
    TiXmlText *AcceptDragAudioFileValue;
    if (config.AcceptDragAudioFile)
        AcceptDragAudioFileValue=new TiXmlText("true");
    else
        AcceptDragAudioFileValue=new TiXmlText("false");
    AcceptDragAudioFile->LinkEndChild(AcceptDragAudioFileValue);
    configure->LinkEndChild(AcceptDragAudioFile);

    TiXmlElement *CloseCuePrompt=new TiXmlElement("CloseCuePrompt");
    TiXmlText *CloseCuePromptValue;
    if (config.CloseCuePrompt)
        CloseCuePromptValue=new TiXmlText("true");
    else
        CloseCuePromptValue=new TiXmlText("false");
    CloseCuePrompt->LinkEndChild(CloseCuePromptValue);
    configure->LinkEndChild(CloseCuePrompt);

    TiXmlElement *AutoCheckCode=new TiXmlElement("AutoCheckCode");
    TiXmlText *AutoCheckCodeValue;
    if (config.AutoCheckCode)
        AutoCheckCodeValue=new TiXmlText("true");
    else
        AutoCheckCodeValue=new TiXmlText("false");
    AutoCheckCode->LinkEndChild(AutoCheckCodeValue);
    configure->LinkEndChild(AutoCheckCode);

    TiXmlElement *AlwaysOnTop=new TiXmlElement("AlwaysOnTop");
    TiXmlText *AlwaysOnTopValue;
    if (config.AlwaysOnTop)
        AlwaysOnTopValue=new TiXmlText("true");
    else
        AlwaysOnTopValue=new TiXmlText("false");
    AlwaysOnTop->LinkEndChild(AlwaysOnTopValue);
    configure->LinkEndChild(AlwaysOnTop);

    TiXmlElement *CharmapConfPath = new TiXmlElement("CharmapConfPath");
    TiXmlText *CharmapConfPathValue;
    CharmapConfPathValue = new TiXmlText(CC4EncodeUTF16::getInstance()->convertWideString(config.MapConfName).c_str());
    CharmapConfPath->LinkEndChild(CharmapConfPathValue);
    configure->LinkEndChild(CharmapConfPath);

    TiXmlElement *Lang = new TiXmlElement("Language");
    TiXmlText *LangValue;
    switch (config.Lang)
    {
    case EN:
        LangValue = new TiXmlText("en");
        break;
    case CHN:
        LangValue = new TiXmlText("chn");
        break;
    case CHT:
        LangValue = new TiXmlText("cht");
        break;
    case JPN:
        LangValue = new TiXmlText("jpn");
        break;
    default:
        LangValue = new TiXmlText("chn");
    }
    Lang->LinkEndChild(LangValue);
    configure->LinkEndChild(Lang);

    TiXmlElement *Output = new TiXmlElement("Output");
    TiXmlText *OutputValue;
    switch (config.OutputEncoding)
    {
    case O_UTF_8:
        OutputValue = new TiXmlText("UTF_8_BOM");
        break;
    case O_UTF_8_NOBOM:
        OutputValue = new TiXmlText("UTF_8_NOBOM");
        break;
    case O_UTF_16_LE:
        OutputValue = new TiXmlText("UTF_16_LE");
        break;
    case O_UTF_16_BE:
        OutputValue = new TiXmlText("UTF_16_BE");
        break;
    default:
        OutputValue = new TiXmlText("UTF_8_BOM");
    }
    Output->LinkEndChild(OutputValue);
    configure->LinkEndChild(Output);

    TiXmlElement *WindowWidth = new TiXmlElement("WindowWidth");
    TiXmlText *WindowWidthValue = new TiXmlText(toSTLString(config.WindowWidth).c_str());
    WindowWidth->LinkEndChild(WindowWidthValue);
    configure->LinkEndChild(WindowWidth);

    TiXmlElement *WindowHeight = new TiXmlElement("WindowHeight");
    TiXmlText *WindowHeightValue = new TiXmlText(toSTLString(config.WindowHeight).c_str());
    WindowHeight->LinkEndChild(WindowHeightValue);
    configure->LinkEndChild(WindowHeight);

    configdoc.LinkEndChild(dec);
    configdoc.LinkEndChild(configure);

    TiXmlPrinter printer;
    configdoc.Accept(&printer);
    CWinFile file(configPath, CWinFile::modeWrite | CWinFile::openCreateAlways | CWinFile::shareExclusive);
    if (file.open())
    {
        file.write(CC4Encode::UTF_8_BOM, 3);
        file.write(printer.CStr(), strlen(printer.CStr()));
        file.close();
    }

    return TRUE;
}

void SetDefault(CConfig &config)
{
    config.AcceptDragAudioFile = TRUE;
    config.AlwaysOnTop = TRUE;
    config.AutoCheckCode = TRUE;
    config.AutoFixCue = TRUE;
    config.AutoFixTTA = FALSE;
    config.CloseCuePrompt = FALSE;
    config.Lang = CHN;
    config.MapConfName = _T("charmap-anisong.xml");
    config.OutputEncoding = O_UTF_8;
    config.TemplateStr = _T(".utf-8");
    config.WindowWidth = 793;
    config.WindowHeight = 717;
}
