#include <errno.h>
#include <stdlib.h>
#include <mpd/client.h>
#include <X11/keysym.h>
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

static void grab_keys(xcb_connection_t *con, xcb_screen_t *screen)
{
    xcb_grab_key(con, 1, screen->root, XCB_MOD_MASK_ANY, XK_j,
                 XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
}

int main()
{
    struct mpd_connection   *mpd;
    struct xcb_connection_t *xcb;
    int                     default_screen, retval = 0;
    struct xcb_screen_t     *screen;

    /* connect to mpd */
    mpd = mpd_connection_new(HOST, PORT, TIMEOUT);
    if (!mpd) {
        retval = -ENOMEM;
        goto mpderror;
    }

    /* connect to xcb */
    retval = xcb_connection_has_error(xcb = xcb_connect(NULL, &default_screen));
    if (retval)
        goto xcberror;
    screen = xcb_screen_of_display(xcb, default_screen);
    if (!screen) {
        retval = -EIO;
        goto screenerror;
    }

    /* grab the keys */
    grab_keys(xcb, screen);

    /* disconnect again */
screenerror:
    xcb_disconnect(xcb);
xcberror:
    mpd_connection_free(mpd);
mpderror:
    return retval;
}

