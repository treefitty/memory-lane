//
// This file is part of the Black Marble Text mode library
//
//            Copyright (c) 1996 Black Marble

extern void __cdecl t_put_char(int x,int y,char ch,char attr);
extern void __cdecl t_attr(int x,int y,char attr);
extern int  __cdecl t_get_char(int x,int y);
extern void __cdecl t_set_80x50(void);
extern void __cdecl t_put_scr(char *scr);
extern void __cdecl t_set_rgb(char col,char r,char g,char b);
extern void __cdecl t_hide_cursor(void);
extern void __cdecl t_show_cursor(void);

