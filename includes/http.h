#ifndef __MPM__HTTP__
#define __MPM__HTTP__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct http_response {
  int sockfd;
  char content_length[16];
  char content_type[24];
  char connection[16];
  char accept_ranges[16]; // default in bytes
  // OK 200 ,or error codes
  char status[8];

} Response;

enum response_header_options {
  STATUS,
  CONTENT_LENGTH,
  CONNECTION,
  CONTENT_TYPE,
  ACCEPT_RANGES,
  BODY
};

#define CHECK_NULL(exp) if (exp == NULL) return NULL

//Query
#include "./string_builder.h"

typedef struct query_data{
  String *_mem;
  char *name;
  char *value;
}Query;

#define MAX_QUERIES 50

typedef struct query{
  Query queries[MAX_QUERIES];
  uint32_t length;
}Queries;

void dispose_queries(Queries *);

typedef struct multipart_form_data{
  char file_name[512];
  char file_path[1024];
  uint32_t file_size;
  //if any file recieved set to  1 and is not set  to 0.
  uint8_t is_any_file;
}__File;

typedef struct post_body{
  Queries *form;

}Post_Body;

typedef struct http_header {
  char *method;
  char *content_type;
  char *boundary;
  //unparsed url
  char *path;

  char *route_name;
  char *file_name;
  char *url_path;

  Queries *qrs;
  
  __File multipart_form;

  Post_Body body;

  char *protocol;  
  char **cookies;
  char *accept_charset;
  char *host;
  char *accept;
  char *accept_language;
  char *accept_encoding;
  char *connection;
  char *origin;

  uint32_t content_length;
  
} Header_t;

typedef struct http_request {
  
  Header_t header;

  int clnt_sock;
  char *mem_buffer; // buffer used to save header data
  //Type Get_Req
  void *get_req_mem; //memory for parsed Get_request and queries must not altered.
} Request;

//GET request parser
//parse queries if detected.
int get_req_parser(Request *const req);

//Parse query string
Queries *parse_query(char *buffer, uint32_t size);

//Note: POST body form-data parser only.(check get_req_parser for get request query parser)
//url-encoded for post request body parser.
Queries *parse_encoded_url(Request *request, char *remain, uint32_t readed_bytes) ;

  // Form data -multipart form data

  /*

  Max number of file recived parse support
   */

#define MAX_NUM_FILES_SUPPORT 10

typedef struct multipart_form_file {
  uint file_size;
  char *file_name;
  char *file_data;
  char *name;
  char *content_type;
} Multipart_Form_Data;

typedef struct multipart_form {
  Multipart_Form_Data forms[MAX_NUM_FILES_SUPPORT];
  unsigned int counter;
} Multipart_Form;



//send http response to client of text/html content_type 
int response_msg(int client_sock_fd,char *_msg);

// Recive multipart file data
char *recive(char *root_dir, Request *request,
             uint32_t *file_size, char *remain, uint32_t remain_bytes);

uint32_t recive_header(Request *request, char *rem,
                       uint32_t size);

// Read buffer and return EOF once readed all bytes.
char readBuffer(char *buffer, const uint32_t size, uint8_t reset);

//void parse_multipart_form_data(Request *req, Multipart_Form *multipart_form);

void parse_multipart_form(char *path, char *root_dir, Request *request,
                          uint32_t file_size);

void free_multipart_form(Multipart_Form *multipart_form);
//------------------------------------------------------

uint32_t parse_header_line(char *header_option, char *option[256]);

int parse_header(char **header_options, size_t size, Header_t *header);

// parser http header
int parse(const char *buffer, size_t size, uint32_t num_header_option,
          Request *request);

// Read data from client
int readChannel(char *buf, size_t size, FILE *file, uint32_t *new_lines);

// Set http Header
void set_responseHeader(Response *res, int res_option, const char *value);

void response(Response *http_response, char *body, uint length, uint size);

// Handle http client
void HandleClient(int clntSock, char *resource_dir, char *root_dir);




void response_error(Response *http_response, Request *request, char *message, const char *error_code);
#endif
