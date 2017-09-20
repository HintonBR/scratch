
import java.awt.*;
import javax.swing.*;
import java.io.*;
import javax.swing.border.*;
public class HelloWorldBean extends JComponent
{
              
        File imageFiles[];
        int maxWidth;
        int maxHeight;
        String source;
        int numberOfImages;
        public HelloWorldBean(String directory)
        {   
            
            
            if (directory != null) {
                    source = directory;
                    File tempFile = new File(source);
                    if (tempFile != null) {
                        imageFiles = tempFile.listFiles(); 
                    }
                    maxWidth = -1;
                    maxHeight = -1;
                    numberOfImages = 0;
            }                                                         // check the details of the
            else {
                source = null;
                imageFiles = null;
            }
             this.setBorder(new LineBorder(Color.black,10));
        }
        
        private void getMaxes() {
            
                Graphics g = this.getGraphics();
                Image currentImage;
                int count = 0;
                while (imageFiles != null && count < imageFiles.length) {
                    if (imageFiles[count].getName().indexOf(".gif") != -1) { 
                        currentImage = getImage(imageFiles[count].getPath()); 
                        Font helveticaFont = new Font("Helvetica",Font.PLAIN,15);
                        g.setFont(helveticaFont);
                        FontMetrics FM = g.getFontMetrics();
                        if (currentImage.getWidth(this) > maxWidth) {
                            maxWidth = currentImage.getWidth(this);
                        }
                        if (FM.stringWidth(imageFiles[count].getName()) > maxWidth) {
                            maxWidth = FM.stringWidth(imageFiles[count].getName());
                        }
                        if (FM.getHeight() + currentImage.getHeight(this) > maxHeight) {
                            maxHeight = FM.getHeight() + currentImage.getHeight(this);
                        }
                        numberOfImages++;
                        
                    }
                    count++;
                }
        
        }
        private Image getImage(String imageName)
                // This method will create a new image from the file specified by
                // imageName. It will also make sure that the image is completely loaded
                // before it returns. This is important to do if we need to know the
                // width and height of the image before we draw it. 
        {
                Toolkit TK = Toolkit.getDefaultToolkit();      // Get the toolkit that this
                                                               // application is currently
                                                               // using
                Image im = TK.getImage(imageName);             // get an image for the named
                                                                    // image from the toolkit.
                                                                    // Getting the image does not
                                                                    // load it. At this point the
                                                                    // image does not have pixels or
                                                                    // a size.
                MediaTracker track = new MediaTracker(this);   // make a media tracker that can
                                                               // watch the image loading
                                                               // until it is done.
                track.addImage(im,0);                          // tell the tracker to watch the
                                                               // image
                try {   track.waitForID(0); }                  // wait for the image to load
                catch(Exception e) {}
                

                return im;
        }
        
        public void paint(Graphics g)
        {
                Image currentImage;
                int count = 0;
                int currentRow;
                int currentColumn;
                int numberPerRow;
                int numberOfRows;
                int imageCount = 0;
                int X = getWidth();
                int Y = getHeight();
                if (maxWidth == -1) {
                    getMaxes();
                    if (maxWidth == -1) { 
                        maxWidth = 0;
                    }
                }
                
                
                this.getBorder().paintBorder(this,g,0,0,this.getWidth(), this.getHeight());
                numberPerRow = X / maxWidth;
                if (numberPerRow == 0) numberPerRow = 1;
                if (numberOfImages % numberPerRow == 0) {
                    numberOfRows = numberOfImages / numberPerRow;
                }
                else {
                    numberOfRows = (numberOfImages / numberPerRow) + 1;
                }
                
                //this.setBounds(0,0,numberPerRow * maxWidth, numberOfRows * maxHeight);
                while (imageFiles != null && count < imageFiles.length) {
                    if (imageFiles[count].getName().toUpperCase().indexOf(".GIF") != -1) { 
                        imageCount++;
                        currentImage = getImage(imageFiles[count].getPath()); 
                        currentColumn = imageCount % numberPerRow;
                        currentRow = imageCount / numberPerRow;
                        if (currentColumn == 0){ 
                            currentColumn = numberPerRow - 1;
                            currentRow--;
                        }
                        else currentColumn--;
                        
                        
                        Font helveticaFont = new Font("Helvetica",Font.PLAIN,15);
                        g.setFont(helveticaFont);
                        FontMetrics FM = g.getFontMetrics();
                        
                        int imageleft = (currentColumn * maxWidth) + ((maxWidth - currentImage.getWidth(this)) /2);
                        int imagetop = (currentRow * maxHeight) + ((maxHeight - (currentImage.getHeight(this) + FM.getHeight()))/2);
                        
                        if ((currentRow+1) * maxHeight < this.getHeight() && (currentColumn + 1) * maxWidth < this.getWidth()) {
                            g.drawImage(currentImage ,   // image to be drawn
                                imageleft,        // left side of the image
                                imagetop,       // top of the image
                                this);                              
                
                        
                            int textWidth = FM.stringWidth(imageFiles[count].getName());
                            int textBase = imagetop + currentImage.getHeight(this) + FM.getHeight();
                                                               // Calculate the base line using
                                                                    // the height of the text and the
                                                                    // height of the image
                            int textLeft = (currentColumn * maxWidth) + ((maxWidth - textWidth)/2);
                            g.setColor(Color.red);
                            g.drawString(imageFiles[count].getName(),textLeft,textBase);
                        }
                    }
                    count++;
                }
        }//end of Paint method
                

}
