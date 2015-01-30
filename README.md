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

ANSI encoding actually is not a single encoding, but a set of encodings defined by various countries as national standard to support non-ASCII characters of local languages. The word of **ANSI** is not accurate， because it is abbreviation of **A**merican **N**ational **S**tandards **I**nstitute. ANSI encoding is used by [Microsoft](http://support.microsoft.com/kb/138813/en-us) first. The right name should be Regional Variable-width encoding (RVW encoding) or pre-Unicode encoding.

###Unicue Traveller
Tool to convert bunch of text files at a time.

###Chinese Converter
Tool to convert Simplified Chinese text into Traditional Chinese text or convert back.

###ExtractAkaiito
A little program to extract readable paragraph from script file of Akaiito, which is a PS2 Game published in 2004.

###Traveller Ext
A system extension to register Unicue Traveller entry into context menu.

##Feedback and Bug Report
If you find any bug or require some new feature, please report it at [Issues board](https://github.com/kuyur/unicue/issues).  
If you find any text file can not be converted by Unicue, please send it to mail:unicue{at}kuyur.info (please replace **{at}** with **@**, and put **[Report Unknown Encoding]** at the head of mail title).

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

所有的程序都不采用Windows系统的代码页，而是内建自定义字符映射表。这使得UniCue的工具可以转换非官方定义但是又经常被使用的用户扩展字符，比如UAO2.50中定义的简体汉字和日文汉字。  
有关码表的详细说明，请参阅[字符映射表说明](docs/en/aboutcharmap.md)。

目前UniCue包含四个程序。

###Unicue
用于转换单个文本编码的工具。输入编码是ANSI，输出编码是UTF-16或者UTF-8.

注：ANSI编码实际上不是一种编码，而是各个国家/地区为了支持本地非ASCII字符而制定的编码标准的集合。"ANSI编码"这个名词最先被[微软](http://support.microsoft.com/kb/138813/en-us)使用，但它是一个古怪的叫法，因为ANSI是美国国家标准协会(**A**merican **N**ational **S**tandards **I**nstitute）)的缩写。准确的叫法应该是区域性的可变长字符编码(Regional Variable-width encoding或者RVW encoding)或者更简单的Unicode史前编码(Pre-Unicode encoding)。

###Unicue Traveller
用于批量转换文本文件编码的工具。

###Chinese Converter
简繁繁简转换工具。

###ExtractAkaiito
提取PS2游戏红线(アカイイト)脚本的小工具。

###Traveller Ext
将Traveller注册到右键菜单的系统扩展。

##反馈以及Bug报告
如发现任何bug或者有新的特性需求，请前往[Issues board](https://github.com/kuyur/unicue/issues)报告。  
如遇到任何Unicue无法转换的文本文件，欢迎附加到附件然后扔到邮箱：unicue{at}kuyur.info （请将**{at}**替换成**@**，标题请注明【报告乱码文件】）。

##文档
1. [版本升级日志](docs/chs/changelog.md)
2. [编译要点](docs/chs/howtobuild.md)
3. [字符映射表说明](docs/chs/aboutcharmap.md)
4. [有关Unicue的注册表项](docs/chs/registry.md)
