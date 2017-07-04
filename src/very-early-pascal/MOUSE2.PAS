        unit Mouse2;

        Interface

        uses Dos;

        Procedure CallMouse (var M1,M2,M3,M4 : Word);
        Procedure ShowMouse;
        Procedure HideMouse;
        Procedure ResetMouse;
        Procedure SetColRange (LeftSet, RightSet : Word);
        Procedure SetRowRange (TopSet, BottomSet : Word);
        Procedure TrackMouse (var X, Y : Word; LButton, RButton, BButton : Boolean);
        Procedure MoveMouseXY (var X, Y : Word);
        Function IsMouseInstalled : Boolean;

        {---------------------------------------------------------------------------}

        Implementation

        var
           M1,M2,M3,M4 : Word;

        {---------------------------------------------------------------------------}

        procedure CallMouse (var M1,M2,M3,M4 : Word);

        var
           Regs :  Registers;

        begin
             with Regs do
                  begin
                       AX := M1; BX := M2; CX := M3; DX := M4
                  end;
             Intr($33,Regs);
             with Regs do
                  begin
                       M1 := AX; M2 := BX; M3 := CX; M4 := DX
                  end
        end;

        {---------------------------------------------------------------------------}

        procedure ShowMouse;
        begin
             M1 := 1;
             CallMouse(M1,M2,M3,M4);
        end;

        {---------------------------------------------------------------------------}

        procedure HideMouse;
        begin
             M1 := 2;
             CallMouse (M1,M2,M3,M4);
        end;

        {---------------------------------------------------------------------------}

        procedure ResetMouse;
        begin
             M1 := 0;
             CallMouse (M1,M2,M3,M4);
        end;

        {---------------------------------------------------------------------------}

        procedure SetColRange (LeftSet, RightSet : Word);
        begin
             M1 := 7;
             M3 := LeftSet;
             M4 := RightSet;
             CallMouse (M1,M2,M3,M4);
        end;

       {---------------------------------------------------------------------------}

       procedure SetRowRange (TopSet, BottomSet : Word);
       begin
            M1 := 8;
            M3 := TopSet;
            M4 := BottomSet;
            CallMouse(M1,M2,M3,M4);
       end;

       {---------------------------------------------------------------------------}

       procedure TrackMouse (var X, Y : Word; LButton, RButton, BButton : Boolean);
       begin
            M1 := 3;
            CallMouse (M1,M2,M3,M4);
            M3 := X;
            M4 := Y;     
            If (M2 and $01) = $01 then LButton := True
               else LButton := False;
            If (M2 and $02) = $02 then RButton := True
               else RButton := False;
            If (M2 and $04) = $03 then BButton := True
               else BButton := False;
       end;

       {---------------------------------------------------------------------------}

       procedure MoveMouseXY (var X, Y : Word);
       begin
            M1 := 4;
            M3 := X;
            M4 := Y;
            CallMouse (M1,M2,M3,M4);
       end;

       {---------------------------------------------------------------------------}

       Function IsMouseInstalled : Boolean;
       type
           BytePtr = ^Byte;

       var
          TestVector : BytePtr;

       begin
            GetIntVec(51,Pointer(TestVector));
            If (TestVector = Nil) or (TestVector^ = $CF) then
               IsMouseInstalled := True
            else IsMouseInstalled := False;
       end;

       {---------------------------------------------------------------------------}
       begin
       end.

