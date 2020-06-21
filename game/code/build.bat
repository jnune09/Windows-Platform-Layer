@echo off

set flag= -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7

set warn= -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505

set mode= -DGAME_INTERNAL=1 -DGAME_SLOW=1 -DGAME_WIN32=1

set link= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

set optimize= /O2 /Oi /fp:fast

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

REM 32-bit build

REM cl %flag% %warn% %mode% ..\game\code\win32_game.cpp /link -subsystem:windows,5.1 %link%

REM 64-bit build

del *.pdb> NUL2> NUL

cl %flag% %warn% %mode% ..\game\code\game.cpp -Fmgame.map -LD /link -incremental:no -opt:ref /PDB:game_%random%.pdb -EXPORT:GameGetSoundSamples -EXPORT:GameUpdateAndRender

cl %flag% %warn% %mode% ..\game\code\win32_game.cpp -Fmwin32_game.map /link %link%
popd
