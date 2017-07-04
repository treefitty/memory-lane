//****************************************************
//
// Procedure prototypes for C callable mode-x routines
//             as found in X_LIB.ASM
//
//    Copyright (c) 1995 Black Marble Productions
//
//****************************************************


//------------- Procedure Prototypes ----------------
//
extern "C" {

	// Mode-x setting routines
	void x_setmode(void);
	void x_textmode(void);
	void x_dword_mode(void);
	void x_byte_mode(void);
	void x_textmode(void);

	// Vertical retrace wait
	void x_wait_vr(void);

	// Pixel functions
	void x_set_pel(int x,int y,char col);
	char x_get_pel(int x,int y);
	void x_set_byte(int x,int y,char col);
	char x_get_byte(int x,int y);
	void x_set_write_plane(int plane);
	void x_set_read_plane(int plane);

	void x_clr_page(void);

	// Misc VGA tweaking functions
	void x_set_hpp(char c_ofs);
	void x_screen_off(void);
	void x_screen_on(void);

	// Palette functions
	void x_set_rgb(char col,char r,char g,char b);
	void x_set_palette(char *array);

	// Screen altering functions
	void x_scr_width(int width);
	void x_scan_start(int ofs);

	// Bitmap routines
	void x_put_pbm(int x,int y,char *bitmap);

	// Double buffering routines
	void x_set_double_buffer(void);
	void x_flip_page(void);
	void x_set_active_page(char page);
}
