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
	board->upper_g_stations->set_stations |= board->upper_g_stations->bitmasks[index];
}

void g_trolley_read(void *board_ptr){
    csepta_board_t *board;
	CURLcode res;
	cJSON *json, *trolleys, *trolley, *next_stop_id;

	board = (csepta_board_t *) board_ptr;	
	res = curl_easy_perform(board->lower_g_stations->handle);
	if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
		exit(2);
	}
	
	board->lower_g_stations->set_stations = 0x0;
	board->upper_g_stations->set_stations = 0x0;
	trolleys = NULL;
	trolley = NULL;
	json = cJSON_ParseWithLength(board->lower_g_stations->chunk->memory, board->lower_g_stations->chunk->size);
	
	if(!json){
		// kinda hate this since it's not valid c89 like rest of the code
		// function always returns const char, but can't define const char at top of function
		// not sure what to do
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
		g_trolley_search_and_set(board, atoi(next_stop_id->valuestring));
		next_stop_id = NULL;
	}
	fprintf(stderr, "lower stations:\n");
	g_trolley_debug_print_binary_unsigned_long(board->lower_g_stations->set_stations);
	fprintf(stderr, "upper stations:\n");
	g_trolley_debug_print_binary_unsigned_long(board->upper_g_stations->set_stations);
	cJSON_Delete(json);
	csepta_clear_chunk(board->lower_g_stations->chunk);
}