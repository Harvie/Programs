public class prubeh {
	public static void line(int i) {
		for(;--i>0;) {
			System.out.print(" ");
		}
		System.out.println("+");
	}

	public static void main(String[] argv) {
		//if(argv.length != 2) { System.out.println("Usage: java filem [int zoom] [outputfile]");	System.exit(0); }
		float a=0, b=0, c=0;
		for(int i = 0;i<=60;i++) {
			a+=0.1;
			b+=0.01;
			c+=0.5;
			line((int) Math.round( 50+(( Math.sin(a)+Math.sin(b)+Math.sin(c) )*20) ));
		}
	}
}
