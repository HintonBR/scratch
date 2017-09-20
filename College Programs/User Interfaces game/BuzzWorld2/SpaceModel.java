/*
 * SpaceModel.java
 *
 * Created on February 3, 2001, 1:03 PM
 */


import java.beans.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import java.awt.image.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class SpaceModel extends Object implements java.io.Serializable {

    public static final String SPACE_NAME = "SpaceName";
    public static final String SPACE_IMAGE = "SpaceImage";   
    public static final String SPACE_ADDED_PORTAL = "AddedPortal"; 
    public static final String SPACE_ENTRY = "ENTRY"; 
    public static final String SPACE_DELETED_PORTAL = Portal.PORTAL_DELETE;
    
    private String name;
    private String imageFilename;
    private Vector portalList;
    private PropertyChangeSupport pcs;
    private Portal entry;
    private Dimension d;
    
    /** Creates new CharacterModel */
    public SpaceModel() {
        pcs = new PropertyChangeSupport ( this );
        portalList = new Vector();
        name = "";
        imageFilename = "";
        entry = new Portal(new Dimension(40,40),new Point(0,0),this,SpaceModel.SPACE_ENTRY);
        d = new Dimension(60, 60);
        addMyListeners();
    }
     public SpaceModel(String nam) {
        pcs = new PropertyChangeSupport ( this );
        portalList = new Vector();
        name = nam;
        imageFilename = "";
        entry = new Portal(new Dimension(40,40),new Point(0,0),this,SpaceModel.SPACE_ENTRY);
        d = new Dimension(60, 60);
        addMyListeners();
    }
    public void initializeListeners() {
        addMyListeners();
        initializePortalListeners();
    }
     private void initializePortalListeners() {     
        Iterator it;
        if (entry != null) entry.addMyListeners();
        if (portalList.isEmpty() == false) {
            it = portalList.iterator();
            while (it.hasNext()) {
                ((Portal) it.next()).addMyListeners();
            }
        }
    }
    public void addMyListeners() {
        if (entry != null) {
            entry.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        }
        
        Iterator it;
        if (portalList.isEmpty() == false) {
            it = portalList.iterator();
            while (it.hasNext()) {
                ((Portal) it.next()).addPropertyChangeListener(new java.beans.PropertyChangeListener() {
                     public void propertyChange(java.beans.PropertyChangeEvent evt) {
                        modelPropertyChange(evt);
                    }
                });
            }
        }
        
    }    
    public Vector getPortalList() {
        return portalList;
    }
    public void addPortal(SpaceModel sm) {
        Portal p = new Portal(null, null, sm);
        portalList.add(p);
        p.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        pcs.firePropertyChange(SPACE_ADDED_PORTAL, null, p);
    }
    public void addPortal(Portal p) {
        Vector oldValue = portalList;
        portalList.add(p);
        p.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        pcs.firePropertyChange(SPACE_ADDED_PORTAL, null, p);
    }
   
    private void modelPropertyChange(java.beans.PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(Portal.PORTAL_DELETE)) {
            portalList.remove(evt.getSource());
            pcs.firePropertyChange(evt);
        }
        else {
            pcs.firePropertyChange(evt);
        }
    }
    public String getName () {
        return name;
    }
   
    public String getImageFilename () {
        return imageFilename;
    }
    public Dimension getImageSize() {
        return d;
    }
    public Portal getEntry() {
        return entry;
    }
    public void setName (String value) {
        String oldValue = name;
        name = value;
        pcs.firePropertyChange (SPACE_NAME, oldValue, name);
    }
    public String toString() {
        return name;
    }

    public void setImageFilename (String value) {
        Image newImage;
        JComponent obs = new JPanel();
        if (value.equals(imageFilename) == false) {
            String oldValue = imageFilename;
            imageFilename = value;
            
            newImage = getImage(obs);
            if (newImage != null) {
                entry.setSize(new Dimension(60,60));
                d = new Dimension(newImage.getWidth(obs), newImage.getHeight(obs));
                entry.setLocation(newImage.getWidth(obs)/2 - 30,newImage.getHeight(obs)/2 - 30);
            }
            else {
                entry.setSize(new Dimension(40, 40));
                d = new Dimension(60, 60);
                imageFilename = "";
                entry.setLocation(0,0);
            }
            while (portalList.isEmpty() == false) {
                    pcs.firePropertyChange(SPACE_DELETED_PORTAL, null, portalList.lastElement());
                    portalList.remove(portalList.lastElement());
            }
             
                //Reset entry rectangle
                pcs.firePropertyChange (SPACE_IMAGE, oldValue, imageFilename);
        }
    }
    private Image getImage(JComponent obs) {
            Image im = null;
            try {
		if (imageFilename.equals("")) return null;
		Toolkit TK = Toolkit.getDefaultToolkit(); 	
		im = TK.getImage(imageFilename);			
                if (im == null) return null;
		MediaTracker track = new MediaTracker(obs);
		track.addImage(im,0);		
	        track.waitForID(0); 
            }	
		catch(Exception e) {return null;}
		return im;
    }

    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }
    
}
