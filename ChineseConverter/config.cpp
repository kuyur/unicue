/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.3                                                 */
/* A tool to convert Simplified Chinese into Traditional Chinese and    */
/* convert back. It is a part of Unicue Project.                        */
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

    // config node
    pElem=hRoot.FirstChildElement().Element();
    if (!pElem) return FALSE;
    if (strcmp(pElem->Value(),"config")!=0)
        return FALSE;

    // template-string node
    hXmlHandle=TiXmlHandle(pElem);
    pElem=hXmlHandle.FirstChild("template-string").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.TemplateStr = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    // charmap-config-path node
    pElem=hXmlHandle.FirstChild("charmap-config-path").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.MapConfName = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    // language node
    pElem = hXmlHandle.FirstChild("language").Element();
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

    // current-choice node
    config.CurrentChoice = -1; // unknow
    pElem = hXmlHandle.FirstChild("current-choice").Element();
    if (pElem && pElem->GetText())
    {
        char *p;
        LONG choice = (LONG)strtol(pElem->GetText(), &p, 10);
        if (*p == 0)
        {
            config.CurrentChoice = choice;
        }
    }

    // output-encoding node
    pElem = hXmlHandle.FirstChild("output-encoding").Element();
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

    // window-width node
    pElem = hXmlHandle.FirstChild("window-width").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.WindowWidth = (LONG)strtol(pElem->GetText(), (char **)NULL, 10);

    // window-height node
    pElem = hXmlHandle.FirstChild("window-height").Element();
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

    TiXmlElement *TemplateStr=new TiXmlElement("template-string");
    std::string &UTF8Str = CC4EncodeUTF16::getInstance()->convertWideString(config.TemplateStr);
    TiXmlText *TemplateStrValue=new TiXmlText(UTF8Str.c_str());
    TemplateStr->LinkEndChild(TemplateStrValue);
    configure->LinkEndChild(TemplateStr);

    TiXmlElement *CharmapConfPath = new TiXmlElement("charmap-config-path");
    TiXmlText *CharmapConfPathValue;
    CharmapConfPathValue = new TiXmlText(CC4EncodeUTF16::getInstance()->convertWideString(config.MapConfName).c_str());
    CharmapConfPath->LinkEndChild(CharmapConfPathValue);
    configure->LinkEndChild(CharmapConfPath);

    TiXmlElement *Lang = new TiXmlElement("language");
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

    TiXmlElement *CurrentChoice = new TiXmlElement("current-choice");
    TiXmlText *CurrentChoiceValue = new TiXmlText(toSTLString(config.CurrentChoice).c_str());
    CurrentChoice->LinkEndChild(CurrentChoiceValue);
    configure->LinkEndChild(CurrentChoice);

    TiXmlElement *Output = new TiXmlElement("output-encoding");
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

    TiXmlElement *WindowWidth = new TiXmlElement("window-width");
    TiXmlText *WindowWidthValue = new TiXmlText(toSTLString(config.WindowWidth).c_str());
    WindowWidth->LinkEndChild(WindowWidthValue);
    configure->LinkEndChild(WindowWidth);

    TiXmlElement *WindowHeight = new TiXmlElement("window-height");
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
    config.Lang = CHN;
    config.CurrentChoice = -1;
    config.MapConfName = L"charmap-chinese.xml";
    config.OutputEncoding = O_UTF_8;
    config.TemplateStr = L".convert";
    config.WindowWidth = 793;
    config.WindowHeight = 717;
}
