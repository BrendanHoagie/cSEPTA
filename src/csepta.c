#include "../lib/csepta.h"
#include "g_trolley.c"
#include "t_trolley.c"
#include "d_trolley.c"

/*
 * csepta_write_callback
 * ---------------------
 * handle cURL data - only called by libcurl
 *
 * contents: data delivered by cURL request
 * size:     size of each element in contents (always 1)
 * nmemb:    size of contents
 * userp:    user-defined structure to store data (cast as void *)
 *
 * returns: the size of data stored in userp
*/
size_t csepta_write_callback(char *contents, size_t size, size_t nmemb, void *userp);

void csepta_init_and_run(){
	csepta_board_t board;
	struct curl_slist *slist;
	csepta_mem_t g_chunk, t_chunk, d_chunk;
	csepta_station_t g_data, t_data, d_data;
	uint64_t g_stations[] = {0x0, 0x0};
	csepta_pair_t g_pairs[] = {
		{
			.length = G_SET_1_SIZE,
			.station_ids = G_SET_1_IDS,
			.bitmasks = G_SET_1_BITMASKS
		},
		{
			.length = G_SET_2_SIZE,
			.station_ids = G_SET_2_IDS,
			.bitmasks = G_SET_2_BITMASKS
		}
	};
	uint64_t t_stations[] = {0x0, 0x0, 0x0, 0x0, 0x0};
	csepta_pair_t t_pairs[] = {
		{
			.length = T_SET_1_SIZE,
			.station_ids = T_SET_1_IDS,
			.bitmasks = T_SET_1_BITMASKS
		},
		{
			.length = T_SET_2_SIZE,
			.station_ids = T_SET_2_IDS,
			.bitmasks = T_SET_2_BITMASKS
		},
		{
			.length = T_SET_3_SIZE,
			.station_ids = T_SET_3_IDS,
			.bitmasks = T_SET_3_BITMASKS
		},
		{
			.length = T_SET_4_SIZE,
			.station_ids = T_SET_4_IDS,
			.bitmasks = T_SET_4_BITMASKS
		},
		{
			.length = T_SET_5_SIZE,
			.station_ids = T_SET_5_IDS,
			.bitmasks = T_SET_5_BITMASKS
		}
	};
	uint64_t d_stations[] = {0x0, 0x0};
	csepta_pair_t d_pairs[] = {
		{
			.length = D_SET_1_SIZE,
			.station_ids = D_SET_1_IDS,
			.bitmasks = D_SET_1_BITMASKS
		},
		{
			.length = D_SET_2_SIZE,
			.station_ids = D_SET_2_IDS,
			.bitmasks = D_SET_2_BITMASKS
		},
	};
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	// setup G trolley
	g_data.num_stations = 2;
	g_data.stations = g_stations;
	g_data.pairs = g_pairs;
	g_data.handle = curl_easy_init();
	if(!g_data.handle){
		fprintf(stderr, "curl handle could not be initalized\n");
		curl_easy_cleanup(g_data.handle);
		exit(1);
	}
	
	g_chunk.memory = NULL;
	g_chunk.size = 0;
	g_data.chunk = &g_chunk;
		
	slist = NULL;
  	slist = curl_slist_append(slist, "accept: application/json");
	curl_easy_setopt(g_data.handle, CURLOPT_WRITEFUNCTION, csepta_write_callback);
	curl_easy_setopt(g_data.handle, CURLOPT_WRITEDATA, g_data.chunk);
	curl_easy_setopt(g_data.handle, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(g_data.handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(g_data.handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(g_data.handle, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	// setup T trolley
	t_data.num_stations = 5;
	t_data.stations = t_stations;
	t_data.pairs = t_pairs;
	t_data.handle = curl_easy_init();
	if(!t_data.handle){
		fprintf(stderr, "curl handle could not be initalized\n");
		curl_easy_cleanup(t_data.handle);
		exit(1);
	}
	
	t_chunk.memory = NULL;
	t_chunk.size = 0;
	t_data.chunk = &t_chunk;
	
	curl_easy_setopt(t_data.handle, CURLOPT_WRITEFUNCTION, csepta_write_callback);
	curl_easy_setopt(t_data.handle, CURLOPT_WRITEDATA, t_data.chunk);
	curl_easy_setopt(t_data.handle, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(t_data.handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(t_data.handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(t_data.handle, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	
	// setup D trolley
	d_data.num_stations = 2;
	d_data.stations = d_stations;
	d_data.pairs = d_pairs;
	d_data.handle = curl_easy_init();
	if(!d_data.handle){
		fprintf(stderr, "curl handle could not be initalized\n");
		curl_easy_cleanup(d_data.handle);
		exit(1);
	}
	
	d_chunk.memory = NULL;
	d_chunk.size = 0;
	d_data.chunk = &d_chunk;
	
	curl_easy_setopt(d_data.handle, CURLOPT_WRITEFUNCTION, csepta_write_callback);
	curl_easy_setopt(d_data.handle, CURLOPT_WRITEDATA, d_data.chunk);
	curl_easy_setopt(d_data.handle, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(d_data.handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(d_data.handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(d_data.handle, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	board.g_data = &g_data;
	board.t_data = &t_data;
	board.d_data = &d_data;
	g_trolley_read(board.g_data);
	// t_trolley_run(board.t_data);
	// d_trolley_run(board.d_data);
	curl_global_cleanup();
}

int32_t csepta_search(int32_t *arr, int32_t size, int32_t target){
	int32_t left, right, mid, i;

	i = 0;
	left = 0;
	right = size - 1;
	while (left <= right) {
		mid = left + (right - left) / 2;
		i++;

		if (arr[mid] == target) {
			return mid;
		}
		if (arr[mid] < target) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return -1;
}


size_t csepta_write_callback(char *contents, size_t size, size_t nmemb, void *userp){
	size_t realsize;
	csepta_mem_t *mem;
	char *ptr;
	
	realsize = nmemb * size;
	mem = (csepta_mem_t *) userp;
	if(mem->size < mem->size + realsize + 1){
		ptr = realloc(mem->memory, (mem->size + realsize) * 2); // probably a better way to mimize reallocs
	}
	if(ptr == NULL){
		return 0;
	}
	mem->memory = ptr;
	memcpy(&mem->memory[mem->size], contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	// printf("Memory:\n-%ld bytes\n-text: %s\n", realsize, mem->memory);
	return realsize;
}

void csepta_clear_chunk(csepta_mem_t *mem){
	free(mem->memory);
	mem->memory = NULL;
	mem->size = 0;
}

void csepta_debug_print_binary_u64(uint64_t num) {
    int32_t num_bits = sizeof(uint64_t) * CHAR_BIT;
    int32_t i;
    uint64_t mask = 1UL << (num_bits - 1);

    for (i = 0; i < num_bits; i++) {
        if (num & mask) {
            printf("1");
        } else {
            printf("0");
        }
        mask >>= 1;
    }
    printf("\n");
}

