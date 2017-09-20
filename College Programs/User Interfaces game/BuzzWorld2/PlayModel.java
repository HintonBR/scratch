/*
 * PlayModel.java
 *
 * Created on February 3, 2001, 1:04 PM
 */


import java.beans.*;
import java.util.*;
import java.lang.*;
import java.awt.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class PlayModel extends Object implements java.io.Serializable {

    public static final int PLAYMODEL_BLUE_TEAM = 1;
    public static final int PLAYMODEL_RED_TEAM= 2;
    public static final String PLAYMODEL_PROPERTY = "playproperty";
    public static final String PLAYMODEL_CHARACTER_INSTANCE_MOVED = "CharacterInstanceMoved";
    public static final String PLAYMODEL_CHARACTER_MODEL_CHANGED = CharacterInstance.CI_CHARACTERMODEL;
    public static final String PLAYMODEL_CHARACTER_INSTANCE_CURRENT_STRENGTH_CHANGED = CharacterInstance.CI_CURRENTSTRENGTH;
    public static final String PLAYMODEL_CHARACTERSPACECHANGED = CharacterInstance.CI_SPACEMODELCHANGED;
    public static final String PLAYMODEL_CHARACTERSPACEMODELPROPERTYCHANGED = CharacterInstance.CI_SPACEMODELPROPERTYCHANGED;
    
    private Vector blueCharacters;
    private Vector redCharacters;
    private PropertyChangeSupport pcs;
    private GameModel gm;
    private SpaceIndex spaceIndex;
    /** Creates new PlayModel */
    public PlayModel(GameModel game) {
        pcs = new PropertyChangeSupport ( this );
        gm = game;
        blueCharacters = new Vector();
        redCharacters = new Vector();
        spaceIndex = new SpaceIndex(new Vector(gm.getSpaceList()));
        InstantiateCharacters(game);
        addMyListeners();
    }

      public  void InstantiateCharacters(GameModel g) {
        CharacterInstance ci;
        CharacterModel cm;
        Iterator it = g.getCharacterList().iterator();
        Point p;
        while (it.hasNext()) {
            cm = (CharacterModel) it.next();
            ci = new CharacterInstance(cm, gm.getRedStart(), CharacterInstance.CI_REDTEAM, new Point(0,0));
           randomizeLocation(ci, gm.getRedStart());
            redCharacters.add(ci);
            spaceIndex.setCharacter(gm.getRedStart(), ci);
            ci.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
                public void propertyChange(java.beans.PropertyChangeEvent evt) {
                    playModelPropertyChange(evt);
                }
            });
           ci = new CharacterInstance(cm, gm.getBlueStart(),CharacterInstance.CI_BLUETEAM, new Point(0,0));
           randomizeLocation(ci, gm.getBlueStart());
           blueCharacters.add(ci);
           spaceIndex.setCharacter(gm.getBlueStart(), ci);
                      ci.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                playModelPropertyChange(evt);
           }
           });

        }
    }
    private void randomizeLocation(CharacterInstance c, SpaceModel p) {
        double rand;
        int rw, rh;
        int numtimes = 0;
        Dimension d = p.getImageSize();
        if (d != null) {
            rw = (int)d.getWidth();
            rh = (int) d.getHeight();
        }
        else {
            rw = 100;
            rh = 100;
        }
        rand = Math.random();
        c.setLocation(new Point((int)(rand*rw), (int) (rand*rh)), false);
        Object o = getObjectAt(c, p);
        Rectangle size = c.getRectangle(c.getX(), c.getY());
        while (numtimes < 10 && (o instanceof CharacterInstance || c.getX() <0 || c.getY() < 0 || c.getX() + size.getSize().getWidth() > p.getImageSize().getWidth() || c.getY() + size.getSize().getHeight() > p.getImageSize().getHeight())) { 
            rand = Math.random();
            c.setLocation(new Point((int)(rand*rw), (int) (rand*rh)), false);
            o = getObjectAt(c, p);
            numtimes++;
        }
        if (numtimes == 10) {
            Rectangle r = p.getEntry().getRectangle();
            Point mid = new Point((int)(r.getX() + r.getWidth()/2), (int)(r.getY() + r.getHeight()/2));
             c.setLocation((int) (mid.getX() - size.getSize().getWidth()/2), (int) (mid.getY() - size.getSize().getHeight()/2), false);
        }
    }
    public Object getObjectAt(CharacterInstance c, SpaceModel sp) {
        CharacterInstance ci;
        Rectangle source = c.getRectangle(c.getX(),c.getY());
        Portal p;
        Rectangle destination;
        Vector chars = new Vector(spaceIndex.getCharacterVector(sp));
        Iterator it = chars.iterator();
        while (it.hasNext()) {
            ci = (CharacterInstance) it.next();
            destination = ci.getRectangle(ci.getX(),ci.getY());
            if (destination.intersects(source) && c != ci && ci.isAlive()) {
                return ci;
            }
        }
        chars = new Vector(sp.getPortalList());
         it = chars.iterator();
        while (it.hasNext()) {
            p = (Portal) it.next();
            destination = p.getRectangle();
            if (destination.intersects(source)) {
                return p;
            }
        }
         return null;
    }
    public Object getObjectAt(Rectangle source, SpaceModel sp) {
        CharacterInstance ci;
        Portal p;
        Rectangle destination;
        Vector chars = new Vector(spaceIndex.getCharacterVector(sp));
        Iterator it = chars.iterator();
        while (it.hasNext()) {
            ci = (CharacterInstance) it.next();
            destination = ci.getRectangle(ci.getX(),ci.getY());
            if (destination.intersects(source) && ci.isAlive()) {
                return ci;
            }
        }
        chars = new Vector(sp.getPortalList());
         it = chars.iterator();
        while (it.hasNext()) {
            p = (Portal) it.next();
            destination = p.getRectangle();
            if (destination.intersects(source)) {
                return p;
            }
        }
         return null;
    }
    public Vector getCharacters(int teamid) {
        if (teamid == this.PLAYMODEL_BLUE_TEAM) return blueCharacters;
        else return redCharacters;
    }
    public Vector getCharacters(SpaceModel sm) {
        return new Vector(spaceIndex.getCharacterVector(sm));
    }
    public Vector getBlueCharacters () {
        return blueCharacters;
    }
    public Vector getRedCharacters () {
        return redCharacters;
    }
    public String getTeamName(int teamid) {
        if (teamid == this.PLAYMODEL_BLUE_TEAM) return "Blue";
        else return "Red";
    }
    
    private void addMyListeners() {
        gm.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                playModelPropertyChange(evt);
            }
        });
    }

    private void playModelPropertyChange(java.beans.PropertyChangeEvent evt) {
        
        if (evt.getPropertyName().equals(CharacterInstance.CI_CHARACTERMODEL) || evt.getPropertyName().equals(CharacterInstance.CI_CURRENTSTRENGTH)) { 
            pcs.firePropertyChange(evt);
        }
        else if (evt.getPropertyName().equals(CharacterInstance.CI_LOCATION)) {
            CharacterInstance ci = (CharacterInstance) evt.getSource();
             CharacterInstance c;
            Point oldLoc = (Point) evt.getOldValue();
            Point newLoc = (Point) evt.getNewValue();
            
            //Find out if there is a object at the location we are trying to move to
            Object o = getObjectAt(ci, ci.getSpaceModel());
            
            //If the object is a Character then attack or move back
            if (o instanceof CharacterInstance) {
                c = (CharacterInstance) o;
                    if (c.getTeam() != ci.getTeam()) {
                        ci.attack(c);
                        ci.setLocation(oldLoc, false);
                    }
                    else {
                        ci.setLocation(oldLoc);    
                    }
            }
            //If object is a portal - check to see if we can teleport
            else if (o instanceof Portal) {
                Portal p = (Portal) o;
                
                //Find size of rectangle to determine if overlap will exist in the 
                //Space that we are going to
                Rectangle r = p.getSpaceModel().getEntry().getRectangle();
                Point mid = new Point((int)(r.getX() + r.getWidth()/2), (int)(r.getY() + r.getHeight()/2));
                Rectangle bound = ci.getRectangle(0,0);
                //Find out if there is an object that overlaps us in the space we are trying to 
                //teleport to.
                o = getObjectAt(new Rectangle((int) (mid.getX() - bound.getSize().getWidth()/2),(int) (mid.getY() - bound.getSize().getHeight()/2),(int)bound.getSize().getWidth(),(int)bound.getSize().getHeight()), p.getSpaceModel());
                if (o instanceof CharacterInstance) {
                     c = (CharacterInstance) o;
                    if (c.getTeam() != ci.getTeam()) {
                        ci.setRotAngle(c.getRotAngle());
                        ci.attack(c);
                        ci.setLocation(oldLoc, false);
                    }
                    else {
                        ci.setLocation(oldLoc);    
                    }
                }
                else { 
                    ci.teleport(p.getSpaceModel());
                    mid.setLocation((int) (mid.getX() - bound.getSize().getWidth()/2),(int) (mid.getY() - bound.getSize().getHeight()/2));
                    ci.setLocation(mid, false);
                }
            }
            pcs.firePropertyChange(this.PLAYMODEL_CHARACTER_INSTANCE_MOVED, evt.getOldValue(), evt.getNewValue());
        }
        else if (evt.getPropertyName().equals(CharacterInstance.CI_SPACEMODELCHANGED)) {  
            spaceIndex.moveCharacter((SpaceModel) evt.getNewValue(), (SpaceModel) evt.getOldValue(), (CharacterInstance) evt.getSource());
            pcs.firePropertyChange(evt);
        }
        else if (evt.getPropertyName().equals(CharacterInstance.CI_SPACEMODELPROPERTYCHANGED)) {
            pcs.firePropertyChange(evt);
        }
        else if (evt.getPropertyName().equals(gm.SPACELIST)) {
            if (evt.getNewValue() instanceof SpaceModel) {
                spaceIndex.addSpaceToIndex((SpaceModel) evt.getNewValue());
            }
            pcs.firePropertyChange(evt);
        }
    }

    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }

}
