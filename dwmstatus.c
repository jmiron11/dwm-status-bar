/* made by Justin Miron on 2015-08-05.
**
** Compile with:
** gcc -Wall -pedantic -lX11 dwmstatus.c -o dwmstatus
** 
** mv dwmstatus /usr/local/bin/
** 
** add dwmstatus& to ~/.xinitrc
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include <X11/Xlib.h>

#include "getstatus.h"

static Display *dpy;

void setstatus(char *str);
void openDisplay(void);

int main(void)
{
	char * status;
	int bat0;
	long volume;
	char * timeString;
	char * wifiStatus;
	char batStatus;

	openDisplay();

	if((status = (char *)malloc(sizeof(char)*200)) == NULL)
	{
		fprintf(stderr, "Error malloc'ing status string");
		exit(1);
	}

	while(!sleep(1))
	{
		bat0 = getBattery();
		batStatus = getBatteryStatus();
		timeString = getTimeString();
		volume = getVolume();
		wifiStatus = getWifiStatus();

		snprintf(status, 200, "WiFi: %s | Volume: %ld | %c%d%% | %s", wifiStatus, volume, batStatus, bat0, timeString);

		free(timeString);
		free(wifiStatus);
		setstatus(status);
	}

	free(status);
	XCloseDisplay(dpy);

	return 0;
}

void setstatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

void openDisplay(void)
{
	if (!(dpy = XOpenDisplay(NULL))){
    	fprintf(stderr, "Cannot open display.\n");
		exit(1);
	}
}