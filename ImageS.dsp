# Microsoft Developer Studio Project File - Name="ImageS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ImageS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImageS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageS.mak" CFG="ImageS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ImageS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 VIC32MS.LIB /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ImageS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Vctw32ms.lib Vic32ms.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ImageS - Win32 Release"
# Name "ImageS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Dibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Graydoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GrayDocS.cpp
# End Source File
# Begin Source File

SOURCE=.\Hough.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageApp.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageS.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageS.idl
# ADD MTL /h "ImageS_i.h" /iid "ImageS_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\ImageS.rc
# End Source File
# Begin Source File

SOURCE=.\ImageView.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgDocS.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgVwS.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Plugins.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Rangeann.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UnRedoStack.cpp
# End Source File
# Begin Source File

SOURCE=.\VGrayPic.cpp
# End Source File
# Begin Source File

SOURCE=.\VicPict.cpp
# End Source File
# Begin Source File

SOURCE=.\VPicEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Dibapi.h
# End Source File
# Begin Source File

SOURCE=.\Graydoc.h
# End Source File
# Begin Source File

SOURCE=.\GrayDocS.h
# End Source File
# Begin Source File

SOURCE=.\Hough.h
# End Source File
# Begin Source File

SOURCE=.\ImageApp.h
# End Source File
# Begin Source File

SOURCE=.\ImageDoc.h
# End Source File
# Begin Source File

SOURCE=.\ImageRegion.h
# End Source File
# Begin Source File

SOURCE=.\ImageS.h
# End Source File
# Begin Source File

SOURCE=.\ImageView.h
# End Source File
# Begin Source File

SOURCE=.\ImgDocS.h
# End Source File
# Begin Source File

SOURCE=.\ImgVwS.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Plugins.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\Rangeann.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "ImageS - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=ImageS
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ImageS - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=ImageS
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnRedoStack.h
# End Source File
# Begin Source File

SOURCE=.\VGrayPic.h
# End Source File
# Begin Source File

SOURCE=.\VicPict.h
# End Source File
# Begin Source File

SOURCE=.\VPicEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImageS.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImageS.rc2
# End Source File
# Begin Source File

SOURCE=.\ImageS.rgs
# End Source File
# Begin Source File

SOURCE=.\res\ImgDocS.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ImageAReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ImageS.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ImageS : {A16BC4A6-EA0B-11D2-8C9E-C437F6C00000}
# 	1:9:IDD_RANGE:102
# 	2:16:Resource Include:resource.h
# 	2:10:Rangeann.h:Rangeann.h
# 	2:9:IDD_RANGE:IDD_RANGE
# 	2:12:Rangeann.cpp:Rangeann.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CRangeDlg:CRangeDlg
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A1-EA0B-11D2-8C9E-C437F6C00000}
# 	2:10:ImageApp.h:ImageApp.h
# 	2:12:ImageApp.cpp:ImageApp.cpp
# 	2:19:Application Include:ImageS.h
# 	2:16:CLASS: CImageApp:CImageApp
# End Section
# Section ImageS : {A16BC4A5-EA0B-11D2-8C9E-C437F6C00000}
# 	1:15:CG_IDD_PROGRESS:103
# 	2:16:Resource Include:resource.h
# 	2:9:ProgDlg.h:ProgDlg.h
# 	2:19:CLASS: CProgressDlg:CProgressDlg
# 	2:11:ProgDlg.cpp:ProgDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:ImageS.h
# 	2:15:CG_IDD_PROGRESS:CG_IDD_PROGRESS
# End Section
# Section ImageS : {68720681-EA67-11D2-8C9E-0439F6C00000}
# 	2:15:CLASS: CGrayDoc:CGrayDoc
# 	2:11:Graydoc.cpp:Graydoc.cpp
# 	2:9:Graydoc.h:Graydoc.h
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A9-EA0B-11D2-8C9E-C437F6C00000}
# 	2:8:VPicEx.h:VPicEx.h
# 	2:13:CLASS: VPicEx:VPicEx
# 	2:10:VPicEx.cpp:VPicEx.cpp
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A4-EA0B-11D2-8C9E-C437F6C00000}
# 	2:17:CLASS: CImageView:CImageView
# 	2:11:ImageView.h:ImageView.h
# 	2:19:ENUM: MouseFunction:MouseFunction
# 	2:10:ENUM: enum:enum
# 	2:19:TYPEDEF: ImageEntry:ImageEntry
# 	2:21:TYPEDEF: LPImageEntry:LPImageEntry
# 	2:13:ImageView.cpp:ImageView.cpp
# 	2:29:ENUM: RgnMarkingModeConstants:RgnMarkingModeConstants
# 	2:20:CLASS: tagImageEntry:tagImageEntry
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {0000162C-1638-0000-8B00-00002C160000}
# 	1:10:IDR_IMAGES:105
# End Section
# Section ImageS : {A16BC4A8-EA0B-11D2-8C9E-C437F6C00000}
# 	2:9:VicPict.h:VicPict.h
# 	2:14:TYPEDEF: pVPic:pVPic
# 	2:11:CLASS: VPic:VPic
# 	2:18:CLASS: VPicStgInfo:VPicStgInfo
# 	2:16:TYPEDEF: pimgdes:pimgdes
# 	2:11:VicPict.cpp:VicPict.cpp
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A3-EA0B-11D2-8C9E-C437F6C00000}
# 	2:15:ImageRegion.cpp:ImageRegion.cpp
# 	2:13:ImageRegion.h:ImageRegion.h
# 	2:19:CLASS: CImageRegion:CImageRegion
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A7-EA0B-11D2-8C9E-C437F6C00000}
# 	2:19:CLASS: CUnRedoStack:CUnRedoStack
# 	2:15:UnRedoStack.cpp:UnRedoStack.cpp
# 	2:13:UnRedoStack.h:UnRedoStack.h
# 	2:19:Application Include:ImageS.h
# End Section
# Section ImageS : {A16BC4A2-EA0B-11D2-8C9E-C437F6C00000}
# 	2:12:ImageDoc.cpp:ImageDoc.cpp
# 	2:16:CLASS: CImageDoc:CImageDoc
# 	2:19:Application Include:ImageS.h
# 	2:10:ImageDoc.h:ImageDoc.h
# End Section
