program FillStyle;

uses graph;

const
     SolidFill = 1;
     Xhatch = 8;

var
   gd, gm : integer;

begin
     gd := detect;
     initgraph(gd,gm,'c:\tp\bgi');
     if graphresult <> grOk then
        halt(1);
     setfillstyle (solidfill, 0);
     Bar(0,0,50,50);
     setfillstyle(xhatchfill,1);
     bar(60,0,110,50);
     readln;
     closegraph;
end.