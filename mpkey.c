#include <stdlib.h>
#include <mpd/client.h>
#include <xcb/xcb_keysyms.h>

#include "config.h"

int main()
{
    struct mpd_connection *mpd;
    struct xcb_connection_t *xcb;
    int screen;

    mpd = mpd_connection_new(HOST, PORT, TIMEOUT);
    if (!mpd)
        return -1;

    if (xcb_connection_has_error((xcb = xcb_connect(NULL, &screen))))
        return -2;

    return 0;
}

