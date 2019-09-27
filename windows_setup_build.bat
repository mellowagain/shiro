@echo off

for /f "tokens=*" %%a in ('where vcpkg.exe') do set VCPKG_ROOT=%%~dpa

if "%VCPKG_ROOT%"=="" (
    echo === UNABLE TO FIND vcpkg.exe ===
    echo Please make sure that you have installed vcpkg.exe, run `vcpkg integrate install` and placed the vcpkg root into your PATH
    exit
)

echo === VCPKG_ROOT is %VCPKG_ROOT% ===

REM install 64bit packages that we need
vcpkg install --triplet x64-windows "boost" "curl" "cryptopp" "zlib" "liblzma" "date" "libmysql" "libmaxminddb"

pushd "%~dp0"
    pushd external
        git clone "https://github.com/HowardHinnant/date"

        git clone "https://github.com/rbock/sqlpp11.git"
        cd "sqlpp11"

        git checkout da5cf192e951c8d276d45a9960503a3d5d02e440 .
        git apply ../sqlpp11.patch

        cd ..
        git clone "https://github.com/rbock/sqlpp11-connector-mysql.git"
        cd "sqlpp11-connector-mysql"

        git checkout 9ab936a217708a30998686d9b4061cbb0ffa547d .
        git apply ../sqlpp11-connector-mysql.patch

        mkdir build
        cd build

        cmake -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DVCPKG_INCLUDE_DIR=%VCPKG_ROOT%\installed\x64-windows\include\ ..
        cmake --build . --config Release --target sqlpp-mysql
        cmake --build . --config Debug --target sqlpp-mysql
    popd

    mkdir build
    pushd build
        cmake -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_EXPORT_COMPILE_COMMANDS=true ..
    popd
popd
