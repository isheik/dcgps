#include <gps.h>
#include <math.h>
#include <time.h>
#include "gpsprint.h"
//#include "gpsdclient.h"

//static enum deg_str_type deg_type = deg_dd;
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
// bool usedflags[MAXCHANNELS];
time_t update_time;
struct tm *update_time_st;
char date_str[256];

void gps_print(struct gps_data_t *gpsdata)
{
  int i = 0;
  // int j = 0;
  // Print gps
  // for (i = 0; i < MAXCHANNELS; i++)
  // {
  //   usedflags[i] = false;
  //   for (j = 0; j < gpsdata->satellites_used; j++)
  //     if (gpsdata->skyview[j].used == gpsdata->skyview[j].PRN)
  //       usedflags[i] = true;
  // }

  if (gpsdata->satellites_visible != 0) // Some satellites are visible {
  {
    for (i = 0; i < MAX_POSSIBLE_SATS; i++)
    {
      if (i < gpsdata->satellites_visible)
      {
        // Print out parameters for each visible satellite {
        // fprintf (stdout, "PRN: %3d gpsdata->PRN[i],
        // .............................
        // .............................);
        fprintf(stdout, "PRN: %3d ", gpsdata->skyview[i].PRN);
        fprintf(stdout, "Elevation: %02d ", gpsdata->skyview[i].elevation);
        fprintf(stdout, "Azimuth: %03d ", gpsdata->skyview[i].azimuth);
        fprintf(stdout, "SNR: %02d ", (int)gpsdata->skyview[i].ss);
        fprintf(stdout, "Used %c\n", (gpsdata->skyview[i].used) ? 'Y' : 'N');
        // fprintf(stdout, "PRN: %3d Elevation: %02d Azimuth: %03d SNR: %02f Used %c\n",
        // gpsdata->skyview[i].PRN, gpsdata->skyview[i].elevation, gpsdata->skyview[i].azimuth, gpsdata->skyview[i].ss, (gpsdata->skyview[i].used) ? 'Y' : 'N');
      }
    }
  }
  else
  {
    fprintf(stderr, "No satellite found\n");
  }

  if (gpsdata->fix.mode >= MODE_2D && isnan(gpsdata->fix.latitude) == 0 && isnan(gpsdata->fix.longitude) == 0 && isnan(gpsdata->fix.time) == 0)
  {
    update_time = (time_t)gpsdata->fix.time;
    update_time_st = gmtime(&update_time);

    // fprintf(stdout, "%04d-%02d-%02dT%02d:%02d:%02d.000Z; ",
    // update_time_st.tm_year, update_time_st.tm);
    strftime(date_str, 256, "%Y-%m-%dT%H:%M:%S.000Z; ", update_time_st);
    fputs(date_str, stdout);
    fprintf(stdout, "Latitude: %f %c ; ", gpsdata->fix.latitude, (gpsdata->fix.latitude < 0) ? 'S' : 'N');
    fprintf(stdout, "Longitude: %f %c\n\n", gpsdata->fix.longitude, (gpsdata->fix.longitude < 0) ? 'W' : 'E');
    // fprintf(stdout, "%f ; Latitude: %f %c ; Longitude: %f %c",
    //         (double)update_time, gpsdata->fix.latitude,
    //         (gpsdata->fix.latitude < 0) ? 'S' : 'N',
    //         gpsdata->fix.longitude, (gpsdata->fix.longitude < 0) ? 'W' : 'E');
    fflush(stdout);
  }
  else
  {
    printf("n/a\n\n");
  }
}
