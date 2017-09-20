/*
 * DragList.java
 *
 * Created on February 17, 2001, 8:17 AM
 */
import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.dnd.*;
import javax.swing.*;

/**
 *
 * @author  Bryan
 * @version 
 */
public class DragList extends javax.swing.JList implements java.awt.dnd.DragSourceListener, java.awt.dnd.DragGestureListener {

    /** Creates new DragList */
    public DragList() {
        DragSource dragSource = DragSource.getDefaultDragSource();

		dragSource.createDefaultDragGestureRecognizer(
					this, // component where drag originates
					DnDConstants.ACTION_COPY_OR_MOVE, // actions
					this); // drag gesture recognizer

    }

    public void dragGestureRecognized(java.awt.dnd.DragGestureEvent e) {
        // drag anything ...
		e.startDrag(DragSource.DefaultCopyDrop, // cursor
			new StringSelection(new String(Integer.toString(this.getSelectedIndex()))), // transferable
			this);  // drag source listener
    }
    
    public void dragDropEnd(java.awt.dnd.DragSourceDropEvent dragSourceDropEvent) {}
    public void dragEnter(java.awt.dnd.DragSourceDragEvent dragSourceDragEvent) {}
    public void dragExit(java.awt.dnd.DragSourceEvent dragSourceEvent) {}
    public void dragOver(java.awt.dnd.DragSourceDragEvent dragSourceDragEvent) {}
    public void dropActionChanged(java.awt.dnd.DragSourceDragEvent dragSourceDragEvent) {}
}
