program DataRead;

uses Crt;
var
   Stuff : text;

   morestuff : String;

begin
     ClrScr;
     assign (stuff, 'c:\tp\stuff.dat');
     reset (stuff);
     while not EOF(stuff) do
           begin
                while not EOLN(stuff) do
                      begin
                           Readln (Stuff, morestuff);
                           writeln (morestuff)
                      end;
           end;
     Readln (stuff);
     Writeln (morestuff);
     close(stuff);
end.
