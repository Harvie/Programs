import java.io.*;
import java.util.*;

public class textstats {

	public static void main(String[] Args) {
		File file = new File("lipsum.txt");

		try {

			FileInputStream fis = new FileInputStream(file);
			BufferedInputStream bis = new BufferedInputStream(fis);
			//DataInputStream dis = new DataInputStream(bis);
			Scanner sc = new Scanner(bis);

			while (sc.hasNext()) {
        //System.out.println(dis.readLine());
        System.out.println(sc.next());
      }

			fis.close();
      bis.close();
      //dis.close();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
