#include <mysql/mysql.h>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C" {
    bool aug_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
}

bool aug_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=1){
    	strcpy(message,"usage:aug(img)\n");
	return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	strcpy(message,"arg[0] is not an image!\n");
	return 1;
    }
    return 0;
}

char* aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    char buff[50];
    snprintf(buff, sizeof(buff), "%ld\n", args->lengths[0]);
    strcpy(result,buff);

    //strcpy(result,"Hello!");
    *length = 10;
    return result;
}
