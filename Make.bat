@echo off
cls

if /i "%1" == "new" (
    call GccBuild.bat c618_0h00-ap-meig-merge
) else (
    if /i "%1" == "clean" (
        call GccBuild.bat ec618_0h00-ap-meig-clall
    ) else (
        echo More parameter is required.
        echo  - "make clean" may clean the compiling, and "make new" may start to compile.
    )
)



