#include <mysql/mysql.h>
#include <cstring>

extern "C" {
    bool foo_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *foo(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
}

bool foo_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    // strcpy(message,"init\n");
    return 0;
}

char* foo(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    //result = new char[10];
    strcpy(result,"Hello!");
    *length = 10;
    return result;
}
