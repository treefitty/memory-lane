program Biorhythm;

uses Crt, Graph;

const
     Physical = 23;
     Emotional = 28;
     Intellectual = 33;
     HorizDir = 0;
     GothicFont = 4;
     CentreText = 1;
     RightText = 2;
                                            
var
   MOB, CurrMonth : String;
   DOB, YOB, TotalDays, Days, YearDiff,
   LYear, FebInt, MOBINT, NewYear, CurrDay, CurrYear : Integer;
   Gd, Gm, i, X1, X2, Y1, Y2 : integer;
   Choice : Char;
   CheckChoice : Array[1..6] of Char;

procedure YearAgeInDays;
Begin
     LYear := 2000;
     FebInt := 28;
     TotalDays := 0;
     while CurrYear < LYear do
           Begin
                LYear := LYear - 4;
           end;
     repeat
           LYear := LYear + 1;
           NewYear := NewYear + 4
     until YOB > NewYear;
     if NewYear = YOB then
        FebInt := FebInt + 1;
     YearDiff := CurrYear - YOB + 1;
     TotalDays := (YearDiff * 365) + LYear;
end;

{---------------------------------------------------------------------------}

procedure MOBAgeInDays;
Begin
     if (MOB = 'JAN') or (MOB = 'jan') then
        Begin
             TotalDays := TotalDays + FebInt + 306;
             MOBINT := 31;
        end

     else if (MOB = 'FEB') or (MOB = 'feb') then
        Begin
             TotalDays := TotalDays + 306;
             MOBINT := FebInt;
        end

     else if (MOB = 'MAR') or (MOB = 'mar') then
        Begin
             TotalDays := TotalDays + 275;
             MOBINT := 31;
        end

     else if (MOB = 'APR') or (MOB = 'apr') then
        Begin
             TotalDays := TotalDays + 245;
             MOBINT := 30;
        end
     else if (MOB = 'MAY') or (MOB = 'may') then
        Begin
             TotalDays := TotalDays + 214;
             MOBINT := 31;
        end
     else if (MOB = 'JUN') or (MOB = 'jun') then
        Begin
             TotalDays := TotalDays + 184;
             MOBINT := 30;
        end
     else if (MOB = 'JUL') or (MOB = 'jul') then
        Begin
             TotalDays := TotalDays + 153;
             MOBINT := 31;
        end
     else if (MOB = 'AUG') or (MOB = 'aug') then
        Begin
             TotalDays := TotalDays + 122;
             MOBINT := 31;
        end
     else if (MOB = 'SEPT') or (MOB = 'sept') then
        Begin
             TotalDays := TotalDays + 92;
             MOBINT := 30;
        end
     else if (MOB = 'OCT') or (MOB = 'oct') then
        Begin
             TotalDays := TotalDays + 61;
             MOBINT := 31;
        end
     else if (MOB = 'NOV') or (MOB = 'nov') then
        Begin
             TotalDays := TotalDays + 30;
             MOBINT := 30;
        end
     else if (MOB = 'DEC') or (MOB = 'dec') then
        Begin
             TotalDays := TotalDays;
             MOBINT := 31;
        end;

     Days := MOBINT - DOB;
     TotalDays := TotalDays + Days;
end;

{---------------------------------------------------------------------------}

procedure CurrMonthAgeInDays;
Begin
     if (CurrMonth = 'JAN') or (CurrMonth = 'jan') then
        TotalDays := TotalDays

     else if (CurrMonth = 'FEB') or (CurrMonth = 'feb') then
          TotalDays := TotalDays + 30

     else if (CurrMonth = 'MAR') or (CurrMonth = 'mar') then
          TotalDays := TotalDays + FebInt + 30

     else if (CurrMonth = 'APR') or (CurrMonth = 'apr') then
          TotalDays := TotalDays + FebInt + 61

     else if (CurrMonth = 'MAY') or (CurrMonth = 'may') then
          TotalDays := TotalDays + FebInt + 92

     else if (CurrMonth = 'JUN') or (CurrMonth = 'jun') then
          TotalDays := TotalDays + FebInt + 123

     else if (CurrMonth = 'JUL') or (CurrMonth = 'jul') then
          TotalDays := TotalDays + FebInt + 153

     else if (CurrMonth = 'AUG') or (CurrMonth = 'aug') then
          TotalDays := TotalDays + FebInt + 184

     else if (CurrMonth = 'SEPT') or (CurrMonth = 'sept') then
          TotalDays := TotalDays + FebInt + 215

     else if (CurrMonth = 'OCT') or (CurrMonth = 'oct') then
          TotalDays := TotalDays + FebInt + 245

     else if (CurrMonth = 'NOV') or (CurrMonth = 'nov') then
          TotalDays := TotalDays + FebInt + 276

     else if (CurrMonth = 'DEC') or (CurrMonth = 'dec') then
          TotalDays := TotalDays + FebInt + 306;

     TotalDays := TotalDays + CurrDay;
end;

{---------------------------------------------------------------------------}

procedure DataInput;
Begin
     ClearDevice;
     SetColor(LightGreen);
     SetTextJustify ( CentreText, CentreText);
     SetTextStyle ( TriplexFont, HorizDir, 2);
     OutTextXY ( 0,0,'Enter DAY of birth> ');
     Delay(1000);
     YearAgeInDays;
     MOBAgeInDays;
     CurrMonthAgeInDays;
end;

{---------------------------------------------------------------------------}

procedure Intro;

Begin
     ClearDevice;
     SetBkColor(Black);
     SetTextJustify(centretext, centretext);
     SetTextStyle(GothicFont, HorizDir, 4);
     OutTextXY ( Succ(GetMaxX) div 2, Succ(GetMaxY) div 13,'Sketch Your');
     SetColor(LightBlue);
     SetTextStyle(TriplexFont, HorizDir, 9);
     OutTextXY ( 320, 130 ,'BIORHYTHM');
     SetColor(Green);
     SetTextStyle(GothicFont, HorizDir, 4);
     OutTextXY ( 320, 250 ,'Written and Developed by');
     SetColor(Magenta);
     SetTextStyle( GothicFont, HorizDir, 6);
     OutTextXY ( 330, 320 ,'By Paul Carroll.');
     SetColor(Yellow);
     SetTextStyle (GothicFont, HorizDir, 2);
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
     OutTextXY(320,460,'<Press ENTER to continue>');
     repeat
     until KeyPressed = True;
     Readln;
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
     SetTextStyle ( TriplexFont, HorizDir, 3);
     OutTextXY(330, 70,'----------------------------');
     SetColor (Magenta);
     OutTextXY ( 320, 110,'1) Sketch Biorhythm - Physical.    ');
     OutTextXY ( 325, 160,'2) Sketch Biorhythm - Emotional.   ');
     OutTextXY ( 314, 210,'3) Sketch Biorhythm - Intellectual.');
     OutTextXY ( 313, 260,'4) Sketch all three Biorhythms.    ');
     OutTextXY ( 313, 310,'5) Read History of Boirhythms.     ');
     OutTextXY ( 320, 360,'6) Exit Biorhythm program.         ');
     SetColor(LightBlue);
     SetTextStyle ( TriplexFont, HorizDir, 4);
     OutTextXY ( 290, 430,'Enter the number of your choice> ');
     Readln (Choice);
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
          CheckChoice[i] := CheckChoice[i];
     end;
     if Choice <> CheckChoice then
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
     Intro;
     MainMenu;
END.



