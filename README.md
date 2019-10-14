VKM ( Virtual Keyboard & Mouse )

# What's VKM?
VKM is a keyboard/mouse sharing app. 

# Why did I develop VKM?
I am a Synergy user. (yes, I paid for the software). In Oct, 2019,
I updated my notebook from CentOS 7 to CentOS 8. Unfortunately, 
Synergy didn't support CentOS 8 at this time.

While waiting Synergy's new release, I just decided to develop VKM
for my temporary personal usage.

# How does VKM work?
VKM has two parts, server side and client side.
The server side runs on a Windows with mouse/keyboard installed.
The client side runs on a Linux without mouse/keyboard installed.
Then the mouse/keyboard can be shared between those two systems.

# The basic logic underhood
On the server side, the VKM capture windows RAW INPUT of keyboard
and mouse, then send those input events to the client side via UDP.

On the client side, the VKM receives the input events via UDP, then
transfer them into Linux input events and regenerate them via Linux
"/dev/uinput". 

# Does VKM work on any Linux distribution?
Yes. As it depends only on Linux kernel's input subsystem, VKM does
NOT depend on any GUI systems. So it works well on XWindow/Wayland.

# Does VKM support Mac?
No.

# How long will VKM stay active?
As long as Synergy works on CentOS8/Wayland, I will not continue 
VKM. 
