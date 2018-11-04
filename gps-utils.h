#ifndef GPS_UTILS
#define GPS_UTILS

/* pseudo-signals indicating reason for termination */
#define CGPS_QUIT 0    /* voluntary yterminastion */
#define GPS_GONE -1    /* GPS device went away */
#define GPS_ERROR -2   /* low-level failure in GPS read */
#define GPS_TIMEOUT -3 /* low-level failure in GPS waiting */

extern void die(int sig);

#endif
