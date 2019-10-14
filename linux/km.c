#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include "km.h"
#include "key.h"

static int fd_uinput = -1;
static int create_km(int fd);
static int delete_km(int fd);

/* module initiator */
int km_init()
{
	fd_uinput = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd_uinput == -1) {
		printf("open(/dev/uinput) failed.\n");
		return -1;
	}
	create_km(fd_uinput);

	return 0;
}

/* module destructor */
void km_exit()
{
	if(-1 != fd_uinput) {
		delete_km(fd_uinput);
		close(fd_uinput);
	}
}

/* simulate input event  */
static void emit(int fd, int type, int code, int val)
{
	struct input_event ie;
	ie.type = type;
	ie.code = code;
	ie.value = val;
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));
}

/* create a virtual mouse/keyboard device  */
static int create_km(int fd)
{
	struct uinput_setup usetup;

	/* enable mouste buttons */
	ioctl(fd, UI_SET_EVBIT, EV_KEY);	/* enable mouste buttons */
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);	/* enable left button */
	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);	/* enable right button */
	ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);	/* enable middle button */

	/* enable mouse move/scroll */
	ioctl(fd, UI_SET_EVBIT, EV_REL);	/* enable mouse move and scroll */
	ioctl(fd, UI_SET_RELBIT, REL_X);	/* enable x direction move */
	ioctl(fd, UI_SET_RELBIT, REL_Y);	/* enable y direction move */
	ioctl(fd, UI_SET_RELBIT, REL_WHEEL);	/* enable scroll */

	/* enable keyboard keys */
	for (int i=0; i<256; i++) {
		ioctl(fd, UI_SET_KEYBIT, i); /* KEY_A, KEY_B, ... */
	}

	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0x1234;
	usetup.id.product = 0x5678;
	strcpy(usetup.name, "Example device");

	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);

	return 0;
}
/* delete a virtual KM */
static int delete_km(int fd)
{
	ioctl(fd, UI_DEV_DESTROY);
	return 0;
}

/* simulate mouse operation */
static int fire_mouse_event(int fd, struct MouseEvent *ev)
{
	emit(fd, EV_REL, REL_X, ev->dx);  /* move mouse */
	emit(fd, EV_REL, REL_Y, ev->dy);
	emit(fd, EV_REL, REL_WHEEL, ev->ds); /* scroll mouse */

	printf("btns[3]=%d,%d,%d\n", ev->btns[0], ev->btns[1], ev->btns[2]);

	/* press/release left, right, middle button */
	if (ev->btns[0] == 1){
		emit(fd, EV_KEY, BTN_LEFT, 1); /* 1-press, 0-up */
	}
	if (ev->btns[0] == 2){
		emit(fd, EV_KEY, BTN_LEFT, 0);
	}
	if (ev->btns[1] == 1){
		emit(fd, EV_KEY, BTN_RIGHT, 1);
	}
	if (ev->btns[1] == 2){
		emit(fd, EV_KEY, BTN_RIGHT, 0);
	}
	if (ev->btns[2] == 1){
		emit(fd, EV_KEY, BTN_MIDDLE, 1);
	}
	if (ev->btns[2] == 2){
		emit(fd, EV_KEY, BTN_MIDDLE, 0);
	}

	emit(fd, EV_SYN, SYN_REPORT, 0);	/* end of event */
	return 0;
}

/* simuate keboard operation */
static int fire_kb_event(int fd, struct KeyEvent* ev)
{
	int key = win_to_linux_key(ev->key); /* VK->KEY */
	if(key == -1) {
		return 0;
	}
	if(ev->state == 1){
		emit(fd, EV_KEY, key, 1);  /* move mouse */
	} else if(ev->state == 2){
		emit(fd, EV_KEY, key, 0);  /* move mouse */
	}else{
		/* wrong state */
	}
	emit(fd, EV_SYN, SYN_REPORT, 0);	/* end of event */
	return 0;
}

/* simuate Keyboard/Mouse event */
int km_fire(struct InputEvent * pEvent)
{
	if(pEvent->type == EVENT_TYPE_MOUSE) {
		fire_mouse_event(fd_uinput, &(pEvent->data.mouseEvent));
	} else if(pEvent->type == EVENT_TYPE_KB) {
		fire_kb_event(fd_uinput, &(pEvent->data.keyEvent));
	} else {
		return -1;
	}
	return 0;
}

#define PROTO_MOUSE "type=%d,dx=%d,dy=%d,ds=%d,btn_l=%d,btn_r=%d,btn_m=%d"
#define PROTO_KB    "type=%d,key=%d,state=%d"


/* proto parse */
int km_bytes_to_event(struct InputEvent *ev, char* buf)
{
	char* head_mouse = "type=1";
	char* head_kb = "type=2";

	if(strncmp(buf, head_mouse, strlen(head_mouse)) == 0){
		ev->type = EVENT_TYPE_MOUSE;
	}else if(strncmp(buf, head_kb, strlen(head_kb)) == 0) {
		ev->type = EVENT_TYPE_KB;
	}else{
		ev->type = -1;
		return -1;
	}
	/* parse mouse event */
	if(ev->type == EVENT_TYPE_MOUSE) {
		if(EOF == sscanf(buf, PROTO_MOUSE,
				&ev->type,
				&(ev->data.mouseEvent.dx),
				&(ev->data.mouseEvent.dy),
				&(ev->data.mouseEvent.ds),
				&(ev->data.mouseEvent.btns[0]),
				&(ev->data.mouseEvent.btns[1]),
				&(ev->data.mouseEvent.btns[2])
		      )){
			return -1;
		}
	/* parse key event */
	}else if(ev->type == EVENT_TYPE_KB) {
		if(EOF == sscanf(buf, PROTO_KB, 
				&ev->type,
				&(ev->data.keyEvent.key),
				&(ev->data.keyEvent.state)
		      )) {
			return -1;
		}
	}
	else {
		return -1;
	}

	return 0;
}
