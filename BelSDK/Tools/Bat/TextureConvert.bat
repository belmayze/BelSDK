@echo off
pushd %~dp0
set TEXTURE_CONVERTER=%..\TextureConverter\TextureConverter.exe
set OUTPUT_ROOT=..\..\Content\Texture
set INPUT_FILEPATH=%1
set FILENAME=%~n1

%TEXTURE_CONVERTER% -i %INPUT_FILEPATH% -o %OUTPUT_ROOT%\%FILENAME%.btex -f %2
if not errorlevel 0 (
    echo TextureConvert error!! %ERRORLEVEL%
)
popd