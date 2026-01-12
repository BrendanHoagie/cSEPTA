long csepta_get_filesize(FILE *fp)
{
    long filesize;

    if( fseek(fp, 0, SEEK_END) != 0) exit(EXIT_FAILURE);
	
    filesize = ftell(fp);
    rewind(fp);
    return filesize;
}

void csepta_test(char *str){
    printf("file name: %s\n", str);
}

// int test(){
// 	CURLcode ret;
// 	CURL *hnd;
// 	struct curl_slist *slist1;

// 	slist1 = NULL;
// 	slist1 = curl_slist_append(slist1, "accept: application/json");

// 	hnd = curl_easy_init();
// 	curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
// 	curl_easy_setopt(hnd, CURLOPT_URL, "https://www3.septa.org/api/TrainView/index.php");
// 	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
// 	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
// 	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.2.1");
// 	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
// 	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
// 	curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
// 	curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

// 	/* Here is a list of options the curl code used that cannot get generated
// 		as source easily. You may choose to either not use them or implement
// 		them yourself.

// 	CURLOPT_WRITEDATA was set to an object pointer
// 	CURLOPT_INTERLEAVEDATA was set to an object pointer
// 	CURLOPT_WRITEFUNCTION was set to a function pointer
// 	CURLOPT_READDATA was set to an object pointer
// 	CURLOPT_READFUNCTION was set to a function pointer
// 	CURLOPT_SEEKDATA was set to an object pointer
// 	CURLOPT_SEEKFUNCTION was set to a function pointer
// 	CURLOPT_ERRORBUFFER was set to an object pointer
// 	CURLOPT_STDERR was set to an object pointer
// 	CURLOPT_HEADERFUNCTION was set to a function pointer
// 	CURLOPT_HEADERDATA was set to an object pointer

// 	*/

// 	ret = curl_easy_perform(hnd);

// 	curl_easy_cleanup(hnd);
// 	hnd = NULL;
// 	curl_slist_free_all(slist1);
// 	slist1 = NULL;

// 	return (int)ret;
// }

// int main(int argc, char** argv){
// 	// CURL *curl = curl_easy_init();
// 	// if(!curl){
// 	// 	return 1;
// 	// }
// 	// curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");
// 	// int res = curl_easy_perform(curl);
// 	// printf("hello world");
// 	// if(res != CURLE_OK){
// 	// 	printf("transfer fail: %s\n", curl_easy_strerror(res));
// 	// }
// 	// test();
// 	// curl_easy_cleanup(curl);
// 	FILE *fp;
// 	long buf_size;
// 	unsigned char *buf;
	
// 	if(argc != 2){
// 		fprintf(stderr, "Usage: %s filename.json\n", argv[0]);
// 		exit(1);
// 	}
	
// 	fp = fopen(argv[1], "r");
// 	if(!fp){
// 		fprintf(stderr, "Not a valid file\n");
// 		exit(1);
// 	}
	
// 	buf_size = get_filesize(fp);
// 	if(buf_size < 1){
// 		fprintf(stderr, "Invalid buf_size\n");
// 		exit(1);
// 	}
	
// 	buf = (unsigned char*) malloc(buf_size * sizeof(unsigned char));
// 	if(!buf){
// 		fprintf(stderr, "Invalid buffer allocation\n");
// 		exit(1);
// 	}
	
// 	if((fread(buf, sizeof(unsigned char), buf_size, fp)) != buf_size){
// 		fprintf(stderr, "Buffer didn't get all of filestream\n");
// 		exit(1);
// 	}
// 	fclose(fp);
	
// 	const cJSON *num_JSON_requests = NULL;
// 	const cJSON *request = NULL;
// 	const cJSON *requests = NULL;
// 	cJSON *json = cJSON_ParseWithLength(buf, buf_size);
// 	if(!json){
// 		const char *error_ptr = cJSON_GetErrorPtr();
// 		if (error_ptr){
// 			fprintf(stderr, "Error before: %s\n", error_ptr);
// 		}
// 		exit(1);
// 	}
// 	free(buf);
	
// 	num_JSON_requests = cJSON_GetObjectItemCaseSensitive(json, "num_requests");
// 	if(!cJSON_IsNumber(num_JSON_requests) || num_JSON_requests->valueint < 0){ 
// 		fprintf(stderr, "There was an error in reading num_requests from valid_requests.json\n");
// 		exit(1);
// 	}
	
// 	request_t **valid_requests = (request_t**) malloc(num_JSON_requests->valueint * sizeof(request_t*));
// 	if(!valid_requests){
// 		fprintf(stderr, "Could not construct array of %d valid requests\n", num_JSON_requests->valueint);
// 		exit(1);
// 	}

// 	requests = cJSON_GetObjectItemCaseSensitive(json, "requests");
// 	int i = 0;
// 	cJSON_ArrayForEach(request, requests){
// 		// need to change init location to reuse pointers + these malloc every time
// 		cJSON *func_name = cJSON_GetObjectItemCaseSensitive(request, "func_name");
// 		cJSON *url = cJSON_GetObjectItemCaseSensitive(request, "url");
		
// 		if(!cJSON_IsString(func_name) || func_name->valuestring == NULL){
// 			fprintf(stderr, "func name is not a string\n");
// 			exit(1);
// 		}
// 		if(!cJSON_IsString(url) || url->valuestring == NULL){
// 			fprintf(stderr, "url is not a string\n");
// 			exit(1);
// 		}
		
		
		
// 		i++;	
// 	}
// 	cJSON_Delete(json);
	
	
// 	return 0;
// }


