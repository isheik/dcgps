#include <gps.h>
#include <math.h>
#include "gpsprint.h"
#include "gpsdclient.h"

static enum deg_str_type deg_type = deg_dd;

void gps_print(struct gps_data_t *gpsdata)
{
  int i = 0;
  int j = 0;
  // Print gps
  for (i = 0; i < MAXCHANNELS; i++)
  {
    usedflags[i] = false;
    for (j = 0; j < gpsdata->satellites_used; j++)
      if (gpsdata->skyview[j].used == gpsdata->skyview[j].PRN)
        usedflags[i] = true;
  }

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
        if (gpsdata->fix.mode >= MODE_2D && isnan(gpsdata->fix.latitude) == 0)
        {
          fprintf(stdout, "Latitude: %s %c;", deg_to_str(deg_type, fabs(gpsdata->fix.latitude)), (gpsdata->fix.latitude < 0) ? 'S' : 'N');
          fflush(stdout);
        }
        else
          printf("n/a\n");
      }
    }
  }
  else
  {
    fprintf(stderr, "No satellite found\n");
  }
}
