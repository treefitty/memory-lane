
// This file is part of the Black Marble Chain-4 graphics library
//
//               Copyright (c) 1996 Black Marble

extern "C" {

	// Mode set functions
	void c_set_mode(void);

	// Pixel handling routines
	void c_set_pel(int x,int y,char col);
	int  c_get_pel(int x,int y);

	// Bitmap routines
	void c_put_lbm(int x,int y,char *bitmap);

	// Palette functions
	void c_set_rgb(char col,char r,char g,char b);
	void c_set_palette(char *array);

	// Vertical retrace wait
	void c_wait_vr(void);
}