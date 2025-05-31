cd ../wxWidgets
rm -R -f ..\wxWidgets\generated
cd ../scripts
cmake.exe ..\wxWidgets\CMakeLists.txt -B..\wxWidgets\generated -G "CodeBlocks - Unix Makefiles" -DwxBUILD_SHARED=OFF -DwxBUILD_USE_STATIC_RUNTIME=ON -DwxBUILD_SAMPLES=ALL
call build.bat
