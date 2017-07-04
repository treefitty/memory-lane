//**************************************************************
//
// Procedure definitions for KEY.ASM keyboard handling routines.
//   KEYFLAGS has been made global so that it can be accessed
//  by in the _KEYCODE segment and any other file (for polling)
//
//         Copyright (c) 1995 Black Marble Productions
//
//**************************************************************

extern "C" {

	void set_kb(void);
	void reset_kb(void);

}
