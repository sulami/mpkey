#include <stdlib.h>
#include <mpd/client.h>
#include <xcb/xcb_keysyms.h>

#include "config.h"

int main()
{
    struct mpd_connection *mpd;

    mpd = mpd_connection_new(HOST, PORT, TIMEOUT);
    if (!mpd)
        exit(1);
    return 0;
}

