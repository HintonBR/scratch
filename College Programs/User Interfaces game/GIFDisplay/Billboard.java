/*
 * Billboard.java
 *
 * Created on January 17, 2001, 9:02 PM
 */


import java.beans.*;
import javax.swing.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class Billboard extends JPanel implements java.io.Serializable {

    private static final String PROP_SAMPLE_PROPERTY = "SampleProperty";

    private String message;
    private int duration;

    private PropertyChangeSupport propertySupport;

    /** Creates new Billboard */
    public Billboard() {
        propertySupport = new PropertyChangeSupport ( this );
    }

    public String getMessage () {
        return message;
    }

    public void setMessage (String value) {
        String oldValue = message;
        message = value;
        propertySupport.firePropertyChange ("message", oldValue, message);
    }

    public int getDuration () {
        return duration;
    }

    public void setDuration (int value) {
        Integer oldValue = new Integer(duration);
        Integer newDuration = new Integer(value);
        duration = value;
        propertySupport.firePropertyChange ("duration", oldValue, newDuration);
    }

    public void addPropertyChangeListener (PropertyChangeListener listener) {
        propertySupport.addPropertyChangeListener (listener);
    }

    public void removePropertyChangeListener (PropertyChangeListener listener) {
        propertySupport.removePropertyChangeListener (listener);
    }

}
