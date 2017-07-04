program RandColor;

uses Graph, Crt;

var
   Col, Gd, Gm : integer;

begin
     Gd := Detect;
     InitGraph(Gd,Gm,'c:\tp\bgi');
     if GraphResult <> GrOK then
        Halt(1);
     Col := 1;
     Repeat
           Col := Col + 1;
           PutPixel(Random(639),Random(479),Col);
     until KeyPressed = True;
     closegraph;
end.