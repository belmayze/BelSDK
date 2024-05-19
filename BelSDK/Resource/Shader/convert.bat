pushd %~dp0

set BEL_SDK_ROOT=%~dp0../..
set SHADER_CONVERTER=%BEL_SDK_ROOT%/Tools/GraphicsTools/ShaderConverter.exe
set INTERMEDIATE_ROOT=%BEL_SDK_ROOT%/Intermediate/Shader

%SHADER_CONVERTER% -i convert.json -o %INTERMEDIATE_ROOT%
if ERRORLEVEL 1 pause

popd
