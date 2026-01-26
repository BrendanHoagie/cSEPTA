void g_trolley_search_and_set(csepta_board_t *board, int target){	
	int index;
    
    if(target < board->upper_g_stations->station_ids[0]){
		index = csepta_search(board->lower_g_stations->station_ids, 
							  board->lower_g_stations->array_length,
							  target);
		if(index == -1){
			fprintf(stderr, "Error in lower search: ID %d not found in search. Exiting.\n", target);
			exit(1);
		}
        unsigned long lower = board->lower_g_stations->bitmasks[index];
        // fprintf(stderr, "lower mask = 0x%016lx. Index = %d\n", lower, index);
        board->lower_g_stations->set_stations |= board->lower_g_stations->bitmasks[index];
        return ;
	}
	
	index = csepta_search(board->upper_g_stations->station_ids, 
							  board->upper_g_stations->array_length,
							  target);
	if(index == -1){
			fprintf(stderr, "Error in upper search: ID %d not found in search. Exiting.\n", target);
			exit(1);
	}
	unsigned long lower = board->upper_g_stations->bitmasks[index];
	// fprintf(stderr, "upper mask = 0x%016lx. Index = %d\n", lower, index);
	board->upper_g_stations->set_stations |= board->upper_g_stations->bitmasks[index];
}

void* g_trolley_read(void *board_ptr){
    csepta_board_t *board;
	// char *error_ptr;
	// CURL *hnd;
	CURLcode res;
	// struct curl_slist *slist1;
	// mem_t chunk;
	cJSON *json, *trolleys, *trolley, *next_stop_id;
	fprintf(stderr, "init g_trolley_read()\n");
    board = (csepta_board_t *) board_ptr;
	fprintf(stderr, "got board from void\n");
    // as part of the updater thread, the chunks and curl handles
    // should be shared instead of allocated in each function. Prototype for now
  	// chunk.memory = NULL;
	// chunk.size = 0;
	// hnd = curl_easy_init();
	// if(!hnd){
	// 	fprintf(stderr, "curl handle could not be initalized\n");
	// 	curl_easy_cleanup(hnd);
	// 	exit(1);
	// }
	
	// slist1 = NULL;
  	// slist1 = curl_slist_append(slist1, "accept: application/json");
	// curl_easy_setopt(hnd, CURLOPT_URL, G_TROLLEY_URL);
	// curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, cespta_write_callback);
	// curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &chunk);
	// curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
	// curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
	// curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	// curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	res = curl_easy_perform(board->lower_g_stations->handle);
	// res = curl_easy_perform(hnd);
	if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
	}
	fprintf(stderr, "curl handle got the data\n");
	trolleys = NULL;
	trolley = NULL;
	// error_ptr = NULL;
	json = cJSON_ParseWithLength(board->lower_g_stations->chunk->memory, board->lower_g_stations->chunk->size);
	
	if(!json){
		// warning for disregarding const. Doesn't really matter since it's used immediately
		// putting back for now to get rid of errors. Not c89 compliant.
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr){
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		exit(1);
	}
	
	trolleys = cJSON_GetObjectItemCaseSensitive(json, "bus");
	cJSON_ArrayForEach(trolley, trolleys){
		next_stop_id = cJSON_GetObjectItemCaseSensitive(trolley, "next_stop_id");
		
		if(cJSON_IsNull(next_stop_id) || next_stop_id->valuestring[0] == '\0'){
			continue;
		}
		printf("Valuestring = %s\n", next_stop_id->valuestring);
        fprintf(stderr, "Before:\nlower stations: 0x%016lx\nupper stations: 0x%016lx\n\n", board->lower_g_stations->set_stations, board->upper_g_stations->set_stations);
        g_trolley_search_and_set(board, atoi(next_stop_id->valuestring));
        fprintf(stderr, "After:\nlower stations: 0x%016lx\nupper stations: 0x%016lx\n\n", board->lower_g_stations->set_stations, board->upper_g_stations->set_stations);
		next_stop_id = NULL;
		
	}

	cJSON_Delete(json);	
	curl_easy_cleanup(board->lower_g_stations->handle);
	// curl_easy_cleanup(hnd);
	return NULL;
}