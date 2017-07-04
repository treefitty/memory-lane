uses Graph;
const
     Triangle: array [1..4] of PointType = ((x: 50; y: 100), (x:100; y:100),
               (x:150;y:150),(x:50;y:100));
var
   gd,gm : integer;
begin
     gd := detect;
     initgraph(gd,gm,'c:\tp\bgi');
     if graphresult <> grok then
        halt(1);
     Drawpoly(sizeof(triangle) div sizeof(pointtype),triangle);
     readln;
     closegraph;
end.