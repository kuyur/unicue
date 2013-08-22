unicue
======

Tools for converting character encodings

##How to build

The solution file(Unicue.sln) is for Visual C++ 2010.

#####If your Visual C++ 2010 is Express version, you need to own a copy of *ATL*.

You can get ATL from [Windows Driver Kit](http://msdn.microsoft.com/en-us/library/windows/hardware/hh852362.aspx).  
After installing of Windows Driver Kit, you might find header files of ATL in such folder:

    D:\WinDDK\7600.16385.1\inc\atl71

You might find lib files of ATL in such folder:

    D:\WinDDK\7600.16385.1\lib\ATL\i386

Now, go to property page of project to change path of ATL:

    Configuration Properties -> VC++ Directories -> Include Directories
    Configuration Properties -> VC++ Directories -> Library Directories


#####If your Visual C++ 2010 is Professional version, you might need to remove needless ATL path.
(ATL has been included in Professional version.)

Go to property page of project to remvoe needless path of ATL:

    Configuration Properties -> VC++ Directories -> Include Directories
    Configuration Properties -> VC++ Directories -> Library Directories


