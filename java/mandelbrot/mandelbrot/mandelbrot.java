/*
 * Mandelbrot set ASCII visualisation
 * http://en.wikipedia.org/wiki/Mandelbrot_set
 * http://www.root.cz/clanky/barvy-pro-shell/
 * Copylefted by: Harvie 2oo9


                                     aa
                                     aa
                                     aa
                                     aa
                                  aaaccaaa
                                 aaai  iaaa
                            aabaab        baabaa
                             aab            baa
                            aad              daa
                             aab            baa
                            aaaaad        daaaaa
                         aaacccl            lcccaaa
                     dbaaab                      baaabd
                    aab                              baa
                    aab                              baa
                aaaaah                                haaaaa
          ba aaaak   c                                c   kaaaa ab
            aabc                                            cbaa
              aacabab                                babacaa
                    aae         ZEN OF CODING        eaa
                     aaa                            aaa
                      ae  n          cc          n  ea
                      aaa  ab aa afa    afa aa ba  aaa

 */

package mandelbrot;
import java.math.*;

public class mandelbrot {
	public String[] chars = " .`-_\':,;^=+/\\\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q".split("");
	public int max_iteration = 140;
	public float zoom = 65;

	public double x_from = -2;
	public double x_to = 2;
	public double y_from = -2.1;
	public double y_to = 2.1;

	public boolean color = false;
	public boolean background = false;


	public mandelbrot() {
		max_iteration = 140;
  	zoom = 65;

		x_from = -2;
		x_to = 2;
		y_from = -2.1;
		y_to = 2.1;

		color = false;
		background = false;
	}

	public int get_pixel_value(double x0, double y0) {
  	double x = 0;
  	double y = 0;

  	int iteration = 0;

  	while( x*x + y*y <= (2*2)  && iteration < max_iteration ) {
    	double xtemp = x*x - y*y + x0;
    	y = 2*x*y + y0;
    	x = xtemp;
    	iteration++;
  	}

  	if( iteration >= max_iteration ) {
			return 0;
		}	else {
			return iteration;
 		}

	}

	public String get_pixel_character(double x, double y) {
		float i =  ((float)get_pixel_value(x, y)/(float)max_iteration) * (chars.length-1);
		chars[0]=" ";
		return chars[Math.round(i)];
	}

	public String get_pixel_xterm_color(double x, double y) {
		int i = Math.round( ((float)get_pixel_value(x, y)/max_iteration)*14 );
		return "\033["+((int)(i%2))+";"+(30+(int)(i/2))+"m";
	}

	public String get_pixel_xterm_background(double x, double y) {
		int i = 40+Math.round( ((float)get_pixel_value(x, y)/max_iteration)*7 );
		return "\033["+i+"m";
	}

	public static void cls() {
		System.out.print("\033[2J");
	}

	public static void top() {
		System.out.print("\033[0;0H");
	}

	public void stat() {
		System.out.print("ASCII Mandelbrot Set Visualisation (Harvie 2oo9) - Iterations: "+max_iteration+", zoom: "+zoom+"     \n");
	}

	public void render() {
		//if(color || background) max_iteration = 14;

		double x, y;
		for(x=x_from; x<=x_to; x+=5/zoom) { for(y=y_from; y<=y_to; y+=2/zoom) {
			if(color) System.out.print(get_pixel_xterm_color(x, y));
			if(background) System.out.print(get_pixel_xterm_background(x, y));

			System.out.print(get_pixel_character(x, y));
			//System.out.print(get_pixel_color(x, y)+" ");
		} System.out.println("\033[0m"); }
	}

}
