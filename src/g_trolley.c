
/*
 * g_trolley_search_and_set
 * ------------------------
 * search for a station given a target ID and set its corresponding station bit
 *
 * g_data: a pointer to the g trolley line's data object
 * target: the current station ID number
 *
 * returns: the index of target in arr
*/
void g_trolley_search_and_set(csepta_station_t *g_data, int32_t target){	
	int32_t index;
    size_t i;
	csepta_pair_t cur_pair;

	// linear search to find what array to look in
	for(i = 0; i < g_data->num_stations; i++){
		cur_pair = g_data->pairs[i];
		if(target < cur_pair.station_ids[cur_pair.length - 1])
			break;
	}
	
	// binary search for index of station bitmask via looking up station ID
	index = csepta_search(cur_pair.station_ids, cur_pair.length, target);
	if(index == -1){
		fprintf(stderr, "Error in G trolley search: ID %d not found in pairs[%ld]. Exiting.\n", target, i);
		exit(1);
	}
	g_data->stations[i] |= cur_pair.bitmasks[index];
}

/*
 * g_trolley_read
 * --------------
 * handle reading data from the g trolley API endpoint and updating station ID bits
 *
 * g_data: a pointer to the g trolley line's data object
 *
 * returns: none
*/
void g_trolley_read(csepta_station_t *g_data){
	CURLcode res;
	cJSON *json, *trolleys, *trolley, *next_stop_id;
	size_t i;
	
	fprintf(stderr, "in trolley read\n");
	res = curl_easy_perform(g_data->handle);
	if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
		exit(2);
	}
	
	fprintf(stderr, "cURL got data\n");
	
	// reset station flags
	for(i = 0; i < g_data->num_stations; i++){
		g_data->stations[i] = 0x0;
	}
	trolleys = NULL;
	trolley = NULL;
	
	// get json data
	json = cJSON_ParseWithLength(g_data->chunk->memory, g_data->chunk->size);	
	if(!json){
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr){
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		exit(1);
	}
	
	trolleys = cJSON_GetObjectItemCaseSensitive(json, "bus");
	cJSON_ArrayForEach(trolley, trolleys){
		next_stop_id = cJSON_GetObjectItemCaseSensitive(trolley, "next_stop_id");
		
		// json contains all trolleys so filter out all inactive trolleys
		if(cJSON_IsNull(next_stop_id) || next_stop_id->valuestring[0] == '\0'){
			continue;
		}
		
		printf("Valuestring = %s\n", next_stop_id->valuestring);
		g_trolley_search_and_set(g_data, atoi(next_stop_id->valuestring));
		next_stop_id = NULL;
	}
	
	for(i = 0; i < g_data->num_stations; i++){
		fprintf(stderr, "set %ld stations:\n", i);
		csepta_debug_print_binary_u64(g_data->stations[i]);
	}
	cJSON_Delete(json);
	csepta_clear_chunk(g_data->chunk);
}