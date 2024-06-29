@echo off
pushd %~dp0

set BEL_SDK_ROOT=%~dp0../..
set SHADER_CONVERTER=%BEL_SDK_ROOT%/Tools/GraphicsTools/ShaderConverter.exe
set SHADER_ARCHIVER=%BEL_SDK_ROOT%/Tools/GraphicsTools/ShaderArchiver.exe
set INTERMEDIATE_ROOT=%BEL_SDK_ROOT%/Intermediate/Shader
set CONTENT_ROOT=%BEL_SDK_ROOT%/Content/Shader

%SHADER_CONVERTER% -i convert.json -o %INTERMEDIATE_ROOT%
if ERRORLEVEL 1 GOTO :ERROR

%SHADER_ARCHIVER% -i %INTERMEDIATE_ROOT% -n Sample -o %CONTENT_ROOT%/Sample.bsh
if ERRORLEVEL 1 GOTO :ERROR

%SHADER_ARCHIVER% -i %INTERMEDIATE_ROOT% -n ToneMapping -o %CONTENT_ROOT%/ToneMapping.bsh
if ERRORLEVEL 1 GOTO :ERROR

popd
exit /b 0

:ERROR
pause
popd
