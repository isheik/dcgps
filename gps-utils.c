/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.c - Utility program for dcgps
--
-- PROGRAM: gps-utils.o
--
-- FUNCTIONS:
-- void *gps_loop(void *arg_gpsdata)
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Keishi Asai / Kiaan Castillo
--
-- PROGRAMMER: Keishi Asai 
--
-- NOTES:
-- Provide utility functions for dcgps.
----------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <gps.h>
#include "gps-utils.h"
#include "gpsprint.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: gps_loop
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Keishi Asai / Kiaan Castillo
--
-- PROGRAMMER: Keishi Asai
--
-- INTERFACE: void *gps_loop(void *arg_gpsdata) 
--                          void *arg_gpsdata: A report stream for GPS
--
-- RETURNS: void
--
-- NOTES:
-- A callback function executed in a thread. Wait for data from gpsd.
-- If data were received before timeout, read data and call a function to display it on console. 
----------------------------------------------------------------------------------------------------------------------*/
void *gps_loop(void *arg_gpsdata)
{
  int wait_clicks = 0;
  struct gps_data_t *gpsdata;
  gpsdata = (struct gps_data_t *)arg_gpsdata;

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
