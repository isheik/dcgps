/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: dcgps.c - A dumb gps console
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- int main()
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Keishi Asai
--
-- PROGRAMMER: Keishi Asai
--
-- NOTES:
-- A dumb gps console. Display report data received from sattelites.
----------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <gps.h>
#include <errno.h>
#include "dcgps.h"
#include "gps-utils.h"

static struct gps_data_t gpsdata;
unsigned int flags = WATCH_ENABLE;

/* Data source structure */
struct fixsource_t
{
  char *spec;
  char *server;
  char *port;
  char *device;
};

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 4, 2018
--
-- REVISIONS:
--
-- DESIGNER: Keishi Asai
--
-- PROGRAMMER: Keishi Asai
--
-- INTERFACE: int main() 
--
-- RETURNS: int
--
-- NOTES:
-- Main function for dcgps. It established a connection with gpsd and open a report stream.
-- Then, create a thread to read received data. The main function also listen to user keyboard input
-- to quit the program.
----------------------------------------------------------------------------------------------------------------------*/
int main()
{
  static struct fixsource_t source;
  source.server = LOCAL_HOST;
  source.port = DEFAULT_GPSD_PORT;
  source.device = NULL;

  // Connect to gpsd
  if (gps_open(source.server, source.port, &gpsdata) != 0)
  {
    (void)fprintf(stderr, "dcgps: no gpsd running or network error: %d, %s\n",
                  errno, gps_errstr(errno));
    exit(EXIT_FAILURE);
  }
  if (source.device != NULL)
  {
    flags |= WATCH_DEVICE;
  }

  // Open report stream
  (void)gps_stream(&gpsdata, flags, source.device);

  // Create a thread to read incoming data
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, gps_loop, &gpsdata);

  // Listen to user keyboard input
  char c;
  while (1)
  {
    c = getchar();

    switch (c)
    {
      /* Quit the program */
    case 'q':
      (void)gps_close(&gpsdata);
      exit(0);
      break;
    default:
      break;
    }
  }

  return 0;
}
