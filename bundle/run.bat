@echo off

rem Detect if the system is 64-bit
if defined ProgramFiles(x86) (
    echo Running 64-bit version
    cd %USERPROFILE% && mkdir shutdown_ctrl
    copy shutdown_ctrl_x64.exe "%USERPROFILE%\shutdown_ctrl"
    call "%USERPROFILE%\shutdown_ctrl\shutdown_ctrl_x64.exe"
) else (
    echo Running 32-bit version
    cd %USERPROFILE% && mkdir shutdown_ctrl
    copy shutdown_ctrl_x86.exe "%USERPROFILE%\shutdown_ctrl"
    call "%USERPROFILE%\shutdown_ctrl\shutdown_ctrl_x86.exe"
)