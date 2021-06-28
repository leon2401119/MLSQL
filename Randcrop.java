import java.util.Scanner;

public class Randcrop {
    public Randcrop(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		float height = s.nextFloat();
		float width = s.nextFloat();
		int r = s.nextInt();
		int g = s.nextInt();
		int b = s.nextInt();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __randcrop(tmp,%s,%s,Scalar(%s,%s,%s));\n", prob, height, width, r, g, b);
		return code;
	}
}
