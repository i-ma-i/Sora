@echo off
setlocal

REM 日本語の文字化けを回避する。
chcp 65001 > NUL

REM パッケージのビルドを4スレッドで実行する。
set VCPKG_MAX_CONCURRENCY=4

REM デフォルトのオプションを設定する。
set VCPKG_DEFAULT_TRIPLET=x64-windows-static

if exist vcpkg (

    echo パッケージをインストールします...
    .\vcpkg\vcpkg install yaml-cpp magic-enum spdlog sdl2 directxtk imgui[sdl2-binding,dx11-binding]
    echo パッケージのインストールが完了しました。

)

endlocal
pause
