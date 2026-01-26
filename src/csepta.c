#include "../lib/csepta.h"
#include "g_trolley.c"

void csepta_init(){
	struct curl_slist *g_slist;
	mem_t g_chunk;
	station_t lower_g, upper_g;
    // setup curl (not threadsafe)
	curl_global_init(CURL_GLOBAL_ALL);
	
	// setup board
	csepta_board_t board;
	
	// setup G line -> lower contains all the control info (chunk, handle, etc)
	lower_g.array_length = LOWER_G_SIZE;
	lower_g.station_ids = LOWER_G_IDS;
	lower_g.bitmasks = LOWER_G_BITMASKS;
	lower_g.set_stations = 0x0;
	
	lower_g.handle = curl_easy_init();
	if(!lower_g.handle){
		fprintf(stderr, "curl handle could not be initalized\n");
		curl_easy_cleanup(lower_g.handle);
		exit(1);
	}
	g_chunk.memory = NULL;
	g_chunk.size = 0;
	lower_g.chunk = &g_chunk;
	
	g_slist = NULL;
  	g_slist = curl_slist_append(g_slist, "accept: application/json");
	curl_easy_setopt(lower_g.handle, CURLOPT_URL, G_TROLLEY_URL);
	curl_easy_setopt(lower_g.handle, CURLOPT_WRITEFUNCTION, cespta_write_callback);
	curl_easy_setopt(lower_g.handle, CURLOPT_WRITEDATA, &lower_g.chunk);
	curl_easy_setopt(lower_g.handle, CURLOPT_HTTPHEADER, g_slist);
	curl_easy_setopt(lower_g.handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(lower_g.handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(lower_g.handle, CURLOPT_BUFFERSIZE, 102400L); // could probably set lower than 10mb
	
	// setup upper g train station
	upper_g.array_length = UPPER_G_SIZE;
	upper_g.station_ids = UPPER_G_IDS;
	upper_g.set_stations = 0x0;
	upper_g.bitmasks = UPPER_G_BITMASKS;
	
	
	
	// setup threads
    // ideally there is a writer thread and a updater thread
    // the writer thread constantly reads from the board struct and updates the LEDs
	// the updater thread round robin pulls data from septa API / timetable files
    // pthread_t thread1;
	// pthread_create(&thread1, NULL, csepta_list_g_trolley, &board);
    // pthread_join(thread1, NULL);
	board.lower_g_stations = &lower_g;
	board.upper_g_stations = &upper_g;
	g_trolley_read((void *) &board);
	curl_global_cleanup();
}

size_t cespta_write_callback(char *contents, size_t size, size_t nmemb, void *userp){
	size_t realsize;
	mem_t *mem;
	char *ptr;
	
	realsize = nmemb * size;
	mem = (mem_t *) userp;
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
	printf("Memory:\n-%ld bytes\n-text: %s\n", realsize, mem->memory);
	return realsize;
}

long csepta_get_filesize(FILE *fp){
    long filesize;

    if( fseek(fp, 0, SEEK_END) != 0) exit(EXIT_FAILURE);
	
    filesize = ftell(fp);
    rewind(fp);
    return filesize;
}

void csepta_print_array(int *arr, int size){
	int i;
	
	for(i = 0; i < size; i++){
		printf("arr[%d] = %d\n", i, arr[i]);
	}
}

int csepta_search(int *arr, int size, int target){
	int left, right, mid, i;

	// printf("csepta_search array:\n");
	// csepta_print_array(arr, size);
	i = 0;
	left = 0;
	right = size - 1;
	// printf("\nTarget = %d\n\n", target);
	while (left <= right) {
		mid = left + (right - left) / 2;
		// printf("Iteration %d:\nleft = %d\nright = %d\nmid = %d\narr[mid] = %d\n\n", i, left, right, mid, arr[mid]);
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
