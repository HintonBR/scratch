/*
 * CharacterModel.java
 *
 * Created on February 3, 2001, 1:02 PM
 */


import java.beans.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;
import java.util.*;
/**
 *
 * @author  Bryan
 * @version Bxfbsdfgsdfgsdfgsdfgsdfgxcvbdfgfgdsfgdfgdfsdfgsdfg
 */
public class CharacterModel extends Object implements java.io.Serializable {

    public static final String CHARACTER_NAME = "CharacterName";
    public static final String CHARACTER_STRENGTH = "CharacterStrength";
    public static final String CHARACTER_IMAGE = "CharacterImage";
    public static final String CHARACTER_IMAGE_DELETED = "CharacterImageDeleted";
    
    private String name = "";
    private int strength = 0;
    private Vector shapes;
    private PropertyChangeSupport pcs = null;

    /** Creates new CharacterModel */
    public CharacterModel() {
        pcs = new PropertyChangeSupport ( this );
        shapes = new Vector();
    }

    public void addShape(Object o) {
        shapes.add(o);
        pcs.firePropertyChange(CHARACTER_IMAGE, null, shapes);
    }
    public void removeShape(Object o) {
        pcs.firePropertyChange(CHARACTER_IMAGE_DELETED, -1, shapes.indexOf(o));
        pcs.firePropertyChange(CHARACTER_IMAGE, null, shapes);
         shapes.remove(o);
    }
    public void removeShape(int index) {
        shapes.remove(index);
        pcs.firePropertyChange(CHARACTER_IMAGE_DELETED, -1, index);
        pcs.firePropertyChange(CHARACTER_IMAGE, null, shapes);
    }
    public boolean moveShape(int index, int xoffset, int yoffset) {
        if (index <= shapes.size() - 1) {
            CC2D c = (CC2D) shapes.get(index);
            if (c.changeShapeLocation(xoffset, yoffset)) {
                pcs.firePropertyChange(CHARACTER_IMAGE, null, shapes);
            }
            else return false;
            return true;
        }
        return false;
    }
    public Rectangle getBoundingRectangle() {
        Rectangle retVal = null;
            if (shapes != null) {
                Iterator i1 = shapes.iterator();
                while (i1.hasNext()) {
                    //Draw each shape
                    CC2D o = (CC2D) i1.next();
                    GeneralPath p = o.getGeneralPath();
                    if (retVal == null) {
                        retVal = p.getBounds();
                    }
                    else {
                        retVal = retVal.union(p.getBounds());
                    }
                }
            }
        if (retVal == null) {
            retVal = new Rectangle(0, 0);
        }
            return retVal;
    }
    //returns index to Shape in the vector
    public int findShapeAtLocation(double x, double y) {
        if (shapes != null) {
            ListIterator i1 = shapes.listIterator();
            int num = shapes.size();
            if (num == 0) num = -1;
            while (i1.hasNext()) {
                i1.next();
            }
            while (i1.hasPrevious()) {
                //Draw each shape
                num--;
                CC2D o = (CC2D) i1.previous();
                GeneralPath p = o.getGeneralPath();
                if (p.contains(x, y)) {
                    return num;
                }
            }
        }
        return -1;    
    }
    public Object getShape(int index) {
        if (index <= shapes.size() - 1) {
            return shapes.get(index);
        }
        return null;
    }
    public Vector getShapes() {
        return shapes;
    }
    public String getName () {
        return name;
    }
    public int getStrength () {
        return strength;
    }
    
    public GeneralPath getGeneralPath() {
            GeneralPath all = new GeneralPath();
            if (shapes != null) {
                Iterator i1 = shapes.iterator();
                while (i1.hasNext()) {
                    //Draw each shape
                    CC2D o = (CC2D) i1.next();
                    GeneralPath p = o.getGeneralPath();
                    all.append(p, true);
                }
            }
            return all;
    }
    public void setName (String value) {
        String oldValue = name;
        name = value;
        pcs.firePropertyChange (CHARACTER_NAME, oldValue, name);
    }
    public void setStrength (int value) {
       int oldValue = strength;
        strength = value;
        pcs.firePropertyChange (CHARACTER_STRENGTH, oldValue, strength);
    }

    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }

}
