@echo off
@echo %PATH% | findstr /c:"%~dp0tools/msys64/usr/bin">nul
@if %errorlevel% equ 1 set PATH=%~dp0tools/msys64/usr/bin;%PATH%
rem @set PATH=%~dp0tools/msys64/usr/bin;%PATH%
rem cls

:: current path
set gb_current_path=%cd%
:: gccout path
set gb_gccout_path=%gb_current_path%\gccout
:: gcc tools path
set gb_tools_gcc_path=%gb_current_path%\tools\gcc

set PROJECT_NAME=EC618
set BOARD_NAME=ec618_0h00
set CHIP_NAME=ec618
set CORE_NAME=ap
set EUTRAN_MODE=cat_mode
set BUILD_OPTION=merge
set UNILOG=false
set TOOLCHAIN_NAME=GCC
set CUST=common
set SDKREL=false
set LSPD=false
set GCCLIB_PATH="%gb_tools_gcc_path%"

echo build.bat version 20180330
echo GCCLIB_PATH: %GCCLIB_PATH%


if not %GCCLIB_PATH% == "" (
   if not exist %GCCLIB_PATH% (
      echo ERROR: Please check GCCLIB_PATH setting, exit!!!s
      goto end
   )
) else (
	echo ERROR: Please set GCCLIB_PATH firstly, exit!!!
	goto end
)


set PARAMETERS=%1
if xx%PARAMETERS%==xx  (
echo no input paramter, use default build setting
echo default Board   is: %BOARD_NAME%
echo default Chip    is: %CHIP_NAME%
echo default Project is: %PROJECT_NAME%
echo default core    is: %CORE_NAME%
echo default option  is: %BUILD_OPTION%
)



rem --------- board/project parsing begain------------------


echo PARAMETERS: %PARAMETERS% | findstr "qcom"
if not errorlevel 1 (
   set CUST=qcom
)

echo PARAMETERS: %PARAMETERS% | findstr "ec618_0h00"
if not errorlevel 1 (
  set BOARD_NAME=ec618_0h00
  set CHIP_NAME=ec618
)

echo PARAMETERS: %PARAMETERS% | findstr "ap"
if not errorlevel 1 (
   set CORE_NAME=ap
   set BUILD_OPTION=no_merge
)

echo PARAMETERS: %PARAMETERS% | findstr "cp"
if not errorlevel 1 (
   set CORE_NAME=cp
   set PROJECT_NAME=cp_project
   set BUILD_OPTION=no_merge
)

echo PARAMETERS: %PARAMETERS% | findstr "nb_mode"
if not errorlevel 1 (
  set EUTRAN_MODE=nb_mode
)


echo PARAMETERS: %PARAMETERS% | findstr "at_command"
if not errorlevel 1 (
  set PROJECT_NAME=at_command
)

echo PARAMETERS: %PARAMETERS% | findstr "bootloader"
if not errorlevel 1 (
  set PROJECT_NAME=bootloader
  set BUILD_OPTION=no_merge
)


echo PARAMETERS: %PARAMETERS% | findstr "socket_example"
if not errorlevel 1 (
  set PROJECT_NAME=socket_example
)

echo PARAMETERS: %PARAMETERS% | findstr "https_example"
if not errorlevel 1 (
  set PROJECT_NAME=https_example
)

echo PARAMETERS: %PARAMETERS% | findstr "pos_example"
if not errorlevel 1 (
  set PROJECT_NAME=pos_example
)

echo PARAMETERS: %PARAMETERS% | findstr "libapi_example"
if not errorlevel 1 (
  set PROJECT_NAME=libapi_example
  set BUILD_OPTION=merge
)


echo PARAMETERS: %PARAMETERS% | findstr "mqtt_example"
if not errorlevel 1 (
  set PROJECT_NAME=mqtt_example
)

echo PARAMETERS: %PARAMETERS% | findstr "tts_example"
if not errorlevel 1 (
  set PROJECT_NAME=tts_example
)

echo PARAMETERS: %PARAMETERS% | findstr "lpuart_test"
if not errorlevel 1 (
  set PROJECT_NAME=lpuart_test
)

echo PARAMETERS: %PARAMETERS% | findstr "driver_example"
if not errorlevel 1 (
  set PROJECT_NAME=driver_example
)

echo PARAMETERS: %PARAMETERS% | findstr "dhrystone"
if not errorlevel 1 (
  set PROJECT_NAME=dhrystone
)

echo PARAMETERS: %PARAMETERS% | findstr "coremark"
if not errorlevel 1 (
  set PROJECT_NAME=coremark
)

echo PARAMETERS: %PARAMETERS% | findstr "slpman_example"
if not errorlevel 1 (
  set PROJECT_NAME=slpman_example
)

echo PARAMETERS: %PARAMETERS% | findstr "sleep_example"
if not errorlevel 1 (
  set PROJECT_NAME=sleep_example
)

echo PARAMETERS: %PARAMETERS% | findstr "mcu_example"
if not errorlevel 1 (
  set PROJECT_NAME=mcu_example
)

echo PARAMETERS: %PARAMETERS% | findstr "merge"
if not errorlevel 1 (
  set BUILD_OPTION=merge
)

echo PARAMETERS: %PARAMETERS% | findstr "sdkrel"
if not errorlevel 1 (
  set SDKREL=true
)


echo PARAMETERS: %PARAMETERS% | findstr "lspd_only"
if not errorlevel 1 (
  set LSPD=true
)


if "%CUST%"=="qcom" (
rem  clear AT ref for  qcom
for /f "tokens=1* delims=" %%a in ('type ".\device\target\board\%BOARD_NAME%\ap\%BOARD_NAME%_ap.mk"') do (
	if "%%a" equ "PS_AT_REF=y" (
		echo PS_AT_REF=n
	) else if "%%a" equ "BUILD_AT_REF=y" (
		echo BUILD_AT_REF=n
	) else (
		echo %%a
	)

)>>temp.txt

type temp.txt | findstr /v ECHO >>temp1.txt
del temp.txt
move temp1.txt ".\device\target\board\%BOARD_NAME%\ap\%BOARD_NAME%_ap.mk" >nul
)


rem --------- if more board/project add here-------------
rem --------- board/project parsing end------------------


echo PARAMETERS: %PARAMETERS% | findstr "help"
if not errorlevel 1 (
echo "=========================================================================================="
echo "                                                                                          "
echo "---------------------------Show How to Build Project--------------------------------------"
echo "                                                                                          "
echo "==========================================================================================¡±
echo "                                                                                          "
echo "                                                                                          "    
echo "                         Gccbuild.bat Board-Core-Project-Option                           "
echo "    Board,Core,Project,Option could be omitted, if so default project will be used        "
echo "           For Board only ec618_0h00 is supportted, should be omitted                     "
echo "                                                                                          "
echo "                                                                                          "
echo "******************************************************************************************"
echo "Supported Options:"
echo "      merge----compile bootloader, cp, ap, then merge to a package, default  setting      "
echo "      doc----run doxygen to generate documents                                            "
echo "      clean----clean the build output files                                               "
echo "      clall----clean all output files for every board and project                         "
echo "      allprojects----trigger compile all example projects under default board             "
echo "      list----list current supported boards and supported project for each board          "
echo "      help---show how to use the build script                                             "
echo "                                                                                          "
echo "                                                                                          "
echo "       Example start -- Example start -- Example start -- Example start                   "
echo "------------------------------------------------------------------------------------------"
echo "-------!!!!!!!!!For internal full source build start !!!!!!!!!----------------------------"
echo "       Gccbuild.bat "
echo "       build bootloader, cp, ap,then merge to a package"
echo "******************************************************************************************"
echo "       Gccbuild.bat  ap-at_command"
echo "       build ap at_command project only"
echo "******************************************************************************************"
echo "       Gccbuild.bat  ap-bootloader"
echo "       build bootloader project only"
echo "******************************************************************************************"
echo "       Gccbuild.bat  cp"
echo "       build cp project only"
echo "******************************************************************************************"
echo "       Gccbuild.bat  ap-driver_example-clean"
echo "       clean ap driver_example project"
echo "******************************************************************************************"
echo "-------!!!!!!!!!For internal full source build end !!!!!!!!!------------------------------"
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
echo "-------!!!!!!!!!For customer SDK build start !!!!!!!!!------------------------------------"
echo "       Gccbuild.bat "
echo "       build bootloader,ap, use prebuild cp bin, then merge to a package"
echo "******************************************************************************************"
echo "       Gccbuild.bat  project_name   e.g. project_name = at_command/driver_example"
echo "       build xxx(project_name) project "
echo "******************************************************************************************"
echo "       NOTE: SDK build  always generate package(project_name.bin+bootloader.bin+cp.bin)"
echo "******************************************************************************************"
echo "-------!!!!!!!!!For customer SDK build end !!!!!!!!!-------------------------------       "
echo "******************************************************************************************"
echo "       Gccbuild.bat clean"
echo "       clean default project for default board
echo "******************************************************************************************"
echo "       Gccbuild.bat clall"
echo "       clean all output files for every board and project
echo "******************************************************************************************"
echo "       Gccbuild.bat list"
echo "       list current supported boards and supported project for each board
echo "------------------------------------------------------------------------------------------"
echo "       Example end -- Example end -- Example end  --  Example end                         "
goto end
)



echo PARAMETERS: %PARAMETERS% | findstr "list"
if not errorlevel 1 (
   echo ---------------Supported Board------------------------------------
   for /d %%i in ("project\*") do (echo %%~nxi )

   echo ------------------------------------------------------------------

   setlocal enabledelayedexpansion
   for /d %%i in ("project\*") do (
   set CURRENT_BOARD=%%~nxi
   echo *********supported projects for board: !CURRENT_BOARD!*************
   for /d %%i in ("project\!CURRENT_BOARD!\ap\apps\*") do (echo ----%%~nxi )
   )
   endlocal

goto end
)



echo PARAMETERS: %PARAMETERS% | findstr "doc"
if not errorlevel 1 (
	pushd doxygen
	CALL genDoc.bat %CHIP_NAME%
	popd
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clall"
if not errorlevel 1 (
	make.exe -j4  clean-gccall TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% 
	echo clean all done ok...
	
	if exist %gb_gccout_path% (
		echo delete %gb_gccout_path%
		rd %gb_gccout_path% /s /q
	)	
	
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clean"
if not errorlevel 1 (
	make.exe -j4  clean-gcc TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME%
	echo clean done ok...
	
	if exist %gb_gccout_path% (
		echo delete %gb_gccout_path%
		rd %gb_gccout_path% /s /q
	)

	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clobj"
if not errorlevel 1 (

	@del /s /q .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.o
	@del /s /q .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.a
	@del /s /q .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.elf
	echo clean objs done ok...
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "verbose"
if not errorlevel 1 (
  set VERBOSE=1
) else (
  set VERBOSE=0
)


echo PARAMETERS: %PARAMETERS% | findstr "allprojects"
if not errorlevel 1 (
  if exist .\gccout\%BOARD_NAME% (
		echo delete %BOARD_NAME% folder......
		rd .\gccout\%BOARD_NAME% /s /q
	)
	
	rem step1 build cp if not SDK
	if not exist .\tools\comdblib.txt (
	echo start logprepass b11
	if exist .\tools\UpdateDBPattern.txt (
	del /q .\tools\UpdateDBPattern.txt
	)

	echo > .failed.tmp
	(make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=cp_project CORE=cp BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\cp\outbuildlog.txt

				

	if exist .failed.tmp (
	del .failed.tmp
	goto failHandle
	)

	rem this header file will compile with device code
	copy .\gccout\%BOARD_NAME%\cp\cp_project\debug_log_cp.h .\middleware\developed\debug\inc\ || (goto:failHandle)


	echo > .failed.tmp
	(make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=cp_project CORE=cp BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\cp\outbuildlog.txt

	if exist .failed.tmp (
	del .failed.tmp
	goto failHandle
	)
)
	rem step2  build bootloader no matter SDK or not
	echo > .failed.tmp
	(make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=bootloader CORE=ap BUILD_UNILOG=false BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt

	if exist .failed.tmp (
	del .failed.tmp
	goto failHandle
	)


  setlocal enabledelayedexpansion

    echo *********supported projects for board: %BOARD_NAME%*************
    for /d %%i in ("project\%BOARD_NAME%\ap\apps\*") do (
    	set CURRENT_PROJECT=%%~nxi
    	echo ----!CURRENT_PROJECT!

			if "!CURRENT_PROJECT!"=="bootloader" (
			
			echo ignore bootloader when loop all project

			) else (
				echo 111
				if exist .\tools\UpdateDBPattern.txt (
				del /q .\tools\UpdateDBPattern.txt
				)
				
			   if exist .\tools\comdblib.txt (
			   (make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%%~nxi CORE=ap BUILD_UNILOG=true SDK=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt
			   ) else (
			   (make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%%~nxi CORE=ap BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt
			   )
				
				rem this header file will compile with device code
				copy .\gccout\%BOARD_NAME%\ap\%%~nxi\debug_log_ap.h .\middleware\developed\debug\inc\ || (goto:failHandle)
				if exist .\gccout\%BOARD_NAME%\ap\%%~nxi\dbversion.h (
				copy .\gccout\%BOARD_NAME%\ap\%%~nxi\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
				)
				
				echo > .failed.tmp
			   (make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%%~nxi CORE=ap BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt

				if exist .failed.tmp (
				del .failed.tmp
				goto failHandle
				)
				
				if not exist .\tools\comdblib.txt (
				.\tools\fcelf.exe -M  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%%~nxi\ap_%%~nxi.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\cp\cp_project\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%%~nxi\%%~nxi.binpkg
				.\tools\fcelf.exe -m  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.elf -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%%~nxi\ap_%%~nxi.elf -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\cp\cp_project\cp-demo-flash.elf -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%%~nxi\%%~nxi.elf
				) else (
				.\tools\fcelf.exe -M  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%%~nxi\ap_%%~nxi.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\prebuild\FW\lib\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%%~nxi\%%~nxi.binpkg
				.\tools\fcelf.exe -m  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.elf -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%%~nxi\ap_%%~nxi.elf -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\prebuild\FW\lib\cp-demo-flash.elf -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%%~nxi\%%~nxi.elf
				)
				 if exist .\tools\UpdateDBPattern.txt (
				 type .\tools\UpdateDBPattern.txt
				 )
			
		)
			
    )

  endlocal

	goto complete
)

set starttime=%time%
echo Start time: %date% %starttime%

if "%BUILD_OPTION%"=="merge" (
	if exist .\tools\comdblib.txt (
		echo start logprepass

rem	    echo > .failed.tmp
rem	    (make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=bootloader CORE=%CORE_NAME% BUILD_UNILOG=false BUILD_CUST=%CUST% SDK=true LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\%CORE_NAME%\outbuildlog.txt

rem		if exist .failed.tmp (
rem		del .failed.tmp
rem		goto failHandle
rem		)

rem	    if exist .\tools\UpdateDBPattern.txt (
rem		del /q .\tools\UpdateDBPattern.txt
rem		)

		make.exe -j4  build-unilog TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true LSPD=%LSPD%

		rem this header file will compile with device code
		copy .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\debug_log_ap.h .\prebuild\PLAT\inc\core\developed\debug\inc\ || (goto:failHandle)
		if exist .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\dbversion.h (
		copy .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\dbversion.h .\prebuild\PLAT\inc\core\developed\debug\inc\ || (goto:failHandle)
		)

	    echo > .failed.tmp
	   (make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=ap BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt

	    if exist .failed.tmp (
	    del .failed.tmp
	    goto failHandle
	    )

rem	    .\tools\fcelf.exe -M  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\prebuild\FW\lib\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
	    .\tools\fcelf.exe -M  -input .\prebuild\BL\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\prebuild\FW\lib\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg

		rem .\tools\fcelf.exe -m -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.elf -addrname  BL_IMG_MERGE_ADDR -input .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.elf -addrname  AP_IMG_MERGE_ADDR -input .\prebuild\FW\lib\cp-demo-flash.elf -addrname CP_IMG_MERGE_ADDR -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.elf

		 if exist .\tools\UpdateDBPattern.txt (
		 type .\tools\UpdateDBPattern.txt
	     )

		 goto complete
  	
	) else (
			
			echo > .failed.tmp
			(make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=bootloader CORE=%CORE_NAME% SDK_REL=%SDKREL% BUILD_UNILOG=false BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\%CORE_NAME%\outbuildlog.txt

			if exist .failed.tmp (
			del .failed.tmp
			goto failHandle
			)
			
			
			
			echo start logprepass b22
			if exist .\tools\UpdateDBPattern.txt (
			del /q .\tools\UpdateDBPattern.txt
			)
			
			echo > .failed.tmp
			(make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=cp_project CORE=cp SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\cp\outbuildlog.txt
			
						
			
			if exist .failed.tmp (
			del .failed.tmp
			goto failHandle
			)
			
			rem this header file will compile with device code
			copy .\gccout\%BOARD_NAME%\cp\cp_project\debug_log_cp.h .\middleware\developed\debug\inc\ || (goto:failHandle)

			
		   echo > .failed.tmp
		   (make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=cp_project CORE=cp SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\cp\outbuildlog.txt

           if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
           )

		   echo > .failed.tmp
		   (make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%PROJECT_NAME% CORE=ap SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt

		   if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
			)
			
			rem this header file will compile with device code
			copy .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\debug_log_ap.h .\middleware\developed\debug\inc\ || (goto:failHandle)
			if exist .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\dbversion.h (
			copy .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
			)
			
		   echo > .failed.tmp
		   (make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=ap SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\ap\outbuildlog.txt

           if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
          )
		  
		  .\tools\fcelf.exe -M  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\cp\cp_project\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
		  .\tools\fcelf.exe -m  -input .\gccout\%BOARD_NAME%\ap\bootloader\ap_bootloader.elf -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.elf -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE -input .\gccout\%BOARD_NAME%\cp\cp_project\cp-demo-flash.elf -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE -def .\device\target\board\%BOARD_NAME%\common\inc\mem_map.h -outfile .\gccout\%BOARD_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.elf
		  
		  
		  if exist .\tools\UpdateDBPattern.txt (
		  type .\tools\UpdateDBPattern.txt
		  )

		   
		  goto complete
	)
    
)


if %PROJECT_NAME%==bootloader goto :buildimage
if %PROJECT_NAME%==driver_example goto :buildimage
set UNILOG=true
echo start logprepass b2
if exist .\tools\UpdateDBPattern.txt (
del /q .\tools\UpdateDBPattern.txt
)
echo > .failed.tmp
(make.exe -j4 build-unilog TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% TOOLCHAIN_NAME=%TOOLCHAIN_NAME% BUILD_UNILOG=true BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\%CORE_NAME%\outbuildlog.txt

if exist .failed.tmp (
del .failed.tmp
goto failHandle
)

rem this header file will compile with device code
copy .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\debug_log_%CORE_NAME%.h .\middleware\developed\debug\inc\ || (goto:failHandle)
if exist .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\dbversion.h (
copy .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
)

endlocal

:buildimage
REM goto end
echo > .failed.tmp
(make.exe -j4 gccall TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% BUILD_UNILOG=%UNILOG% BUILD_CUST=%CUST% LSPD=%LSPD% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%BOARD_NAME%\%CORE_NAME%\outbuildlog.txt

if exist .failed.tmp (
del .failed.tmp
goto failHandle
)

rem copy log database to output dir after compile successfully
REM cp .\tools\comdb_%CORE_NAME%.txt .\gccout\%BOARD_NAME%\%CORE_NAME%\%PROJECT_NAME%\comdb.txt

:complete

set endtime=%time%
echo .
echo End time: %date% %endtime%

set /a h1=%starttime:~0,2%
set /a m1=1%starttime:~3,2%-100
set /a s1=1%starttime:~6,2%-100
set /a h2=%endtime:~0,2%
set /a m2=1%endtime:~3,2%-100
set /a s2=1%endtime:~6,2%-100
if %h2% LSS %h1% set /a h2=%h2%+24
set /a ts1=%h1%*3600+%m1%*60+%s1%
set /a ts2=%h2%*3600+%m2%*60+%s2%
set /a ts=%ts2%-%ts1%
set /a h=%ts%/3600
set /a m=(%ts%-%h%*3600)/60
set /a s=%ts%%%60
echo Built took %h% hours %m% minutes %s% seconds

echo     #######################################################################
echo     ##                                                                   ##
echo     ##                 ########     ###     ######   ######              ##
echo     ##                 ##     ##   ## ##   ##    ## ##    ##             ##
echo     ##                 ##     ##  ##   ##  ##       ##                   ##
echo     ##                 ########  ##     ##  ######   ######              ##
echo     ##                 ##        #########       ##       ##             ##
echo     ##                 ##        ##     ## ##    ## ##    ##             ##
echo     ##                 ##        ##     ##  ######   ######              ##
echo     ##                                                                   ##
echo     #######################################################################

echo build successfully

if %UNILOG%==true (
if exist .\tools\UpdateDBPattern.txt (
type .\tools\UpdateDBPattern.txt
)
)

:end
goto:eof

:failHandle
echo fail
echo     #######################################################################
echo     ##                                                                   ##
echo     ##                    ########    ###     ####  ##                   ##
echo     ##                    ##         ## ##     ##   ##                   ##
echo     ##                    ##        ##   ##    ##   ##                   ##
echo     ##                    ######   ##     ##   ##   ##                   ##
echo     ##                    ##       #########   ##   ##                   ##
echo     ##                    ##       ##     ##   ##   ##                   ##
echo     ##                    ##       ##     ##  ####  ########             ##
echo     ##                                                                   ##
echo     #######################################################################
goto:eof

