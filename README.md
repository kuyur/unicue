UniCue
======

**UniCue** is an open source project which contains various tools for character encodings converting.  
Its name is composed by **Uni** and **cue**. **Uni** is short for Unicode when **cue** means cuesheet.  
**Unicue** is also the name of main program.

All programs are not using System CodePages of Windows platform but using customized charmaps to cover the unofficial extended code points, such as simplified Chinese characters/Japanese KANJI characters defined in UAO 2.50.  
The detail of customized charmaps is introduced in [aboutcharmap.md](docs/en/aboutcharmap.md)

Currently UniCue contains 4 programs and one extension dll.

###Unicue
Tool to convert a single text file from ANSI encoding to Unicode form (UTF-8 or UTF-16).

ANSI encoding actually is not a single encoding, but a set of encodings defined by various countries as national standard to support non-ASCII characters of local languages. The word of **ANSI** is not accurate. It is used by [Microsoft](http://support.microsoft.com/kb/138813/en-us) first. ANSI is abbreviation of **A**merican **N**ational **S**tandards **I**nstitute. The right name should be Regional Variable-width encoding (RVW encoding).

###Unicue Traveller
Tool to convert bunch of text files at a time.

###Chinese Converter
Tool to convert Simplified Chinese text into Traditional Chinese text or convert back.

###ExtractAkaiito
A little program to extract readable paragraph from script file of Akaiito, which is a PS2 Game published in 2004.

###Traveller Ext
A system extension to register Unicue Traveller entry at context menu.

##Documentation
1. [Change log](docs/en/changelog.md)
2. [How to build](docs/en/howtobuild.md)
3. [About charmaps](docs/en/aboutcharmap.md)
4. [About registry entry of Unicue](docs/en/registry.md)

***

Unicue 中文版说明
===========================

**UniCue** 是包含一系列的文字编码转换工具的开源项目。同时**Unicue**也是其中最主要的转换工具的名字。UniCue着重于将各种编码转换到Unicode。

名称来由：**Uni**代表Unicode，**Cue**代表cuesheet。 

所有的程序都不采用Windows系统的代码页，而是内建自定义码表。这使得UniCue的工具可以转换非官方定义但是又经常被使用的用户扩展字符，比如UAO2.50中定义的简体汉字和日文汉字。  
有关码表的详细说明，请参阅[aboutcharmap.md](docs/en/aboutcharmap.md)。

目前UniCue包含四个程序。

###Unicue
用于转换单个文本编码的工具。输入编码是ANSI，输出编码是UTF-16或者UTF-8.

注：ANSI编码实际上不是一种编码，而是各个国家/地区为了支持本地非ASCII字符而制定的编码标准的集合。ANSI编码是[微软](http://support.microsoft.com/kb/138813/en-us)的说法，尽管这个叫法非常古怪。ANSI是美国国家标准协会(**A**merican **N**ational **S**tandards **I**nstitute）)的缩写。准确的叫法应该是区域性的可变长字符编码(Regional Variable-width encoding或者RVW encoding)。

###Unicue Traveller
用于批量转换文本文件编码的工具。

###Chinese Converter
简繁繁简转换工具。

###ExtractAkaiito
提取PS2游戏红线(アカイイト)脚本的小工具。

###Traveller Ext
将Traveller注册到右键菜单的系统扩展。

##文档
1. [版本升级日志](docs/chs/changelog.md)
2. [编译要点](docs/chs/howtobuild.md)
3. [Charmap说明](docs/chs/aboutcharmap.md)
4. [有关Unicue的注册表项](docs/chs/registry.md)
