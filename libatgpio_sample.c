#include <glib.h>
#include "libraries/atgpio.h"

unsigned int timer_flag = 1;

static gboolean on_quit(GIOChannel *ch, gpointer d)
{
    GMainLoop *loop = d;

    g_io_channel_read_to_end(ch, NULL, NULL, NULL);
    g_main_loop_quit(loop);
    g_print("bye\n");

    return FALSE;
}

static gboolean on_timer(GIOChannel *ch, gpointer d)
{
    gchar *str = d;

    if(timer_flag)
    {
        g_print("timer_on %d\r\n",timer_flag);
        timer_flag = 0;
    }
    else
    {
        g_print("timer_off %d\r\n",timer_flag);
        timer_flag = 1;
    }
    g_io_channel_read_to_end(ch, NULL, NULL, NULL);

    return TRUE;
}

int main()
{
    GMainLoop *loop;
    GError *error = NULL;

    loop = g_main_loop_new(NULL, FALSE);

    at_gpio_add(13,  AT_GPIO_EDGE_FALLING,    on_quit,  loop,  &error);
    if (error)
        g_error("%s\n", error->message);
    at_gpio_add(11,  AT_GPIO_EDGE_BOTH,  on_timer, "11",  NULL);
 //   at_gpio_add(99,  AT_GPIO_EDGE_FALLING, on_print, "99",  NULL);
 //   at_gpio_add(100, AT_GPIO_EDGE_NONE,    on_print, "100", NULL);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return 0;
}