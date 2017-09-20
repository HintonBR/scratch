/*
 * CharacterInstance.java
 *
 * Created on February 19, 2001, 9:25 PM
 */

import java.beans.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;
import java.lang.Math;
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
    private double rotAngle;
    
    //private AffineTransform 
    PropertyChangeListener pcl1, pcl2;
    /** Creates new CharacterInstance */
    public CharacterInstance(CharacterModel c, SpaceModel s, int side, Point loc) {
        cm = c;
        sm = s;
        currentStrength = cm.getStrength();
        team = side;
        location = loc;
        pcs = new PropertyChangeSupport ( this );
        Rectangle r = cm.getBoundingRectangle();
        addMyListeners();
        rotAngle = 0;
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
    public double getRotAngle() {
        return rotAngle;
    }
    public void setRotAngle(double val) {
        rotAngle = val;
        pcs.firePropertyChange(this.CI_CHARACTERMODEL, 0, (int)val);
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
     public Rectangle getRectangle(double translatex, double translatey) {
         AffineTransform trans = getTransform(translatex,translatey);
         GeneralPath p = cm.getGeneralPath();
         p.transform(trans);
        return p.getBounds();
    } 
    public AffineTransform getTransform(double translatex, double translatey) {
        AffineTransform transform = new AffineTransform();
        GeneralPath p = cm.getGeneralPath();
        Rectangle c = p.getBounds();
        transform.translate(translatex, translatey);
        transform.scale(((double) currentStrength)/((double)cm.getStrength()), ((double) currentStrength)/((double)cm.getStrength()));
        transform.translate(c.getWidth()/2, c.getHeight()/2);
        transform.rotate(rotAngle);
        transform.translate(-(c.getWidth()/2), -(c.getHeight()/2));
        return transform;
    }
    public boolean isHit(Point hitPoint, double translatex, double translatey) {
        AffineTransform transform = getTransform(translatex, translatey);
        GeneralPath p = cm.getGeneralPath();
        p.transform(transform);
        return p.contains(hitPoint.getX(), hitPoint.getY());
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
        setRotAngle(Math.atan2(loc.getY() - oldValue.getY(), loc.getX() - oldValue.getX()));
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, loc);
    }
    public void setLocation(int x, int y) {
        Point oldValue = location;
        location = new Point(x, y);
        setRotAngle(Math.atan2(y - oldValue.getY(), x - oldValue.getX()));
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, location);
    }
    public void setLocation(Point loc, boolean setAngle) {
        Point oldValue = location;
        location = loc;
        if (setAngle) {
            setRotAngle(Math.atan2(loc.getY() - oldValue.getY(), loc.getX() - oldValue.getX()));
        }
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, loc);
    }
    public void setLocation(int x, int y, boolean setAngle) {
        Point oldValue = location;
        location = new Point(x, y);
        if (setAngle) {
            setRotAngle(Math.atan2(y - oldValue.getY(), x - oldValue.getX()));
        }
        pcs.firePropertyChange(this.CI_LOCATION, oldValue, location);
    }

    public void move(Point loc) {
        setLocation(new Point((int) (location.getX() + loc.getX()), (int) (location.getY() + loc.getY())));
    }
    public void attack(CharacterInstance ci) {
        double tempStrength, tempStrength2;
        tempStrength = (ci.getRotAngle()/(Math.PI * 2)) * 360.0;
        tempStrength2 = (this.getRotAngle()/(Math.PI * 2)) * 360.0;
        if (tempStrength2 > 180) tempStrength2 = tempStrength2 - 180;
        if (tempStrength > 180) tempStrength = tempStrength - 180;
        tempStrength = tempStrength2 - tempStrength;
        tempStrength = Math.abs(tempStrength);
        if (tempStrength >= 0 && tempStrength < 45) {
            tempStrength = ci.getCurrentStrength() - 2;
        }
        else if (tempStrength >= 45 && tempStrength < 135) {
            tempStrength = ci.getCurrentStrength() - 1;
        }    
        else if (tempStrength >= 135 && tempStrength < 180) {
            tempStrength = ci.getCurrentStrength();
        }
        if (tempStrength <= currentStrength) {
            ci.decrementStrength(2);
        }
        else {
            decrementStrength(2);
            ci.decrementStrength(1);
        }
    }
    public void teleport(SpaceModel space) {
        setSpaceModel(space);
        rotAngle = 0;
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
