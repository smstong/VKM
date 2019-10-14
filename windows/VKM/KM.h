#pragma once
void KMInit();
void KMExit();
void KMSendMouseEvent(int x, int y, int ds, int btn[3]);
void KMSendKeyEvent(int key, int state);