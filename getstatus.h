/*
** Functions that get the status of the system for the dwm status bar.
 */


#ifndef GETSTATUS_H_
#define GETSTATUS_H_

int getBattery(void);
char getBatteryStatus(void);
long getVolume(void);
char * getTimeString(void);
char * getWifiStatus(void);

#endif