#include <mysql/mysql.h>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;

extern "C" {
    bool hflip_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *hflip(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void hflip_deinit(UDF_INIT *const initid);
    bool vflip_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *vflip(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void vflip_deinit(UDF_INIT *const initid);
    bool chshuffle_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *chshuffle(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void chshuffle_deinit(UDF_INIT *const initid);
    bool randcrop_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *randcrop(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void randcrop_deinit(UDF_INIT *const initid);

}

Mat __hflip(Mat &in){
	Mat out;
	flip(in,out,0);
	return out;
}

Mat __vflip(Mat &in){
	Mat out;
	flip(in,out,1);
	return out;
}

Mat __chshuffle(Mat &in){
	Mat out;
	Mat ch1, ch2, ch3;
	std::vector<Mat> channels(3);
	split(in, channels);
  	// get the channels (follow BGR order in OpenCV)
  	ch1 = channels[0];
  	ch2 = channels[1];
  	ch3 = channels[2];
	switch(rand()%6){
	  case 0:
                channels[0] = ch1;
                channels[1] = ch2;
                channels[2] = ch3;
                break;
          case 1:
                channels[0] = ch1;
                channels[1] = ch3;
                channels[2] = ch2;
                break;
          case 2:
                channels[0] = ch2;
                channels[1] = ch1;
                channels[2] = ch3;
                break;
          case 3:
                channels[0] = ch2;
                channels[1] = ch3;
                channels[2] = ch1;
                break;
          case 4:
                channels[0] = ch3;
                channels[1] = ch1;
                channels[2] = ch2;
                break;
          case 5:
                channels[0] = ch3;
                channels[1] = ch2;
                channels[2] = ch1;
                break;

	}
	merge(channels, out);
	return out;
}

Mat __randcrop(Mat &in, float p_row, float p_col, Scalar color){
        Mat out = in;
	Point root_points[1][4];
  	int len_row = in.rows*p_row, len_col = in.cols*p_col;
  	
  	int start_row = rand()%(in.rows-len_row);
  	int start_col = rand()%(in.cols-len_col);
  	
  	root_points[0][0] = Point(start_col, start_row);
  	root_points[0][1] = Point(start_col, start_row + len_row);
  	root_points[0][2] = Point(start_col + len_col, start_row + len_row);
  	root_points[0][3] = Point(start_col + len_col, start_row);
  	
  	const Point* ppt[1] = { root_points[0] };
  	int npt[] = { 4 };

  	fillPoly(out ,ppt, npt, 1, Scalar(255, 255, 255));
	return out;
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
    cv::Mat image_fliped = __hflip(image);
    //cv::flip(image, image_fliped, 0);
    //cv::flip(i2, image_fliped, 0);

    std::vector<unsigned char> img_encoded;
    
    /*
    std::vector<int> param = std::vector<int>(2);
    param[0] = IMWRITE_JPEG_OPTIMIZE;
    param[1] = 1;
    */

    cv::imencode(".jpg", image_fliped, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void hflip_deinit(UDF_INIT *const initid){
    return;
}


bool vflip_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=1){
    	strcpy(message,"usage:vflip(img)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* vflip(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_fliped = __vflip(image);
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_fliped, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void vflip_deinit(UDF_INIT *const initid){
}

bool chshuffle_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=1){
        strcpy(message,"usage:vflip(img)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
        strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* chshuffle(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_shuffled = __chshuffle(image);
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_shuffled, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void chshuffle_deinit(UDF_INIT *const initid){
}

bool randcrop_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=6){
        strcpy(message,"usage:randcrop(img,crop_row%,crop_col%,r,g,b)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
        strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    if(args->arg_type[1] != DECIMAL_RESULT){
        strcpy(message,"arg[1] is not an float!\n");
        return 1;
    }
    if(!(atof(args->args[1]) <= 1 && atof(args->args[1]) >= 0)){
    	strcpy(message,"arg[1] should have value between 0 and 1!\n");
	return 1;
    }
    if(args->arg_type[2] != DECIMAL_RESULT){
        strcpy(message,"arg[2] is not an float!\n");
        return 1;
    }
    if(!(atof(args->args[2]) <= 1 && atof(args->args[2]) >= 0)){
        strcpy(message,"arg[2] should have value between 0 and 1!\n");
        return 1;
    }
    if(args->arg_type[3] != INT_RESULT){
        strcpy(message,"arg[3] is not an int!\n");
        return 1;
    }
    if(!(*(long long*)args->args[3] <= 255 && *(long long*)args->args[3] >= 0)){
        strcpy(message,"arg[3] should have value between 0 and 255!\n");
        return 1;
    }
    if(args->arg_type[4] != INT_RESULT){
        strcpy(message,"arg[4] is not an int!\n");
        return 1;
    }
    if(!(*(long long*)args->args[4] <= 255 && *(long long*)args->args[4] >= 0)){
        strcpy(message,"arg[4] should have value between 0 and 255!\n");
        return 1;
    }
    if(args->arg_type[5] != INT_RESULT){
        strcpy(message,"arg[5] is not an int!\n");
        return 1;
    }
    if(!(*(long long*)args->args[5] <= 255 && *(long long*)args->args[5] >= 0)){
        strcpy(message,"arg[5] should have value between 0 and 255!\n");
        return 1;
    }

    initid->max_length = 16000000;
    return 0;
}

char* randcrop(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_shuffled = __randcrop(image,atof(args->args[1]),atof(args->args[2]),Scalar(*(long long*)args->args[3], *(long long*)args->args[4], *(long long*)args->args[5]));
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_shuffled, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void randcrop_deinit(UDF_INIT *const initid){
}

