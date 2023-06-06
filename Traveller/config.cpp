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

#include "config.h"
#include "..\c4-lib\c4encode.h"
#include "..\common\winfile.h"

BOOL LoadConfigFile(TiXmlDocument *xmlfile, CConfig &config)
{
    TiXmlHandle hRoot(xmlfile);
    TiXmlElement *pElem;
    TiXmlHandle hXmlHandle(0);

    // config node
    pElem = hRoot.FirstChildElement().Element();
    if (!pElem) return FALSE;
    if (strcmp(pElem->Value(), "config") != 0) return FALSE;
    hXmlHandle = TiXmlHandle(pElem);

    // IgnoreUtf8 node
    pElem = hXmlHandle.FirstChild("IgnoreUtf8").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isIgnoreUtf8 = TRUE;
    else
        config.isIgnoreUtf8 = FALSE;

    // IgnoreUtf8WithoutBom node
    pElem = hXmlHandle.FirstChild("IgnoreUtf8WithoutBom").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isIgnoreUtf8WithoutBom = TRUE;
    else
        config.isIgnoreUtf8WithoutBom = FALSE;

    // IgnoreHidden node
    pElem = hXmlHandle.FirstChild("IgnoreHidden").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isIgnoreHidden = TRUE;
    else
        config.isIgnoreHidden = FALSE;

    // Override node
    pElem = hXmlHandle.FirstChild("Override").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isOverride = TRUE;
    else
        config.isOverride = FALSE;

    // BackupOrig node
    pElem = hXmlHandle.FirstChild("BackupOrig").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isBackupOrig = TRUE;
    else
        config.isBackupOrig = FALSE;

    // AutoFixCueError node
    pElem = hXmlHandle.FirstChild("AutoFixCueError").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    if (_stricmp(pElem->GetText(),"true") == 0)
        config.isAutoFixCueError = TRUE;
    else
        config.isAutoFixCueError = FALSE;

    // Extensions node
    pElem = hXmlHandle.FirstChild("Extensions").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.extensions = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    // Template node
    pElem = hXmlHandle.FirstChild("Template").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.templateString = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    // CharmapConfig node
    pElem = hXmlHandle.FirstChild("CharmapConfig").Element();
    if (!pElem) return FALSE;
    if (!pElem->GetText()) return FALSE;
    config.charmapConfig = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

    return TRUE;
}

BOOL SaveConfigFile(LPCTSTR configPath, const CConfig &config)
{
    TiXmlDocument configdoc;
    TiXmlDeclaration *dec = new TiXmlDeclaration("1.0","utf-8","");
    TiXmlElement *configure = new TiXmlElement("config");

    TiXmlElement *IgnoreUtf8 = new TiXmlElement("IgnoreUtf8");
    TiXmlText *IgnoreUtf8Value;
    if (config.isIgnoreUtf8)
        IgnoreUtf8Value = new TiXmlText("true");
    else
        IgnoreUtf8Value = new TiXmlText("false");
    IgnoreUtf8->LinkEndChild(IgnoreUtf8Value);
    configure->LinkEndChild(IgnoreUtf8);

    TiXmlElement *IgnoreUtf8WithoutBom = new TiXmlElement("IgnoreUtf8WithoutBom");
    TiXmlText *IgnoreUtf8WithoutBomValue;
    if (config.isIgnoreUtf8WithoutBom)
        IgnoreUtf8WithoutBomValue = new TiXmlText("true");
    else
        IgnoreUtf8WithoutBomValue = new TiXmlText("false");
    IgnoreUtf8WithoutBom->LinkEndChild(IgnoreUtf8WithoutBomValue);
    configure->LinkEndChild(IgnoreUtf8WithoutBom);

    TiXmlElement *IgnoreHidden = new TiXmlElement("IgnoreHidden");
    TiXmlText *IgnoreHiddenValue;
    if (config.isIgnoreHidden)
        IgnoreHiddenValue = new TiXmlText("true");
    else
        IgnoreHiddenValue = new TiXmlText("false");
    IgnoreHidden->LinkEndChild(IgnoreHiddenValue);
    configure->LinkEndChild(IgnoreHidden);

    TiXmlElement *Override = new TiXmlElement("Override");
    TiXmlText *OverrideValue;
    if (config.isOverride)
        OverrideValue = new TiXmlText("true");
    else
        OverrideValue = new TiXmlText("false");
    Override->LinkEndChild(OverrideValue);
    configure->LinkEndChild(Override);

    TiXmlElement *BackupOrig = new TiXmlElement("BackupOrig");
    TiXmlText *BackupOrigValue;
    if (config.isBackupOrig)
        BackupOrigValue = new TiXmlText("true");
    else
        BackupOrigValue = new TiXmlText("false");
    BackupOrig->LinkEndChild(BackupOrigValue);
    configure->LinkEndChild(BackupOrig);

    TiXmlElement *AutoFixCueError = new TiXmlElement("AutoFixCueError");
    TiXmlText *AutoFixCueErrorValue;
    if (config.isAutoFixCueError)
        AutoFixCueErrorValue = new TiXmlText("true");
    else
        AutoFixCueErrorValue = new TiXmlText("false");
    AutoFixCueError->LinkEndChild(AutoFixCueErrorValue);
    configure->LinkEndChild(AutoFixCueError);

    TiXmlElement *Extensions = new TiXmlElement("Extensions");
    TiXmlText *ExtensionsValue = new TiXmlText(CC4EncodeUTF16::convert2utf8(config.extensions, config.extensions.GetLength()).c_str());
    Extensions->LinkEndChild(ExtensionsValue);
    configure->LinkEndChild(Extensions);

    TiXmlElement *Template = new TiXmlElement("Template");
    TiXmlText *TemplateValue = new TiXmlText(CC4EncodeUTF16::convert2utf8(config.templateString, config.templateString.GetLength()).c_str());
    Template->LinkEndChild(TemplateValue);
    configure->LinkEndChild(Template);

    TiXmlElement *CharmapConfig = new TiXmlElement("CharmapConfig");
    TiXmlText *CharmapConfigValue = new TiXmlText(CC4EncodeUTF16::convert2utf8(config.charmapConfig, config.charmapConfig.GetLength()).c_str());
    CharmapConfig->LinkEndChild(CharmapConfigValue);
    configure->LinkEndChild(CharmapConfig);

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
    config.isIgnoreUtf8 = TRUE;
    config.isIgnoreUtf8WithoutBom = FALSE;
    config.isIgnoreHidden = FALSE;
    config.extensions = L"cue";
    config.isOverride = TRUE;
    config.isBackupOrig = TRUE;
    config.templateString = L".utf-8";
    config.isAutoFixCueError = TRUE;
    config.charmapConfig = L"charmap-anisong.xml";
}
