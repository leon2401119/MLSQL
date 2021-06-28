import java.util.Scanner;

public class Rotation {
    public Rotation(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		float angle = s.nextFloat();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __rotation(tmp,%s);\n", prob, angle);
		return code;
	}
}
