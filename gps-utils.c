#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include "gps-utils.h"
#include <gps.h>
// #include "gps.h"

static struct gps_data_t gpsdata;

/* Function to call when we're all done.  Does a bit of clean-up. */
void die(int sig)
{
  if (!isendwin())
  {
    /* Move the cursor to the bottom left corner. */
    (void)mvcur(0, COLS - 1, LINES - 1, 0);

    /* Put input attributes back the way they were. */
    (void)echo();

    /* Done with curses. */
    (void)endwin();
  }

  /* We're done talking to gpsd. */
  (void)gps_close(&gpsdata);

  switch (sig)
  {
  case CGPS_QUIT:
    break;
  case GPS_GONE:
    (void)fprintf(stderr, "cgps: GPS hung up.\n");
    break;
  case GPS_ERROR:
    (void)fprintf(stderr, "cgps: GPS read returned error\n");
    break;
  case GPS_TIMEOUT:
    (void)fprintf(stderr, "cgps: GPS timeout\n");
    break;
  default:
    (void)fprintf(stderr, "cgps: caught signal %d\n", sig);
    break;
  }

  /* Bye! */
  exit(EXIT_SUCCESS);
}
