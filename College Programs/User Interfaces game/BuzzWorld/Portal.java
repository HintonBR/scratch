/*
 * Portal.java
 *
 * Created on February 13, 2001, 10:07 PM
 */


import java.beans.*;
import javax.swing.*;
import java.awt.*;
/**
 *
 * @author  Bryan
 * @version
 */
public class Portal implements java.io.Serializable {
    
    public  static final String PORTAL_LOCATION = "location";
    public  static final String PORTAL_SPACEMODEL = "spacemodel";
    public static final String PORTAL_SIZE = "size";
    public static final String PORTAL_DELETE = "delete";
    public static final String PORTAL_NAME = "portalname";
    private SpaceModel sm;
    private PropertyChangeSupport pcs;
    private String portalName;
    private Dimension size;
    private Point location; 
    
    /** Creates new Portal */
    public Portal() {
        pcs = new PropertyChangeSupport ( this );
        sm = null;
        portalName = "";
        addMyListeners();
    }
    
    public Portal(Dimension d, Point p, SpaceModel space, String pName) {
        pcs = new PropertyChangeSupport ( this );
        setLocation(p);
        sm = space;
        portalName = pName;
        setSize(d);
        addMyListeners();
    }
    
    public Portal(Dimension d, Point p, SpaceModel space) {
        pcs = new PropertyChangeSupport ( this );
        setLocation(p);
        sm = space;
        portalName = sm.getName();
        setSize(d);
        addMyListeners();
    }
    
    public String getPortalName() {
        return portalName;
    }
    public Rectangle getRectangle() {
        return new Rectangle((int) getX(),(int) getY(), (int) size.getWidth(), (int) size.getHeight());
    }
    public void setPortalName(String name) { 
        if (portalName.equals(sm.SPACE_ENTRY) == false) {
            pcs.firePropertyChange(this.PORTAL_NAME, portalName, name);
            portalName = name;
        }
    }
    public void addMyListeners() {
        sm.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                spaceModelChange(evt);
            }
        });
    }
    public boolean isEntryPortal() {
        return portalName.equals(sm.SPACE_ENTRY);
    }
    private void spaceModelChange(java.beans.PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(SpaceModel.SPACE_NAME) && portalName.equals(SpaceModel.SPACE_ENTRY) == false) {
            this.setPortalName(sm.getName());
        }
    }
    public Dimension getSize() {
        return size;
    }
    public Point getLocation() {
        return location;
    }
    public double getX() {
        return location.getX();
    }
    public double getY() {
        return location.getY();
    }
    public void setLocation(double x, double y) {
        Point value = new Point((int) x, (int) y);
        Point oldValue = location;
        location = value;
        pcs.firePropertyChange (PORTAL_LOCATION, oldValue, value);
    }
    public void setSize (Dimension value) {
        Dimension oldValue = size;
        size = value;
        pcs.firePropertyChange (PORTAL_SIZE, oldValue, value);
    }
    
    public void setLocation (Point value) {
        Point oldValue = location;
        location = value;
        pcs.firePropertyChange (PORTAL_LOCATION, oldValue, value);
    }
    public double getWidth() {
        return size.getWidth();
    }
    public double getHeight() {
        return size.getHeight();
    }
    
    public SpaceModel getSpaceModel () {
        return sm;
    }
    
    public void setSpaceModel (SpaceModel value) {
        SpaceModel oldValue = sm;
        sm = value;
        pcs.firePropertyChange (PORTAL_SPACEMODEL, oldValue, sm);
    }
    
    public void deletePortal() {
       pcs.firePropertyChange(this.PORTAL_DELETE, null, this);
    }
    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }
    
    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }
}
