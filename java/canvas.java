import javax.swing.*;
import java.awt.*;

class DrawingCanvas extends Canvas { 
  public DrawingCanvas() {  
    setSize(100, 50);
  }
  public String text = new String("LOOOL");
  public void paint(Graphics g) { 
		createBufferStrategy(2); //double buffered

		Graphics2D g2D=(Graphics2D) g; // cast to 2D
    g2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                         RenderingHints.VALUE_ANTIALIAS_ON);
    

    g.drawRect(0, 0, 99, 49); // draw border 
    g.drawString(text, 20,20); 
  } 
}

public class canvas {
	public static void main(String[] args) throws InterruptedException {
		JFrame window = new JFrame();
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setSize(640,480);
		window.setTitle("hello");
		window.setVisible(true);

		DrawingCanvas canvas = new DrawingCanvas();
		window.getContentPane().add(canvas); 

		Thread.sleep(2000);
    canvas.text="omg";
		canvas.repaint();
	}
}
