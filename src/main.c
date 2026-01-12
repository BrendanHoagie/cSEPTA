int main(int argc, char **argv){    
    CURL *curl = curl_easy_init();
	if(!curl){
		return 1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");
	int res = curl_easy_perform(curl);
	printf("hello there curl\n");
	if(res != CURLE_OK){
		printf("transfer fail: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
    
	csepta_test(argv[1]);
    return 0;
}
