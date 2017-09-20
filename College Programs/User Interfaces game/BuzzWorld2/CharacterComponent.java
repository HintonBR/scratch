/*
 * CharacterComponent.java
 *
 * Created on February 21, 2001, 7:29 AM
 */
import java.beans.*;
import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.awt.geom.*;

/**
 *
 * @author  Bryan
 * @version 
 */
public class CharacterComponent extends JComponent implements java.io.Serializable {

    private CharacterInstance ci;
    private Point dragStart;
    private PropertyChangeSupport pcs;
    private boolean listen;
    private Point location;
    
    /** Creates new CharacterComponent */
    public CharacterComponent(CharacterInstance c, boolean listenForEvents) {
        ci =c;
        pcs = new PropertyChangeSupport ( this );
        listen = listenForEvents;
        addMyListeners();
        if (ci.getTeam() == ci.CI_REDTEAM) {
            setBorder(new javax.swing.border.LineBorder(Color.red, 3));
       }
       else {
            setBorder(new javax.swing.border.LineBorder(Color.blue, 3));
       }
        this.setSize(ci.getRectangle(0,0).getSize());
        this.setLocation(new Point(ci.getX(), ci.getY()));
    }
   
    public void addMyListeners() {
        ci.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                characterModelChange(evt);
            }
        });
        if (listen) {
        addMouseListener( new java.awt.event.MouseAdapter() {
                public void mousePressed(java.awt.event.MouseEvent evt) {
                    pressed(evt);
                }
                public void mouseReleased(java.awt.event.MouseEvent evt) {
                      released(evt);
                  }
            }
            );
            addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
                public void mouseDragged(java.awt.event.MouseEvent evt) {
                    dragged(evt);
                }
            }
            );
        }
    }
    public void characterModelChange(java.beans.PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(CharacterInstance.CI_LOCATION)) {
            this.setLocation(new Point(ci.getX(), ci.getY()));
        }
       repaint();
    }
    private void pressed(java.awt.event.MouseEvent evt) {
        if (ci.isHit(evt.getPoint(),0,0)) {
            dragStart = evt.getPoint();
            this.setCursor(new Cursor(Cursor.MOVE_CURSOR));
        }
        else {
            dragStart = null;
        }
    }
    private void released(java.awt.event.MouseEvent evt) {
       ci.setLocation(new Point(this.getX(), this.getY())); 
       this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }
    private void dragged(java.awt.event.MouseEvent evt) {
        if (dragStart != null) {
            if (evt.getX() - dragStart.getX() + this.getX() >= 0 && evt.getY()-dragStart.getY() + this.getY() >=0 && evt.getX() - dragStart.getX() + this.getX() + getWidth() <= this.getParent().getWidth() && evt.getY()-dragStart.getY() +this.getY() + getHeight() <= this.getParent().getHeight()) {
                    this.setLocation((int) (evt.getX()- dragStart.getX() + getX()), (int)(evt.getY()-dragStart.getY() + getY()));
                    repaint();
            }
        }
    }
    
        
    public void paint(Graphics g) {
        CharacterModel cm = ci.getCharacterModel();
        Rectangle r = ci.getRectangle(0,0);
        this.setSize(r.getSize());
        super.paint(g);
        Graphics2D g2 = (Graphics2D) g;
        //AffineTransform cur = g2.getTransform();
        AffineTransform at = ci.getTransform( -r.getX(), - r.getY());
        //g2.setTransform(at);
        if (cm != null) {
            Vector v = cm.getShapes();
            if (v != null) {
                Iterator i1 = v.iterator();
                
                while (i1.hasNext()) {
                    //Draw each shape
                    CC2D o = (CC2D) i1.next();
                    GeneralPath p = o.getGeneralPath();
                    p.transform(at);
                    g2.setColor(o.getFillColor());
                    g2.fill(p);
                }
            }
        }
        //g2.setTransform(cur);
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
