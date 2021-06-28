import java.util.Scanner;

public class Hflip {
    public Hflip(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __hflip(tmp);\n", prob);
		return code;
	}
}
