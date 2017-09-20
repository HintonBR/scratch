/*
 * Convolution1D.java
 *
 * Created on February 20, 2001, 2:00 PM
 */
 
import javax.swing.*;
import java.io.*;
import java.util.Date;
import jigl.image.*;
import jigl.image.utils.*;
import jigl.image.io.*;
import jigl.image.ops.*;
/** 
 *
 * @author  bhinton
 * @version 
 */
public class Convolution2D extends javax.swing.JFrame {

  RealGrayImage input, impulse;
  RealGrayImage output;
  String textOutput[];
  float kern[][] = {{1,1,1},{1,8,1},{1,1,1}};
  float dataArray[][] = {{0,1,2,3,5},{1,2,3,5,7},{2,3,5,7,8},{3,5,7,8,9},{5,7,8,9,10}};
  /** Creates new form Convolution1D */
  public Convolution2D() {
    textOutput = new String[2];
  }
     
  
  /** This method is called from within the constructor to
   * initialize the form.
   * WARNING: Do NOT modify this code. The content of this method is
   * always regenerated by the FormEditor.
   */
  private void initComponents() {//GEN-BEGIN:initComponents
      jPanel2 = new javax.swing.JPanel();
      getContentPane().setLayout(new java.awt.GridBagLayout());
      java.awt.GridBagConstraints gridBagConstraints1;
      addWindowListener(new java.awt.event.WindowAdapter() {
          public void windowClosing(java.awt.event.WindowEvent evt) {
              exitForm(evt);
          }
      }
      );
      
      jPanel2.setLayout(new java.awt.GridLayout(1, 1));
      
      gridBagConstraints1 = new java.awt.GridBagConstraints();
      gridBagConstraints1.gridx = 2;
      gridBagConstraints1.gridy = 1;
      gridBagConstraints1.fill = java.awt.GridBagConstraints.BOTH;
      gridBagConstraints1.anchor = java.awt.GridBagConstraints.NORTHWEST;
      gridBagConstraints1.weightx = 1.0;
      gridBagConstraints1.weighty = 1.0;
      getContentPane().add(jPanel2, gridBagConstraints1);
      
      pack();
      java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
      java.awt.Dimension dialogSize = getSize();
      setSize(new java.awt.Dimension(700, 700));
      setLocation((screenSize.width-700)/2,(screenSize.height-700)/2);
  }//GEN-END:initComponents
private RealGrayImage readFromFile(String filename) {
      RealGrayImage in;
      Image im;
      ImageConverter ic;
      try {
        ImageInputStream is = new ImageInputStream(filename);
        im = is.read();
        ic = new ImageConverter();
        in = ic.toRealGray(im);
        is.close();
        return in;
      }
      catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());  
        return null;
      }
  }
private void writeToFile(RealGrayImage image, String filename) {
      try {
        ImageOutputStream ios = new ImageOutputStream(filename);
        ios.write(new ImageConverter().toGray(image));
        ios.close();
      }
      catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
      }
      
  }
  private RealGrayImage performConvolution(RealGrayImage in,RealGrayImage im, String data) {
     RealGrayImage out = null;
    Convolve c = null;
    Date start;
    Date stop;
    try {
        
        if (data.equals("X")) {  
             c = new Convolve(new ImageKernel(ImageKernel.SOBEL_X));
             out = new RealGrayImage(in.X() + 3 -1, in.Y() + 3 -1);
        }
        else if (data.equals("Y")) {
            c = new Convolve(new ImageKernel(ImageKernel.SOBEL_Y));
            out = new RealGrayImage(in.X() + 3 -1, in.Y() + 3 -1);
        }
        else {
            ImageKernel ik = new ImageKernel(im);
            c = new Convolve(ik);
            out = new RealGrayImage(in.X() + im.X() -1, in.Y() + im.Y() -1);
        }
        System.out.println("Starting Convolution");
        start = new java.util.Date();
        out = new ImageConverter().toRealGray(c.apply(in));
        out.byteSize();
         stop  = new java.util.Date();
         System.out.println("Stopping Convolution");
         textOutput[0] = String.valueOf(stop.toString());
         textOutput[1] = String.valueOf(start.toString());
         System.out.println("Time Stop was " + textOutput[0]);
         System.out.println("Time Start was " + textOutput[1]);
         
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
    
    return out;
  }

  private void writeToTextFile(String filename, String[] text) {
      try {
        RandomAccessFile rw = new RandomAccessFile(filename, "rw");
        for (int i=0; i < text.length; i++) {
            if (text[i] != null)
            rw.writeChars(text[i] + "\r\n");
        }
        rw.close();
      }
      catch (Exception e) {
          JOptionPane.showMessageDialog(null, e.getMessage());
      }
  }
  /** Exit the Application */
  private void exitForm(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_exitForm
    System.exit (0);
  }//GEN-LAST:event_exitForm

  /**
  * @param args the command line arguments
  */
  public static void main (String args[]) {
    Convolution2D c = new Convolution2D ();
    c.input = c.readFromFile("c:\\bryan\\programs\\java\\convolution\\" + args[0] + ".pgm");
    if (args[1].equals("X") == false && args[1].equals("Y") == false)
    c.impulse = c.readFromFile("c:\\bryan\\programs\\java\\convolution\\" + args[1] + ".pgm");
    c.output = c.performConvolution(c.input, c.impulse, args[1]);
    c.writeToFile(c.output, "c:\\bryan\\programs\\java\\convolution\\output" + args[2] + ".pgm");
    c.writeToTextFile("c:\\bryan\\programs\\java\\convolution\\output" + args[2] + ".txt", c.textOutput);
    System.exit(0);
  }
  
  // Variables declaration - do not modify//GEN-BEGIN:variables
  private javax.swing.JPanel jPanel2;
  // End of variables declaration//GEN-END:variables

}