import java.util.Scanner;

class seznam {
	public int hodnota = 0;
	public seznam dalsi = null;
}

public class linkedlist {
  public static void main (String argv[]) {
		Scanner sc = new Scanner(System.in);

		System.out.print("pocet: ");
		int pocet = sc.nextInt();

		System.out.println("cisla: ");
		//vytvoreni spojoveho seznamu a pridani dalsich prvku
		seznam s = new seznam();
		seznam prvni = s;
		for(int i = 0;i<pocet;i++) {
			s.hodnota = sc.nextInt();
			s = (s.dalsi = new seznam());
		}

		//cyklus na prochazeni spojoveho seznamu
		for(s=prvni; s.dalsi != null; s=s.dalsi) {
			System.out.println(s.hodnota);
		}
	}
}
