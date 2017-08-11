#include <cstdint>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <zconf.h>
#include <fcntl.h>
#include "log.h"
#include "mongoose.h"
#include "mongoose_timer.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



static const char *s_http_port = "8100";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    switch (ev) {

        case MG_EV_TIMER: {
            double now = *(double *) ev_data;
            double next = mg_set_timer(c, 0) + 2;
            printf("timer event, current time: %.2lf\n", now);
            printf("last mg_set_timer value: %.2lf\n", next - 2);
//            sleep(10);
            mg_set_timer(c, next);  // Send us timer event again after 0.5 seconds
//            mg_set_timer(c, 0);
            break;
        }
    }
}

int g_main(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr, NULL);
    c = mg_bind(&mgr, s_http_port, ev_handler);
    if (c == NULL) {
        printf("Cannot start on port %s\n", s_http_port);
        return EXIT_FAILURE;
    }

    // Send us MG_EV_TIMER event after 2.5 seconds
    mg_set_timer(c, get_current_precision_time() + 2.5);

    printf("Starting on port %s, time: %.2lf\n", s_http_port, get_current_precision_time());
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return EXIT_SUCCESS;
}