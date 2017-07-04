program Tunnel;

uses graph;

var
   x, y,
   Gd, Gm,
   NoCirc,Radius: Integer;

procedure CircDraw;
Begin
     Gd := Detect;
     InitGraph(Gd, Gm, 'c:\tp\bgi');
     if GraphResult <> grOk then
         Halt(1);
     for Radius := 1 to NoCirc do
         Circle(x,y, Radius * 10);
     Readln;
end;

begin
     Write ('Enter X coordinate for first circle>');
     Readln (x);
     Write ('Enter Y coordinate for first circle>');
     Readln (y);
     Write ('Enter number of circles to draw>');
     Readln (NoCirc);
     CircDraw;
end.