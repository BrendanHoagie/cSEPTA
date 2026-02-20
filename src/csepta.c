#include "../lib/csepta.h"
#include "g_trolley.c"

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
	struct curl_slist *g_slist;
	csepta_mem_t g_chunk;
	csepta_station_t g_data;
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
	csepta_board_t board;
	// pthread_t thread1;
	
	curl_global_init(CURL_GLOBAL_ALL);
		
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
		
	g_slist = NULL;
  	g_slist = curl_slist_append(g_slist, "accept: application/json");
	curl_easy_setopt(g_data.handle, CURLOPT_URL, G_TROLLEY_URL);
	curl_easy_setopt(g_data.handle, CURLOPT_WRITEFUNCTION, csepta_write_callback);
	curl_easy_setopt(g_data.handle, CURLOPT_WRITEDATA, g_data.chunk);
	curl_easy_setopt(g_data.handle, CURLOPT_HTTPHEADER, g_slist);
	curl_easy_setopt(g_data.handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(g_data.handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(g_data.handle, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	// setup threads
    // ideally there is a writer thread and a updater thread
    // the writer thread constantly reads from the board struct and updates the LEDs
	// the updater thread round robin pulls data from septa API / timetable files
	
	// pthread_create(&thread1, NULL, csepta_list_g_trolley, &board);
    // pthread_join(thread1, NULL);
	board.g_data = &g_data;
	g_trolley_read(board.g_data);
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

void csepta_debug_print_binary_unsigned_long(uint64_t num) {
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

