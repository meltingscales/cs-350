@ECHO ON

SET compiler=LC3Edit.exe
SET simulator=Simulate.exe
SET asmLoc=prog\test_program_arc_2017.asm
SET objLoc=prog\test_program_arc_2017.obj
SET path1=D:\MEGA\School\Sophomore\S1\CS350\hpost_Lab7\

%path1%%compiler% %path1%%asmLoc%

ECHO assembled. running "%objLoc%" with "%simulator%"

%path1%%simulator% %path1%%objLoc%

@ECHO ON
ECHO hi. asmLoc is at %asmLoc%.
ECHO hi. path1 is at %path1%.
