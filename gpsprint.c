/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gpsprint.c - Print program for dcgps
--
-- PROGRAM: gpsprint.o
--
-- FUNCTIONS:
-- void gps_print(struct gps_data_t *gpsdata)
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Kiaan Castillo
--
-- PROGRAMMER: Kiaan Castillo
--
-- NOTES:
-- Provide output functions for dcgps
----------------------------------------------------------------------------------------------------------------------*/

#include <gps.h>
#include <math.h>
#include <time.h>
#include "gpsprint.h"

#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
time_t update_time;
struct tm *update_time_st;
char date_str[256];

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: gps_print
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Kiaan Castillo
--
-- PROGRAMMER: Kiaan Castillo
--
-- INTERFACE: void gps_print(struct gps_data_t *gpsdata) 
--                          struct gps_data_t *gpsdata: A report stream for GPS 
--
-- RETURNS: void
--
-- NOTES:
-- Prints data read from the report stream.
----------------------------------------------------------------------------------------------------------------------*/
void gps_print(struct gps_data_t *gpsdata)
{
  int i = 0;

  // List visible satellites
  if (gpsdata->satellites_visible != 0)
  {
    for (i = 0; i < MAX_POSSIBLE_SATS; i++)
    {
      if (i < gpsdata->satellites_visible)
      {
        fprintf(stdout, "PRN: %3d ", gpsdata->skyview[i].PRN);
        fprintf(stdout, "Elevation: %02d ", gpsdata->skyview[i].elevation);
        fprintf(stdout, "Azimuth: %03d ", gpsdata->skyview[i].azimuth);
        fprintf(stdout, "SNR: %02d ", (int)gpsdata->skyview[i].ss);
        fprintf(stdout, "Used %c\n", (gpsdata->skyview[i].used) ? 'Y' : 'N');
      }
    }
  }
  else
  {
    fprintf(stderr, "No satellite found\n");
  }

  // If a fix obtained, display received information
  if (gpsdata->fix.mode >= MODE_2D && isnan(gpsdata->fix.latitude) == 0 && isnan(gpsdata->fix.longitude) == 0 && isnan(gpsdata->fix.time) == 0)
  {
    // Display update time (UTC)
    update_time = (time_t)gpsdata->fix.time;
    update_time_st = gmtime(&update_time);
    strftime(date_str, 256, "%Y-%m-%dT%H:%M:%S.000Z; ", update_time_st);
    fputs(date_str, stdout);

    fprintf(stdout, "Latitude: %f %c ; ", gpsdata->fix.latitude, (gpsdata->fix.latitude < 0) ? 'S' : 'N');
    fprintf(stdout, "Longitude: %f %c\n\n", gpsdata->fix.longitude, (gpsdata->fix.longitude < 0) ? 'W' : 'E');

    fflush(stdout);
  }
  else
  {
    // Display update time (UTC)
    time(&update_time);
    update_time_st = gmtime(&update_time);
    strftime(date_str, 256, "%Y-%m-%dT%H:%M:%S.000Z; ", update_time_st);
    fputs(date_str, stdout);

    fprintf(stdout, "n/a\n\n");
  }
}
