Unicue
======

Tools for character encodings converting.

##How to build

The solution file(Unicue.sln) is for Visual C++ 2010.  
Make sure you have add [WTL](http://sourceforge.net/projects/wtl/files/WTL%208.1/)'s header files into including folder of VC++, or 
you should add path of WTL's header files into project.

#####If your Visual C++ 2010 is Express version, you need to own a copy of *ATL*.

You can get an old version of ATL from [Windows Driver Kit 7.1](http://www.microsoft.com/en-us/download/details.aspx?id=11800).  
After installing of Windows Driver Kit, you might find header files of ATL in such folder:

    D:\WinDDK\7600.16385.1\inc\atl71

You might find lib files of ATL in such folder:

    D:\WinDDK\7600.16385.1\lib\ATL\i386

Now, go to property page of project to add path of ATL:

    Configuration Properties -> VC++ Directories -> Include Directories
    Configuration Properties -> VC++ Directories -> Library Directories

But the version of ATL in WDK is too old to run the compiled exe in Windows7. It is recommended to get a copy as same as that in VC2010 Professional.
You may find it at [Visual Studio Ultimate trial version](http://download.microsoft.com/download/2/4/7/24733615-AA11-42E9-8883-E28CDCA88ED5/X16-42552VS2010UltimTrial1.iso).

If you are using Visual C++ is 2012 Express, you may get ATL library from [Visual Studio Professional 2012 90-day trial](http://www.microsoft.com/visualstudio/eng/downloads).

Good luck!
