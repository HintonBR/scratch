/*
 * ColorGray.java
 *
 * Created on January 22, 2001, 10:35 PM
 */


/**
 *
 * @author  Bryan
 * @version 
 */
import jigl.image.io.*;
import jigl.image.*;
import jigl.gui.*;
import jigl.signal.*;
import jigl.signal.io.*;
import javax.swing.*;
import java.awt.*;
import jigl.signal.ops.*;
public class ImageDisplay extends javax.swing.JFrame {

    

    private GrayImage grayImage2;
    private GrayImage  grayImage;
    private JImageCanvas canvas1;
    private JImageCanvas canvas2;
    private SignalCanvas sgCanvas;
    private Histogram hgImage;
    private Histogram hgImage2;
    private CumulativeHistogram chgImage;
    private RealSignal rsSignal;
    private int area;
    int lookupTable[];
    
    /** Creates new form ColorGray */
    public ImageDisplay() {
        initComponents ();
        lookupTable = new int[256];
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the FormEditor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        jLabel2 = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        scrollPane1 = new java.awt.ScrollPane();
        jPanel1 = new javax.swing.JPanel();
        scrollPane2 = new java.awt.ScrollPane();
        jPanel3 = new javax.swing.JPanel();
        jPanel4 = new javax.swing.JPanel();
        cmdInitiate = new javax.swing.JButton();
        jPanel5 = new javax.swing.JPanel();
        cmdHistogram = new javax.swing.JButton();
        cmdNewHistogram = new javax.swing.JButton();
        setTitle("Imaging Display");
        setForeground(java.awt.Color.white);
        setBackground(java.awt.Color.white);
        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                formComponentResized(evt);
            }
        }
        );
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                exitForm(evt);
            }
        }
        );
        
        jLabel2.setBorder(new javax.swing.border.BevelBorder(1, java.awt.Color.white, java.awt.Color.white, java.awt.Color.darkGray,
        java.awt.Color.black));
        jLabel2.setText("Imaging Display");
        jLabel2.setForeground(java.awt.Color.black);
        jLabel2.setBackground(new java.awt.Color (204, 204, 255));
        jLabel2.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel2.setFont(new java.awt.Font ("Garamond", 0, 36));
        jLabel2.setOpaque(true);
        jLabel2.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        
        getContentPane().add(jLabel2, java.awt.BorderLayout.NORTH);
        
        
        jPanel2.setLayout(new java.awt.GridLayout(1, 1));
        jPanel2.setBorder(new javax.swing.border.TitledBorder("Gray Image - Pre-Equalization"));
        jPanel2.setOpaque(false);
        
        scrollPane1.setFont(new java.awt.Font ("Dialog", 0, 11));
          scrollPane1.setBackground(new java.awt.Color (204, 204, 204));
          scrollPane1.setForeground(java.awt.Color.black);
          jPanel2.add(scrollPane1);
          
          
        getContentPane().add(jPanel2, java.awt.BorderLayout.WEST);
        
        
        jPanel1.setLayout(new java.awt.GridLayout(1, 1));
        jPanel1.setBorder(new javax.swing.border.TitledBorder(
        new javax.swing.border.EtchedBorder(), "Gray Image - Equalized"));
        jPanel1.setOpaque(false);
        
        scrollPane2.setFont(new java.awt.Font ("Dialog", 0, 11));
          scrollPane2.setBackground(new java.awt.Color (204, 204, 204));
          scrollPane2.setForeground(java.awt.Color.black);
          jPanel1.add(scrollPane2);
          
          
        getContentPane().add(jPanel1, java.awt.BorderLayout.EAST);
        
        
        jPanel3.setLayout(new java.awt.GridLayout(1, 1));
        jPanel3.setBorder(new javax.swing.border.TitledBorder("Histogram"));
        jPanel3.setBackground(java.awt.Color.lightGray);
        jPanel3.setOpaque(false);
        jPanel3.setAutoscrolls(true);
        
        getContentPane().add(jPanel3, java.awt.BorderLayout.CENTER);
        
        
        jPanel4.setLayout(new java.awt.GridLayout(2, 1));
        
        cmdInitiate.setBorder(new javax.swing.border.BevelBorder(0));
          cmdInitiate.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
          cmdInitiate.setName("cmdIntiatiate");
          cmdInitiate.setFont(new java.awt.Font ("Garamond", 1, 18));
          cmdInitiate.setText("Generate New Image");
          cmdInitiate.addActionListener(new java.awt.event.ActionListener() {
              public void actionPerformed(java.awt.event.ActionEvent evt) {
                  cmdInitiateActionPerformed(evt);
              }
          }
          );
          jPanel4.add(cmdInitiate);
          
          
        jPanel5.setLayout(new java.awt.GridLayout(1, 2));
          
          cmdHistogram.setBorder(new javax.swing.border.BevelBorder(0));
            cmdHistogram.setFont(new java.awt.Font ("Garamond", 1, 18));
            cmdHistogram.setText("Show Original Histogram");
            cmdHistogram.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent evt) {
                    cmdHistogramActionPerformed(evt);
                }
            }
            );
            jPanel5.add(cmdHistogram);
            
            
          cmdNewHistogram.setBorder(new javax.swing.border.BevelBorder(0));
            cmdNewHistogram.setFont(new java.awt.Font ("Garamond", 1, 18));
            cmdNewHistogram.setText("Show New Histogram");
            cmdNewHistogram.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent evt) {
                    cmdNewHistogramActionPerformed(evt);
                }
            }
            );
            jPanel5.add(cmdNewHistogram);
            
            jPanel4.add(jPanel5);
          
          
        getContentPane().add(jPanel4, java.awt.BorderLayout.SOUTH);
        
        pack();
        java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        java.awt.Dimension dialogSize = getSize();
        setSize(new java.awt.Dimension(1080, 661));
        setLocation((screenSize.width-1080)/2,(screenSize.height-661)/2);
    }//GEN-END:initComponents

  private void cmdNewHistogramActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdNewHistogramActionPerformed
  
      if (grayImage2 != null) {
        if (sgCanvas != null) jPanel3.remove(sgCanvas);
        createHistogram(eqGrayImage, ehgImage);
     
        JOptionPane.showMessageDialog(null, "New Image Histogram Displayed");
        jPanel3.add(sgCanvas);
        sgCanvas.setBackground(Color.white);
        sgCanvas.setSize(jPanel3.getWidth(), jPanel3.getHeight()/3);
        sgCanvas.setLocation(0, jPanel3.getHeight() - sgCanvas.getHeight());
        jPanel3.repaint();
        writeToFile(eqGrayImage, "c:\\mysteryrevealed.pgm");
        writeToFile(rsSignal, "c:\\histogram2.dat");
        writeToFile(chgImage.toSignal(), "c:\\histogram3.dat");
      }
  }//GEN-LAST:event_cmdNewHistogramActionPerformed

  private void formComponentResized(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_formComponentResized
    this.setTitle("CS450 Imaging Project #2 - Form size X=" + this.getWidth() + ", Y=" + this.getHeight());
    
  }//GEN-LAST:event_formComponentResized
   
  private void cmdInitiateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdInitiateActionPerformed
      
  }//GEN-LAST:event_cmdInitiateActionPerformed

  private void cmdHistogramActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdHistogramActionPerformed
      if (grayImage != null) {
          if (sgCanvas != null) jPanel3.remove(sgCanvas);

          createHistogram(grayImage,hgImage);
          JOptionPane.showMessageDialog(null, "Original Histogram Displayed");
          jPanel3.add(sgCanvas);
          sgCanvas.setBackground(Color.white);
          sgCanvas.setSize(jPanel3.getWidth(), jPanel3.getHeight()/3);
          sgCanvas.setLocation(0, jPanel3.getHeight() - sgCanvas.getHeight());
          jPanel3.repaint();
      }
  }//GEN-LAST:event_cmdHistogramActionPerformed

  private Histogram createHistogram(GrayImage gImage, Histogram hg) {
      RealSignal test;
      try {
            hg = new Histogram(gImage, 1);
            rsSignal = hg.toSignal();
            rsSignal.byteSize();
            if (sgCanvas == null) {
                sgCanvas = new SignalCanvas(rsSignal);
            }
            else {
                sgCanvas.setSignal(rsSignal);
            }
      }
      catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getClass().toString() + "\n" + e.getMessage());
      }
      return hg;
  }
  
  private void writeToFile(Signal mySignal, String filename) {
      try {
    SignalOutputStream ios = new SignalOutputStream(filename);
    ios.write(mySignal);
    ios.close();
      }
      catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.getMessage());
      }
      
  }
  
  private void writeToFile(GrayImage myImage, String filename) {
      try {
    ImageOutputStream ios = new ImageOutputStream(filename);
    ios.write(myImage);
    ios.close();
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
        ImageDisplay cg = new ImageDisplay ();
        //Load Image
        
        try {
        ImageInputStream ins = new ImageInputStream(args[0]);
        cg.grayImage = (GrayImage) ins.read();
        ins.close();
        cg.scrollPane1.add(cg.canvas2 = new JImageCanvas(cg.grayImage));
        cg.scrollPane1.setSize(cg.getWidth()/3, cg.getHeight() * (3/5));
        cg.scrollPane2.setSize(cg.getWidth()/3, cg.getHeight() * (3/5));
        }
        catch (Exception e) {
            if (e.getMessage() != null && e.getMessage().equals("") == false) {
                JOptionPane.showMessageDialog(null, e.getMessage());
            }
         }
        cg.show();
    }


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel jLabel2;
    private javax.swing.JPanel jPanel2;
    private java.awt.ScrollPane scrollPane1;
    private javax.swing.JPanel jPanel1;
    private java.awt.ScrollPane scrollPane2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JButton cmdInitiate;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JButton cmdHistogram;
    private javax.swing.JButton cmdNewHistogram;
    // End of variables declaration//GEN-END:variables

}
