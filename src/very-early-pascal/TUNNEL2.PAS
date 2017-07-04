program Tunnel;

uses Graph, Crt;

var
   x, y,
   Gd, Gm,
   NoCirc, Radius, Expand: Integer;
   Tun, again: String;
   Ch: Char;
   Another: Boolean;

procedure CircDraw;
Begin
     Gd := Detect;
     InitGraph(Gd, Gm, '\tp\bgi');
     if GraphResult <> grOk then
         Halt(1);
     for Radius := 1 to NoCirc do
     begin
     Circle(x,y, Radius * Expand);
     inc(x,5);
     end;
     repeat
     until keypressed;
     Ch := ReadKey;
     CloseGraph;
end;

Procedure InputCoord;
begin
     Another := False;
     Write ('Enter X coordinate for first circle, whole numbers only> ');
     Readln (x);
     Writeln;
     Write ('Enter Y coordinate for first circle, whole numbers only> ');
     Readln (y);
     Writeln;
     Write ('Enter number of circles to draw, whole numbers only> ');
     Readln (NoCirc);
     Writeln;
     Write ('Enter number for circles to increase by, whole numbers only> ');
     Readln (Expand);
     Writeln;
     CircDraw;
     Write ('Would you like to do another?> ');
     Readln (Again);
     if (again = ('y')) or (Again = ('Y')) then another := True
     else another := false;
     if Another then
        Begin             
             InputCoord;
        end
     else
       end;
Begin
     Inputcoord;
end.








