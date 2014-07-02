#include <stdlib.h>
#include <mpd/client.h>
#include <xcb/xcb_keysyms.h>

#include "config.h"

/* get screen of display */
static xcb_screen_t *xcb_screen_of_display(xcb_connection_t *con, int screen)
{
    xcb_screen_iterator_t iter;

    iter = xcb_setup_roots_iterator(xcb_get_setup(con));
    for (; iter.rem; --screen, xcb_screen_next(&iter))
        if (screen == 0)
            return iter.data;

    return NULL;
}

int main()
{
    struct mpd_connection *mpd;
    struct xcb_connection_t *xcb;
    int screen;

    /* connect to mpd */
    mpd = mpd_connection_new(HOST, PORT, TIMEOUT);
    if (!mpd)
        return -1;

    /* connect to xcb */
    if (xcb_connection_has_error((xcb = xcb_connect(NULL, &screen))))
        return -2;

    /* disconnect again */
    xcb_disconnect(xcb);
    mpd_connection_free(mpd);

    return 0;
}

