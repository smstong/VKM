vkm: Virtual Keyboard Mouse

# Server side (who owns the mouse/keyboard)
* Windows

# Client side (who doesn't have real mouse/keyboard)
* Linux

# How does it do?

The Windows server app handles WM_INPUT, and transfer the Keyboard/Mouse
event to Linux client.

The Linux client creates a virtual Keyboard/Mouse via /dev/uinput. Then it
generates the same event as what it received from the server side.
