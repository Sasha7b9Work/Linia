cd ../wxWidgets
rmdir generated /s /q
cd ../scripts
cmake.exe ..\wxWidgets\CMakeLists.txt -B..\wxWidgets\generated -G "Visual Studio 17 2022" -A Win32 -DwxBUILD_STRIPPED_RELEASE=ON -DwxBUILD_SHARED=OFF -DwxBUILD_USE_STATIC_RUNTIME=ON -DwxBUILD_SAMPLES=ALL
call build.bat
