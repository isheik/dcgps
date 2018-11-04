#include "dcgps.h"
// #include <stdio.h>
// #include <stdbool.h>
// #include <string.h>
// #include <math.h>
// #include <curses.h>
// #include <time.h>
// #include <signal.h>
// #include <unistd.h>
// #include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <gps.h>
#include "gps-utils.h"

static struct gps_data_t gpsdata;
unsigned int flags = WATCH_ENABLE;
// WINDOW datawin;

/* describe a data source */
struct fixsource_t
{
  char *spec; /* pointer to actual storage */
  char *server;
  char *port;
  char *device;
};

int main()
{
  static struct fixsource_t source;
  source.server = LOCAL_HOST;
  source.port = DEFAULT_GPSD_PORT;
  source.device = NULL;

  /* note: we're assuming BSD-style reliable signals here */
  //  (void)signal(SIGINT, die);
  //  (void)signal(SIGHUP, die);
  // (void)signal(SIGWINCH, resize);

  if (gps_open(source.server, source.port, &gpsdata) != 0)
  {
    (void)fprintf(stderr,
                  "dcgps: no gpsd running or network error: %d, %s\n",
                  errno, gps_errstr(errno));
    exit(EXIT_FAILURE);
  }
  if (source.device != NULL)
  {
    flags |= WATCH_DEVICE;
  }

  (void)gps_stream(&gpsdata, flags, source.device);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, gps_loop, gpsdata);

  char c;
  while (1)
  {
    c = getchar();

    switch (c)
    {
      /* Quit */
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
