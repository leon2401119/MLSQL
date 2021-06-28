import java.util.Scanner;

public class Noise {
    public Noise(){}
	public static String parse(String line) throws Exception{
		//System.out.print("1");
		Scanner s = new Scanner(line);
		float prob = s.nextFloat();
		s.next();
		String mode = s.next();
		int i1 = s.nextInt();
		int i2 = s.nextInt();
		String code = String.format("if(((float)(rand()%%100))/100.0 < %s) tmp = __noise(tmp,\"%s\",%s,%s);\n",prob,mode,i1,i2);
		return code;
	}
}
