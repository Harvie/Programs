import java.io.*;
import java.net.*;

class PongNetwork {
	boolean startServer(int port) {
		try {
			ServerSocket ss = new ServerSocket(port);
			//while (true) {
				final Socket sock = ss.accept();
				InputStream is = sock.getInputStream();
				OutputStream os = sock.getOutputStream();

				BufferedReader br = new BufferedReader(new InputStreamReader(is));
	    
				String s = "";
				while (s != null) {
					s = br.readLine();
					if (s != null)
					System.out.println(s);
				}

				sock.close();
			//}
		} catch(Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	boolean startClient(int port) {
		try {
			System.out.println("Connecting...");
			Socket sc = new Socket();
			sc.connect(new InetSocketAddress("localhost", port), 2000);

			System.out.println("Connected!");

			BufferedReader br = new BufferedReader(new InputStreamReader(sc.getInputStream()));
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(sc.getOutputStream()));

			bw.write("GET /\n\n");  // zapíšeme předem připravený požadavek
			bw.flush();         // odeslání z bufferu

			String line = "";

			// dokud jsou data, opakuj
			while (line != null) {
				line = br.readLine();
				if (line != null) System.out.println(line);  // platná data vypisuj
			}

			sc.close(); // zavření socketu

		} catch (Exception e) {
			//e.printStackTrace();
			System.out.println("Could not connect!");
			return false;
		}
		return true;
	}
}

class socket {
	public static void main(String[] args) {
		PongNetwork pn = new PongNetwork();
		if(!pn.startClient(1033)) pn.startServer(1033);
	}
}
