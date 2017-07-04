        program PaulMouseDemo;

        uses Mouse2, Graph, Crt, Dos;

        const
             HorizDir = 0;
        var
           Mouse, RBut, LBut, BBut: Boolean;
           Gd, Gm, simp, Continue : integer;
           Leftset,RightSet,TopSet,Bottomset,
           X, Y, i, mX, mY : Word;

        {$I man.pas}

        {--------------------------------------------------------------------------}

        Procedure CheckGraph;
        begin
             Gd := Detect;
             InitGraph (Gd,Gm,'C:\tp\bgi');
             If GraphResult <> GrOk then
                Halt(1);
        end;

        {--------------------------------------------------------------------------}

        procedure CheckMouse;
        begin
             If IsMouseInstalled = True then
                begin
                     CloseGraph;
                     clrscr;
                     Write ('Sorry this program requires a mouse.');
                     Delay (2000);
                     Halt(1);
                end;
        end;

        {--------------------------------------------------------------------------}

        procedure DrawBoxes;

        var
           BoxX1, BoxX2, BoxY1, BoxY2 : Integer;

        begin
             SetColor (LightBlue);
             BoxX1 := 50;
             BoxY1 := 100;
             BoxX2 := 80;
             BoxY2 := 130;
             i := 0;
             for i := 1 to 3 do
                 begin
                      SetfillStyle (9,LightGreen);
                      Bar (BoxX1 + 8, BoxY1 + 8, BoxX2 - 8, BoxY2 - 8);
                      SetColor(LightBlue);
                      rectangle (BoxX1,BoxY1,BoxX2,BoxY2);
                      BoxY1 := BoxY1 + 100;
                      BoxY2 := BoxY2 + 100;
                 end;
             simp := InstallUserFont ('simp');
             if GraphResult <> GrOk then
                begin
                     CloseGraph;
                     write('Error when installing fonts.');
                     halt(1);
                end;
             SetTextStyle(simp,horizdir,2);
             SetColor(Yellow);
             OutTextXY(53,65,'(1)');
             OutTextXY(53,165,'(2)');
             OutTextXY(53,265,'(3)');
        end;

        {---------------------------------------------------------------------------}

        begin
             CheckMouse;
             CheckGraph;
             ClearDevice;
             SetFillStyle(9,DarkGray);
             Bar(0,40,130,390);
             ResetMouse;
             LeftSet := 0;
             RightSet := 125;
             TopSet := 40;
             BottomSet := 375;
             SetColRange (Leftset,RightSet);
             SetRowRange (Topset,BottomSet);
             DrawBoxes;
             manMouse;
             ShowMouse;
             X := 115;
             Y := 45;
             Continue:= 0;
             MoveMouseXY (X, Y);
             repeat
                   TrackMouse(mX, mY, LBut, RBut, BBut);
                   if LBut then continue := 1;
                   if KeyPressed then continue := 1;
             Until Continue = 1;
             HideMouse;
             CloseGraph;
             ClrScr;
        end.
