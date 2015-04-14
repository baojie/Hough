@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by IMAGES.HPJ. >"hlp\ImageS.hm"
echo. >>"hlp\ImageS.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\ImageS.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\ImageS.hm"
echo. >>"hlp\ImageS.hm"
echo // Prompts (IDP_*) >>"hlp\ImageS.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\ImageS.hm"
echo. >>"hlp\ImageS.hm"
echo // Resources (IDR_*) >>"hlp\ImageS.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\ImageS.hm"
echo. >>"hlp\ImageS.hm"
echo // Dialogs (IDD_*) >>"hlp\ImageS.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\ImageS.hm"
echo. >>"hlp\ImageS.hm"
echo // Frame Controls (IDW_*) >>"hlp\ImageS.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\ImageS.hm"
REM -- Make help for Project IMAGES


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\ImageS.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\ImageS.hlp" goto :Error
if not exist "hlp\ImageS.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\ImageS.hlp" Debug
if exist Debug\nul copy "hlp\ImageS.cnt" Debug
if exist Release\nul copy "hlp\ImageS.hlp" Release
if exist Release\nul copy "hlp\ImageS.cnt" Release
echo.
goto :done

:Error
echo hlp\ImageS.hpj(1) : error: Problem encountered creating help file

:done
echo.
