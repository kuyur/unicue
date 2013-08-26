Unicue
======

Tools for converting character encodings.

##How to build

The solution file(Unicue.sln) is for Visual C++ 2010.  
Make sure you have add [WTL](http://sourceforge.net/projects/wtl/files/WTL%208.1/)'s header files into including folder of VC++, or 
you should add path of WTL's header files into project.

#####If your Visual C++ 2010 is Express version, you need to own a copy of *ATL*.

You can get ATL from [Windows Driver Kit](http://msdn.microsoft.com/en-us/library/windows/hardware/hh852362.aspx).  
After installing of Windows Driver Kit, you might find header files of ATL in such folder:

    D:\WinDDK\7600.16385.1\inc\atl71

You might find lib files of ATL in such folder:

    D:\WinDDK\7600.16385.1\lib\ATL\i386

Now, go to property page of project to add path of ATL:

    Configuration Properties -> VC++ Directories -> Include Directories
    Configuration Properties -> VC++ Directories -> Library Directories

But the version of ATL in WDK is too old to run the compiled exe in Windows7.
It is recommended to get a copy as same as that in VC2010 Professional, no matter how you get.

Good luck!
