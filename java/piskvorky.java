import java.io.*;
import java.util.*;
import java.text.*;

/**
 * trida reprezentujici pole na hrani piskvorek
 * @author Honza
 */
class PiskvorciPole {
	public int size;
	public char pole[][];
  public char prazdno='_';
	private int x, y;
  public boolean kriz = true;

	PiskvorciPole(int s) { //konstruktor
		size = s;
    pole = new char[size][size];
    for(x=0;x<size;x++) {
    	for(y=0;y<size;y++) {
      	pole[x][y]=prazdno;
      }
    }
	}

	PiskvorciPole() { //pretizeni konstruktoru (pouzije se, pokud neni zadana velikost)
		this(10);
	}

	public String toString() {
    char ascii = 'a';

		String out = new String("\n  \t|");
    for(char c=ascii;c<size+ascii;c++) out += (c+"|");
    out += "\n\n";
    for(x=0;x<size;x++) {
    	out += (x+":\t|");
    	for(y=0;y<size;y++) {
      	out += pole[x][y]+"|";
      }
      out += "\n";
    }

		return out;
	}

	/*
  public void vykresli() { // udelat to metodou toString jinak je to chyba....
		System.out.print(this);
  }
	*/

	public void vykresliDoSouboru(String outputfile) {
		double x, y;
		FileOutputStream out;
		PrintStream p;


		try {
			out = new FileOutputStream(outputfile, true); //true znamena, ze se bude vzdy pridavat na konec souboru
			p = new PrintStream(out);
			p.println(new SimpleDateFormat("dd-MM-yyyy HH:mm:ss").format(new Date()));
			p.println(this);
			p.close();
		} catch(Exception e) {
			System.err.println("Error writing to file");
		}
	}


    public void hrat() {
        char hrac;
				boolean chyba = false;
        Scanner sc = new Scanner(System.in);
        while(true) {
					System.out.println(this);
					vykresliDoSouboru("piskvorky.txt");
            kriz = !kriz;
            hrac = 'O'; if(kriz) hrac = 'X';
            System.out.println("Hraje hrac s "+hrac);

						System.out.print("pismeno: ");
            y=(int)(sc.next().charAt(0)-'a');

            try {
								System.out.print("cisilko: ");
                x=sc.nextInt();
            } catch(Exception e) {
							chyba = true;
            }

            if(chyba || x < 0 || x >= size || y < 0 || y >= size || pole[x][y] != prazdno) {
								chyba = false;
                System.out.println("\t\t\tneplatne pole!");
                kriz = !kriz; continue; //dalsi pokus
            }
            pole[x][y]=hrac;
        }

    }
}

/**
 * trida demonstrujici pouziti tridy PiskvorciPole
 * @author Honza
 */
public class piskvorky {
  public static void main (String argv[]) {
		System.out.println("\t\t===> PISKVORKY <===");
		System.out.println("\tHru lze prerusit stisknutim ctrl+c, nebo pres netbeans.");
		PiskvorciPole pp = new PiskvorciPole(20); //vytvorime nove pole 20x20
		pp.hrat(); //spustime hru
	}
}
