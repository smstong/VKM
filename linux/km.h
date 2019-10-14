#ifndef XQ_KM_H
#define XQ_KM_H

/**************************************************************
 *                 Keybaord Mouse emulator
 * Emulate a keboard/mouse by creating a virtual device
 * via Linux input subsystem, espically /dev/uinput
 *
**************************************************************/

#define EVENT_TYPE_MOUSE	1
#define EVENT_TYPE_KB		2

/* three buttons: [0]-LEFT, [1]-RIGHT, [2]-MIDDLE */
struct MouseEvent
{
	int btns[3]; 		/* 0-no event, 1-down, 2-up */
	int dx;			/* relative move of x */
	int dy;			/* relative move of y */
	int ds;			/* relative move of scroll */
};

struct KeyEvent
{
	int key;		/* the key changed */
	int state;		/* 0 - no event, 1 - pressed down , 2 - up*/
};


struct InputEvent
{
	int type;		/* 1 - Mouse, 2 - Keyboard */
	union {
		struct MouseEvent mouseEvent;
		struct KeyEvent keyEvent;
	} data;
};

int km_init();
void km_exit();
int km_fire(struct InputEvent * pEvent);
int km_bytes_to_event(struct InputEvent *ev, char* buf);

#endif
