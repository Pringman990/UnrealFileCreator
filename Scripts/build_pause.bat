@ECHO OFF
pushd %~dp0..

mkdir Binaries\Win64

mkdir Intermediate\
mkdir Intermediate\ProjectFiles
mkdir Intermediate\Temp

call "Scripts/Premake/premake5" --file=Source/premake5.lua vs2022 || @PAUSE

popd
@Pause
@ECHO ON