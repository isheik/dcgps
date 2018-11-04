#include <stdlib.h>
#include <errno.h>
// #include <curses.h>
#include <gps.h>
#include "gps-utils.h"
#include "gpsprint.h"
// #include "gps.h"

// static struct gps_data_t gpsdata;

/* Function to call when we're all done.  Does a bit of clean-up. */
// void die(int sig)
// {
//   if (!isendwin())
//   {
//     /* Move the cursor to the bottom left corner. */
//     (void)mvcur(0, COLS - 1, LINES - 1, 0);

//     /* Put input attributes back the way they were. */
//     (void)echo();

//     /* Done with curses. */
//     (void)endwin();
//   }

//   /* We're done talking to gpsd. */
//   (void)gps_close(&gpsdata);

//   switch (sig)
//   {
//   case CGPS_QUIT:
//     break;
//   case GPS_GONE:
//     (void)fprintf(stderr, "cgps: GPS hung up.\n");
//     break;
//   case GPS_ERROR:
//     (void)fprintf(stderr, "cgps: GPS read returned error\n");
//     break;
//   case GPS_TIMEOUT:
//     (void)fprintf(stderr, "cgps: GPS timeout\n");
//     break;
//   default:
//     (void)fprintf(stderr, "cgps: caught signal %d\n", sig);
//     break;
//   }

//   /* Bye! */
//   exit(EXIT_SUCCESS);
// }
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
bool usedflags[MAXCHANNELS];
static enum deg_str_type deg_type = deg_dd;

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
      // die(GPS_TIMEOUT);
    }
    else
    {
      wait_clicks = 0;
      // errno = 0;
      if (gps_read(gpsdata) == -1)
      {
        fprintf(stderr, "dcgps: socket error 4\n");
        (void)gps_close(gpsdata);
        exit(1);
        // die(errno == 0 ? GPS_GONE : GPS_ERROR);
      }
      else
      {
        gps_print(gpsdata);
      }
    }
  }
}
