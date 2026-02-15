#ifndef __CSEPTA_H__
#define __CSEPTA_H__


/*
 * csepta_mem_t
 * ------------
 * structure to hold cURL data
 *
 * memory: a string of cURL data
 * size:   length of memory
*/
typedef struct{
    char   *memory;
    size_t  size;
} csepta_mem_t;


/*
 * csepta_station_t
 * ----------------
 * structure to hold all data related to a set of stations
 * there are at most 64 stations in a struct
 * for lines with more than 64 stations, only fill out handle & chunk sections in one and use subsequent ones just for holding station info
 *
 * array_length: the size of station_ids and bitmasks arrays
 * station_ids:  an array of station IDs
 * bitmasks:     an array of bitmasks. For an index i, bitmasks[i] is the bitmask for the station at station_ids[i]
 * set_stations: a ul where each bit represents a station
 * handle:       a cURL handle associated with this train line
 * chunk:        a store of data returned by cURL calls
*/
typedef struct{
    int            array_length;
    int           *station_ids;
    unsigned long *bitmasks;
    unsigned long  set_stations;
    CURL          *handle;
    csepta_mem_t  *chunk;
} csepta_station_t;

/*
 * csepta_board_t
 * --------------
 * structure to represent the board
 *
 * lower_g_stations: the first 64 stations for the G trolley & cURL info for the line
 * upper_g_stations: the final 50 stations for the G trolley
*/
typedef struct{
    csepta_station_t  *lower_g_stations;
    csepta_station_t  *upper_g_stations;
} csepta_board_t;

/*
 * csepta_init_and_run
 * -------------------
 * configure board structure and run reader / writer threads
 *
 * returns: none
*/
void csepta_init_and_run();


/*
 * csepta_search
 * -------------
 * implementation of binary search for read lookups
 *
 * arr:    a pointer to an array of station IDs to search in
 * size:   the length of arr
 * target: the value to be found in arr
 *
 * returns: the index of target in arr, else -1 if not found
*/
int csepta_search(int *arr, int size, int target);


/*
 * csepta_clear_chunk
 * ---------------------
 * safely free and clean up csepta_mem_t chunks
 *
 * mem: a csepta_mem_t callback written to by csepta_write_callback after a cURL operation
 *
 * returns: none
*/
void csepta_clear_chunk(csepta_mem_t *mem);

/*
 * csepta_debug_print_binary_unsigned_long
 * ---------------------------------------
 * debug function to display ul as binary
 *
 * num: an unsigned long to display as binary
 *
 * returns: none
*/
void csepta_debug_print_binary_unsigned_long(unsigned long num);


// G1
#define LOWER_G_SIZE 64
#define UPPER_G_SIZE 50
int LOWER_G_IDS[] = {341, 342, 343, 344, 345, 349, 350, 352, 481, 649, 650, 12196, 12218, 20978, 20979, 20981, 20982, 20983, 20984, 20986, 20988, 20989, 20993, 20994, 20995, 20996, 20998, 20999, 21001, 21002, 21005, 21006, 21008, 21009, 21010, 21014, 21016, 21017, 21018, 21019, 21021, 21022, 21025, 21026, 21027, 21028, 21030, 21032, 21033, 21035, 21037, 21038, 21040, 21041, 21042, 21044, 21047, 21048, 21050, 21051, 21053, 21055, 21056, 21058};
unsigned long LOWER_G_BITMASKS[] = {0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000};
int UPPER_G_IDS[] = {21060, 21061, 21062, 21063, 21067, 21068, 21069, 21070, 21071, 21072, 21073, 21075, 21078, 21079, 21080, 21081, 21082, 21083, 21086, 21087, 21090, 21091, 21093, 21095, 21096, 21098, 21100, 21101, 21103, 21105, 21107, 21108, 21109, 21110, 21111, 21113, 21114, 21481, 23992, 24038, 25779, 30290, 30291, 30292, 30550, 30605, 30791, 31347, 31443, 31540};
unsigned long UPPER_G_BITMASKS[] = {0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000, 0x0002000000000000};

#endif