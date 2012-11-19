#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <evhtp.h>
//FIXME#include <mongo.h>

void
receive_plurals(evhtp_request_t * req, void * a) {
    if (req->method != htp_method_POST) { 
        return;
    }
    
    int len = evbuffer_get_length(req->buffer_in);
    if (len == 0) {
        return;
    }


    char buf[1024];
    int n;
    while ((n = evbuffer_remove(req->buffer_in, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, n, stdout);
    }
    printf("\n");

    evhtp_send_reply(req, EVHTP_RES_OK);
}

void
provide_score(evhtp_request_t * req, void * a) {
    const char * str = a;

    evbuffer_add_printf(req->buffer_out, "%s", str);
    evhtp_send_reply(req, EVHTP_RES_OK);
}

int
main(int argc, char ** argv) {
    evbase_t         * evbase      = event_base_new();
    evhtp_t          * htp         = evhtp_new(evbase, NULL);
    evhtp_callback_t * cb_plurals  = NULL;
    evhtp_callback_t * cb_score    = NULL;
    int                num_threads = 6;

    cb_plurals = evhtp_set_cb(htp, "/blahblah/plurals", receive_plurals, "plurals");
    cb_score = evhtp_set_cb(htp, "/blahblah/score", provide_score, "score");
#ifndef EVHTP_DISABLE_EVTHR
    evhtp_use_threads(htp, NULL, num_threads, NULL);
#else
    if (num_threads != 1) {
        printf("Error: multithreading is not supported\n");
        exit(1);
    }
#endif
    evhtp_bind_socket(htp, "0.0.0.0", 8081, 1024);

    event_base_loop(evbase, 0);

    evhtp_unbind_socket(htp);
    evhtp_callback_free(cb_score);
    evhtp_callback_free(cb_plurals);
    evhtp_free(htp);
    event_base_free(evbase);

    return 0;
}

