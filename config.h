#define HOST    "localhost" /* mpd connection host, NULL = default */
#define PORT    0           /* mpd connection port, 0 = default */
#define TIMEOUT 0           /* mpd connection timeout, 0 = default */

typedef struct {
    const xcb_keysym_t key;
    const char *command;
} HotKey;

static const HotKey hks[] = {
    { XK_p, "play" },
    { XK_o, "stop" },
};

