/*
 * PortalComponent.java
 *
 * Created on February 13, 2001, 11:57 PM
 */
import java.beans.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class PortalComponent extends JComponent implements java.io.Serializable {

    public static final String PORTAL_DELETE = "Component delete";
    private static final int RESIZE_NW  = 1;
    private static final int RESIZE_N   = 2;
    private static final int RESIZE_NE  = 3;
    private static final int RESIZE_E   = 4;
    private static final int RESIZE_SE  = 5;
    private static final int RESIZE_S   = 6;
    private static final int RESIZE_SW  = 7;
    private static final int RESIZE_W   = 8;
    
    private Portal pm;
    private PropertyChangeListener pcl;
    private MouseMotionListener pcl2;
    private MouseListener pcl3;
    private boolean askFlag = false;
    private boolean resizeFlag = false;
    private boolean dragFlag = false;
    private int resizeLocation = 0;
    private Rectangle resizeRectangle;
    private Point dragStart;
    
    /** Creates new Portal */
    public PortalComponent(Portal p) {
        pm = p;
        this.setMySize(pm.getSize());
        this.setLoc(pm.getLocation());
        setBorder(new javax.swing.border.LineBorder(Color.black));
        addMyListeners();
        setBorder(new javax.swing.border.LineBorder(java.awt.Color.cyan));
        setForeground(java.awt.Color.cyan);
        this.setToolTipText(pm.getPortalName());
    }
    
   
    public  void addMyListeners() {
        pm.addPropertyChangeListener(pcl = new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                portalModelChange(evt);
            }
        });
        addMouseMotionListener(pcl2 = new java.awt.event.MouseMotionAdapter() {
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                dragged(evt);
            }
        }
        );
       addMouseListener(pcl3 = new java.awt.event.MouseAdapter() {
              public void mousePressed(java.awt.event.MouseEvent evt) {
                  pressed(evt);
              }
              public void mouseReleased(java.awt.event.MouseEvent evt) {
                  released(evt);
              }
          }
          );
    }
    public void portalModelChange(java.beans.PropertyChangeEvent evt) {
       repaint();
    }
    private void pressed(java.awt.event.MouseEvent evt) {
        int shortestPoint = this.RESIZE_NW;
        double shortestDistance = 0, tempDistance = 0;
        
        //Calculate for NW
        shortestDistance = evt.getPoint().distance(0,0);
        this.setCursor(new Cursor(Cursor.NW_RESIZE_CURSOR));
        //Calculate for N
        if ((tempDistance = evt.getPoint().distance(getWidth()/2,0)) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_N;
            this.setCursor(new Cursor(Cursor.N_RESIZE_CURSOR));
        }
        //Calculate for NE
        if ((tempDistance = evt.getPoint().distance(getWidth(),0)) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_NE;
            this.setCursor(new Cursor(Cursor.NE_RESIZE_CURSOR));
        }
        //Calculate for E
        if ((tempDistance = evt.getPoint().distance(getWidth(),getHeight()/2)) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_E;
            this.setCursor(new Cursor(Cursor.E_RESIZE_CURSOR));
        }
        //Calculate for SE
        if ((tempDistance = evt.getPoint().distance(getWidth(),getHeight())) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_SE;
            this.setCursor(new Cursor(Cursor.SE_RESIZE_CURSOR));
        }
        //Calculate for S        
        if ((tempDistance = evt.getPoint().distance(getWidth()/2,getHeight())) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_S;
            this.setCursor(new Cursor(Cursor.S_RESIZE_CURSOR));
        }
        //Calculate for SW
        if ((tempDistance = evt.getPoint().distance(0,getHeight())) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_SW;
            this.setCursor(new Cursor(Cursor.SW_RESIZE_CURSOR));
        }
        //Calculate for W
        if ((tempDistance = evt.getPoint().distance(0,getHeight()/2)) < shortestDistance) {
            shortestDistance = tempDistance;
            shortestPoint = this.RESIZE_W;
            this.setCursor(new Cursor(Cursor.W_RESIZE_CURSOR));
        }
        if (shortestDistance < 10) {
            resizeFlag = true;
            resizeLocation = shortestPoint;
            dragStart = evt.getPoint();
        }
        else {
            dragFlag = true;
            dragStart = evt.getPoint();
            this.setCursor(new Cursor(Cursor.MOVE_CURSOR));
        }
    }
    private void released(java.awt.event.MouseEvent evt) {
        if (resizeFlag == true && resizeRectangle != null) {
            this.setLoc(new Point((int) resizeRectangle.getX(), (int) resizeRectangle.getY()));
            this.setMySize(resizeRectangle.getSize());
        }
        resizeFlag = false;
        dragFlag = false;
        this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }
    private void doResize(java.awt.event.MouseEvent evt) {
        Graphics g = this.getParent().getGraphics();
        Color curColor = g.getColor();
        g.setColor(Color.cyan);
        switch (resizeLocation) {
            case RESIZE_NW:
                resizeRectangle = new Rectangle(evt.getX() + getX(), evt.getY() + getY(), getWidth() - evt.getX(), getHeight() - evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_N:
                resizeRectangle = new Rectangle(getX(), evt.getY() + getY(), getWidth(), getHeight() - evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_NE:
                resizeRectangle = new Rectangle(getX(), evt.getY() + getY(), evt.getX(), getHeight() - evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_E:
                resizeRectangle = new Rectangle(getX(), getY(), evt.getX(), getHeight());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_SE:
                resizeRectangle = new Rectangle(getX(), getY(), evt.getX(), evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_S:
                resizeRectangle = new Rectangle(getX(), getY(), getWidth(), evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_SW:
                resizeRectangle = new Rectangle(getX() + evt.getX(), getY(), getWidth() - evt.getX(), evt.getY());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
            case RESIZE_W:
                resizeRectangle = new Rectangle(getX()+evt.getX(), getY(), getWidth() - evt.getX(), getHeight());
                g.drawRect((int)resizeRectangle.getX(),(int) resizeRectangle.getY(), (int)resizeRectangle.getWidth(), (int) resizeRectangle.getHeight());
                break;
        }
        g.setColor(curColor);
    }
    private void dragged(java.awt.event.MouseEvent evt) {
        if (evt.getX() - dragStart.getX() + pm.getX() >= 0 && evt.getY()-dragStart.getY() + pm.getY() >=0 && evt.getX() - dragStart.getX() + pm.getX() + pm.getWidth() <= this.getParent().getWidth() && evt.getY()-dragStart.getY() +pm.getY() + pm.getHeight() <= this.getParent().getHeight()) {
            if (dragFlag == true) {
                this.setLoc(new Point((int)(evt.getX()- dragStart.getX()) + getX(), (int)(evt.getY()-dragStart.getY()) + getY()));
            }
            else if (resizeFlag == true) {
                doResize(evt);
                if (this.getParent() != null) this.getParent().repaint();
            }
        }
        else if (askFlag == false && pm.isEntryPortal() == false) {
            askFlag = true;
            if (JOptionPane.showConfirmDialog(null, "Do you want to remove this portal?", "Remove Portal?", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
                pm.removePropertyChangeListener(pcl);
                this.removeMouseListener(pcl3);
                this.removeMouseMotionListener(pcl2);
                pm.deletePortal();
            }
            askFlag = false;
        }
    }
    
        
    public void paint(Graphics g) {
        super.paint(g);
       setMySize(pm.getSize());
       setLoc(pm.getLocation());
        if (pm.getPortalName() != null) {
            this.setToolTipText(pm.getPortalName());
            g.drawString(pm.getPortalName(), (getWidth() /2) - (g.getFontMetrics().stringWidth(pm.getPortalName())/2), getHeight() /2);
        }
        else {
            g.drawString("No Space Model Available",(getWidth() /2) - (g.getFontMetrics().stringWidth("No Space Model Available")/2), getHeight() /2);
        }
    }
    
    public void setMySize (Dimension value) {
        pm.setSize(value);
        super.setSize(value);
    }
    
    public void setLoc (Point value) {
        pm.setLocation(value);
        super.setLocation(value);
    }
    public Portal getPortalModel() {
        return pm;
    }
}
