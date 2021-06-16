#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "../includes/http_parser_utility.h"

#define MAX_ERROR_MSG 0x1000

static int compile_regex(regex_t *r, const char *regex_text)
{
    int status = regcomp(r, regex_text, REG_EXTENDED | REG_NEWLINE);
    if (status != 0)
    {
        char error_message[MAX_ERROR_MSG];
        regerror(status, r, error_message, MAX_ERROR_MSG);
        printf("Regex error compiling '%s': %s\n",
               regex_text, error_message);
        return 1;
    }
    return 0;
}

/*
  Match the string in "to_match" against the compiled regular
  expression in "r".
 */

int is_file_regex(char *regexp, char *to_match, char *filename, unsigned int size)
{
    regex_t r;
    const char *regex_text;
    const char *find_text;

    /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char *p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = MAX_QUERIES;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    regex_text = regexp;

    compile_regex(&r, regex_text);

    while (1)
    {
        int i = 0;
        int nomatch = regexec(&r, p, n_matches, m, 0);
        if (nomatch)
        {
            regfree(&r);
            return -1;
        }
        for (i = 0; i < n_matches; i++)
        {
            int start;
            int finish;
            if (m[i].rm_so == -1)
            {
                break;
            }
            //m[i].rm_so is start  position inside the string.
            //Base is changing
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);
                       
            if (i == 2)
            {
                snprintf(filename, size, "%.*s", (finish - start), to_match + start);
                regfree(&r);
                return 0;
            }
        }
        p += m[0].rm_eo;
    }

    regfree(&r);
    return -1;
}

int http_get_req_is_file(char *str, char *store, unsigned int size)
{

    const char *regex_text;
    const char *find_text;

    int res = -1;
    regex_text = "(/([a-zA-Z][a-z0-9A-Z.-]+))+";
    find_text = str;

    res = is_file_regex(regex_text, find_text, store, size);

    return res;
}

int is_route_regex(char *regexp, char *to_match)
{
    regex_t r;
    const char *regex_text;

    /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char *p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = MAX_QUERIES;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    regex_text = regexp;

    compile_regex(&r, regex_text);

    while (1)
    {
        int i = 0;
        int nomatch = regexec(&r, p, n_matches, m, 0);

        if (nomatch)
        {
            regfree(&r);
            return -1;
        }

        regfree(&r);
        return 1;
    }

    regfree(&r);
    return -1;
}

int http_get_req_is_route(char *str)
{

    const char *regex_text;
    const char *find_text;

    int res = -1;

    regex_text = "\\b[.]\\b";

    find_text = str;

    res = is_route_regex(regex_text, find_text);

    if(res==1){ //file not route
        return -1;
    }else
    {
       return 0;
    }      //route not file
}

int get_path_regex(char *regexp, char *to_match, char *filename, unsigned int size)
{
    regex_t r;
    const char *regex_text;
    const char *find_text;

    /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char *p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = MAX_QUERIES;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    regex_text = regexp;

    compile_regex(&r, regex_text);

    // match_regex(&r, find_text);

    while (1)
    {
        int i = 0;
        int nomatch = regexec(&r, p, n_matches, m, 0);
        if (nomatch)
        {
            printf("No more matches.\n");

            regfree(&r);
            return -1;
        }
        for (i = 0; i < n_matches; i++)
        {
            int start;
            int finish;
            if (m[i].rm_so == -1)
            {
                break;
            }
            //m[i].rm_so is start  position inside the string.
            //Base is changing
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);

            if (i == 0)
            {
                snprintf(filename, size, "%.*s", (finish - start), to_match + start);

                regfree(&r);
                return 0;
            }
        }
        p += m[0].rm_eo;
    }

    regfree(&r);
    return -1;
}

int http_get_path_file(char *str, char *store, unsigned int size)
{

    const char *regex_text;
    const char *find_text;
    int res = -1;
    regex_text = "(/([0-9@#a-zA-Z][a-z0-9A-Z.-]+))+";
    find_text = str;

    res = get_path_regex(regex_text, find_text, store, size);

    return res;
}

int is_query_regex(char *regexp, char *to_match, Queries *queries)
{
    regex_t r;
    const char *regex_text;
    char *_state;
    int query_len=0;
    
    char temp_str[1024];

     /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char *p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = MAX_QUERIES;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    regex_text = regexp;

    compile_regex(&r, regex_text);

    while (1)
    {
        int i = 0;
        String *string = allocate_string(0);
        int nomatch = regexec(&r, p, n_matches, m, 0);
        if (nomatch)
        {
            free_string(string);
            if(query_len>0){
                queries->length = query_len;
                regfree(&r);
                return 1;
            }else{
             regfree(&r);                                        
            return -1;
            }
        }

        for (i = 0; i < n_matches; i++)
        {
            int start;
            int finish;
            if (m[i].rm_so == -1)
            {
                break;
            }
            //m[i].rm_so is start  position inside the string.
            //Base is changing
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);
          
            //key
            if(i==1){
                //eg:name
               snprintf(temp_str,1024,"%.*s",(finish-start),to_match+start);
               insert_string(&string,temp_str);

            }//value
            else if(i==2)
            {
                 int temp;
                 //string: [name\r\nvalue]
                 //make easy to saprate value from key
                 insert_string(&string,"\r\n");
                 //[name\r\np1] p1=current_position for next insertion point for value                 
                 temp=string->current;

                 snprintf(temp_str,1024,"%.*s",(finish-start),to_match+start);
                 
                 insert_string(&string,temp_str);
                 
                 queries->queries[query_len].name=strtok_r(string->str,"\r\n",&_state);
               
                 queries->queries[query_len].value = string->str+temp;

                 queries->queries[query_len]._mem = string;

            }

           
        }
        ++query_len;

        p += m[0].rm_eo;
    }

    regfree(&r);
    return 1;
}

int http_get_req_is_query(char *str, Queries *queries)
{

    const char *regex_text;
    const char *find_text;

    int res = -1;
    regex_text ="\\b(\\w+)=([%a-zA-Z0-9.+-\\s_]+)\\b";
    find_text  = str;

    res = is_query_regex(regex_text, find_text, queries);

    return res;
}

int http_get_req_parser(char *str, Get_Req *get_r)
{
    Queries *query = NULL;

     if((http_get_req_is_file(str,get_r->file_name,512))!=-1){
         if(http_get_req_is_route(get_r->file_name)!=-1){
             strncpy(get_r->route_name,get_r->file_name,512);
             memset(get_r->file_name,'\0',10);
         }
     }
     //full path of url
     if((http_get_path_file(str,get_r->path,1024))==-1){
         fprintf(stderr,"unabel to get full path:%s\n",str);
     }
     query  = (Queries *)malloc(sizeof(Queries));

     if(http_get_req_is_query(str,query)!=-1){
         get_r->query = query;
     }else{
          get_r->query=NULL;
          free(query);
     }

    return 0;
}

