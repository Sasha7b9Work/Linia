cd ../wxWidgets
rmdir generated /s /q
cd ../scripts
cmake.exe ..\wxWidgets\CMakeLists.txt -B..\wxWidgets\generated -G "Visual Studio 18 2026" -A Win32 -DwxBUILD_STRIPPED_RELEASE=ON -DwxBUILD_SHARED=OFF -DwxBUILD_USE_STATIC_RUNTIME=ON -DwxBUILD_SAMPLES=OFF

MSBuild.exe ..\wxWidgets\generated\wxWidgets.slnx /p:Configuration=Release -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success

:Exit
