#include <stdlib.h>
#include <gps.h>
#include "gps-utils.h"
#include "gpsprint.h"

void *gps_loop(void *gpsdatam)
{
  int wait_clicks = 0;
  struct gps_data_t *gpsdata;
  gpsdata = (struct gps_data_t *)gpsdatam;

  while (1)
  {
    /* wait 1/2 second for gpsd */
    if (!gps_waiting(gpsdata, 500000))
    {
      /* 240 tries at .5 Sec a try is a 2 minute timeout */
      if (240 < wait_clicks++)
      {
        (void)gps_close(gpsdata);
        exit(0);
      }
    }
    else
    {
      wait_clicks = 0;
      if (gps_read(gpsdata) == -1)
      {
        fprintf(stderr, "dcgps: socket error 4\n");
        (void)gps_close(gpsdata);
        exit(1);
      }
      else
      {
        gps_print(gpsdata);
      }
    }
  }
}
