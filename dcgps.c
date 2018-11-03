#include "dcgps.h"

static struct gps_data_t gpsdata;
struct fixsource_t
/* describe a data source */
{
  char *spec; /* pointer to actual storage */
  char *server;
  char *port;
  char *device;
};

int main()
{
  printf("%d", 12);

  return 0;
}
