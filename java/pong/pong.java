import static java.lang.Math.*;
import javax.swing.*;
import java.awt.*;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;

import java.io.*;
import java.net.*;

/** Class implementing network interface for PongPanel objects */
class PongNetwork implements Runnable {
  BufferedReader br;
  BufferedWriter bw;
	Socket sock;
  String host = "localhost";
  int port = 1033;

	PongPanel pp = null;

  /** constructor should be given instance of PongPanel to interface with and hostname to connect to */
	PongNetwork(PongPanel pp, String host) {
		this.pp = pp;
    this.host = host;
    Thread t = new Thread(this);
    t.start();
	}

  /** creates new thread which tries to connect to existing server on desired host and if it's not listening it starts own socket server. */
  public void run() {
		try {
	    if(!this.startClient(port)) this.startServer(port);

			br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			bw = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));

			if(pp.right == 0) {
				send("0.5,-5,-7");
				send("0.5,-5,-7");
			}

			String s = "";
			while (s != null) {
				s = br.readLine();
				if (s == null) continue;
				System.out.println(s);
        pp.putBall(s);
			}
			sock.close();
			System.out.println("Remote host closed connection :-(");
			System.out.println("Your score was: "+ pp.score + " against oponent's score: " + pp.score_oponent);
      System.exit(0);
		} catch(Exception e) {
			System.out.println("Something's wrong with connection :-(");
		}
  }

  /** start own server */
	boolean startServer(int port) {
		pp.right = 1;
    System.out.println("Starting server on port "+port+" ...");
		try {
			ServerSocket ss = new ServerSocket(port);
			sock = ss.accept();
      System.out.println("Client connected!");	    
		} catch(Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

  /** try to connect to existing server */
	boolean startClient(int port) {
		pp.right = 0;
		System.out.println("Connecting to server "+host+":"+port+" ...");
		try {
			sock = new Socket();
			sock.connect(new InetSocketAddress(host, port), 2000);
			System.out.println("Connected to server!");
		} catch (Exception e) {
			System.out.println("Connection failed!");
			return false;
		}
		return true;
	}

  /** once connection to another pong instance is estabilished, we can send data using this method */
  public void send(String s) {
    try {
      bw.write(s+"\n");  // zapíšeme předem připravený požadavek
      bw.flush();         // odeslání z bufferu
    } catch(Exception e) {
    }
  }

}

/** Pong Panel is object painted on main pong window (PongFrame) and contains all methods and variables needed to play pong */
class PongPanel extends JPanel implements KeyListener, MouseMotionListener { 
  PongNetwork pn;

	int right = 1; //right (1) or left (0)?

	int pad_pos = 50; //0-100;
	int pad_speed = 10;
	int pad_thickness = 10;
	int pad_size = 70;

	public int score = 0;
	public int score_oponent = 0;

  boolean have_ball = false;
	int ball_pos_x, ball_pos_y;
	int ball_mov_x, ball_mov_y;
	int ball_size = 10;

  /** new round is called when your opponent scored */
	public void newRound() {
    if(!have_ball) return;
		ball_pos_x = getWidth()/2;
		ball_pos_y = getHeight()/2;
		ball_mov_x = 7*(right==0?-1:1);
		ball_mov_y = 3;
	}

  /** Constructor */
  public PongPanel(int x, int y, String host) { 
    setSize(x, y);
		//newRound();
		addKeyListener(this);
		addMouseMotionListener(this);
    pn = new PongNetwork(this, host);
  }

  /** Repaint game window */
  public void paint(Graphics g) { 
		//createBufferStrategy(2); //double buffered
		Graphics2D g2D=(Graphics2D) g; // cast to 2D
		g2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
			RenderingHints.VALUE_ANTIALIAS_ON);

		g.setColor(Color.WHITE);
		g.setColor(Color.BLACK);
		g.fillRect(0,0, getWidth(), getHeight());
		g.setColor(Color.GREEN);

    if(have_ball) g.fillOval(ball_pos_x-(ball_size/2), ball_pos_y-(ball_size/2), ball_size, ball_size); // draw ball
    
		g.fillRect(right*(getWidth()-pad_thickness), pad_pos, pad_thickness, pad_size); // draw pad
    g.drawString("PONG! "+score+":"+score_oponent, 20,20);
  } 

  /** increment score of your oponent */
	public void oponentScores() {
		score_oponent++;
    pn.send("+gotme :-)"); //HEY! do not comment this you cheater! :-P
		newRound();
	}

  /** compute next iteration of game and repaint screen */
	public boolean tick() {
    if(have_ball) {
      ball_pos_x += ball_mov_x;
      ball_pos_y += ball_mov_y;
      if(ball_pos_y >= getHeight() || ball_pos_y <= 0) ball_mov_y = -ball_mov_y; //bounce

      if(
          (
            (right == 1 && ball_pos_x >= (getWidth()-pad_thickness-round(ball_size/2))) ||
            (right == 0 && ball_pos_x <= pad_thickness+round(ball_size/2))
          ) &&
          (
            ball_pos_y >= pad_pos &&
            ball_pos_y <= pad_pos + pad_size
          )
      ) {
        ball_mov_x = -ball_mov_x;
      }

      else if(
        (right == 1 && ball_pos_x >= getWidth()) ||
        (right == 0 && ball_pos_x <= 0)
      ) oponentScores();

      else if(
        (right == 0 && ball_pos_x >= getWidth()) ||
        (right == 1 && ball_pos_x <= 0)
      ) {
        //send ball over network
        pn.send(getBall());
        have_ball = false; super.repaint();
        //newRound();
        return false;
      }

      //putBall(getBall()); //haloooz s roundem
    }

		super.repaint();
		return true;
	}

  /** serialize ball (position and speed) to be sent over TCP/IP socket */
	public String getBall() {
    have_ball = false;
    return(
      ((float)ball_pos_y / (float)getHeight()) +
      ","+ball_mov_x +
      ","+ball_mov_y +
      ","+right

		);

	}

  /** deserialize ball received through TCP/IP connection */
	public void putBall(String ball) {
    if(ball.charAt(0) == '+') {
      score++;
      return;
    }
    have_ball = true;
    String[] vars = ball.trim().split(",");
    System.out.println("Parsing: "+vars[0]+" "+vars[1]+" "+vars[2]);
    ball_pos_x = (right==1)?pad_thickness+ball_size:getWidth()-(pad_thickness+ball_size);
    ball_pos_y = (int)round(getHeight()*(new Double(vars[0])));
    ball_mov_x = new Integer(vars[1]);
    ball_mov_y = new Integer(vars[2]);
    System.out.println("Parsed: "+ball_pos_y+" "+ball_mov_x+" "+ball_mov_y);
	}

	public void keyPressed(KeyEvent evt) {
		int keyCode = evt.getKeyCode();
		if(keyCode == KeyEvent.VK_UP && pad_pos >= pad_speed) pad_pos -= pad_speed;
		if(keyCode == KeyEvent.VK_DOWN && pad_pos <= (100-pad_speed)) pad_pos += pad_speed;
		super.repaint();
  }

  public void keyReleased(KeyEvent evt) {
  }

  public void keyTyped(KeyEvent evt) {
  }
  public boolean isFocusTraversable() { return true; }

	public void mouseMoved(MouseEvent e) {
		pad_pos = e.getY()-(pad_size/2);
	}
	public void mouseDragged(MouseEvent e) {
	}

}

/** Main window of game */
class PongFrame extends JFrame {
	PongPanel pp;
	PongFrame(int x, int y, String host) throws InterruptedException {
		this.setSize(x,y);
		this.setTitle("NetPong");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		pp=new PongPanel(getWidth(),getHeight(),host);
		this.getContentPane().add(pp); 

		this.setVisible(true);
	}

  /** start game */
	public void play() throws InterruptedException {
		while(true) {
			pp.tick();
			//if(!pp.tick()) Thread.sleep(2*1000);
			Thread.sleep(1000/24);
		}
	}

}

/**
 * Implementation of classical "PONG" game modified to pass ball over TCP/IP connection.
 * You need to run two instances (client + server).
 * Game takes one optional commandline argument specifiing remote host running the server (localhost is default).
 * If there's no server running on desired hostname, server is started on local machine on port 1033.
 * Copylefted by: Harvie 2o1o
 * @author      Thomas Harvie Mudrunka (mudruto1)
 * @version     1.0
 */

public class pong {
	public static void main(String[] args) throws InterruptedException {
		PongFrame pf = new PongFrame(640,480,args.length>0?args[0]:"127.0.0.1");
		pf.play();
	}
}
