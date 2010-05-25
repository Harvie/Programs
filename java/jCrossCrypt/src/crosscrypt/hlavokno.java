import java.io.*;

/*
 * hlavokno.java
 *
 * Created on 24. září 2007, 0:37
 */

/**
 *
 * @author  harvie
 */
public class hlavokno extends javax.swing.JFrame {
    
    /** Creates new form hlavokno */
    public hlavokno () {
        initComponents ();
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">//GEN-BEGIN:initComponents
    private void initComponents() {
        jDialog1 = new javax.swing.JDialog();
        jFileChooser1 = new javax.swing.JFileChooser();
        jButton1 = new javax.swing.JButton();
        jTextField1 = new javax.swing.JTextField();
        jButton2 = new javax.swing.JButton();
        jTextField2 = new javax.swing.JTextField();
        jButton3 = new javax.swing.JButton();
        jTextField3 = new javax.swing.JTextField();
        jButton4 = new javax.swing.JButton();
        jTextField4 = new javax.swing.JTextField();
        jButton5 = new javax.swing.JButton();

        jFileChooser1.setCurrentDirectory(null);

        org.jdesktop.layout.GroupLayout jDialog1Layout = new org.jdesktop.layout.GroupLayout(jDialog1.getContentPane());
        jDialog1.getContentPane().setLayout(jDialog1Layout);
        jDialog1Layout.setHorizontalGroup(
            jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jFileChooser1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
        );
        jDialog1Layout.setVerticalGroup(
            jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jFileChooser1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
        );

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("One-time pad (XOR) symetric encryption [<-Harvie 2oo7]");
        jButton1.setText("Input");
        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton1MouseClicked(evt);
            }
        });

        jButton2.setText("Output");
        jButton2.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton2MouseClicked(evt);
            }
        });

        jButton3.setText("Key");
        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton3MouseClicked(evt);
            }
        });

        jButton4.setText("Start");
        jButton4.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                crypt(evt);
            }
        });

        jTextField4.setEditable(false);
        jTextField4.setText("Waiting...");

        jButton5.setText(".ccr");
        jButton5.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton5MouseClicked(evt);
            }
        });

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jButton4, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 467, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                            .add(jButton3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 78, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, jButton2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, jButton1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 78, Short.MAX_VALUE))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                            .add(jTextField3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 383, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, jTextField1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 383, Short.MAX_VALUE)
                            .add(layout.createSequentialGroup()
                                .add(jTextField2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 323, Short.MAX_VALUE)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(jButton5))))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jTextField4, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 467, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jButton1)
                    .add(jTextField1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 24, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                            .add(jTextField2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 25, Short.MAX_VALUE)
                            .add(jButton5))
                        .add(6, 6, 6))
                    .add(layout.createSequentialGroup()
                        .add(jButton2)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                    .add(jTextField3)
                    .add(jButton3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jButton4, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 26, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jTextField4, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void crypt (java.awt.event.MouseEvent evt) {//GEN-FIRST:event_crypt
// TODO crypt code here:
       
        jTextField4.setText("Working...");
        //jProgressBar1.setValue(0);
        
        if(jTextField1.getText().equals(jTextField2.getText())) { jTextField4.setText("Output to Input - Can't do this!"); return; }
        
        try { 
            if( !(new File(jTextField1.getText()).isFile()) ) { jTextField4.setText("Input file not found!"); return; }
            if( !(new File(jTextField3.getText()).isFile()) ) { jTextField4.setText("Key file not found!"); return; }
            if( new File(jTextField2.getText()).isFile() ) { jTextField4.setText("Working (Rewriting output file)..."); }
            long size = (new File(jTextField1.getText()).length());
            long keysize = (new File(jTextField3.getText()).length());
            long pos = 0;
            
            BufferedInputStream in = new BufferedInputStream(new FileInputStream(jTextField1.getText()));
            BufferedInputStream key = new BufferedInputStream(new FileInputStream(jTextField3.getText()));
            BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(jTextField2.getText()));
            
            //Alghorythm
            int inb, keyb, progress;
            while((inb = in.read()) != -1) {
                while((keyb = key.read()) == -1 /*|| keyb == 0x0A*/ ) {
                  key.close();
                  key = new BufferedInputStream(new FileInputStream(jTextField3.getText()));
                }
                out.write(inb^keyb);
                //pos++;
                //jProgressBar1.setValue((int)((pos/size)*100));
            }
            
            out.close();
            in.close();
            key.close();
            
            jTextField4.setText("Waiting... ["+size+" b (en|de)crypted using "+keysize+" b key]");
        } catch(Exception e) { jTextField4.setText("I/O Error!"); }
    }//GEN-LAST:event_crypt

    private void jButton5MouseClicked (java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton5MouseClicked
// TODO add your handling code here:
        jTextField2.setText(jTextField2.getText()+".ccr");
    }//GEN-LAST:event_jButton5MouseClicked

    private void jButton3MouseClicked (java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MouseClicked
// TODO add your handling code here:
        int returnVal = jFileChooser1.showOpenDialog(jDialog1);
        if(jFileChooser1.getSelectedFile() != null) {
           jTextField3.setText(jFileChooser1.getCurrentDirectory()+"/"+jFileChooser1.getSelectedFile().getName());
        }
    }//GEN-LAST:event_jButton3MouseClicked

    private void jButton2MouseClicked (java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton2MouseClicked
// TODO add your handling code here:
        int returnVal = jFileChooser1.showOpenDialog(jDialog1);
        if(jFileChooser1.getSelectedFile() != null) {
           jTextField2.setText(jFileChooser1.getCurrentDirectory()+"/"+jFileChooser1.getSelectedFile().getName());
        }
    }//GEN-LAST:event_jButton2MouseClicked

    private void jButton1MouseClicked (java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseClicked
// TODO add your handling code here:
        int returnVal = jFileChooser1.showOpenDialog(jDialog1);
        if(jFileChooser1.getSelectedFile() != null) {
           jTextField1.setText(jFileChooser1.getCurrentDirectory()+"/"+jFileChooser1.getSelectedFile().getName());
           jTextField2.setText(jFileChooser1.getCurrentDirectory()+"/"+jFileChooser1.getSelectedFile().getName());
        }
    }//GEN-LAST:event_jButton1MouseClicked
    
    /**
     * @param args the command line arguments
     */
    public static void main (String args[]) {
        java.awt.EventQueue.invokeLater (new Runnable () {
            public void run () {
                new hlavokno ().setVisible (true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JDialog jDialog1;
    private javax.swing.JFileChooser jFileChooser1;
    private javax.swing.JTextField jTextField1;
    private javax.swing.JTextField jTextField2;
    private javax.swing.JTextField jTextField3;
    private javax.swing.JTextField jTextField4;
    // End of variables declaration//GEN-END:variables
    
}
