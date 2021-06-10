#include <mysql/mysql.h>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C" {
    bool hflip_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *hflip(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    //void hflip_deinit(UDF_INIT *const initid);
}

bool hflip_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=1){
    	strcpy(message,"usage:aug(img)\n");
	return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	strcpy(message,"arg[0] is not an image!\n");
	return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* hflip(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_fliped;
    cv::flip(image, image_fliped, 0);

	// above doesn't cause error

    std::vector<unsigned char> img_encoded;
    std::vector<int> param = std::vector<int>(2);
    param[0] = IMWRITE_PNG_COMPRESSION;
    param[1] = 3;

    	// above also error-free

    cv::imencode(".png", image_fliped, img_encoded, param);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    
    	// also no error	

    char *r = new char[img_encoded.size()];
    memcpy(r, tmp, img_encoded.size());
    *length = (unsigned long)img_encoded.size();
    return r;
}
/*
void hflip_deinit(UDF_INIT *const initid){
	
}
*/
