public class obrazce {
	private static void obrazec(int o) {
		switch(o) {
			case 0: //ctverec
				System.out.println("+--+");
				System.out.println("|  |");
				System.out.println("+--+");
			break;
			case 1: //kruh
				/*System.out.println("  ###");
				System.out.println(" #   #");
				System.out.println("#  +  #");
				System.out.println(" #   #");
				System.out.println("  ###");*/

				System.out.println("                        __.......__                        ");
				System.out.println("                   _.-''           '-..                    ");
				System.out.println("                ,-'                    '-.                 ");
				System.out.println("              ,'                          '.               ");
				System.out.println("            ,'                              '\\             ");
				System.out.println("           /                                  `            ");
				System.out.println("          /                                    `.          ");
				System.out.println("         /                                      \\          ");
				System.out.println("        |                                        |         ");
				System.out.println("        |                                        |         ");
				System.out.println("        |                                         |        ");
				System.out.println("        |                                         |        ");
				System.out.println("        |                                        .'        ");
				System.out.println("        |                                        |         ");
				System.out.println("         |                                      .'         ");
				System.out.println("          \\                                     /          ");
				System.out.println("           \\                                  ,'           ");
				System.out.println("            `                                /             ");
				System.out.println("             '.                            ,'              ");
				System.out.println("               '-.                      _,'                ");
				System.out.println("                  '-._              _,-'                   ");
				System.out.println("                      '`--......---'                       ");

			break;
			case 2: //trojuhelnik
				System.out.println("  .");
				System.out.println(" / \\");
				System.out.println("/___\\");
			break;
		}
	}

	public static void main(String[] Args) {
		for(int i = 0; i <= 2; i++) {
			obrazec(i);
			System.out.println("");
		}
	}
}
