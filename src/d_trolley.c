
/*
 * d_trolley_search_and_set
 * ------------------------
 * search for a station given a target ID and set its corresponding station bit
 *
 * d_data: a pointer to the t trolley line's data object
 * target: the current station ID number
 *
 * returns: the index of target in arr
*/
void d_trolley_search_and_set(csepta_station_t *d_data, int32_t target){	
	int32_t index;
    size_t i;
	csepta_pair_t cur_pair;
	
	// linear search to find what array to look in
	for(i = 0; i < d_data->num_stations; i++){
		cur_pair = d_data->pairs[i];
		if(target < cur_pair.station_ids[cur_pair.length - 1])
			break;
	}
	
	// binary search for index of station bitmask via looking up station ID
	index = csepta_search(cur_pair.station_ids, cur_pair.length, target);
	if(index == -1){
		fprintf(stderr, "Error in D trolley search: ID %d not found in pairs[%ld]. Exiting.\n", target, i);
		exit(1);
	}
	d_data->stations[i] |= cur_pair.bitmasks[index];
}

/*
 * d_trolley_read
 * --------------
 * handle reading data from the t trolley API endpoint and updating station ID bits
 *
 * d_data: a pointer to the t trolley line's data object
 *
 * returns: none
*/
void d_trolley_read(csepta_station_t *d_data){
	CURLcode res;
	cJSON *json, *trolleys, *trolley, *nexd_stop_id;
	size_t i;
	
	fprintf(stderr, "in T trolley read\n");
	res = curl_easy_perform(d_data->handle);
	if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
		exit(2);
	}
	
	fprintf(stderr, "cURL got data\n");
	
	trolleys = NULL;
	trolley = NULL;
	
	// get json data
	json = cJSON_ParseWithLength(d_data->chunk->memory, d_data->chunk->size);	
	if(!json){
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr){
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		exit(1);
	}
    
	trolleys = cJSON_GetObjectItemCaseSensitive(json, "bus");
	cJSON_ArrayForEach(trolley, trolleys){
		nexd_stop_id = cJSON_GetObjectItemCaseSensitive(trolley, "next_stop_id");
        		
		// json contains all trolleys so filter out all inactive trolleys
		if(cJSON_IsNull(nexd_stop_id) || nexd_stop_id->valuestring[0] == '\0'){
			continue;
		}
		
		printf("Valuestring = %s\n", nexd_stop_id->valuestring);
		d_trolley_search_and_set(d_data, atoi(nexd_stop_id->valuestring));
		nexd_stop_id = NULL;
	}
	
	for(i = 0; i < d_data->num_stations; i++){
		fprintf(stderr, "set %ld stations:\n", i);
		csepta_debug_print_binary_u64(d_data->stations[i]);
	}
	cJSON_Delete(json);
	csepta_clear_chunk(d_data->chunk);
}

/*
 * d_trolley_run
 * -------------
 * outer runner function for d_trolley_read, runs inner function 5 times, once for each trolley line API endpoint
 *
 * d_data: a pointer to the t trolley line's data object
 *
 * returns: none
*/
void d_trolley_run(csepta_station_t *d_data){
    size_t i;
    char url[64];
    
    // reset station flags
	for(i = 0; i < d_data->num_stations; i++){
		d_data->stations[i] = 0x0;
	}
    
    for(i = 1; i <= NUM_D_URLS; i++){
        snprintf(url, sizeof(url), "%sD%ld", TROLLEY_BASE_URL, i);
        fprintf(stderr, "Iteration %ld: url= %s\n", i, url);
        curl_easy_setopt(d_data->handle, CURLOPT_URL, url);
        d_trolley_read(d_data);
    }
}



