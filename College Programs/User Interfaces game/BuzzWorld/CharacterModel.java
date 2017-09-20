/*
 * CharacterModel.java
 *
 * Created on February 3, 2001, 1:02 PM
 */


import java.beans.*;
import java.awt.*;
import javax.swing.*;
/**
 *
 * @author  Bryan
 * @version Bxfbsdfgsdfgsdfgsdfgsdfgxcvbdfgfgdsfgdfgdfsdfgsdfg
 */
public class CharacterModel extends Object implements java.io.Serializable {

    public static final String CHARACTER_NAME = "CharacterName";
    public static final String CHARACTER_STRENGTH = "CharacterStrength";
    public static final String CHARACTER_IMAGE = "CharacterImage";
    
    private String name = "";
    private int strength = 0;
    private String imageFilename = "";
    private PropertyChangeSupport pcs = null;

    /** Creates new CharacterModel */
    public CharacterModel() {
        pcs = new PropertyChangeSupport ( this );
    }

    public String getName () {
        return name;
    }
    public int getStrength () {
        return strength;
    }
    
    public String getImageFilename () {
        return imageFilename;
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
    public void setImage (String value) {
        String oldValue = imageFilename;
        imageFilename = value;
        pcs.firePropertyChange (CHARACTER_IMAGE, oldValue, imageFilename);
    }

    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }

}
