// ExtractAkaiito.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\win32helper.h"
#include "..\common\winfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define JIS0201OFFSET     0xA1
#define JIS0201LENGTH     63
#define JIS0208OFFSET     0x8140

// 筛选有效文本字符
/*
ASCII（0x00～0x7F）中剔除控制符后有效的文本字符范围为：0x20～0x7E
半角片假名区：0xA1～0xDF
双字节区：0x8140～0xFFFF，其中未定义字符在jis2u-little-endian.map中已映射到unicode的0xFFFD
*/
UINT ReplaceDumpChar(const char* src, char* dst, UINT length)
{
    if (!src)
        return 0;
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    WTL::CString mapPath(GetProcessFolder());
    mapPath += _T("jis2u-little-endian.map");

    //加载映射表
    CWinFile loadMap(mapPath, CWinFile::modeRead|CWinFile::shareDenyWrite);
    if (!loadMap.open())
    {
        loadMap.close();
        ::MessageBox(NULL, _T("JIStoUnicode map loading error!"), _T("ExtractAkaiito"), MB_OK);
        return 0;
    }
    UINT mapLength=loadMap.length();
    char *mapBuffer=new char[mapLength];
    loadMap.read(mapBuffer,mapLength);
    loadMap.close();

    unsigned char low=0,high=0;
    WCHAR chr=0,UnknownChr=0xFFFD;
    int ReplaceChr=0x20;
    UINT j;
    for (UINT i=0;i<length;)
    {
        //Shift-JIS码的双字节字符是先高位后低位
        //高位至少从0x81起
        //jisChr[1]：高位
        //jisChr[0]：低位
        memcpy(&high,src+i,1); //读取第一个byte
        j=i;
        i++;
        if (high<=0x7F)  //ASCII码区
        {
            low=high;
            high=0;
        }
        else if ((high>=0xA1)&&(high<=0xDF))  //半角片假名区
        {
            low=high;
            high=0;
        }
        else  //双字节区
        {
            memcpy(&low,src+i,1); //读取低位
            i++;
        }

        chr=low+high*256;
        if (chr<0x80) // ASCII
        {
            if ((chr>=0x20)&&(chr<=0x7E))
                memcpy(dst+j,src+j,1);
            else
                memset(dst+j,ReplaceChr,1);
        }
        else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 未定义空间
        {
            memset(dst+j,ReplaceChr,1);
        }
        else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF 半角假名区
        {
            //memcpy(dst+j,src+j,1);
            memset(dst+j,ReplaceChr,1); //应该不存在这些字符
        }
        else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F 未定义空间
        {
            if (i==(j+2))
                memset(dst+j,ReplaceChr,2);
            else
                memset(dst+j,ReplaceChr,1);
        }
        else // 0x8140 - 0xFFFF
        {
            int offset;
            offset=chr-JIS0208OFFSET+JIS0201LENGTH;
            memcpy((void*)&chr,mapBuffer+offset*2,2);
            if (chr==0xFFFD)
                memset(dst+j,ReplaceChr,2);
            else
                memcpy(dst+j,src+j,2);
        }
    }

    delete []mapBuffer;
    mapBuffer=NULL;

    return length;
}

//删除干扰字符 $w $s 等
UINT DeleteDollarChar(const char* src, char* dst, UINT length)
{
    if (!src)
        return 0;
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    UINT j=0;
    char chr;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,src+i++,1);
        //捕捉$
        if (chr==0x24)
        {
            memcpy(&chr,src+i++,1);
            if ((chr>=0x61)&&(chr<=0x7a))
            {
                //捕捉成功
                memset(dst+j,0x20,1);
                j++;
            }
            else
            {
                memcpy(dst+j,src+i-2,2);
                j=j+2;
            }
        }
        else
        {
            memcpy(dst+j,src+i-1,1);
            j=j+1;
        }
    }

    return j;
}

//删除干扰字符(单个ascii字符,0x21~0x7E)
UINT DeleteSingleChar(char* dst, UINT length)
{
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    char chr;
    int state=0;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,dst+i++,1);
        //捕捉空格,自动机模型
        if (chr==0x20)
        {
            state=1;
            continue;
        }

        if ((state==1)&&(chr>=0x21)&&(chr<=0x7e))
            state=2;
        else
            state=0;

        if (state==2)
        {
            if (dst[i]==0x20)
            {
                memset(dst+i-1,0x20,1);
                state=1;
            }
            else
                state=0;
        }
    }

    return length;
}

//替换回车字符 #cr0 为换行
UINT ReplaceCR(const char* src, char* dst, UINT length)
{
    if (!src)
        return 0;
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    UINT j=0;
    char chr;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,src+i++,1);
        //捕捉#cr0
        if (chr==0x23)
        {
            memcpy(&chr,src+i++,1);
            if (chr==0x63)
            {
                memcpy(&chr,src+i++,1);
                if (chr==0x72)
                {
                    memcpy(&chr,src+i++,1);
                    if (chr==0x30)
                    {
                        //捕捉成功
                        memset(dst+j,0x0D,1);
                        j++;
                        memset(dst+j,0x0A,1);
                        j++;
                    }
                    else
                    {
                        memcpy(dst+j,src+i-4,4);
                        j=j+4;
                    }
                }
                else
                {
                    memcpy(dst+j,src+i-3,3);
                    j=j+3;
                }
            }
            else
            {
                memcpy(dst+j,src+i-2,2);
                j=j+2;
            }
        }
        else
        {
            memcpy(dst+j,src+i-1,1);
            j=j+1;
        }
    }

    return j;
}

//删除回车字符 #cr0
UINT DeleteCR(const char* src,char* dst,UINT length)
{
    if (!src)
        return 0;
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    UINT j=0;
    char chr;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,src+i++,1);
        //捕捉#cr0
        if (chr==0x23)
        {
            memcpy(&chr,src+i++,1);
            if (chr==0x63)
            {
                memcpy(&chr,src+i++,1);
                if (chr==0x72)
                {
                    memcpy(&chr,src+i++,1);
                    if (chr==0x30)
                    {
                        //捕捉成功
                        //什么也不用干
                    }
                    else
                    {
                        memcpy(dst+j,src+i-4,4);
                        j=j+4;
                    }
                }
                else
                {
                    memcpy(dst+j,src+i-3,3);
                    j=j+3;
                }
            }
            else
            {
                memcpy(dst+j,src+i-2,2);
                j=j+2;
            }
        }
        else
        {
            memcpy(dst+j,src+i-1,1);
            j=j+1;
        }
    }

    return j;
}

//替换连续数字
UINT DeleteNumbers(char* dst, UINT length)
{
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    char chr;
    int state=0;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,dst+i++,1);
        //捕捉空格,自动机模型
    }

    return length;
}

//长空格串替换为换行（三个空格以上）
UINT ReplaceSpaceChar(const char* src, char* dst, UINT length)
{
    if (!src)
        return 0;
    if (!dst)
        return 0;
    if (length==0)
        return 0;

    UINT j=0;
    char chr;
    int state=0;
    for (UINT i=0;i<length;)
    {
        memcpy(&chr,src+i++,1);
        if (chr!=0x20)
        {
            if (state==1)
            {
                memset(dst+j++,0x20,1);
            }
            if (state==2)
            {
                memset(dst+j++,0x20,1);
                memset(dst+j++,0x20,1);
            }
            if (state==3)
            {
                memset(dst+j++,0x0D,1);
                memset(dst+j++,0x0A,1);
            }
            state=0;
            memcpy(dst+j++,src+i-1,1);
        }
        else
        {
            if (state==0)
            {
                state=1; //一个空格
                continue;
            }
            if (state==1)
            {
                state=2; //二个空格
                continue;
            }
            if (state==2)
            {
                state=3; //三个空格，可以准备替换
                continue;
            }
            if (state==3)
            {
                state=3;
                continue;
            }
        }
    }

    return j;
}

int _tmain(int argc, TCHAR* argv[])
{
    int nRetCode = 0;

    if (argc>=2)
    {
        //第一步：筛选有效字符
        if (wcscmp(argv[1],L"-step1")==0)
        {
            CWinFile OpenFile(_T("SCRIPT.AFS"),CWinFile::modeRead|CWinFile::shareDenyWrite);
            if (!OpenFile.open())
            {
                OpenFile.close();
                ::MessageBox(NULL, _T("打开失败，请检查SCRIPT.AFS是否已放置在程序目录"), _T("ExtractAkaiito"), MB_OK);
                return 0;
            }
            UINT length;
            length=OpenFile.length();
            char *akaiito_script; // 注意刚开始时不能当作字符串来处理，原始文件含有大量终止符
            akaiito_script=new char[length];
            OpenFile.read(akaiito_script,length);
            OpenFile.close();
            char *akaiito_script_dealing;
            akaiito_script_dealing=new char[length];
            memset((void*)akaiito_script_dealing,0,length);
            ReplaceDumpChar(akaiito_script,akaiito_script_dealing,length);

            CWinFile SaveFile(_T("akaiito_script_step1.txt"),CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
            if (!SaveFile.open())
            {
                ::MessageBox(NULL, _T("无法写入文件！"), _T("ExtractAkaiito"), MB_OK);
                if (!akaiito_script)
                    delete []akaiito_script;
                akaiito_script=NULL;
                if (!akaiito_script_dealing)
                    delete []akaiito_script_dealing;
                akaiito_script_dealing=NULL;
                return 0;
            }
            SaveFile.write(akaiito_script_dealing,length);
            SaveFile.close();

            if (!akaiito_script)
                delete []akaiito_script;
            akaiito_script=NULL;
            if (!akaiito_script_dealing)
                delete []akaiito_script_dealing;
            akaiito_script_dealing=NULL;
        }
        //第二步：
        if (wcscmp(argv[1],L"-step2")==0)
        {
            CWinFile OpenFile(_T("akaiito_script_step1.txt"),CWinFile::modeRead|CWinFile::shareDenyWrite);
            if (!OpenFile.open())
            {
                OpenFile.close();
                ::MessageBox(NULL, _T("打开失败，请检查akaiito_script_step1.txt是否程序目录下已存在"), _T("ExtractAkaiito"), MB_OK);
                return 0;
            }
            UINT length;
            length=OpenFile.length();
            char *akaiito_script; // 第二步开始已经可以当作字符串来处理
            akaiito_script=new char[length];
            OpenFile.read(akaiito_script,length);
            OpenFile.close();
            char *akaiito_script_dealing;
            akaiito_script_dealing=new char[length];

            memset((void*)akaiito_script_dealing,0,length);
            length=DeleteDollarChar(akaiito_script,akaiito_script_dealing,length);
            length=DeleteSingleChar(akaiito_script_dealing,length);

            UINT titleLength = 107186;
            CWinFile SaveFile1(_T("akaiito_script_step2-1.txt"),CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
            if (!SaveFile1.open())
            {
                ::MessageBox(NULL, _T("无法写入文件！"), _T("ExtractAkaiito"), MB_OK);
                if (!akaiito_script)
                    delete []akaiito_script;
                akaiito_script=NULL;
                if (!akaiito_script_dealing)
                    delete []akaiito_script_dealing;
                akaiito_script_dealing=NULL;
                return 0;
            }
            SaveFile1.write(akaiito_script_dealing,titleLength);
            SaveFile1.close();

            CWinFile SaveFile2(_T("akaiito_script_step2-2.txt"),CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
            if (!SaveFile2.open())
            {
                ::MessageBox(NULL, _T("无法写入文件！"), _T("ExtractAkaiito"), MB_OK);
                if (!akaiito_script)
                    delete []akaiito_script;
                akaiito_script=NULL;
                if (!akaiito_script_dealing)
                    delete []akaiito_script_dealing;
                akaiito_script_dealing=NULL;
                return 0;
            }
            SaveFile2.write(akaiito_script_dealing+titleLength,length - titleLength);
            SaveFile2.close();

            if (!akaiito_script)
                delete []akaiito_script;
            akaiito_script=NULL;
            if (!akaiito_script_dealing)
                delete []akaiito_script_dealing;
            akaiito_script_dealing=NULL;
        }
        if (wcscmp(argv[1],L"-step3-1")==0)
        {
            CWinFile OpenFile(_T("akaiito_script_step2-1.txt"),CWinFile::modeRead|CWinFile::shareDenyWrite);
            if (!OpenFile.open())
            {
                OpenFile.close();
                ::MessageBox(NULL, _T("打开失败，请检查akaiito_script_step2-1.txt是否程序目录下已存在"), _T("ExtractAkaiito"), MB_OK);
                return 0;
            }
            UINT length;
            length=OpenFile.length();
            char *akaiito_script; // 可以当作字符串来处理
            akaiito_script=new char[length];
            OpenFile.read(akaiito_script,length);
            OpenFile.close();
            char *akaiito_script_dealing;
            akaiito_script_dealing=new char[length];

            memset((void*)akaiito_script_dealing,0,length);
            length=ReplaceCR(akaiito_script,akaiito_script_dealing,length);
            memset((void*)akaiito_script,0,length);
            length=ReplaceSpaceChar(akaiito_script_dealing,akaiito_script,length);

            CWinFile SaveFile(_T("akaiito_script_step3-1.txt"),CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
            if (!SaveFile.open())
            {
                ::MessageBox(NULL, _T("无法写入文件！"), _T("ExtractAkaiito"), MB_OK);
                if (!akaiito_script)
                    delete []akaiito_script;
                akaiito_script=NULL;
                if (!akaiito_script_dealing)
                    delete []akaiito_script_dealing;
                akaiito_script_dealing=NULL;
                return 0;
            }
            SaveFile.write(akaiito_script,length);
            SaveFile.close();

            if (!akaiito_script)
                delete []akaiito_script;
            akaiito_script=NULL;
            if (!akaiito_script_dealing)
                delete []akaiito_script_dealing;
            akaiito_script_dealing=NULL;
        }
        if (wcscmp(argv[1],L"-step3-2")==0)
        {
            CWinFile OpenFile(_T("akaiito_script_step2-2.txt"),CWinFile::modeRead|CWinFile::shareDenyWrite);
            if (!OpenFile.open())
            {
                OpenFile.close();
                ::MessageBox(NULL, _T("打开失败，请检查akaiito_script_step2-2.txt是否程序目录下已存在"), _T("ExtractAkaiito"), MB_OK);
                return 0;
            }
            UINT length;
            length=OpenFile.length();
            char *akaiito_script; // 可以当作字符串来处理
            akaiito_script=new char[length];
            OpenFile.read(akaiito_script,length);
            OpenFile.close();
            char *akaiito_script_dealing;
            akaiito_script_dealing=new char[length];

            memset((void*)akaiito_script_dealing,0,length);
            length=DeleteCR(akaiito_script,akaiito_script_dealing,length);
            memset((void*)akaiito_script,0,length);
            length=ReplaceSpaceChar(akaiito_script_dealing,akaiito_script,length);

            CWinFile SaveFile(_T("akaiito_script_step3-2.txt"),CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
            if (!SaveFile.open())
            {
                ::MessageBox(NULL, _T("无法写入文件！"), _T("ExtractAkaiito"), MB_OK);
                if (!akaiito_script)
                    delete []akaiito_script;
                akaiito_script=NULL;
                if (!akaiito_script_dealing)
                    delete []akaiito_script_dealing;
                akaiito_script_dealing=NULL;
                return 0;
            }
            SaveFile.write(akaiito_script,length);
            SaveFile.close();

            if (!akaiito_script)
                delete []akaiito_script;
            akaiito_script=NULL;
            if (!akaiito_script_dealing)
                delete []akaiito_script_dealing;
            akaiito_script_dealing=NULL;
        }
    }

    return nRetCode;
}
