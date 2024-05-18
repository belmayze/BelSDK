pushd %~dp0

set BEL_SDK_ROOT=%~dp0../..
set SHADER_CONVERTER=%BEL_SDK_ROOT%/Tools/ShaderConverter/ShaderConverter.exe

%SHADER_CONVERTER% -i convert.json
if ERRORLEVEL 1 pause

popd
