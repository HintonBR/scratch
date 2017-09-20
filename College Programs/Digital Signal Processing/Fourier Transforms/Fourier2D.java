/*
 * Convolution1D.java
 *
 * Created on February 20, 2001, 2:00 PM
 */
 
import jigl.image.*;
import jigl.image.io.*;
import jigl.gui.ImageCanvas;
import javax.swing.*;
import java.io.*;
import jigl.image.ops.levelOps.*;
import jigl.image.ops.*;
import jigl.image.utils.*;
import java.lang.Math;
import jigl.math.Complex;
import jigl.image.warp.*;

/** 
 *
 * @author  bhinton
 * @version 
 */
public class Fourier2D extends javax.swing.JFrame {

    private RealGrayImage imagex, imagey, imageadd, imagerotate, imagemult;
   /** Creates new form Convolution1D */
  public Fourier2D() {
    initComponents ();
  }
     
  private Image readFromFile(String filename) {
      Image input;
      try {
        ImageInputStream is = new ImageInputStream(filename);
        input = is.read();
        is.close();
        return input;
      }
      catch (Exception e) {
          JOptionPane.showMessageDialog(null, e.getMessage());
        return null;
      }
  }
  private Image readFromFile() {
      Image input;
      String filename = ImageFileBrowse();
      try {
        ImageInputStream is = new ImageInputStream(filename);
        input = is.read();
        is.close();
        return input;
      }
      catch (Exception e) {
          JOptionPane.showMessageDialog(null, e.getMessage());
        return null;
      }
  }
   private void writeToFile(RealGrayImage mySignal, String filename) {
      try {
        ImageOutputStream ios = new ImageOutputStream(filename);
        mySignal.byteSize();
        ios.write(ImageConverter.toGray(mySignal));
        ios.close();
      }
      catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
      }
      
  }
  
  
    private RealGrayImage generateSine(int freq, int s, char o) {
        RealGrayImage val = new RealGrayImage(s, s);
        double calc = 0;
        for (int i=0; i<s; i++) {
            calc =  Math.sin(2 * Math.PI * freq * (((double)i)/((double)s)));
            if (o == 'x') {
                for (int j=0; j<s; j++) {
                    val.set(i, j, (float)calc);
                }
            }
            else {
                for (int j=0; j<s; j++) {
                    val.set(j, i ,(float)calc);
                }
            }
        }
        return val;
    }
    
    private RealGrayImage generateCosine(int freq, int s, char o) {
        RealGrayImage val = new RealGrayImage(s, s);
        double calc = 0;
        for (int i=0; i<s; i++) {
            calc =  Math.cos(2 * Math.PI * freq * (((double)i)/((double)s)));
            if (o == 'x') {
                for (int j=0; j<s; j++) {
                    val.set(i, j, (float)calc);
                }
            }
            else {
                for (int j=0; j<s; j++) {
                    val.set(j, i ,(float)calc);
                }
            }
        }
        return val;
    }
    //Perform a rotation on an image by degree degrees
  private RealGrayImage rotateImage(RealGrayImage testImage, int degree) {
    int i, j, centerx, centery;
    double tempY, tempX;
    RealGrayImage result = new RealGrayImage(testImage.X(), testImage.Y());
    centerx = testImage.X() /2;
    centery = testImage.Y() /2;

    //Loop through the new image and perform backwards mapping
    for (i=0;i<testImage.X(); i++) {
        for (j=0;j<testImage.Y(); j++) {
            //Equations that does the backward mapping algorithm
            tempY = (j-centery) * Math.cos(-(degree * 2 * Math.PI)/360) - (i-centerx) * Math.sin(-(degree * 2 * Math.PI)/360) + centery;
            tempX = (j-centery) * Math.sin(-(degree * 2 * Math.PI)/360) + (i-centerx) * Math.cos(-(degree * 2 * Math.PI)/360) + centerx;
            
            //Translates the image to the origin (Cartesain origin - lower lefthand corner)
            //tempY = j - centery;
            //tempX = i + centerx;
            
            //Functions that find x and y values so that we can do forward mapping algorithm
            //tempY = (i-centerx) * Math.sin(-(degree * 2 * Math.PI)/360) + (j-centery) * Math.cos(-(degree * 2 * Math.PI)/360) + centerx;
            //tempX = (i-centerx) * Math.cos(-(degree * 2 * Math.PI)/360) - (j-centery) * Math.sin(-(degree * 2 * Math.PI)/360) + centery;
            
            //Do range checking and if valid then set the value in the result image after interpolating the values from the old image
            if (tempY <testImage.Y()-1 && tempX < testImage.X()-1 && tempY >= 0 && tempX >= 0) result.set(i, j, interpolate(testImage, tempX, tempY));
        }
    }
    return result;
  }
  
  //Function that takes an image given a set of coordinates and interpolates an integer value
  private int interpolate(RealGrayImage testImage, double x, double y) {
    double val;
    int zerox, zeroy, onex, oney;
    double diffx, diffy;
    zerox = (int) x;
    zeroy = (int) y;
    onex = ((int) x) + 1;
    oney = ((int) y) + 1;
    diffx = x - zerox;
    diffy = y - zeroy;
    val = (testImage.get(onex, zeroy) - testImage.get(zerox, zeroy)) * diffx + (testImage.get(zerox, oney) - testImage.get(zerox, zeroy)) * diffy + (testImage.get(onex, oney) + testImage.get(zerox, zeroy) - testImage.get(zerox, oney) - testImage.get(onex, zeroy)) * diffx * diffy + testImage.get(zerox, zeroy);
    return (int) val;
  }
  /** This method is called from within the constructor to
   * initialize the form.
   * WARNING: Do NOT modify this code. The content of this method is
   * always regenerated by the FormEditor.
   */
  private void initComponents() {//GEN-BEGIN:initComponents
      jPanel1 = new javax.swing.JPanel();
      jPanel3 = new javax.swing.JPanel();
      jPanel4 = new javax.swing.JPanel();
      jButton1 = new javax.swing.JButton();
      getContentPane().setLayout(new java.awt.GridBagLayout());
      java.awt.GridBagConstraints gridBagConstraints1;
      addWindowListener(new java.awt.event.WindowAdapter() {
          public void windowClosing(java.awt.event.WindowEvent evt) {
              exitForm(evt);
          }
      }
      );
      
      jPanel1.setLayout(new java.awt.GridLayout(1, 1));
      jPanel1.setPreferredSize(new java.awt.Dimension(172, 463));
      jPanel1.setBorder(new javax.swing.border.TitledBorder("Input Signal Border"));
      
      gridBagConstraints1 = new java.awt.GridBagConstraints();
      gridBagConstraints1.gridx = 0;
      gridBagConstraints1.gridy = 0;
      gridBagConstraints1.fill = java.awt.GridBagConstraints.BOTH;
      gridBagConstraints1.weightx = 1.0;
      gridBagConstraints1.weighty = 80.0;
      getContentPane().add(jPanel1, gridBagConstraints1);
      
      
      jPanel3.setLayout(new java.awt.GridLayout(1, 1));
      jPanel3.setPreferredSize(new java.awt.Dimension(172, 463));
      jPanel3.setBorder(new javax.swing.border.TitledBorder("Impulse Signal"));
      
      gridBagConstraints1 = new java.awt.GridBagConstraints();
      gridBagConstraints1.gridx = 1;
      gridBagConstraints1.gridy = 0;
      gridBagConstraints1.fill = java.awt.GridBagConstraints.BOTH;
      gridBagConstraints1.weightx = 1.0;
      gridBagConstraints1.weighty = 80.0;
      getContentPane().add(jPanel3, gridBagConstraints1);
      
      
      jPanel4.setLayout(new java.awt.GridLayout(1, 1));
      jPanel4.setPreferredSize(new java.awt.Dimension(172, 463));
      jPanel4.setBorder(new javax.swing.border.TitledBorder("Output Signal Border"));
      
      gridBagConstraints1 = new java.awt.GridBagConstraints();
      gridBagConstraints1.gridx = 2;
      gridBagConstraints1.gridy = 0;
      gridBagConstraints1.fill = java.awt.GridBagConstraints.BOTH;
      gridBagConstraints1.weightx = 1.0;
      gridBagConstraints1.weighty = 80.0;
      getContentPane().add(jPanel4, gridBagConstraints1);
      
      
      jButton1.setText("Load Input");
      jButton1.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(java.awt.event.ActionEvent evt) {
              jButton1ActionPerformed(evt);
          }
      }
      );
      
      gridBagConstraints1 = new java.awt.GridBagConstraints();
      gridBagConstraints1.gridx = 0;
      gridBagConstraints1.gridy = 1;
      gridBagConstraints1.gridwidth = 3;
      gridBagConstraints1.fill = java.awt.GridBagConstraints.BOTH;
      gridBagConstraints1.anchor = java.awt.GridBagConstraints.WEST;
      gridBagConstraints1.weightx = 1.0;
      gridBagConstraints1.weighty = 15.0;
      getContentPane().add(jButton1, gridBagConstraints1);
      
      pack();
      java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
      java.awt.Dimension dialogSize = getSize();
      setSize(new java.awt.Dimension(700, 700));
      setLocation((screenSize.width-700)/2,(screenSize.height-700)/2);
  }//GEN-END:initComponents

  private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
    partA();
    JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
    jPanel1.removeAll();
    jPanel3.removeAll();
    partB();
    JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
    jPanel3.removeAll();
    partC();
     JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
    jPanel1.removeAll();
    jPanel3.removeAll();
    partD();
     JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
    jPanel1.removeAll();
    jPanel3.removeAll();
    partE();
  }//GEN-LAST:event_jButton1ActionPerformed

    private void partA() {
    ComplexImage transx;
    RealGrayImage r, cosine, scadd;
    imagex = generateSine(8, 128, 'x');
    FFT f = new FFT();
    transx = f.forward(imagex);
    try {
    r  = transx.getMagnitudeImage();  
    writeToFile(imagex, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputA.pgm");
    writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputA.pgm");
        ImageCanvas sg = new ImageCanvas(imagex);
        jPanel1.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ImageCanvas ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
        
         JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
        jPanel3.removeAll();
        
        cosine = generateCosine(8, 128, 'x');
        f = new FFT();
        transx = f.forward(cosine);
        r  = transx.getMagnitudeImage();  
        writeToFile(cosine, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputA2.pgm");
        writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputA2.pgm");
        sg = new ImageCanvas(cosine);
        jPanel4.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
        
        JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
        jPanel1.removeAll();
        jPanel3.removeAll();
        jPanel4.removeAll();
        cosine.add(imagex);
        f = new FFT();
        transx = f.forward(cosine);
        r  = transx.getMagnitudeImage();  
        writeToFile(cosine, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputA3.pgm");
        writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputA3.pgm");
        sg = new ImageCanvas(cosine);
        jPanel1.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
  }
  private void partB() {
    ComplexImage transx;
    RealGrayImage r;
    ImageCanvas sg, ig2;
    imagey = generateSine(4, 128, 'y');
    FFT f = new FFT();
    transx = f.forward(imagey);
    try {
    r  = transx.getMagnitudeImage();  
    writeToFile(imagey, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputB.pgm");
    writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputB.pgm");
    
        sg = new ImageCanvas(imagey);
        jPanel1.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
    JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);
    jPanel1.removeAll();
    jPanel3.removeAll();
    //Sum
    imageadd = (RealGrayImage) imagex.copy();
    imageadd.add(imagey);
    transx = f.forward(imageadd);
    try {
    r  = transx.getMagnitudeImage();  
    writeToFile(imageadd, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputAB.pgm");
    writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputAB.pgm");
    
        sg = new ImageCanvas(imageadd);
        jPanel1.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
  }
  
  private void partC() {
      ImageCanvas sg, ig2;
      RealGrayImage r;
      ComplexImage transx;
      Mapper m;
      try {
        imagerotate = (RealGrayImage) imageadd.copy();    
        imagerotate = rotateImage(imagerotate, 15);
       FFT f = new FFT();
       transx = f.forward(imagerotate);
       r  = transx.getMagnitudeImage();  
       writeToFile(imagerotate, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputC.pgm");
       writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputC.pgm");
       imagerotate.byteSize();
        sg = new ImageCanvas(imagerotate);
        jPanel4.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
      }
      catch (Exception e) {
          JOptionPane.showMessageDialog(null, e.getMessage());
      }
  }
  private void partD() {
    ComplexImage transx;
    RealGrayImage r;
    ImageCanvas sg, ig2;
    
    FFT f = new FFT();
    //Multiply
    imagemult = (RealGrayImage) imagex.copy();
    imagemult.multiply(imagey);
    transx = f.forward(imagemult);
    try {
    r  = transx.getMagnitudeImage();  
    writeToFile(imagemult, "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\inputD.pgm");
    writeToFile((RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage()), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputD.pgm");
    
        sg = new ImageCanvas(imagemult);
        jPanel1.add(sg);
        r = (RealGrayImage) (new Shift(r.X()/2,r.Y()/2, Shift.WRAP)).apply(transx.getMagnitudeImage());
        r.byteSize();
        ig2 = new ImageCanvas(r);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
  }

    private void partE() {
    ImageCanvas sg, ig2;
    RealGrayImage out1, out2;
    PhaseImage out3, out4;
    ComplexImage cg1, cg2,cg3,cg4;
    
    GrayImage rg1 = (GrayImage) readFromFile();
    GrayImage rg2 = (GrayImage) readFromFile();  
    
     try {
        sg = new ImageCanvas(rg1);
        jPanel1.add(sg);
         ig2 = new ImageCanvas(rg2);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
    
    JOptionPane.showConfirmDialog(this, "Continue?", "Continue?", JOptionPane.YES_OPTION, JOptionPane.PLAIN_MESSAGE);    
    jPanel1.removeAll();
    jPanel3.removeAll();
    
    FFT f = new FFT();
    cg1 = f.forward(rg1);
    cg2 = f.forward(rg2);
    
    out1 =  cg1.getMagnitudeImage();
    out2 =   cg2.getMagnitudeImage();
    out3 = new PhaseImage(cg1);
    out4 = new PhaseImage(cg2);
    
    cg3 = new ComplexImage();
    cg4 = new ComplexImage();
    
    cg3 = new ComplexImage(out1.X(), out1.Y());
    cg4 = new ComplexImage(out2.X(), out2.Y());
    
    for (int i=0; i<cg3.X(); i++) {
        for (int j=0; j<cg3.Y(); j++) {
            if (i < out4.X()  && j < out4.Y()) {
                cg3.setReal(i, j,(float) (out1.get(i,j) * Math.cos(out4.get(i,j))));
                cg3.setImag(i, j, (float) (out1.get(i,j) * Math.sin(out4.get(i,j))));
            }
            else {
                cg3.setReal(i, j, (float) (out1.get(i,j) * Math.cos(0)));
                cg3.setImag(i, j, (float) (out1.get(i,j) * Math.sin(0)));
            }
        }
    }
    
    for (int i=0; i<cg4.X(); i++) {
        for (int j=0; j<cg4.Y(); j++) {
            if (i < out3.X() && j < out3.Y()) {
                cg4.setReal(i, j, (float) (out2.get(i,j) * Math.cos(out3.get(i,j))));
                cg4.setImag(i, j, (float) (out2.get(i,j) * Math.sin(out3.get(i,j))));
            }
            else {
                cg4.setReal(i, j, (float) (out2.get(i,j) * Math.cos(0)));
                cg4.setImag(i, j, (float) (out2.get(i,j) * Math.sin(0)));
            }
        }
    }
    
    cg3 = f.reverse(cg3);
    cg4 = f.reverse(cg4);
    
    writeToFile(ImageConverter.toRealGray(cg3), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputE1.pgm");
    writeToFile(ImageConverter.toRealGray(cg4), "c:\\bryan\\programs\\java\\fourier transforms\\Data for 2D\\outputE2.pgm");
    
    try {
        sg = new ImageCanvas(cg3);
        jPanel1.add(sg);
         ig2 = new ImageCanvas(cg4);
        jPanel3.add(ig2);
    }
    catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
    }
  }

  private String ImageFileBrowse() {
       File file;
      String name;
      JFileChooser selectFile = new JFileChooser();
      selectFile.setCurrentDirectory(new File("c:\\bryan\\programs\\java\\Fourier Transforms"));
      selectFile.showDialog(this, "Select File");
      file = selectFile.getSelectedFile();
      if (file != null) {
          return file.getAbsolutePath();
      }
      return "";
  }
  /** Exit the Application */
  private void exitForm(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_exitForm
    System.exit (0);
  }//GEN-LAST:event_exitForm

  /**
  * @param args the command line arguments
  */
  public static void main (String args[]) {
    new Fourier2D ().show ();
  }

  // Variables declaration - do not modify//GEN-BEGIN:variables
  private javax.swing.JPanel jPanel1;
  private javax.swing.JPanel jPanel3;
  private javax.swing.JPanel jPanel4;
  private javax.swing.JButton jButton1;
  // End of variables declaration//GEN-END:variables

}