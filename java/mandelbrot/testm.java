/*
 * knihovna mandelbrot umi vykreslit do terminalu mandelbrotovu mnozinu
 * mnoha ruznymy zpusoby - vcetne barev.
 * tento program demonstruje jeji pouziti
 * cely sem to zbastlil za jeden vecer podle wikipedie,
 * takze se neda cekat zadne zazracne formatovani, ani komentare
 * ucelem je prokazat znalost javy a OP.
 *
 * Copylefted by Tomas "Harvie" Mudrunka 2oo9
 */


import mandelbrot.*;

public class testm {
	public static void main(String[] argv) {
		//String[] chars = " abc".split("");
		//System.out.println(chars.length+" "+chars[5]+"---");

		//basic:
		mandelbrot m = new mandelbrot();
		m.cls();
		m.render();

		//advanced:
		m.max_iteration=28;
		m.color=true;
		m.stat();
		m.render();

		m.zoom=20;
		m.color=false;
		m.max_iteration=350;
		m.stat();
		m.render();

		m.zoom=65;
		m.max_iteration=14;
		m.chars = " ".split("");
		m.color=false;
		m.background=true;
		m.stat();
		m.render();

		m = new mandelbrot(); //reset to defaults (garbage collector will get rid of the old instance)

		m.zoom=1000;
		m.max_iteration=14;
		m.background=true;
		m.color=true;
		m.x_from=-0.6;
		m.x_to=-0.3;
		m.y_from=-0.7;
		m.y_to=-0.5;
		m.stat();
		m.render();
	}
}
