import java.util.Scanner;

public class Medianblur {
    public Medianblur(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		float f = s.nextFloat();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __medianblur(tmp,%s);\n", prob,f);
		return code;
	}
}
