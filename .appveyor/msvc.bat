@echo on

:: Download Meson and Ninja, create install directory
mkdir _build
mkdir graphene-shared-%MSVC_PLATFORM%
cd _build
curl -LsSO https://github.com/mesonbuild/meson/releases/download/0.44.1/meson-0.44.1.tar.gz
7z x meson-0.44.1.tar.gz
move dist\meson-0.44.1.tar .
7z x meson-0.44.1.tar
rmdir dist
del meson-0.44.1.tar meson-0.44.1.tar.gz
curl -LsSO https://github.com/ninja-build/ninja/releases/download/v1.7.2/ninja-win.zip
7z x ninja-win.zip
del ninja-win.zip
cd ..

:: Build and install
cd _build
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %MSVC_PLATFORM%
@echo on
C:\Python36\python.exe meson-0.44.1\meson.py .. . --backend=ninja --prefix=%APPVEYOR_BUILD_FOLDER%\graphene-shared-%MSVC_PLATFORM% || goto :error
ninja || goto :error
ninja test || goto :error
ninja install || goto :error
cd ..

:: Copy license into install directory and create .zip file
copy LICENSE graphene-shared-%MSVC_PLATFORM% || goto :error
dir graphene-shared-%MSVC_PLATFORM% /s /b || goto :error
7z a -tzip graphene-shared-%MSVC_PLATFORM%.zip graphene-shared-%MSVC_PLATFORM% || goto :error

goto :EOF

:error
exit /b 1
