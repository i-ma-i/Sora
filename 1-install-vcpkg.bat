@echo off
setlocal

REM 日本語の文字化けを回避する。
chcp 65001 > NUL

if not exist vcpkg (

    REM vcpkgをダウンロード。
    git clone --depth 1 https://github.com/microsoft/vcpkg %VCPKG_ROOT%
    echo vcpkgのダウンロードが完了しました。
    echo.

    echo vcpkgをインストールします...
    .\vcpkg\bootstrap-vcpkg.bat
    .\vcpkg\vcpkg integrate install
    echo vcpkgのインストールが完了しました。
    pause

)

endlocal
pause
