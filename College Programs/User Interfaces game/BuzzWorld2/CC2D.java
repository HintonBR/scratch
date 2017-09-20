/*
 * CC2D.java
 *
 * Created on March 13, 2001, 8:19 PM
 */

import java.awt.*;
import java.awt.geom.*;
import java.util.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class CC2D implements java.io.Serializable {

    private Color fillColor;
    private Vector curves;
    /** Creates new CC2D */
    public CC2D(Color c) {
        
        fillColor = c;
        curves = new Vector();
    }

public void addCurve(Object o) {  
    curves.add(o);
}
public Vector getCurves() {
    return curves;
}

public GeneralPath getGeneralPath() {
    GeneralPath p = new GeneralPath();
    Iterator i2 = curves.iterator();
    while (i2.hasNext()) {
        BuzzCurve b = (BuzzCurve) i2.next();
            CubicCurve2D.Double curve = new CubicCurve2D.Double(
                b.xPoints[0],b.yPoints[0],
                b.xPoints[1],b.yPoints[1],
                b.xPoints[2],b.yPoints[2],
                b.xPoints[3],b.yPoints[3]);
             p.append(curve,true);
    }
    return p;
}
public void setFillColor(Color c) {
    fillColor = c;
}

public Color getFillColor() {
    return fillColor;
}
public boolean changeShapeLocation(int xoffset, int yoffset) {
    GeneralPath p = this.getGeneralPath();
    Rectangle r = p.getBounds();
    r.translate(xoffset, yoffset);
    double x, y, maxX, maxY;
    x = r.getX();
    y = r.getY();
    maxX = r.getX() + r.getWidth();
    maxY = r.getY() + r.getHeight();
    if (x > 0 && y > 0 && maxX <= 200 && maxY <= 200) {
        Iterator i2 = curves.iterator();
        while (i2.hasNext()) {
            BuzzCurve b = (BuzzCurve) i2.next();
            b.changeLocation(xoffset, yoffset);
        }
    }
    else return false;
    return true;
}
}
