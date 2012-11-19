/*Build inside of https://github.com/ellzey/libevhtp
by adding these lines

add_executable(taalspel taalspel.c)
target_link_libraries(taalspel libevhtp ${LIBEVHTP_EXTERNAL_LIBS} ${SYS_LIBS})

to CMakeLists.txt and doing

cd build
cmake ..
make
make test
./taalspel

open test-form.html in firefox and watch command line for 3rd and 4th test
*/

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
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }
    
    int max_len = 1024;
    int len = evbuffer_get_length(req->buffer_in);
    if (len == 0 || len >= max_len) {
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }
    char buf[max_len];
    evbuffer_remove(req->buffer_in, buf, len);
    buf[len] = '\0';

    if (buf[0] != 'd' || buf[1] != '=' || strstr(buf + 2, "=") != NULL || strstr(buf, "&") != NULL) {
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }

    printf("d: %s\n", buf + 2);

    evhtp_send_reply(req, EVHTP_RES_OK);
}

void
provide_score(evhtp_request_t * req, void * a) {
    if (req->method != htp_method_POST) { 
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }
    
    int max_len = 1024;
    int len = evbuffer_get_length(req->buffer_in);
    if (len == 0 || len >= max_len) {
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }
    char buf[max_len];
    evbuffer_remove(req->buffer_in, buf, len);
    buf[len] = '\0';

    char* amp_pos = strchr(buf, '&');
    if (amp_pos == NULL || amp_pos != strrchr(buf, '&')) {
        evhtp_send_reply(req, EVHTP_RES_BADREQ);
        return;
    }

    buf[amp_pos - buf] = '\0';
    if (buf[0] == 'u') {
        if (buf[1] != '=' || strstr(buf + 2, "=") != NULL || strstr(buf, "&") != NULL || amp_pos[2] != '=' || strstr(amp_pos + 3, "=") != NULL || amp_pos[1] != 'd') {
            evhtp_send_reply(req, EVHTP_RES_BADREQ);
            return;
        }
        printf("u: %s\n", buf + 2);
        printf("d: %s\n", amp_pos + 3);
    } else {
        if (buf[0] == 'd') {
           if (buf[1] != '=' || strstr(buf + 2, "=") != NULL || strstr(buf, "&") != NULL || amp_pos[2] != '=' || strstr(amp_pos + 3, "=") != NULL || amp_pos[1] != 'u') {
                evhtp_send_reply(req, EVHTP_RES_BADREQ);
                return;
            }
            printf("u: %s\n", amp_pos + 3);
            printf("d: %s\n", buf + 2);
        } else {
            evhtp_send_reply(req, EVHTP_RES_BADREQ);
            return;
        }
    }

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

