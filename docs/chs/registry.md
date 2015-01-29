

1.0.3版起可以注册到文件右键菜单，程序会写如下注册表项：

[HKEY_CLASSES_ROOT\.uni]
@="UniCue.UNI"

[HKEY_CLASSES_ROOT\UniCue.UNI]
@="UniCue 文件"

[HKEY_CLASSES_ROOT\UniCue.UNI\shell]
@="Open"

[HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open]
@="使用 UniCue 打开"

'AppPathName为程序路径，如E:\\Program Files (x86)\\UniCue\\Ansi2Unicode.exe
[HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open\command]
@="\"AppPathName\" \"%1\""

[HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue]
@="使用 UniCue 转换编码"

[HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue\command]
@="\"AppPathName\" \"%1\""

[HKEY_CLASSES_ROOT\txtfile\shell\unicue]
@="使用 UniCue 转换编码"

[HKEY_CLASSES_ROOT\txtfile\shell\unicue\command]
@="\"AppPathName\" \"%1\""

'程序会查找cue文件的注册表信息，假定cue已经关联到foobar2000.CUE。如果cue文件类型的信息不存在，会将cue关联到UniCue.UNI
[HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue]
@="使用 UniCue 转换编码"

[HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue\command]
@="\"AppPathName\" \"%1\""

[HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue]
@="使用 UniCue 转换编码"

[HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue\command]
@="\"AppPathName\" \"%1\""

[HKEY_CLASSES_ROOT\Applications\ANSI2Unicode.exe\shell\open\command]
@="\"AppPathName\" \"%1\""

'AppFolder为程序目录，如E:\\Program Files (x86)\\UniCue
[HKEY_CLASSES_ROOT\UniCue.UNI\DefaultIcon]
@="AppFolder\\icons\\file.ico"

1.0.3版起可以将新建文本文档(txt)由系统默认模板变更为带utf-8 BOM标记的模板。程序会更改下面的注册表项：

删除[HKEY_CLASSES_ROOT\.txt\ShellNew]下的 NullFile 键值

写键值：（AppFolder为程序所在目录）
[HKEY_CLASSES_ROOT\.txt\ShellNew]
"FileName"="AppFolder\\null.uni"

win7或vista系统在更改注册表时需要管理员权限
