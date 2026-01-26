#include <curl/curl.h>
#include <pthread.h>

#define VALID_REQUESTS "JSON_data/valid_requests.json"
#define REGIONAL_RAIL_URL "https://www3.septa.org/api/TrainView/index.php"
#define TROLLEY_URL "https://www3.septa.org/api/TransitViewAll/index.php"
#define G_TROLLEY_URL "https://api.septa.org/api/TransitView/index.php?route=G1"

