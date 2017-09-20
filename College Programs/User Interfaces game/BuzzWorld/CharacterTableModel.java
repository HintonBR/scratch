/*
 * CharacterTableModel.java
 *
 * Created on February 19, 2001, 8:12 PM
 */

import java.util.*;
import java.beans.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class CharacterTableModel extends javax.swing.table.AbstractTableModel implements java.io.Serializable {

    public static final String CHARACTER_TABLE_MODEL  = "CHARACTERTABLEMODEL";
    Vector data;
     private PropertyChangeSupport pcs;
    /** Creates new CharacterTableModel */
    public CharacterTableModel(Vector characters) {
        data = characters;
        pcs = new PropertyChangeSupport(this);
    }
  
    public java.lang.Class getColumnClass(int param) {
        if (param == 1) return Integer.class;
        return String.class;
    }
    
    public int getColumnCount() {
        return 3;
    }
    
    public String getColumnName(int param) {
        switch (param) {
            case 0: return "Name";
            case 1: return "Strength";
            case 2: return "Current Space";
        }
        return "";
    }
    
    public int getRowCount() {
        if (data != null) return data.size();
        else return 0;
    }
    
    public java.lang.Object getValueAt(int row,int column) {
        CharacterInstance cm = (CharacterInstance) data.get(row);
        switch (column) {
            case 0: return cm.getCharacterModel().getName();
            case 1: return String.valueOf(cm.getCurrentStrength());
            case 2: return cm.getSpaceModel();
        }
        return "";
    }
    
    public boolean isCellEditable(int param,int param1) {
        return false;
    }
    
    public void setValueAt(Object val,int row,int col) {
        CharacterInstance cm = (CharacterInstance) data.get(row);
        try {
            switch (col) {
                case 0:  cm.getCharacterModel().setName((String) val);
                         break;
                case 1: cm.setCurrentStrength(Integer.parseInt((String) val));
                        break;
                case 2: cm.setSpaceModel((SpaceModel) val);
                        break;
            }
        }
        catch (Exception e) {}
        pcs.firePropertyChange(CharacterTableModel.CHARACTER_TABLE_MODEL, null, null);
    }
    public void changeVector(Vector val) {
        data = val;
        pcs.firePropertyChange(CharacterTableModel.CHARACTER_TABLE_MODEL, null, null);
    }
    public void addPropertyChangeListener (PropertyChangeListener listener) {
        pcs.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        pcs.removePropertyChangeListener (listener);
    }

}
