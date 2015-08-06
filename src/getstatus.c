#include "getstatus.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <alsa/asoundlib.h> //alsa mixer functions

int getBattery(void)
{
	FILE *fd;
	int batterylife;
	fd = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if(fd == NULL)
	{
		fprintf(stderr, "Error opening capacity");
		exit(1);
	}
	fscanf(fd, "%d", &batterylife);
	fclose(fd);
	
	return batterylife;
}


// + for charging, - for discharing, = for full
char getBatteryStatus(void)
{
	FILE *fd;
	char batteryStatus;
	fd = fopen("/sys/class/power_supply/BAT0/status", "r");
	if(fd == NULL)
	{
		fprintf(stderr, "Error opening battery status");
		exit(1);
	}
	fscanf(fd, "%c", &batteryStatus);
	fclose(fd);
	switch(batteryStatus)
	{
		case 'D': 
			return '-';
		case 'C':
			return '+';
		case 'F':
			return '=';
		default:
			return '?';
	}
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