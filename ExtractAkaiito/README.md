ExtractAkaiito
==============

#Usage

Copy `SCRIPT.AFS` to the folder of `ExtractAkaiito.exe`, or copy `ExtractAkaiito.exe` to the folder of `SCRIPT.AFS`, as you like.

Copy `jis2u-little-endian.map` to the folder above.

Run commands as following in order:

    ExtractAkaiito.exe -step1
    ExtractAkaiito.exe -step2
    ExtractAkaiito.exe -step3-1
    ExtractAkaiito.exe -step3-2

You will get two files: `akaiito_script_step3-1.txt` and `akaiito_script_step3-2.txt`.

You can use Unicue to convert them into UTF-8 and merge the txt files.

Some characters are obviously not words of scripts. It is very easy to remove them. Now you get a readable text with lots of numbers which may have some unknown meanings. It is recommended to use Replacing Function(Regular expression) of Notepad++ to deal with those numbers. 

You will finally get the clean scripts of Akaiito, just as this one: 
[akaiito\_script\_step4.pdf](http://kuyur.net/blog/uploads/2010/08/akaiito_script_step4.pdf)
