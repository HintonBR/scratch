import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.util.*;
import javax.swing.*;
import VisualNumerics.math.*;

/**
 * SplineDraw creates a frame for drawing splines.
 * Create a spline curve object by clicking points around the frame, and
 * close the curve by clicking close to the first point.  An arbitrary
 * color is assigned to each object
 *
 * Written by Travis Nielsen
 */
public class CharacterDraw extends JPanel implements java.io.Serializable {
    
    private Polygon newPoly;
    private Point dragStart;
    private int selected;
    private CharacterModel cm;
    private Color curColor;
    private boolean enableEditing;
    private boolean enableSelecting;
    private boolean editable;
    private MouseListener ml;
    private final static int SELECTBOXSIZE = 2;
    
        /** Bezier characteristic matrix */
    public double[][] BZ = new double[][]{
        { -1,  3, -3, 1 },
        {  3, -6,  3, 0 },
        { -3,  3,  0, 0 },
        {  1,  0,  0, 0 }
    };
    
        /** B-Spline characteristic matrix.  Still needs to be divided by 6 for correct conversion.
         *  Not currently used */
    public double[][] BS = new double[][]{
        { -1,  3, -3, 1 },
        {  3, -6,  3, 0 },
        { -3,  0,  3, 0 },
        {  1,  4,  1, 0 }
    };
    
        /** Catmull-Rom characteristic matrix.  Still needs to be divided by 2 for correct conversion. */
    public double[][] CR = new double[][]{
        { -1,  3, -3,  1 },
        {  2, -5,  4, -1 },
        { -1,  0,  1,  0 },
        {  0,  2,  0,  0 }
    };
    public CharacterDraw() {
        curColor = Color.green;
        cm = null;
        setPreferredSize(new Dimension(200, 200));
        setEnableEditing(false);
        setEnableSelecting(false);
        editable = false;
        addMyListeners();
        //Divide all elements of the CR matrix by 2 for correct conversion and BS by 6.
        for (int i=0; i < 4; i++)
            for (int j=0; j < 4; j++){
                CR[i][j] /= 2;
                BS[i][j] /= 6;
            }
    }
    
    public void deleteSelected() {
        if (enableSelecting && selected != -1) {
            cm.removeShape(selected);
            selected = -1;
        }
    }
    public void setEnableEditing(boolean val) {
        enableEditing = val;
        if (val) enableSelecting = false;
        selected = -1;
        eraseCurrentPolygon();
        repaint();
    }
    public void setEnableSelecting(boolean val) {
        enableSelecting = val;
        if (val) enableEditing = false;
        selected = -1;
        eraseCurrentPolygon();
        repaint();
    }
    public boolean getEditable() {
        return editable;
    }
    public void setEditable(boolean val) {
        editable = val;
        if (ml != null) removeMouseListener(ml);
        addMyListeners();
    }
    public boolean getEnableEditing() {
        return enableEditing;
    }
     public boolean getEnableSelecting() {
        return enableSelecting;
    }
    public void eraseCurrentPolygon() {
        newPoly = null;
    }
    public void setCharacterModel(CharacterModel val) {
        cm = val;
        removeMouseListener(ml);
        addMyListeners();
    }
    public CharacterModel getCharacterModel() {
        return cm;
    }
    
    public void setCurColor(Color val) {
        curColor = val;
    }
    public Color getCurColor() {
        return curColor;
    }
    
    private void pressed(java.awt.event.MouseEvent e) {
        //Enter Draw Mode
        if (enableEditing) {
            if (newPoly == null){
                //If you're not currently drawing a polygon, start a new one
                newPoly = new Polygon();
                newPoly.addPoint(e.getX(), e.getY());
            }
            else if (Math.abs(newPoly.xpoints[0] - e.getX()) + Math.abs(newPoly.ypoints[0] - e.getY()) < 15){
                //If you clicked close enough to the start point, complete this polygon and
                // convert it to a GeneralShape spline object
                addShape(newPoly);
                newPoly = null;
            }
            else {
                //Add a new point to the polygon
                newPoly.addPoint(e.getX(), e.getY());
            }
        }
       //Enter Select Mode
       else if (enableSelecting) {
           selected = cm.findShapeAtLocation(e.getX(), e.getY());;
           if (selected != -1) {
                dragStart = e.getPoint();
                this.setCursor(new Cursor(Cursor.MOVE_CURSOR));
            }
        }
        repaint();
    }
    private void released(java.awt.event.MouseEvent evt) {
       this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR)); 
       dragStart = null;
    }
    private void dragged(java.awt.event.MouseEvent evt) {
        if (enableSelecting && selected != -1 && dragStart != null) {
            if (evt.getX() > 0  && evt.getY() > 0 && evt.getX() < 200 && evt.getY() < 200) {
                if (cm.moveShape(selected, (int) (evt.getX()- dragStart.getX()), (int)(evt.getY()-dragStart.getY()))) {
                    dragStart = evt.getPoint();
                    repaint();
                }
            }
            else {
                dragStart = null;
                this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR)); 
            }
        }
    }
    private void addMyListeners() {
        if (editable) {
            addMouseListener(ml = new java.awt.event.MouseAdapter() {
                public void mousePressed(java.awt.event.MouseEvent evt) {
                    pressed(evt);
                }
                public void mouseReleased(java.awt.event.MouseEvent evt) {
                      released(evt);
                  }
            }
            );
            addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
                public void mouseDragged(java.awt.event.MouseEvent evt) {
                    dragged(evt);
                }
            }
            );
        }
        if (cm != null) {
            cm.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
                public void propertyChange(java.beans.PropertyChangeEvent evt) {
                    modelPropertyChange(evt);
                }
            });
        }
    }
    private void modelPropertyChange(java.beans.PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(cm.CHARACTER_IMAGE_DELETED)) {
            if (((Integer) evt.getNewValue()).intValue() == selected) selected = -1;
            repaint();
        }
        if (evt.getPropertyName().equals(cm.CHARACTER_IMAGE)) {
            repaint();
        }
    }
        /**
         * Convert a polygon to a GeneralPath.
         */
    private void addShape(Polygon poly){
        //Require at least 4 points in the polygon
        if (poly.npoints < 4)
            return;
        
        try {
            CC2D path = new CC2D(curColor);
            double[][] inverseBZ = DoubleMatrix.inverse(BZ);
            
            //Create a spline for each segment in the polygon and add it to the GeneralPath
            for (int i=0; i < poly.npoints; i++){
                double[] xPoints = new double[4];
                double[] yPoints = new double[4];
                for (int j=0; j < 4; j++){
                    //Initialize points i-1, i, i+1, i+2
                    int index = (i + j - 1 + poly.npoints) % poly.npoints;
                    xPoints[j] = poly.xpoints[index];
                    yPoints[j] = poly.ypoints[index];
                }
                
                //Convert the Catmull-Rom points to Bezier points (equation from page 273 in the CS456 book)
                // If you want to try B-Splines instead of CR, substitute BS for CR in the next 2 lines
                double[] xResult = DoubleMatrix.multiply(inverseBZ, DoubleMatrix.multiply(CR, xPoints));
                double[] yResult = DoubleMatrix.multiply(inverseBZ, DoubleMatrix.multiply(CR, yPoints));
                
                //Create a CubicCurve and append it to the path
                BuzzCurve curve = new BuzzCurve(xResult, yResult);
                path.addCurve(curve);
            }
            
            //Add the new spline object to the list of shapes
            cm.addShape(path);
        }
        catch (MathException ex){
            ex.printStackTrace();
        }
    }
    
        /**
         * paintComponent overridden to provide custom drawing.
         */
    public void paint(Graphics g){
        super.paint(g);
        if (cm != null) {
            Vector v = cm.getShapes();
            if (v != null) {
                Iterator i1 = v.iterator();
                while (i1.hasNext()) {
                    //Draw each shape
                    CC2D o = (CC2D) i1.next();
                    GeneralPath p = o.getGeneralPath();
                    g.setColor(o.getFillColor());
                    ((Graphics2D)g).fill(p);
                }
                    
                //Draw the new polygon in process
                if (enableEditing) {
                    g.setColor(curColor);
                    if (newPoly != null)
                        for (int i=0; i < newPoly.npoints-1; i++)
                            g.drawLine(newPoly.xpoints[i], newPoly.ypoints[i], newPoly.xpoints[(i+1)%newPoly.npoints], newPoly.ypoints[(i+1)%newPoly.npoints]);
                }
                //Draw selection
                else if (enableSelecting) {
                    if (selected != -1) {
                        CC2D selectShape = (CC2D) cm.getShape(selected);
                        if (selectShape != null) {
                            GeneralPath p = selectShape.getGeneralPath();
                            Rectangle r = p.getBounds();
                            int x, y, width, height;
                            x = (int) r.getX();
                            y = (int) r.getY();
                            width = (int) r.getWidth();
                            height = (int) r.getHeight();
                            g.setColor(Color.cyan);
                            g.drawRect(x-SELECTBOXSIZE, y-SELECTBOXSIZE, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE+(width/2), y-SELECTBOXSIZE, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE+width, y-SELECTBOXSIZE, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE+width, y-SELECTBOXSIZE + (height/2), SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE+width, y-SELECTBOXSIZE+height, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE+(width/2), y-SELECTBOXSIZE+height, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE, y-SELECTBOXSIZE+height, SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                            g.drawRect(x-SELECTBOXSIZE, y-SELECTBOXSIZE+(height/2), SELECTBOXSIZE*2, SELECTBOXSIZE*2);
                        }
                    }
                }
            }
        }     
    }
}