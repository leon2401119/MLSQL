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
/*
char* aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    
    char buff[50];
    snprintf(buff, sizeof(buff), "%ld\n", args->lengths[0]);
    strcpy(result,buff);
    

    Mat img = imdecode(args->args[0],CV_LOAD_IMAGE_COLOR);

    //strcpy(result,"Hello!");
    *length = 10;
    return result;
}
*/

char* aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, 2);
    cv::Mat image_fliped;
    cv::flip(image, image_fliped, 0);
    std::vector<unsigned char> img_encoded;
    std::vector<int> param = std::vector<int>(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 100;
    cv::imencode(".png", image_fliped, img_encoded, param);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    strcpy(result, tmp);
    //*length = img_encoded.size();
    return result;
}
