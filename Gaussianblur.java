import java.util.Scanner;

public class Gaussianblur {
    public Gaussianblur(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		float w = s.nextFloat();
		float h = s.nextFloat();
		float sigmaX = s.nextFloat();
		float sigmaY = sigmaX;
		if(s.hasNext()){
			sigmaY = s.nextFloat();
		}
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __gaussianblur(tmp,%s,%s,%s,%s);\n", prob,w,h,sigmaX,sigmaY);
		return code;
	}
}
