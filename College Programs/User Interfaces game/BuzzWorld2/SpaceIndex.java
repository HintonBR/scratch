/*
 * SpaceIndex.java
 *
 * Created on February 19, 2001, 11:20 PM
 */

import java.util.*;
/**
 *
 * @author  Bryan
 * @version 
 *Class used to create a quick index by space of which characters are in
 *what space - primarily used by views like the play view which render 
 *characters in their current view
 */
public class SpaceIndex extends java.lang.Object {

    private Vector spaceList;
    private Vector characters;
    /** Creates new SpaceIndex */
    public SpaceIndex(Vector spaces) {
        Iterator it;
        characters = new Vector();
        spaceList = spaces;
        it = spaceList.iterator();
        while (it.hasNext()) {
            characters.add(new Vector());
            it.next();
        }
    }
    public void moveCharacter(SpaceModel newLocation, SpaceModel oldLocation, CharacterInstance ci) {
        int i;
        Vector v;
        i = spaceList.indexOf(oldLocation);
        v = (Vector) characters.get(i);
        v.remove(ci);
        i = spaceList.indexOf(newLocation);
        v = (Vector) characters.get(i);
        v.add(ci);
    }
    public void setCharacter(SpaceModel location, CharacterInstance ci) {
        int i;
        Vector v;
        i = spaceList.indexOf(location);
        v = (Vector) characters.get(i);
        v.add(ci);
    }

    public Vector getCharacterVector(SpaceModel location) {
        int i;
        Vector v;
        i = spaceList.indexOf(location);
        v = (Vector) characters.get(i);
        return v; 
    }
    public void addSpaceToIndex(SpaceModel sm) {
        spaceList.add(sm);
        characters.add(new Vector());
    }
}
