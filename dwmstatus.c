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
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>

static Display *dpy;

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

int getBattery(void)
{
	FILE *fd;
	int batterylife;
	fd = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if(fd == NULL)
	{
		fprintf(stderr, "Error opening capacity");
	}
	fscanf(fd, "%d", &batterylife);
	fclose(fd);
	
	return batterylife;
}

char * getTimeString(void)
{
	char * retTime;
	time_t rawTime;
	struct tm * timeinfo;


	if((retTime = (char *)malloc(sizeof(char)*10)) == NULL)
	{
		fprintf(stderr, "Error allocating retTime string");
		exit(1);
	}

	time(&rawTime);
	timeinfo = localtime(&rawTime);

	if(timeinfo->tm_min < 10)
		snprintf(retTime, 10, "%d:0%d", timeinfo->tm_hour, timeinfo->tm_min);
	else
		snprintf(retTime, 10, "%d:%d", timeinfo->tm_hour, timeinfo->tm_min);
	
	return retTime;
	
}


int main(void)
{
	char * status;
	int bat0;
	char * timeString;

	openDisplay();

	if((status = (char *)malloc(sizeof(char)*200)) == NULL)
	{
		fprintf(stderr, "Error malloc'ing status string");
		exit(1);
	}

	while(!sleep(1))
	{
		bat0 = getBattery();
		timeString = getTimeString();

		snprintf(status, 200, "%d%% | %s", bat0, timeString);

		free(timeString);
		setstatus(status);
	}

	free(status);
	XCloseDisplay(dpy);

	return 0;
}