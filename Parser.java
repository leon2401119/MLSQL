import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Parser{
    public Scanner in;
    public PrintWriter out;
    public Parser(String file) throws FileNotFoundException,IOException{
        this.in = new Scanner(new BufferedReader(new FileReader(file)));
        this.out = new PrintWriter(new BufferedWriter(new FileWriter("myaug.cpp")));
    }

    public static void main(String[] args) throws FileNotFoundException, IOException{
        Parser io = new Parser(args[0]);
        String pipeline = new String();
        int lineNum = 0;
        while(io.in.hasNextLine()){
            lineNum += 1;
            String line = io.in.nextLine();
            String[] splited = line.split("\\s+");
            String code;
            try{
                //System.out.println(line);
                switch(splited[1]){
                    case "cvt2gray":
                        if(splited.length!=2){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Cvt2gray.parse(line);
                        break;
                    case "hflip":
                        if(splited.length!=2){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Hflip.parse(line);
                        break;
                    case "vflip":
                        if(splited.length!=2){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Vflip.parse(line);
                        break;
                    case "chshuffle":
                        if(splited.length!=2){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Chshuffle.parse(line);
                        break;
                    case "randcrop":
                        if(splited.length!=7){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Randcrop.parse(line);
                        break;
                    case "rotation":
                        if(splited.length!=3){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Rotation.parse(line);
                        break;
                    case "noise":
                        if(splited.length!=5){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Noise.parse(line);
                        break;
                    case "medianblur":
                        if(splited.length!=3){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Medianblur.parse(line);
                        break;
                    case "gaussianblur":
                        if(splited.length!=6 && splited.length!=5){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Gaussianblur.parse(line);
                        break;
                    case "resize":
                        if(splited.length!=4){
                            System.out.println("syntax error at line "+ lineNum);
                            return;
                        }
                        code = Resize.parse(line);
                        break;
                    default:
                        System.out.println("syntax error at line "+ lineNum);
                        continue;
                }
                pipeline += code;
            }
            catch(Exception e){
                System.out.println("syntax error at line "+ lineNum);
            }
        }
        //System.out.print(pipeline);
        io.out.print("#include <mysql/mysql.h>\n#include <cstring>\n#include <opencv2/opencv.hpp>\n#include <time.h>\n");
        io.out.print("using namespace cv;\n\n");
        io.out.print("extern \"C\" {\n");
        io.out.print("bool aug_init(UDF_INIT *initid, UDF_ARGS *args, char *message);\n");
        io.out.print("char* aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error);\n");
        io.out.print("void aug_deinit(UDF_INIT *const initid);\n}\n");
        io.out.print("Mat __hflip(Mat);\n");
        io.out.print("Mat __vflip(Mat);\n");
        io.out.print("Mat __chshuffle(Mat);\n");
        io.out.print("Mat __randcrop(Mat, float, float, Scalar);\n");
        io.out.print("Mat __noise(Mat, const char*, int, int);\n");
        io.out.print("Mat __cvt2gray(Mat);\n");
        io.out.print("Mat __medianblur(Mat, double);\n");
        io.out.print("Mat __gaussianblur(Mat,double,double,double,double);\n");
        io.out.print("Mat __resize(Mat, double, double);\n");
        io.out.print("Mat __rotation(Mat, double);\n");
        io.out.print("bool aug_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {\nif(args->arg_count != 1){\n");
        io.out.print("strcpy(message,\"usage:aug(blob img)\");\nreturn 1;\n}\ninitid->max_length = 16000000;\nreturn 0;\n}\n");
        io.out.print("char* aug(UDF_INIT *const initid, UDF_ARGS *const args, char *const result, unsigned long *const length, char *const is_null, char *const error)\n{");
        io.out.print("srand(time(NULL));\n");
        io.out.print("std::vector<char> buffer(args->args[0], args->args[0]+args->lengths[0]);\n");  
        io.out.print("Mat image = cv::imdecode(buffer, IMREAD_ANYCOLOR);\nMat tmp = image.clone();\n");       
        io.out.print(pipeline);
        io.out.print("std::vector<unsigned char> img_encoded;\n");
        io.out.print("cv::imencode(\".jpg\", tmp, img_encoded);\n");
        io.out.print("char *t = reinterpret_cast<char*>(img_encoded.data());\n");
        io.out.print("*length = (unsigned long)img_encoded.size();\n");
        io.out.print("return t;\n");
        io.out.print("}\n");
        io.out.print("void aug_deinit(UDF_INIT *const initid){}");
        io.in.close();
        io.out.close();
        return;
    }
}
