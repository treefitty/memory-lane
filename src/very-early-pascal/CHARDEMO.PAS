program nothing;

type
    month = array [1..3] of char;

var
   Jan, feb : month;
   stuff : string;
   i, x : integer;

begin
     writeln ('yep> ');
     for i := 1 to 3 do
         Read (Jan[i]);
     writeln ('yep2> ');
     for x := 1 to 3 do
         Read (feb[i]);
     writeln (jan);
     writeln (feb);
     writeln;
end.