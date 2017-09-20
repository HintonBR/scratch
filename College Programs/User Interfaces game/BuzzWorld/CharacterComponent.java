/*
 * CharacterComponent.java
 *
 * Created on February 21, 2001, 7:29 AM
 */
import java.beans.*;
import javax.swing.*;
import java.awt.*;

/**
 *
 * @author  Bryan
 * @version 
 */
public class CharacterComponent extends javax.swing.JComponent implements java.io.Serializable{

    private CharacterInstance ci;
    private Point dragStart;
    private PropertyChangeSupport pcs;
    private Image source;
    private boolean listen;
    /** Creates new CharacterComponent */
    public CharacterComponent(CharacterInstance c, boolean listenForEvents) {
        ci =c;
        pcs = new PropertyChangeSupport ( this );
        this.setLocation(ci.getLocation());
        this.setToolTipText(ci.getCharacterModel().getName());
        listen = listenForEvents;
        if (ci.getTeam() == ci.CI_BLUETEAM)  setBorder(new javax.swing.border.LineBorder(java.awt.Color.blue,3));
        else setBorder(new javax.swing.border.LineBorder(java.awt.Color.red, 3));
        addMyListeners();
    }
   
    public void addMyListeners() {
        ci.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                characterModelChange(evt);
            }
        });
        if (listen) {
            addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
                public void mouseDragged(java.awt.event.MouseEvent evt) {
                    dragged(evt);
                }
            }
            );
           addMouseListener(new java.awt.event.MouseAdapter() {
                  public void mousePressed(java.awt.event.MouseEvent evt) {
                      pressed(evt);
                  }
                  public void mouseReleased(java.awt.event.MouseEvent evt) {
                      released(evt);
                  }
              }
              );
        }
    }
    public void characterModelChange(java.beans.PropertyChangeEvent evt) {
       this.setLocation(ci.getLocation());
       if (evt.getPropertyName().equals(CharacterInstance.CI_CHARACTERMODEL)) {
        this.setToolTipText(ci.getCharacterModel().getName());
        getImage(ci.getCharacterModel().getImageFilename());
       }
       repaint();
    }
    private void pressed(java.awt.event.MouseEvent evt) {
            dragStart = evt.getPoint();
            this.setCursor(new Cursor(Cursor.MOVE_CURSOR));
    }
    private void released(java.awt.event.MouseEvent evt) {
       ci.setLocation(new Point(this.getX(), this.getY())); 
       this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }
    private void dragged(java.awt.event.MouseEvent evt) {
        if (evt.getX() - dragStart.getX() + this.getX() >= 0 && evt.getY()-dragStart.getY() + this.getY() >=0 && evt.getX() - dragStart.getX() + this.getX() + getWidth() <= this.getParent().getWidth() && evt.getY()-dragStart.getY() +this.getY() + getHeight() <= this.getParent().getHeight()) {
                this.setLocation((int) (evt.getX()- dragStart.getX() + getX()), (int)(evt.getY()-dragStart.getY() + getY()));
        }
    }
    
        
    public void paint(Graphics g) {
        super.paint(g);
        if (source == null) getImage(ci.getCharacterModel().getImageFilename());
        if (source != null) {
            g.drawImage(source,5,5, this);
        }
        else {
            g.drawString("No Image",(getWidth() /2) - (g.getFontMetrics().stringWidth("No Image")/2), getHeight() /2);
        }
    }
     private void getImage(String filename) {
            setSize(40, 40);
            ci.setSize(new Dimension(40, 40));
            try {
                if (filename.equals("")) {
                    source = null;
                    return;
                }
		Toolkit TK = Toolkit.getDefaultToolkit(); 	
		source = TK.getImage(filename);			
                if (source == null) return;
		MediaTracker track = new MediaTracker(this);
		track.addImage(source,0);		
	        track.waitForID(0); 
            }	
		catch(Exception e) {}
             setSize(source.getWidth(this) + 10, source.getHeight(this)+ 10);
             ci.setSize(new Dimension(source.getWidth(this), source.getHeight(this)));
    }
    public CharacterInstance getCharacterInstance() {
        return ci;
    }
    public boolean isMe(CharacterInstance compare) {
        return (compare == ci);
    }
    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }
    
    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }
}
