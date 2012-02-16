# Microsoft Developer Studio Project File - Name="zlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak" CFG="zlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zlib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zlib - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zlib - Win32 DLL Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zlib - Win32 LIB Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 LIB Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "zlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release"
# PROP BASE Intermediate_Dir "Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release"
# PROP Intermediate_Dir "Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /O2 /D "NO_DUMMY_DECL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386
# ADD LINK32 /dll /incremental:yes /machine:I386 /out:"Win32_Release\libzlib.dll"
# SUBTRACT LINK32 /profile /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "zlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug"
# PROP BASE Intermediate_Dir "Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug"
# PROP Intermediate_Dir "Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /D "NO_DUMMY_DECL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"Win32_Debug\libzlibd.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "zlib - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_DLL_Debug"
# PROP BASE Intermediate_Dir "Win32_DLL_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_DLL_Debug"
# PROP Intermediate_Dir "Win32_DLL_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /D "NO_DUMMY_DECL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"Win32_DLL_Debug\libzlibd.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "zlib - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_DLL_Release"
# PROP BASE Intermediate_Dir "Win32_DLL_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_DLL_Release"
# PROP Intermediate_Dir "Win32_DLL_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /O2 /D "NO_DUMMY_DECL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386
# ADD LINK32 /dll /incremental:yes /machine:I386 /out:"Win32_DLL_Release\libzlib.dll"
# SUBTRACT LINK32 /profile /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "zlib - Win32 LIB Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_LIB_Debug"
# PROP BASE Intermediate_Dir "Win32_LIB_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_LIB_Debug"
# PROP Intermediate_Dir "Win32_LIB_Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "NO_DUMMY_DECL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /GX /ZI /Od /D "NO_DUMMY_DECL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 LIB Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_LIB_Release"
# PROP BASE Intermediate_Dir "Win32_LIB_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_LIB_Release"
# PROP Intermediate_Dir "Win32_LIB_Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NO_DUMMY_DECL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /GX /O2 /D "NO_DUMMY_DECL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "zlib - Win32 Release"
# Name "zlib - Win32 Debug"
# Name "zlib - Win32 DLL Debug"
# Name "zlib - Win32 DLL Release"
# Name "zlib - Win32 LIB Debug"
# Name "zlib - Win32 LIB Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Adler32.c
# End Source File
# Begin Source File

SOURCE=.\Compress.c
# End Source File
# Begin Source File

SOURCE=.\Crc32.c
# End Source File
# Begin Source File

SOURCE=.\Deflate.c
# End Source File
# Begin Source File

SOURCE=.\Gzio.c
# End Source File
# Begin Source File

SOURCE=.\infback.c
# End Source File
# Begin Source File

SOURCE=.\Inffast.c
# End Source File
# Begin Source File

SOURCE=.\Inflate.c
# End Source File
# Begin Source File

SOURCE=.\Inftrees.c
# End Source File
# Begin Source File

SOURCE=.\Trees.c
# End Source File
# Begin Source File

SOURCE=.\Uncompr.c
# End Source File
# Begin Source File

SOURCE=.\Zutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\crc32.h
# End Source File
# Begin Source File

SOURCE=.\deflate.h
# End Source File
# Begin Source File

SOURCE=.\inffast.h
# End Source File
# Begin Source File

SOURCE=.\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\inflate.h
# End Source File
# Begin Source File

SOURCE=.\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\trees.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zutil.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\win32\zlib.def
# End Source File
# Begin Source File

SOURCE=.\zlib.ico
# End Source File
# Begin Source File

SOURCE=.\zlib1.rc
# End Source File
# End Group
# End Target
# End Project
