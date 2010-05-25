//////////////////////////////////////////////////////////
//							//
//	Chater 1.4					//
//	Thomas Harvie Mudrunka (C) 2006			//
//							//
//	> Czech Republic				//
//	> ICQ 283-782-978				//
//							//
//////////////////////////////////////////////////////////


// Importy
import java.lang.*;
import java.io.*;
import java.net.*;
import javax.swing.JOptionPane;


//Veřejná třída
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

public class Chater {
	
//Metoda main
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	public static void main(String[] args) throws java.io.IOException {
	System.out.println("Chater 1.4\nby:  Thomas Mudrunka (C) 2006 - Czech Rep.\nICQ: 283782978\n\nProgram Started!\nInitializing...\n");
	int port = 12345;
	String line, pass, goodpass, popup;
	popup = "false";
	goodpass = "false";
	//InetAddress add = getLocalhost(); //Adresa localhosta
	Runtime runtime = Runtime.getRuntime();
	if(args[0].equals("help")){
		System.out.println("You need help ha?!");
		System.exit(0);
			}
		
	try {port = Integer.parseInt(args[0]);} catch (java.lang.ArrayIndexOutOfBoundsException ex) {System.out.println("- ERROR, YOU MUST SPECIFY PORT !!!\nSyntax: java Chater {port, popup, pass}\njava Chater help - for detail help"); System.exit(1);}
	if(args.length > 1){popup = args[1];}
	
	ServerSocket ss = new ServerSocket(port); //Vytvořit serverový soket
	System.out.println("Server started on: localhost:" + port + "\nPopups enabled: " + popup + "\nNow waiting for incoming connection!");
	
//Přijetí příchozího spojení
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	while(true){
	Socket sckt = ss.accept();
		System.out.println("Connected!");
	BufferedReader br = new BufferedReader (new InputStreamReader(sckt.getInputStream())); //BufferedReader vytvoříme z InputStreamReaderu vytvořeného z InputStreamu vráceného metodou soketu vráceného přijmutím spojení serverovým soketem.
	

/*
//Ověření hesla
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	line = br.readLine(); //Přečíst další řádek z telnetu	
		
	System.out.println("Waiting for password.");
		
	if(args.length > 2){
		
		line = "ahoj";//debug
		if(args[2].equals(line)) {goodpass = "true";}
		line = br.readLine(); //Přečíst další řádek z telnetu
		line = "ahoj";//debug
		
		while((line != null) || (goodpass.equals("false"))){
			if(args[2].equals(line)){goodpass = "true"; System.out.println("GOOD"}
			System.out.println("Invalid Password! Valid=" + args[2]);
			//if(args[2] == line){System.out.println("Good Password!");}
			line = br.readLine();
			line = "ahoj";//debug
				}
					}
*/
//Zpracování příkazů					
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

			System.out.println("Incoming data:");
			line = br.readLine(); //Přečíst další řádek z telnetu
			
		while(line != null){
			if(line.equals("|exit") || line.equals("|exit1") || line.equals("|cmd") || line.equals("|popup") || line.equals("|plain")){
				System.out.println("! Incoming command:");
				if(line.equals("|popup")) 
					{
					popup = "true";
					System.out.println("- Enabling popups!");
						}
				if(line.equals("|plain")) 
					{
					popup = "false";
					System.out.println("- Disabling popups!");
						}
				if(line.equals("|exit")) 
					{
					System.out.println("- Remote daemon shutdown request...\n- Exit code: 0");
					System.exit(0);
						}
				if(line.equals("|exit1")) 
					{
					System.out.println("- Remote daemon shutdown request (because of error)...\n- Exit code: 1");
					System.exit(1);
						}
				if(line.equals("|cmd")) 
					{
					try {
						System.out.println("- Waiting for remote command line...");
						line = br.readLine();
						System.out.println("-	Executing...");
						runtime.exec(line);
						System.out.println("-	Done!");
						} catch (Exception ex) {}
							}
				
				}
			
//Tisk zpráv do konzole a otevírání popapek				
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
				
				else {
				System.out.println(line);
				if(popup.equals("true")){
					JOptionPane.showMessageDialog(null, line);
				}
				}
				line = br.readLine();//Přečíst další řádek z telnetu
				
		}
	System.out.print("Connection closed by client!\nNow waiting for incoming connection!");
			}
		}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	public static void slow() { //Metoda pro zahlcení CPU.
		int a, b, c;
		a=1; b=1; c=0;
				
		while(true) {
			if ((a < 2147438648) && (b < 2147438648) && (c < 2147438648)) {
				c = a + b; b = c - a;
				}
			else {
				a=1; b=1; c=0;
				}
			}
		}

//Konec třídy		
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	}