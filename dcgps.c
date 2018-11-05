#include <stdlib.h>
#include <gps.h>
#include <errno.h>
#include "dcgps.h"
#include "gps-utils.h"

static struct gps_data_t gpsdata;
unsigned int flags = WATCH_ENABLE;

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

  (void)gps_stream(&gpsdata, flags, source.device);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, gps_loop, &gpsdata);

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
