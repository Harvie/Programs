import java.util.Scanner;
import java.text.NumberFormat;
//import java.lang.Math;
import java.io.*;

/** Trida BitevniPole je implementaci hry lode pro jednoho hrace
 * @author      hejnama2
 * @version     1.0
 */

class BitevniPole implements Serializable {
	/** pole znaku, ve kterem je ulozen stav bitevniho pole */
	public char[][] pole;
	public final int size; //velikost pole (x je stejne jako y)
	//private Random rnd = new Random();
	private Scanner sc = new Scanner(System.in);
	private long draws = 0, impacts = 0, total_impacts = 0; //pocet tahu, pocet zasahu, celkovy pocet okupovanych policek
	private int x,y; //pomocne promenne
	public boolean debug = false; //urcuje, jestli se budou lode zobrazovat (takovy cheat pro odladeni programu)

	public char //tady jsou ruzne znaky reprezentujici ruzne stavy jednotlivych bunek pole:
		neznamo = '_',
		lod = '#',
		voda = '~',
		zasah = 'X';

	/** vypise napovedu ke hre */
	public void printHelp() {
		System.out.print(
			"Pri zadavani souradnic dodrzujte mala a velka pismenka.\n"+
			neznamo + " - zatim neproskoumane policko\n"+
			zasah + " - policko se zasazenou lodi\n"+
			voda + " - policko se splouchnutim vody\n"+
			lod + " - lod (zobrazuje se jen pri debug = true)\n"
		);
	}

	/** konstruktor - vytvori prazdne bitevni pole o velikosti s */
	BitevniPole(int s) {
		int max_size = 59;
		if(s < 1 || s > max_size) {
			System.err.println("Can't make battlefield smaller than 1x1 or bigger than "+max_size+"x"+max_size+"!");
			System.exit(max_size);
		}
		size = s;
		pole = new char[size][size];
		for(x = 0;x < size;x++) for(y = 0;y < size;y++) pole[x][y] = neznamo;
	}

	/** druhy konstruktor - vytvori pole o vychozi s pouzitim prvniho konstruktoru velikosti (bude zavolan, pokud neni zadna velikost zadana) */
	BitevniPole() {
		this(10);
	}

	/** metoda zajistujici rozmisteni lodi - ve skutecnosti zatim rozmistuje jen nahodne ctverecky, ne cele lode*/
	public void rozmistiLode(int i) {
		for(;i>0;i--) {
			x = (int)(Math.random()*(size-1));
			y = (int)(Math.random()*(size-1));
			pole[x][y] = lod;
			total_impacts++;
		}
	}

	/** metoda, ktera vytvori string znazornujici toto bitevni pole */
	public String toString() {
		String out = new String("\n");
		out += " \tPocet tahu: "+draws+"\n";
		out += " \tPocet zasahu: "+impacts+" z "+total_impacts+"\n";
		out += " \t";
		for(y = 0;y < size;y++) out += "|"+(char)((int)'A'+y);
		out += "|\n\n";
		for(x = 0;x < size;x++) {
			out += x+"\t";
			for(y = 0;y < size;y++) {
				if(!debug && pole[x][y] == lod) {
					out += "|"+neznamo;
					continue;
				}
				out += "|"+pole[x][y];
			}
			out += "|\n";
		}
		return out;
	}

	/** zjisti, jestli uz byly zniceny vsechny lode a vrati jako boolean */
	public boolean jeKonec() {
		for(x = 0;x < size;x++) for(y = 0;y < size;y++) if(pole[x][y] == lod) return false;
		return true;
	}

	/** strili na souradnice x,y */
	public boolean strilej(int a, int b) {
		if(a >= size || b >= size || a < 0 || b < 0) {
			System.out.println("No such cell!");
			return false;
		}
		if(pole[a][b] == voda || pole[a][b] == zasah) {
			System.out.println("This cell was already impacted!");
			return false;
		}
		if(pole[a][b] == lod) {
			pole[a][b] = zasah;
			impacts++;
		}
		if(pole[a][b] == neznamo) pole[a][b] = voda;
		draws++;
		return true;
	}

	/** provede hrace dalsim tahem */
	public void dalsiTah() {
		System.out.println(this);
		System.out.print("pismenko: ");
		y = (int)(sc.next().charAt(0)-'A');
		System.out.print("cislicko: ");
		x = sc.nextInt();
		strilej(x, y);
	}

	/** provadi tahy, dokud neni pravda, ze jeKonec(), pak vypise gratulace a zkonci */
	public void hrat() {
		while(!jeKonec()) dalsiTah();
		System.out.println("\n!!! CONGRATULATIONS !!!");
		System.out.println("You have defeated "+impacts+" ships within "+draws+" draws!");
		System.out.println("!!! CONGRATULATIONS !!!\n");
	}

}

/** trida lode vytvori BytevniPole a provede vse potrebne ke spusteni hry */
public class lode {
	public static void main(String[] argv) {
		System.out.println("Lode (verze pro jednoho hrace)");

		BitevniPole bp = new BitevniPole(); //nove bitevni pole (pokud neni zadana velikost, bude pouzita vychozi velikost)
		bp.printHelp(); //vypis napovedu
		bp.debug = true; //zapne zobrazeni lodi (to je pri hre nezadouci podvod)
		bp.rozmistiLode(5); //kolik lodi chceme?

		bp.hrat(); //hrajeme

	}
}

