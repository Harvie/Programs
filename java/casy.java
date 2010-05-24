/* CasoStroj
 * Copylefted by Harvie 2oo9
 */

import java.util.*;
import java.lang.Math.*;
import java.text.*;

public class casy {
	public static void main(String[] Args) {
	try {
		String format = new String("yyyy-MM-dd HH:mm:ss");
		DateFormat dfm = new SimpleDateFormat(format);
		Scanner sc = new Scanner(System.in);

		System.out.println("Zadejte prosim dve data v nasledujicim formatu:\n"+format+" (rok-mesic-den hodina:minuta:vterina)");
		System.out.println("Napriklad:\n1990-03-21 00:00:00\n2009-10-09 14:06:50\n");

		Date a = dfm.parse(sc.nextLine());
		Date b = dfm.parse(sc.nextLine());

		DateFormat dfmyear = new SimpleDateFormat("yyyy");

		Date d = new Date(Math.abs(a.getTime() - b.getTime()));
		int y = (d.getYear()-70);
		int l = d.getMonth();
		long x = d.getDate()-1;
		long h = d.getHours()-1;
		long m = d.getMinutes();
		long s = d.getSeconds();

		/*
		int y = Math.abs(a.getYear()-b.getYear());
		int l = Math.abs(a.getMonth()-b.getMonth());
		long x = Math.abs(a.getDate()-b.getDate());
		long h = Math.abs(a.getHours()-b.getHours());
		long m = Math.abs(a.getMinutes()-b.getMinutes());
		long s = Math.abs(a.getSeconds()-b.getSeconds());
		*/

		System.out.println("\nMezi "+dfm.format(a)+" a "+dfm.format(b));
		System.out.println("je vzdalenost "+y+" roku, "+l+" mesicu, "+x+" dnu, "+h+" hodin, "+m+" minut a "+s+" sekund.");

		s = (d.getTime()/1000);
		//s = Math.abs(a.getTime()-b.getTime())/1000;
		m = s/60;
		h = m/60;
		x = h/24;
		System.out.println("To lze vyjadrit take jako "+s+" sekund, "+m+" minut, "+h+" hodin, "+x+" dnu, nebo "+(y*12+l)+" mesicu.");

	} catch(Exception e) { System.out.println("Something's freaked up! ;-("); }
	}
}

