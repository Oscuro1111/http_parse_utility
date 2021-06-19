#ifndef __MPM_HTTP__PARSER__
#define __MPM_HTTP__PARSER__

#include "./http_internel.h"
typedef struct get_req{
    char path[1024];
    char file_name[512];
    char route_name[512];

    Queries *query;

}Get_Req;

//parse http request GET request.
int http_get_req_parser(char *str,Get_Req *get_r);

int http_get_req_is_file(char *str, char *store, unsigned int size);

int http_get_req_is_route(char *str);
#endif