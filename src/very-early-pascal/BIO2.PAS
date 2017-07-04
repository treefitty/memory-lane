program Biorhythm;

uses Crt, Graph, Dos;

const
     Physical = 23;
     Emotional = 28;
     Intellectual = 33;
     HorizDir = 0;
     GothicFont = 4;
     CentreText = 1;
     RightText = 2;

var
   MOB, DOB, YOB, TotalDays, Days,
   YearsDiffInDays, LYear, NewLYear,
   Gd, Gm, i, euro, lcom, scri, simp, tscr: integer;
   Choice : Char;
   CurrDay, CurrMonth, CurrYear, DayWeek : word;
   CheckValidChoice : Array[1..6] of Char;

{----------------------------------------------------------------------}

procedure FlushKey;
var
   Regs : Registers;

begin
     Regs.AH := $01;
     Intr($16,regs);
     if (regs.Flags and $0040) = 0 then
        repeat
              Regs.AH := 0;
              Intr($16,Regs);
              Regs.AH := $01;
              Intr($16,Regs);
        until (regs.Flags and $0040) <> 0;
end;

{---------------------------------------------------------------------------}

procedure YearAgeInDays;
Begin
     LYear := 2000;
     TotalDays := 0;
     while CurrYear < LYear do
           Begin
                LYear := LYear - 4;
           end;
     LYear := NewLYear;
     repeat
           LYear := LYear + 1;
           NewLYear := NewLYear - 4
     until YOB > NewLYear;
     NewLYear := NewLYear + 1;
     YearsDiffInDays := CurrYear - YOB;
     TotalDays := (YearsDiffInDays * 365) + (LYear * 366);
end;

{---------------------------------------------------------------------------}


procedure DataInput;
Begin
     ClearDevice;
     {SetColor(LightGreen);
     SetTextJustify ( CentreText, CentreText);
     SetTextStyle ( TriplexFont, HorizDir, 2);
     Delay(1000);
     GetDate (CurrYear, CurrMonth, CurrDay, Dayweek);
     OutTextXY (320,80,'Current Year is');
     OutTextXY (500,80,CurrYear);
     ,'Do You Wish to change this?');}

end;

{---------------------------------------------------------------------------}

procedure Intro;

Begin
     ClearDevice;
     SetBkColor(Black);
     SetTextJustify(centretext, centretext);
     SetTextStyle(Gothicfont, HorizDir, 6);
     OutTextXY ( Succ(GetMaxX) div 2, Succ(GetMaxY) div 13,'Sketch Your');
     SetColor(LightBlue);
     SetTextStyle(TriplexFont, HorizDir, 9);
     OutTextXY ( 320, 130 ,'BIORHYTHM');
     SetColor(Green);
     SetTextStyle(GothicFont, HorizDir, 5);
     OutTextXY ( 320, 250 ,'Written and Developed by');
     SetColor(Magenta);
     SetTextStyle( TriplexFont, HorizDir, 7);
     OutTextXY ( 330, 330 ,'By Paul Carroll.');
     SetColor(Yellow);
     SetTextStyle (TSCR, HorizDir, 2);
     OutTextXY ( 500, 410,'Copyright 1993');
     SetColor(Red);
     Delay(5000);
end;

{---------------------------------------------------------------------------}

Procedure DetectGraph;
Begin
     Gd := Detect;
     InitGraph(Gd,Gm,'c:\tp\bgi');
     if GraphResult <> GrOK then
        Begin
             ClrScr;
             Sound (170);
             Delay (180);
             NoSound;
             Write ('You do not have the correct graphics driver.');
             Write ('Check that the driver is in the current directory.');
             Halt(1);
        end
     else
     Begin
          SetColor (Red);
          SetTextStyle (TriplexFont, HorizDir, 3);
          SetTextJustify ( CentreText, CentreText); 
          OutTextXY( 319, 239,'Correct graphics driver has been installed.');
          Delay(2000);
     end;
end;

{---------------------------------------------------------------------------}

procedure MyExit;

const WideDotFill = 11;

begin
     ClearDevice;
     SetColor (LightGreen);
     SetFillStyle(WideDotFill,LightBlue);
     Bar(0,0,GetMaxX,GetMaxY);
     SetTextStyle ( GothicFont, HorizDir, 10);
     OutTextXY ( 319, 200,'Good Bye!!');
     Delay (2000);
     ClearDevice;
     SetTextStyle( TriplexFont, HorizDir, 4);
     OutTextXY ( 319, 200,'Graphics Driver Has Been Uninstalled.');
     Delay(2000);
     CloseGraph;
     Halt(1);
end;

{---------------------------------------------------------------------------}

procedure History;
Begin
     ClearDevice;
     SetTextJustify(centretext,centretext);
     SetColor(Magenta);
     SetTextStyle(TriplexFont,HorizDir,5);
     OutTextXY(220,20,'Biorhythm History -');
     SetColor(LightGreen);
     SetTextStyle(TriplexFont,HorizDir,2);
     OutTextXY(320,80 ,'   Near the turn of the century a German Doctor, William');
     OutTextXY(310,120,'Fleiss, as  well as an  Austrian psychologist,  Professor');
     OutTextXY(330,160,'Herman Swoboda (working seperately at the time) discovered');
     OutTextXY(318,200,'that their patients Emotional, Physical  and  Intellectual');
     OutTextXY(315,240,'patterns would all fluctuate in regular cycles. They found');
     OutTextXY(330,280,'that the Physical cycle spanned 23 days while the Emtional');
     OutTextXY(130,320,'cycle spanned 28 days.');
     SetColor(Red);
     OutTextXY(320,460,'<Press ANY KEY to continue>');
     repeat
     until KeyPressed = True;
     flushkey;
end;

{----------------------------------------------------------------------------}

Procedure MainMenu;
Begin
     SetBkColor (Black);
     ClearDevice;
     SetColor (LightGreen);
     SetTextStyle ( TriplexFont, HorizDir, 8);
     SetTextJustify ( centretext, centretext);
     OutTextXY ( 320, 20,'Main Menu' );
     SetColor (Yellow);
     SetTextStyle ( Tscr, HorizDir, 3);
     OutTextXY(330, 70,'----------------------------');
     SetColor (Magenta);
     OutTextXY ( 320, 110,'1) Sketch Biorhythm - Physical.    ');
     OutTextXY ( 325, 160,'2) Sketch Biorhythm - Emotional.   ');
     OutTextXY ( 309, 210,'3) Sketch Biorhythm - Intellectual.');
     OutTextXY ( 308, 260,'4) Sketch all three Biorhythms.    ');
     OutTextXY ( 323, 310,'5) Read History of Boirhythms.     ');
     OutTextXY ( 330, 360,'6) Exit Biorhythm program.         ');
     SetColor(LightBlue);
     SetTextStyle ( TriplexFont, HorizDir, 4);
     OutTextXY ( 290, 430,'Enter the number of your choice> ');
     FlushKey;
     Repeat
     Until KeyPressed = True;
     If KeyPressed then
        Begin
             Choice := ReadKey;
        end;
     case Choice of
          '1' : DataInput;
          '2' : DataInput;
          '3' : DataInput;
          '4' : DataInput;
          '5' : History;
          '6' : MyExit
     end;
     for i := 1 to 6 do
     Begin
          CheckValidChoice[i] := CheckValidChoice[i];
     end;
     if Choice <> CheckValidChoice then
        Begin
             ClearDevice;
             SetTextJustify(centretext,centretext);
             OutTextXY(320,240,'Invalid Menu Item!!');
             Delay (2500);
             ClearDevice;
             MainMenu;
        end;
     MainMenu;
end;

{---------------------------------------------------------------------------}

Begin
     DetectGraph;
     euro := InstallUserFont ('euro');
     lcom := InstallUserFont ('lcom');
     scri := InstallUserFont ('scri');
     simp := InstallUserFont ('simp');
     tscr := InstallUserFont ('tscr');
     if GraphResult <> GrOk then
        Begin
             closegraph;
             write('Error when installing fonts!!');
             Delay(2000);
             halt(1);
        end;
     Intro;
     MainMenu;
END.



