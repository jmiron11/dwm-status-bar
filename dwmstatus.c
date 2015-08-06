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
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <X11/Xlib.h>
#include <alsa/asoundlib.h>

static Display *dpy;

void setstatus(char *str);
void openDisplay(void);
int getBattery(void);
long getVolume(void);
char * getTimeString(void);
char * getWifiStatus(void);

int main(void)
{
	char * status;
	int bat0;
	long volume;
	char * timeString;
	char * wifiStatus;

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
		volume = getVolume();
		wifiStatus = getWifiStatus();

		snprintf(status, 200, "WiFi: %s | Volume: %ld | %d%% | %s", wifiStatus, volume, bat0, timeString);

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

long getVolume(void)
{
	snd_mixer_t* handle;
    snd_mixer_elem_t* elem;
    snd_mixer_selem_id_t* sid;
	const char *card = "default";
	const char *selem_name = "Master";

	long retVolume;

	snd_mixer_open(&handle, 0);	
	snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);


    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    elem = snd_mixer_find_selem(handle, sid);
    snd_mixer_selem_get_playback_volume(elem, 0, &retVolume);

    return retVolume;
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

char * getWifiStatus(void)
{
	FILE *pipe;
	char buffer[64];
	char * status = (char *)malloc(sizeof(char)*30);
	
	pipe = popen("netctl-auto current", "r");
	if(!pipe){
		status = "Error";
		return status;
	}
	fgets(buffer, 64, pipe); // should not exceed 64
	int len = strlen(buffer);
	buffer[len-1] = '\0';

    if(strlen(buffer) == 0)
    {
    	strcpy(status, "Disconnected");
    }
    else
    {
    	strcpy(status, buffer);
    }

    pclose(pipe);
    return status;
}