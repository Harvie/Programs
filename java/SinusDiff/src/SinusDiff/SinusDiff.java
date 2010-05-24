package SinusDiff;

import static java.lang.Math.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import java.awt.*;

/**
 * This example shows how to create GUI application which can be run both
 * as HTML embeded web applet and as standalone application
 * @author harvie
 */
class AppGui extends JPanel implements Runnable {

    double  offA=0,offB=0,
            speedA=50,speedB=-55,
            freqA=25,freqB=35;

    @Override
    public void paint(Graphics g) {
        /*
        Graphics2D g2D = (Graphics2D) g; // cast to 2D
        g2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
            RenderingHints.VALUE_ANTIALIAS_ON);
        */
        
        g.setColor(Color.BLACK);
        g.fillRect(0, 0, getWidth(), getHeight());
        g.setColor(Color.GREEN);
        //g.setXORMode(Color.GREEN);

        int mid = round(getHeight()/2);
        int y1,y2;
        for(int x = 0;x<=getWidth();x++) {
            y1=(int)round(mid+mid*sin((double)(x+offA)/freqA));
            y2=(int)round(mid+mid*sin((double)(x+offB)/freqB));
            g.drawLine(x,y1,x,y2);
        }
    }

    int fps = 24;

    synchronized public void run() {
      System.out.println("FPS::: "+fps);
      while(true) {
        try {
            Thread.sleep(round(1000 / fps));
        } catch (InterruptedException ex) {
            Logger.getLogger(AppGui.class.getName()).log(Level.SEVERE, null, ex);
        }
        offA += round(speedA/fps);
        offB += round(speedB/fps);
        this.repaint();
      }
    }

    Thread t = null;

    AppGui(int fps) {
        this.fps = fps;
        this.setDoubleBuffered(true);

        t = new Thread(this);
        t.start();
    }

    AppGui() {
        this(20);
    }

    AppGui(String fps) {
        this(new Integer(fps)!=null?(int)new Integer(fps):20);
        System.out.println("Setting FPS from string: "+fps);
    }

    synchronized public void start() throws InterruptedException {
        notifyAll();
    }

    public void stop() throws InterruptedException {
        t.wait();
    }

    public void destroy() {
        this.setVisible(false);
        t.interrupt();
    }
    
}

public class SinusDiff extends JApplet {

    static AppGui appGui = null;

    @Override
    public void init() {
        appGui = new AppGui(getParameter("fps"));
        getContentPane().add(appGui);
    }

    public static void main(String[] args) {
        JFrame f = new JFrame("Applet window");
        f.setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        f.setSize(640, 480);
        appGui = new AppGui(args.length > 0?args[0]:"20");
        f.getContentPane().add(appGui);
        f.setVisible(true);
    }
    
    // TODO overwrite start(), stop() and destroy() methods
    @Override
    public void start() {
        try {
            appGui.start();
        } catch (InterruptedException ex) {
            Logger.getLogger(SinusDiff.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void stop() {
        try {
            appGui.stop();
        } catch (InterruptedException ex) {
            Logger.getLogger(SinusDiff.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void destroy() {
        appGui.destroy();
    }
}
