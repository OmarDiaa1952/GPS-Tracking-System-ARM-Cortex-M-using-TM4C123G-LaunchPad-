int readGPS(char * lat, char * log);

double calcDistance(volatile double latitude_1, volatile double logitude_1, volatile double latitude_2, volatile double logitude_2);

double floatVal(char *c);

void readGPSDouble(volatile double * dlat,volatile double * dlog);

void concat(char * holder, char * lat, char *log);

void splitAndStoreDouble(char * data, volatile double * startLat, volatile double * startLog, volatile double * endLat, volatile double * endLog);
	
