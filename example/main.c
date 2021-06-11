#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include "includes/http_parser_utility.h"
#include "includes/string_builder.h"
#include <string.h>

int main(int argc , char **argv){

	Get_Req get_req={
		.query=NULL,
	};

	if(argc!=2){
		fprintf(stderr,"%s <http_get_url>\n",argv[0]);
		return -1;
	}

	if(http_get_req_parser(argv[1],&get_req)==-1){

		return -1;
	}

	if(strlen(get_req.file_name)>0)
	fprintf(stderr,"name=%s\n",get_req.file_name);

	fprintf(stderr,"path=%s\n",get_req.path);
	
	if(strlen(get_req.route_name)>0)
	fprintf(stderr,"route=%s\n",get_req.route_name);


	if(get_req.query!=NULL&&get_req.query->length>0)
	for(int i = 0 ; i<get_req.query->length;i++){

		fprintf(stderr,"%s:%s\n",get_req.query->queries[i].name,get_req.query->queries[i].value);
	}		


	return 0;
}