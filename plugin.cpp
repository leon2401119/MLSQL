#include <mysql/mysql.h>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
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
    bool noise_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *noise(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void noise_deinit(UDF_INIT *const initid);
    bool cvt2gray_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char *cvt2gray(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void cvt2gray_deinit(UDF_INIT *const initid);
    bool rotation_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char* rotation(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void rotation_deinit(UDF_INIT *const initid);
    bool resize_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char* resize(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void resize_deinit(UDF_INIT *const initid);
    bool gaussianblur_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char* gaussianblur(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void gaussianblur_deinit(UDF_INIT *const initid);
    bool medianblur_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    char* medianblur(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    void medianblur_deinit(UDF_INIT *const initid);
    //bool decode_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
    //char* decode(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);
    //void decode_deinit(UDF_INIT *const initid);
}

Mat __hflip(Mat in){
	Mat out;
	flip(in,out,0);
	return out;
}

Mat __vflip(Mat in){
	Mat out;
	flip(in,out,1);
	return out;
}

Mat __chshuffle(Mat in){
  Mat out=in.clone();
  int p = rand()%6;
  switch(p){
    case 0:
      for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[0];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[1];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[2];
        }
      }
      break;
    case 1:
      for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[0];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[2];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[1];
        }
      }
      break;
    case 2:
      for (int i = 0; i < in.rows; i++){
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[1];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[0];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[2];
        }
      }
      break;
    case 3:
      for (int i = 0; i < in.rows; i++){
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[1];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[2];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[0];
        }
      }
      break;
    case 4:
      for (int i = 0; i < in.rows; i++){
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[2];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[0];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[1];
        }
      }
      break;
    case 5:
      for (int i = 0; i < in.rows; i++){
        for (int j = 0; j < in.cols; j++){ 
          out.at<Vec3b>(i, j)[0] = in.at<Vec3b>(i,j)[2];
          out.at<Vec3b>(i, j)[1] = in.at<Vec3b>(i,j)[1];
          out.at<Vec3b>(i, j)[2] = in.at<Vec3b>(i,j)[0];
        }
      }
      break;
  }
	return out;
}

Mat __randcrop(Mat in, float p_row, float p_col, Scalar color){
        Mat out = in.clone();
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

  	fillPoly(out ,ppt, npt, 1, color);
	return out;
}


Mat __noise(Mat in, const char* type, int i1, int i2){
	/* 
	    for gaussian noise, i1 = mean & i2 = stdev
	    for uniform noise, i1 = min & i2 = max
	*/

	Mat out = in.clone();
	Mat noise_mat = Mat::zeros (out.rows, out.cols, CV_8UC1);
        if(out.channels()==1){  //gray scale image
		if(!strcmp(type,"Gaussian") || !strcmp(type,"gaussian"))
                	randn(noise_mat, i1, i2);
		else if(!strcmp(type,"Uniform") || !strcmp(type,"uniform"))
			randu(noise_mat, i1, i2);
                out += noise_mat;
        }
        else{
                std::vector<Mat> channels(3);
                split(out, channels);
		if(!strcmp(type,"Gaussian") || !strcmp(type,"gaussian")){
                	for(int i=0;i<3;i++){
                        	randn(noise_mat, i1, i2);
                        	channels[i] += noise_mat;
                	}
		}
		else if(!strcmp(type,"Uniform") || !strcmp(type,"uniform")){
                        for(int i=0;i<3;i++){
                                randu(noise_mat, i1, i2);
                                channels[i] += noise_mat;
                        }
                }
                merge(channels, out);
		/*
		Mat noise_mat_3(Size(out.rows,out.cols), CV_8UC(3));
		randn(noise_mat_3,i1,i2);
		out += noise_mat_3;
		*/
        }
	return out;
}

Mat __cvt2gray(Mat in){
	Mat out;
	cvtColor(in, out, CV_BGR2GRAY);
	return out;
}

Mat __medianblur(Mat image, double kernel_ratio_to_image_height){
    Mat image_blurred;
    int ksize = (int)(kernel_ratio_to_image_height*image.rows);
    if(ksize > image.cols)
      ksize = image.cols;
    if(ksize > image.rows)
      ksize = image.rows;
    if(ksize > 255)
      ksize = 255;
    if(ksize%2 == 0)
      ksize -= 1;
    medianBlur(	image, image_blurred, ksize);
    return image_blurred;
}

Mat __gaussianblur(Mat image, double kernel_width_ratio, double kernel_height_ratio, double sigmaX, double sigmaY){
    Mat image_blurred;
    int w = image.cols*kernel_width_ratio;
    int h = image.rows*kernel_height_ratio;
    if(w%2==0)
      w+=1;
    if(h%2==0)
      h+=1;
    if(w>image.cols)
      w-=2;
    if(h>image.rows)
      h-=2;
    GaussianBlur(image, image_blurred, Size(w, h), sigmaX, sigmaY, 0);
    return image_blurred;
}

Mat __resize(Mat image, double width_ratio, double height_ratio){
    Mat image_resized;
    resize(image, image_resized, Size(image.cols*width_ratio, image.rows*height_ratio), 0, 0, 0);
    return image_resized;
}

Mat __rotation(Mat image, double angle){
    Point2f center(image.cols/2.0F, image.rows/2.0F);
    Mat M = getRotationMatrix2D(center, angle, 1.0);

    Mat image_rotated;
    warpAffine(image, image_rotated, M, image.size());
    return image_rotated;
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

bool noise_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=4){
        strcpy(message,"usage:noise(img,noise_type('Gaussian','Uniform',...etc),mean,stdev)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
        strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    if(args->arg_type[1] != STRING_RESULT){
        strcpy(message,"arg[1] is not an string!\n");
        return 1;
    }
    if(strcmp(args->args[1],"Gaussian") && strcmp(args->args[1],"gaussian") && strcmp(args->args[1],"Uniform") && strcmp(args->args[1],"uniform")){
    	strcpy(message,"arg[1] is not a valid noise type!\n Currently available noises : Gaussian, Uniform\n");
	return 1;
    }
    if(args->arg_type[2] != INT_RESULT){
        strcpy(message,"arg[2] is not an int!\n");
        return 1;
    }
    if(args->arg_type[3] != INT_RESULT){
        strcpy(message,"arg[3] is not an int!\n");
        return 1;
    }

    initid->max_length = 16000000;
    return 0;
}

char* noise(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_out = __noise(image,args->args[1],*(long long*)args->args[2],*(long long*)args->args[3]);
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_out, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void noise_deinit(UDF_INIT *const initid){
}

bool cvt2gray_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
    if(args->arg_count!=1){
        strcpy(message,"usage:cvt2gray(img)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
        strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* cvt2gray(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    cv::Mat image_out = __cvt2gray(image);
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_out, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void cvt2gray(UDF_INIT *const initid){
}

bool medianblur_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count != 2){
    	strcpy(message,"usage:medianBlur(blob img,double kernel_ratio_to_image_height)\n");
	    return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	    strcpy(message,"arg[0] is not an image!\n");
	    return 1;
    }
    if(args->arg_type[1] != INT_RESULT && args->arg_type[1] != DECIMAL_RESULT){
        strcpy(message,"arg[1] is not a number!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* medianblur(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    Mat image_blurred;
    if(args->arg_type[1] == INT_RESULT)
        image_blurred = __medianblur(image, *(long long*)args->args[1]);
    else
        image_blurred = __medianblur(image, atof(args->args[1]));
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_blurred, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void medianblur_deinit(UDF_INIT *const initid){
}

bool gaussianblur_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count != 4 && args->arg_count != 5){
    	strcpy(message,"usage:gaussianBlur(blob img,double kernel_width_ratio,double kernel_height_ratio,double sigmaX[,double sigmaY])\n");
	    return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	    strcpy(message,"arg[0] is not an image!\n");
	    return 1;
    }
    if(args->arg_type[1] != INT_RESULT && args->arg_type[1] != DECIMAL_RESULT){
        strcpy(message,"arg[1] is not a number!\n");
        return 1;
    }
    if(args->arg_type[2] != INT_RESULT && args->arg_type[2] != DECIMAL_RESULT){
        strcpy(message,"arg[2] is not a number!\n");
        return 1;
    }
    if(args->arg_type[3] != INT_RESULT && args->arg_type[3] != DECIMAL_RESULT){
        strcpy(message,"arg[3] is not a number!\n");
        return 1;
    }
    if(args->arg_count == 5){
        if(args->arg_type[4] != INT_RESULT && args->arg_type[4] != DECIMAL_RESULT){
            strcpy(message,"arg[4] is not a number!\n");
            return 1;
        }
    }
    if(args->arg_type[1] == INT_RESULT && *(long long*)args->args[1]>1){
        strcpy(message,"The kernel width should be smaller than the image width!\n");
        return 1;
    }
    if(args->arg_type[1] == DECIMAL_RESULT && atof(args->args[1])>1){
        strcpy(message,"The kernel width should be smaller than the image width!\n");
        return 1;
    }
    if(args->arg_type[2] == INT_RESULT && *(long long*)args->args[2]>1){
        strcpy(message,"The kernel height should be smaller than the image height!\n");
        return 1;
    }
    if(args->arg_type[2] == DECIMAL_RESULT && atof(args->args[2])>1){
        strcpy(message,"The kernel height should be smaller than the image height!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* gaussianblur(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    Mat image_blurred;
    if(args->arg_type[1] == INT_RESULT
        && args->arg_type[2] == INT_RESULT
        && args->arg_type[3] == INT_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], *(long long*)args->args[3], *(long long*)args->args[4]);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], *(long long*)args->args[3], sigma);
            }
        }
        else
            image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], *(long long*)args->args[3], *(long long*)args->args[3]);
    }
    else if(args->arg_type[1] == DECIMAL_RESULT
        && args->arg_type[2] == INT_RESULT
        && args->arg_type[3] == INT_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], *(long long*)args->args[3], sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], *(long long*)args->args[3], sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], *(long long*)args->args[3], *(long long*)args->args[3]);
    }
    else if(args->arg_type[1] == INT_RESULT
        && args->arg_type[2] == DECIMAL_RESULT
        && args->arg_type[3] == INT_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), *(long long*)args->args[3], sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), *(long long*)args->args[3], sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), *(long long*)args->args[3], *(long long*)args->args[3]);
    }
    else if(args->arg_type[1] == INT_RESULT
        && args->arg_type[2] == INT_RESULT
        && args->arg_type[3] == DECIMAL_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], atof(args->args[3]), sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], atof(args->args[3]), sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, *(long long*)args->args[1], *(long long*)args->args[2], atof(args->args[3]), atof(args->args[3]));
    }
    else if(args->arg_type[1] == INT_RESULT
        && args->arg_type[2] == DECIMAL_RESULT
        && args->arg_type[3] == DECIMAL_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), atof(args->args[3]), sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), atof(args->args[3]), sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, *(long long*)args->args[1], atof(args->args[2]), atof(args->args[3]), atof(args->args[3]));
    }
    else if(args->arg_type[1] == DECIMAL_RESULT
        && args->arg_type[2] == INT_RESULT
        && args->arg_type[3] == DECIMAL_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], atof(args->args[3]), sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], atof(args->args[3]), sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, atof(args->args[1]), *(long long*)args->args[2], atof(args->args[3]), atof(args->args[3]));
    }
    else if(args->arg_type[1] == DECIMAL_RESULT
        && args->arg_type[2] == DECIMAL_RESULT
        && args->arg_type[3] == INT_RESULT){
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), *(long long*)args->args[3], sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), *(long long*)args->args[3], sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), *(long long*)args->args[3], *(long long*)args->args[3]);
    }
    //all decimal
    else{
        if(args->arg_count == 5){
            if(args->arg_type[4] == INT_RESULT){
                long long sigma = *(long long*)args->args[4];
                image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), *(long long*)args->args[3], sigma);
            }
            else{
                double sigma = atof(args->args[4]);
                image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), *(long long*)args->args[3], sigma);
            }

        }
        else
            image_blurred = __gaussianblur(image, atof(args->args[1]), atof(args->args[2]), atof(args->args[3]), atof(args->args[3]));
    }
    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_blurred, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());

    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void gaussianblur_deinit(UDF_INIT *const initid){
}

bool resize_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count != 3 && args->arg_count != 4){
    	strcpy(message,"usage:resize(blob img,int width_ration,int height_ration) or resize(blog img,int width_ration,int height_ration,int interpolation)\n");
	    return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	    strcpy(message,"arg[0] is not an image!\n");
	    return 1;
    }
    if(args->arg_type[1] != INT_RESULT && args->arg_type[1] != DECIMAL_RESULT){
        strcpy(message,"arg[1] is not a number!\n");
        return 1;
    }
    if(args->arg_type[2] != INT_RESULT && args->arg_type[2] != DECIMAL_RESULT){
        strcpy(message,"arg[2] is not a number!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* resize(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    Mat image_resized;
    if(args->arg_type[1] == INT_RESULT && args->arg_type[2] == INT_RESULT){
        image_resized = __resize(image, *(long long*)args->args[1], *(long long*)args->args[2]);
    }
    else if(args->arg_type[1] == INT_RESULT && args->arg_type[2] == DECIMAL_RESULT){
        image_resized = __resize(image, *(long long*)args->args[1], atof(args->args[2]));
    }
    else if(args->arg_type[1] == DECIMAL_RESULT && args->arg_type[2] == DECIMAL_RESULT){
        image_resized = __resize(image, atof(args->args[1]), atof(args->args[2]));
    }
    else{
        image_resized = __resize(image, atof(args->args[1]), *(long long*)args->args[2]);
    }

    std::vector<unsigned char> img_encoded;
    cv::imencode(".jpg", image_resized, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());

    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void resize_deinit(UDF_INIT *const initid){
}

bool rotation_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count != 2){
    	strcpy(message,"usage:rotation(blob img,double angle) or rotation(blob img,int angle)\n");
	    return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
	    strcpy(message,"arg[0] is not an image!\n");
	    return 1;
    }
    if(args->arg_type[1] != DECIMAL_RESULT && args->arg_type[1] != INT_RESULT){
        strcpy(message,"arg[1] is not a number!\n");
	    return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* rotation(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);
    Mat image_rotated;
    if(args->arg_type[1] == DECIMAL_RESULT){
        image_rotated = __rotation(image, atof(args->args[1]));
    }
    else{
        image_rotated = __rotation(image, *(long long*)args->args[1]);
    }
    std::vector<unsigned char> img_encoded;
    imencode(".jpg", image_rotated, img_encoded);
    char *tmp = reinterpret_cast<char*>(img_encoded.data());
    *length = (unsigned long)img_encoded.size();
    return tmp;
}

void rotation_deinit(UDF_INIT *const initid){
}


bool decode_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if(args->arg_count!=1){
        strcpy(message,"usage:decode(img)\n");
        return 1;
    }
    if(args->arg_type[0] != STRING_RESULT){
        strcpy(message,"arg[0] is not an image!\n");
        return 1;
    }
    initid->max_length = 16000000;
    return 0;
}

char* decode(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)
{
    std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);
    cv::Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);

    cv::Mat flat = image.reshape(1, image.total()*image.channels());
    std::vector<unsigned char> arr = image.isContinuous()? flat : flat.clone();

    char *tmp = reinterpret_cast<char*>(arr.data());
    *length = (unsigned long)arr.size();
    return tmp;
}

void decode_deinit(UDF_INIT *const initid){
}

