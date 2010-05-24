/*
0 ;) harvie@harvie-ntb prg $ java clock 1234567890-
Clock:

            ###    ###           ###    ###    ###    ###    ###    ###        
        #      #      #  #   #  #      #          #  #   #  #   #  #   #       
        #      #      #  #   #  #      #          #  #   #  #   #  #   #       
            ###    ###    ###    ###    ###           ###    ###           ### 
        #  #          #      #      #  #   #      #  #   #      #  #   #       
        #  #          #      #      #  #   #      #  #   #      #  #   #       
            ###    ###           ###    ###           ###           ###        
*/

class digital {
	public String[] digits = new String[5];
	public StringBuffer[] digit = new StringBuffer[5];

	public void next_digit() {
		int i;
		for(i=4;i>=0;i--) digits[i]=digits[i]+"  "+digit[i];
		for(i=4;i>=0;i--) digit[i]=new StringBuffer("     ");
	}

	public void segment(int i) {
		switch(i) {
			case 1: digit[0]=new StringBuffer(" --- ");	break;
			case 4: digit[2]=new StringBuffer(" --- ");	break;
			case 7: digit[4]=new StringBuffer(" --- ");	break;
			case 2: digit[1].setCharAt(0,'|');	break;
			case 3: digit[1].setCharAt(4,'|');	break;
			case 5: digit[3].setCharAt(0,'|');	break;
			case 6: digit[3].setCharAt(4,'|');	break;
		}
	}

	public void segments(String segs) {
		for(int i=segs.length()-1;i>=0;i--) segment( Integer.parseInt(Character.toString(segs.charAt(i))) );
	}

	public void numero(int i) {
		switch(i) {
			case 0: segments("123567");	break;
			case 1: segments("36");	break;
			case 2: segments("13457");	break;
			case 3: segments("13467");	break;
			case 4: segments("2346");	break;
			case 5: segments("12467");	break;
			case 6: segments("124567");	break;
			case 7: segments("136");	break;
			case 8: segments("1234567");	break;
			case 9: segments("12346");	break;
			default: segments("4");	break;
		}
		next_digit();
	}
	public digital() {
		int i;
		for(i=4;i>=0;i--) digit[i]=new StringBuffer("");
		for(i=4;i>=0;i--) digits[i]=new String("");
		next_digit();
	}

	public void parse(String str) {
		for(int i=0;i<str.length();i++) {
			try {
				numero( Integer.parseInt(Character.toString(str.charAt(i))) );
			} catch(Exception e) {
				numero(-1);
			}
		}
	}

	public void print() {
		int i;
		for(i=0;i<5;i++) {
			System.out.println(digits[i]);
			if(i == 1 || i == 3) System.out.println(digits[i]);
		}
	}
}

public class clock {
	public static void main(String[] argv) {
		if(argv.length != 1) { System.out.println("Usage: java clock 22:10");	System.exit(0); }
		System.out.println("Clock:\n");
		digital d = new digital();
		//for(int i=-1;i<=9;i++) d.numero(i);
		//d.parse("12:10");
		d.parse(argv[0]);
		d.print();

	}
}
