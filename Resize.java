import java.util.Scanner;

public class Resize {
    public Resize(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		float w = s.nextFloat();
		float h = s.nextFloat();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __resize(tmp,%s,%s);\n", prob, w, h);
		return code;
	}
}