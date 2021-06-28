import java.util.Scanner;

public class Chshuffle {
    public Chshuffle(){}
	public static String parse(String line) throws Exception{
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __chshuffle(tmp);\n",prob);
		return code;
	}
}
