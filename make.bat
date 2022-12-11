@echo off

set VERSION="0.3.0"

if {%1} == {} (
    goto USAGE
)

if {%1} == {help} (
    goto USAGE
)

if {%1} == {clean} (
    goto CLEAN
)

if {%1} == {build} (
    goto BUILD
)

if {%1} == {release} (
    goto RELEASE
)

if {%1} == {dist} (
    goto DIST
)

goto USAGE

:BUILD
msbuild -p:Configuration=Debug
msbuild -p:Configuration=Release
goto :EOF

:RELEASE
msbuild -t:Rebuild -p:Configuration=Release
goto :EOF

:CLEAN
msbuild -t:Clean -p:Configuration=Release
msbuild -t:Clean -p:Configuration=Debug
del /s /q dist
rmdir dist
goto :EOF

:DIST
mkdir dist
upx x64/release/wincheckip.exe
powershell -Command "Compress-Archive -Path 'x64/release/wincheckip.exe' -DestinationPath 'dist/wincheckip_x64_%VERSION%.zip'"
goto :EOF


:USAGE
echo.
echo Usage:
echo.  make ^<task^>
echo.
echo The tasks are:
echo.
echo make clean         Removed generated files
echo make build         Build debug and release targets
echo make release       Rebuild project and generate release target
