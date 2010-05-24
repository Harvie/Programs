package Kalkulator;

import javax.swing.*;

/**
 * This example shows how to create GUI application which can be run both
 * as HTML embeded web applet and as standalone application
 * @author harvie
 */

class AppGui extends JPanel {
    AppGui() {
        setLayout(new java.awt.GridLayout(0, 2));

        JLabel lblVklad = new JLabel("vklad");
        final JTextField txtVklad = new JTextField();

        JLabel lblMira = new JLabel("urok. mira");
        final JTextField txtMira = new JTextField();

        JLabel lblRoky = new JLabel("pocet let");
        final JTextField txtRoky = new JTextField();

        JButton btnGo = new JButton("Go");
        final JLabel lblCelkem = new JLabel("zadej data");

        add(lblVklad);
        add(txtVklad);
        add(lblMira);
        add(txtMira);
        add(lblRoky);
        add(txtRoky);
        add(btnGo);
        add(lblCelkem);

        btnGo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                double v = new Double(txtVklad.getText());
                double u = new Double(txtMira.getText());
                double r = new Double(txtRoky.getText());
                for(;r>0;r--) v += v*(u/100);
                lblCelkem.setText(v+"");
            }
        });
    }
}

public class Kalkulator extends JApplet {

    public void init() {
        getContentPane().add(new AppGui());
    }

    public static void main(String[] args) {
        JFrame f = new JFrame("Applet window");
        f.setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        f.setSize(640,480);
        f.getContentPane().add(new AppGui());
        f.setVisible(true);
    }

    // TODO overwrite start(), stop() and destroy() methods
}
