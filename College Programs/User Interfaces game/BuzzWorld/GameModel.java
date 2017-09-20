/*
 * GameModel.java
 *
 * Created on February 3, 2001, 12:50 PM
 */


import java.beans.*;
import javax.swing.*;
import java.io.*;
import java.util.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class GameModel extends Object implements java.io.Serializable {

    public static final String SPACELIST = "Space";
    public static final String CHARACTERLIST = "Character";
    public static final String GAMEMODEL = "Gamemodel";

    private boolean savedFlag;
    private Vector characterList;
    private Vector spaceList;
    private PropertyChangeSupport pcs;
    private String fileName;
    private SpaceModel redStart;
    private SpaceModel blueStart;
    
    /** Creates new GameModel */
    public GameModel(String filename) {
        pcs = new PropertyChangeSupport ( this );
        characterList = new Vector();
        spaceList = new Vector();
        fileName = filename;
        savedFlag = true;
        //addMyListeners();
    }

/**
 * @return  */    
    private void addMyListeners() {
    }

    //Forward changes to listeners for any of the charater of space models
    private void modelPropertyChange(java.beans.PropertyChangeEvent evt) {
        savedFlag = false;
        if (evt.getPropertyName().indexOf(this.CHARACTERLIST) != -1) {
            pcs.firePropertyChange(CHARACTERLIST, evt.getOldValue(), evt.getNewValue());
        }
        if (evt.getPropertyName().indexOf(this.SPACELIST) != -1) {
            pcs.firePropertyChange(SPACELIST, evt.getOldValue(), evt.getNewValue());
        }
    }
    public SpaceModel getRedStart() {
        return redStart;
    }
    public SpaceModel getBlueStart() {
        return blueStart;
    }
    
    public void setRedStart(SpaceModel value) {
        savedFlag = false;
        redStart = value;
        pcs.firePropertyChange(this.SPACELIST, characterList, null);
    }
    
    public void setBlueStart(SpaceModel value) {
        savedFlag = false;
        blueStart = value;
        pcs.firePropertyChange(this.SPACELIST, characterList, null);
    }
    
    public Vector getSpaceList() {
        return spaceList;
    }
    
    public void addCharacter(CharacterModel newCM) {
        savedFlag = false;
        newCM.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        characterList.add(newCM);
        pcs.firePropertyChange(CHARACTERLIST, characterList, null);
    }
    
    public void addSpace(SpaceModel newSM) {
        savedFlag = false;
        newSM.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                modelPropertyChange(evt);
            }
        });
        spaceList.add(newSM);
        pcs.firePropertyChange(SPACELIST, spaceList, newSM);
    }
    
    public Vector getCharacterList() {
        return characterList;
    }
    
    public SpaceModel getSpace(String spaceName) {
        SpaceModel next;
        if (spaceList.isEmpty() == false) {
            Iterator it = spaceList.iterator();
            while (it.hasNext()) {
                next = (SpaceModel) it.next();
                if (next.getName().equals(spaceName)) { 
                    return next;
                }
            }
        }
        return null;
    }    
    
    public SpaceModel getSpace(int index) {
        return (SpaceModel) spaceList.get(index);
    }
    
    public CharacterModel getCharacter(String characterName) {
        CharacterModel next;
        if (characterList.isEmpty() == false) {
            Iterator it = characterList.iterator();
            while (it.hasNext()) {
                next = (CharacterModel) it.next();
                if (next.getName().equals(characterName)) { 
                    return next;
                }
            }
        }
        return null;
    }
    
    public CharacterModel getCharacter(int index) {
        return (CharacterModel) characterList.get(index);
    }
    private void recreateListeners() {
        Iterator it;
        if (spaceList.isEmpty() == false) {
            it = spaceList.iterator();
            while (it.hasNext()) {
                ((SpaceModel) it.next()).addPropertyChangeListener(new java.beans.PropertyChangeListener() {
                         public void propertyChange(java.beans.PropertyChangeEvent evt) {
                            modelPropertyChange(evt);
                        }
                });
            }
        }
        if (characterList.isEmpty() == false) {
            it = characterList.iterator();
            while (it.hasNext()) {
                ((CharacterModel) it.next()).addPropertyChangeListener(new java.beans.PropertyChangeListener() {
                         public void propertyChange(java.beans.PropertyChangeEvent evt) {
                            modelPropertyChange(evt);
                        }
                });
            }
        }
    }
    public static GameModel openGame(String fileName) {
        GameModel test;
        Iterator it;
        try {
			FileInputStream istream = new FileInputStream(fileName);
			ObjectInputStream p = new ObjectInputStream(istream);
			
			test = (GameModel)p.readObject();
			
			istream.close();
                        if (test.getSpaceList().isEmpty() == false) {
                            it = test.getSpaceList().iterator();
                            while (it.hasNext()) {
                                ((SpaceModel) it.next()).initializeListeners();
                            }
                        }
                        test.recreateListeners();
                        return test;
		}
		catch (IOException ex){
		    JOptionPane.showMessageDialog(null, "Could not open file: "+fileName + ex.toString());
		}	
		catch (ClassNotFoundException ex){
                    JOptionPane.showMessageDialog(null, "Could not open file: "+fileName + ex.toString());
                }	
                return null;
    }
    
    public void saveGame() {
        int answer;
        if (savedFlag == false && fileName.equals("") == false) {
            answer = JOptionPane.showConfirmDialog(null, "Do you want to save the game?", "Save Game?", JOptionPane.YES_NO_OPTION);    
            if (answer == JOptionPane.YES_OPTION) {
                try {
                            FileOutputStream ostream = new FileOutputStream(fileName);
                            ObjectOutputStream p = new ObjectOutputStream(ostream);

                            p.writeObject(this);

                            p.flush();
                            ostream.close();
                            savedFlag = true;
                    }
                    catch (IOException ex){
                            JOptionPane.showMessageDialog(null, "Could not save file: "+fileName + ex.toString());
                    }			
            }
        }
    }
    
    public void setChanged() {
        savedFlag = false;
    }
       
    public String getDirectory() {
        int index = fileName.lastIndexOf('\\');
        return fileName.substring(0, index);
    }
    
    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }

}
