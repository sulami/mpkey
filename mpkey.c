#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpd/client.h>
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>

#include "config.h"

#define LENGTH(x) (sizeof(x)/sizeof(*x))
#define SYMTOKEY(k) *xcb_key_symbols_get_keycode(keysyms, k)

static struct mpd_connection *mpd;
static struct xcb_connection_t *xcb;
static struct xcb_screen_t *screen;
static xcb_key_symbols_t *keysyms;

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

static void send_command(const char *command)
{
    if (!mpd_send_command(mpd, command, NULL) || !mpd_response_finish(mpd))
        /* TODO maybe find out what happened? */
        printf("error: something bad happened with mpd\n");
}

int main()
{
    int                     default_screen, retval = 0;
    xcb_generic_event_t     *ev;

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
    keysyms = xcb_key_symbols_alloc(xcb);
    for (unsigned int i = 0; i < LENGTH(hks); i++)
        xcb_grab_key(xcb, true, screen->root, XCB_MOD_MASK_ANY,
                     SYMTOKEY(hks[i].key), XCB_GRAB_MODE_ASYNC,
                     XCB_GRAB_MODE_ASYNC);

    /* check for events and act accordingly */
    xcb_flush(xcb);
    while ((ev = xcb_wait_for_event(xcb))) {
        if ((ev->response_type & ~0x80) == XCB_KEY_PRESS) {
            xcb_key_press_event_t *ke = (xcb_key_press_event_t *)ev;
            for (unsigned int i = 0; i < LENGTH(hks); i++)
                if (SYMTOKEY(hks[i].key) == ke->detail)
                    send_command(hks[i].command);
        }
        free(ev);
    }

    /* disconnect again */
screenerror:
    xcb_disconnect(xcb);
xcberror:
    mpd_connection_free(mpd);
mpderror:
    return retval;
}

