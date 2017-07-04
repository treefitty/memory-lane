
#include "mouse256.h"

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

/* colours in this 256-colour palette */
#define BLACK		0
#define WHITE		1

#define GREY_1		2
#define GREY_2		3
#define GREY_3		4
#define GREY_4		5
#define GREY_5		6
#define GREY_6		7
#define GREY_7		8
#define GREY_8		9
#define GREY_9		10
#define TITLE_1	11
#define TITLE_2	12
#define TITLE_3	13
#define TITLE_4	14
#define TITLE_5	15

#define TITLE_HEIGHT 15

#define BTN_UP			1
#define BTN_DOWN		2
#define BTN_PRESSED		0
#define BTN_NOT_PRESSED 	1
#define BTN_NO_EXECUTE 	0
#define BTN_EXECUTE 	1
#define BTN_NO_ARROW	0
#define BTN_ARROW_UP 	1
#define BTN_ARROW_DOWN	2

#define LIST_UP	1
#define LIST_DOWN	2

#define MAX_ELEMS		15
#define MAX_LIST_ELEMS 	10
#define MAX_FRAMES       5

typedef struct {

	char *text;
	int value;

} elem_type;

typedef struct {

	int x1,y1;
	int x2,y2;

} frame_type;

// Function prototypes
int  in_rect(rect *a_rect,int x,int y);

// Class definitions
class WINDOW {

private:
	char *title_text;
	rect frame;
	frame_type frames [MAX_FRAMES];
	int num_frames;

public:
	WINDOW(int win_x,int win_y,int width,int height,char *text);
	void put(void);
	void def_frame(int x1,int y1,int x2,int y2);

};

class BUTTON {

private:
	rect frame;
	char *button_text;
	char btn_type;
	char arrow_dir;

public:
	char state;
	void (*execute)(void);

	BUTTON(int but_x,int but_y,int w,int h,char *text,char type);
	BUTTON(int but_x,int but_y,int w,int h,char dir,char type);
	void put(void);
	int  pressed(void);
	void change_state(void);
	int  in_bounds(int x,int y);
};

class DROP_DOWN {

private:
	char *back_ground;
	char state;
	rect box_frame;
	rect list_frame;
	BUTTON list_btn;
	int	num_elems;
	char select_list[MAX_ELEMS];
	int  selected_elem;
	void list_dropped(void);

public:
	elem_type elems[MAX_ELEMS];

	DROP_DOWN(int x,int y,int w);
	void put(void);
	void add_elem(char *text,int value);
	void change_state(void);
	void	in_bounds(int x,int y);
};

class LIST_BOX {

private:
	rect frame;
	int	num_elems;
	BUTTON list_btn1,list_btn2;
	char select_list[MAX_ELEMS];
	int  selected_elem;

public:
	elem_type elems[MAX_ELEMS];

	LIST_BOX(int x,int y,int w,int rows);
	void put(void);
	void pressed(void);
	void add_elem(char *text,int value);
	void in_bounds(int x,int y);
};


// GUI function defs
int in_rect(rect *a_rect,int x,int y);