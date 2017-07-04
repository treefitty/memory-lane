        program WindowDemo;

        uses App, Drivers, Views, Menus, Objects, Dialogs;

        const
             MaxLines = 100;
             
             WinCount: Integer = 0;
             
             cmSave = 101;
             cmSaveAs = 102;
             cmOpen = 103;
             cmNewDirWindow = 104;
             cmAddEntry = 105;
             cmDeleteEntry = 106;
             cmReplaceEntry = 107;
             cmEditEntry = 108;
             cmOpenText = 109;
             cmSaveText = 110;
             cmRenameFile = 111;
             cmExitProg = 112;

        type
            TDrawBuffer = array [0..MaxViewWidth  - 1] of Word;

            TDirApp = object(TApplication)                
                procedure InitMenuBar; virtual;
                procedure InitStatusLine; virtual;
                procedure HandleEvent(var Event: TEvent); virtual;
                procedure NewTextWindow;
                procedure NewDirWindow;
                procedure ReadFile; virtual;
                procedure DoneFile; virtual;
                procedure ExitBox; virtual;
            end;

            PTextWindow = ^TTextWindow;
            TTextWindow = object(TWindow)
                constructor Init (var Bounds: TRect; WindowTitle: String;WinNo: Word);
            end;

            PNewDirWindow = ^TNewDirWindow;
            TNewDirWindow = object(TWindow)
            end;

            PTextInterior = ^TTextInterior;
            TTextInterior = object(TView)
                constructor Init (var Bounds: TRect);
                procedure Draw; virtual;
            end;

            {PDialog = ^TExitBox;
            TExitBox = object(TDialog)
            end;}

            var
               TextFile, TextContents: String;
               F: Text;
               LineCount: Integer;
               Lines : array [0..MaxLines - 1] of PString;
               TextRead, ExitButtonYes : Boolean;

            { Init MenuBar }
            procedure TDirApp.InitMenuBar;
            var
               R: TRect;
            begin
                 GetExtent(R);
                 R.B.Y := R.A.Y + 1;
                 MenuBar := New(PMenuBar,Init(R,NewMenu(
                     NewSubMenu('~F~ile',hcNoContext,NewMenu(
                         NewItem('~S~ave','F1',kbF1,cmSave,hcNoContext,
                         NewItem('~S~ave Entries As','F2',kbF2,cmSaveAs,hcNoContext,
                         NewItem('~O~pen File','F3',kbF3,cmOpen,hcNoContext,
                         NewItem('~N~ew Directory','F4',kbF4,cmNewDirWindow,hcNoContext,
                         NewLine(
                         NewItem('~E~xit','Alt-X',kbAltX,cmExitProg,hcNoContext,
                         nil))))))),
                     NewSubMenu('~E~dit Commands',hcNoContext,NewMenu(
                         NewItem('~A~dd Entry','F5',kbF5,cmAddEntry,hcNoContext,
                         NewItem('~D~elete Entry','F6',kbF6,cmDeleteEntry,hcNoContext,
                         NewItem('~R~eplace Entry','F7',kbF7,cmReplaceEntry,hcNoContext,
                         NewItem('~E~dit Entry','F8',kbF8,cmEditEntry,hcNoContext,
                         nil))))),
                     NewSubMenu('~W~indow Views',hcNoContext,NewMenu(
                         NewItem('~Z~oom','F9',kbF9,cmZoom,hcNoContext,
                         NewItem('~C~lose','F10',kbF10,cmClose,hcNoContext,
                         nil))),
                     NewSubMenu('~T~ext Viewer',hcNoContext,NewMenu(
                         NewItem('~O~pen File','Alt-F1',kbAltF1,cmOpenText,hcNoContext,
                         NewItem('~S~ave File','Alt-F2',kbAltF2,cmSaveText,hcNoContext,
                         NewItem('~R~ename File','Alt-F3',kbAltF3,cmRenameFile,hcNoContext,
                         nil)))),
                     nil))
                 )))));
            end;

            { Init StatusLine }
            procedure TDirApp.InitStatusLine;
            var
               R: TRect;
            begin
                 GetExtent(R);
                 R.A.Y := R.B.Y - 1;
                 StatusLine := New(PStatusLine,Init(R,
                     NewStatusDef(0,$FFFF,
                         NewStatusKey('~E~xit Alt-X',kbAltX,cmQuit,
                         NewStatusKey('~S~ave F1',kbF1,cmSave,
                         NewStatusKey('~N~ew F4',kbF4,cmNewDirWindow,
                         NewStatusKey('~A~dd F5',kbF5,cmAddEntry,
                         NewStatusKey('~D~elete F6',kbF6,cmDeleteEntry,
                         NewStatusKey('~E~dit F8',kbF8,cmEditEntry,
                         NewStatusKey('~C~lose Alt-F3',kbAltF3,cmClose,
                         nil))))))),
                     nil)
                 ));
            end;
      {-----------------------------------------------------------------------}
            { HandleEvents }
            procedure TDirApp.HandleEvent(var Event: TEvent);
            begin
                 TApplication.HandleEvent(Event);
                 if Event.What = evCommand then
                    begin
                         case Event.Command of
                              cmOpenText     : begin
                                                    ReadFile;
                                                    NewTextWindow;
                                               end;

                              cmNewDirWindow : NewDirWindow;
                              cmExitProg     : ExitBox;
                              else Exit;
                         end;
                         ClearEvent(Event);
                    end;
                 end;
      {------------------------------------------------------------------------}
            { Create Text Viewer Window }
            procedure TDirApp.NewTextWindow;
            var
               Window: PTextWindow;
               R: TRect;
               X1 ,X2 ,Y1 ,Y2 : Integer;

            begin
                 X1 := 20; X2 := 60; Y1 := 0; Y2 := 20;
                 Inc(WinCount);
                 R.Assign(X1,Y1,X2,Y2);
                 Window := New(PTextWindow,Init(R,'Text File Viewer',WinCount));
                 DeskTop^.Insert(Window);
            end;

            { TextWindow Init }
            constructor TTextWindow.Init (var Bounds: TRect; WindowTitle: String;WinNo: Word);
            var
               TextInterior: PTextInterior;
               S: String[3];
            begin
                 Str(WinNo,S);
                 TWindow.Init(Bounds,WindowTitle + ' ' + S,wnNoNumber);
                 GetClipRect(Bounds);
                 Bounds.Grow(-1,-1);
                 TextInterior := New(PTextInterior, Init(Bounds));
                 Insert(TextInterior);
            end;

            { TextWindow Interior Init }
            constructor TTextInterior.Init (var Bounds: TRect);
            begin
                 TView.Init (Bounds);
                 GrowMode := gfGrowHiX + gfGrowHiY;
            end;

            { TextWindow Draw }
            procedure TTextInterior.Draw;
            var
               Y: Integer;
               Buff: TDrawBuffer;
               Color: Byte;
            begin
                 Color := GetColor(2);
                 for Y := 0 to Size.Y - 1 do
                     begin
                          MoveChar(Buff,' ',Color,Size.X);
                          if (Y<LineCount) and (Lines[Y] <> nil) then
                             MoveStr (Buff,Copy(Lines[Y]^,1,Size.X),Color);
                          WriteLine (0,Y,Size.X,1,Buff);
                     end;
            end;
      {--------------------------------------------------------------------------}
            { Create New Telephone Directory Window }
            procedure TDirApp.NewDirWindow;
            var
               Window: PNewDirWindow;
               R: TRect;
               X1, X2, Y1, Y2: Integer;
            begin
                 X1 := 20;X2 := 60;Y1 := 0;Y2 := 20;
                 Inc(WinCount);
                 R.Assign(X1,Y1,X2,Y2);
                 Window := New(PNewDirWindow,Init(R,'Directory Window',WinCount));
                 DeskTop^.Insert(Window);
            end;
      {-------------------------------------------------------------------------}
            procedure TDirApp.ReadFile;
            begin
                 LineCount := 0;
                 TextFile := ('c:\tp\dumb.dat');
                 Assign (F, TextFile);
                 Reset (F);
                 if IOResult <> 0 then
                    Halt(1);
                 While not eof(F) do
                       begin
                            Readln (F, TextContents);
                            Lines [LineCount] := NewStr(TextContents);
                            Inc(LineCount);
                            TextRead := True;
                       end;
                 Close(F);                
            end;

            procedure TDirApp.DoneFile;
            var
               i: Integer;
            begin
                 if TextRead then
                    begin
                         for i := 0 to LineCount - 1 do
                         if Lines[i] <> nil then DisposeStr(Lines[i]);
                    end;
                 TextRead := False;
            end;
      {-------------------------------------------------------------------------}
            procedure TDirApp.ExitBox;
            var
               ExBox: PDialog;
               R: TRect;
               Control: Word;
               B: PView;
            begin
                 R.Assign (23,5,59,15);
                 ExBox := New(PDialog,Init(R,'??? Exit ???'));
                 with ExBox^ do
                      begin
                           R.Assign (4,3,33,4);
                           Insert (New(PStaticText, Init(R,'Do You Really Want To Exit??')));
                           R.Assign (5,6,15,8);
                           Insert (New(PButton,Init(R,'~Y~es',cmQuit,bfDefault)));
                           R.Assign (20,6,30,8);
                           Insert (New(PButton,Init(R,'~N~o',cmCancel,bfNormal)));
                      end;
                 Control := DeskTop^.ExecView(ExBox);
                 if Control = cmCancel then
                    begin
                         Dispose(ExBox,Done);                         
                         exit;
                    end
                 else if Control = cmQuit then
                      begin
                           Dispose(ExBox,Done);
                           exit;
                      end;
                 Dispose(ExBox,Done);
            end;

            var
               DirApp: TDirApp;
            begin
                 DirApp.Init;
                 DirApp.Run;
                 DirApp.Done;
            end.