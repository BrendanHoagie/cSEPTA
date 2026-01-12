#ifndef __CSEPTA_H__
#define __CSEPTA_H__

typedef struct request request_t;

typedef void(*request_handler_t)(request_t *req);

struct request{
    request_handler_t handler;
    char *url;
};

request_t *new_request_t();
long csepta_get_filesize(FILE *fp)

#endif