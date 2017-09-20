/*
 * CharacterInstance.java
 *
 * Created on February 19, 2001, 9:25 PM
 */

import java.beans.*;
import java.awt.*;
import javax.swing.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class CharacterInstance extends java.lang.Object implements java.io.Serializable {

    public static final String CI_CHARACTERMODEL = "cicmodel";
    public static final String CI_SPACEMODELPROPERTYCHANGED = "cismodelproperty";
    public static final String CI_CURRENTSTRENGTH = "cicurrentstrength";
    public static final String CI_SPACEMODELCHANGED = "cismodelchanged";
    public static final String CI_LOCATION = "cicurrentlocation";
    public static final int CI_BLUETEAM = 1;
    public static final int CI_REDTEAM = 2;
    
    private CharacterModel cm;
    private SpaceModel sm;
    private PropertyChangeSupport pcs;
    private int currentStrength;
    private Point location;
    private int team;
    private Dimension size;
    PropertyChangeListener pcl1, pcl2;
    /** Creates new CharacterInstance */
    public CharacterInstance(CharacterModel c, SpaceModel s, int side, Point loc) {
        cm = c;
        sm = s;
        currentStrength = cm.getStrength();
        team = side;
        location = loc;
        pcs = new PropertyChangeSupport ( this );  
        getImage(cm.getImageFilename());
        addMyListeners();
    }
     private void getImage(String filename) {
         JPanel j = new JPanel();
         Image source = null;
            setSize(new Dimension(40, 40));
            try {
                if (filename.equals("")) {
                    source = null;
                    return;
                }
		Toolkit TK = Toolkit.getDefaultToolkit(); 	
		source = TK.getImage(filename);			
                if (source == null) return;
		MediaTracker track = new MediaTracker(j);
		track.addImage(source,0);		
	        track.waitForID(0); 
            }	
		catch(Exception e) {}
             setSize(new Dimension(source.getWidth(j), source.getHeight(j)));
    }
    public Rectangle getRectangle() {
        return new Rectangle(getX(), getY(), (int) size.getWidth(), (int) size.getHeight());
    }
     private void addMyListeners() {
        cm.addPropertyChangeListener(pcl1 = new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        sm.addPropertyChangeListener(pcl2 = new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
    }
    private void modelPropertyChange(java.beans.PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(cm.CHARACTER_NAME) || evt.getPropertyName().equals(cm.CHARACTER_IMAGE)) { 
            pcs.firePropertyChange(this.CI_CHARACTERMODEL, null, cm);
        }
        if (evt.getPropertyName().equals(sm.SPACE_ADDED_PORTAL) || evt.getPropertyName().equals(sm.SPACE_ENTRY) || evt.getPropertyName().equals(sm.SPACE_IMAGE) || evt.getPropertyName().equals(sm.SPACE_NAME) || evt.getPropertyName().equals(sm.SPACE_DELETED_PORTAL)) {
            pcs.firePropertyChange(this.CI_SPACEMODELPROPERTYCHANGED, null, sm);
        }
    }
    public CharacterModel getCharacterModel() {
        return cm;
    }
    public void setCharacterModel(CharacterModel c) {
        CharacterModel oldValue = cm;
        cm.removePropertyChangeListener(pcl1);
        cm = c;
        cm.addPropertyChangeListener(pcl1 = new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        pcs.firePropertyChange(this.CI_CHARACTERMODEL, oldValue, cm);
    }
    
    public SpaceModel getSpaceModel() {
        return sm;
    }
    public void setSpaceModel(SpaceModel s) {
        SpaceModel oldValue = sm;
        sm.removePropertyChangeListener(pcl2);
        sm = s;
        sm.addPropertyChangeListener(pcl2 = new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        pcs.firePropertyChange(this.CI_SPACEMODELCHANGED, oldValue,sm);    
    }

    public int getCurrentStrength() {
        return currentStrength;
    }
    public void decrementStrength(int amount) {
        int oldValue = currentStrength;
        currentStrength -= amount;
        if (currentStrength < 0) currentStrength = 0;
        pcs.firePropertyChange(this.CI_CURRENTSTRENGTH, oldValue, currentStrength);    
    }
    public void setCurrentStrength(int val) {
        int oldValue = currentStrength;
        currentStrength = val;
        pcs.firePropertyChange(this.CI_CURRENTSTRENGTH, oldValue, currentStrength);    
    }
    
    public int getX() {
        return (int) location.getX();
    }
    public int getY() {
        return (int) location.getY();
    }
    public Point getLocation() {
        return location;
    }
    public void setLocation(Point loc) {
        Point oldValue = location;
        location = loc;
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, loc);
    }
    public void setLocation(int x, int y) {
        Point oldValue = location;
        location = new Point(x, y);
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, location);
    }
    public void setSize(Dimension s) {
        size = s;
    }
    public void setSize(int width, int height) {
        size = new Dimension(width, height);
    }
    public Dimension getSize() {
        return size;
    }
    public void move(Point loc) {
        setLocation(new Point((int) (location.getX() + loc.getX()), (int) (location.getY() + loc.getY())));
    }
    public void attack(CharacterInstance ci) {
        if (ci.getCurrentStrength() <= currentStrength) {
            ci.decrementStrength(2);
        }
        else {
            decrementStrength(2);
            ci.decrementStrength(1);
        }
    }
    public void teleport(SpaceModel space) {
        setSpaceModel(space);
    }
    public boolean isAlive() {
        if (currentStrength > 0) return true;
        return false;
    }
    public int getTeam() {
        return team;
    }
     public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }
}
