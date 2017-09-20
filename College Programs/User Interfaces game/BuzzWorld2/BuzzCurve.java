/*
 * BuzzCurve.java
 *
 * Created on March 20, 2001, 9:15 PM
 */

import java.awt.*;
/**
 *
 * @author  Bryan
 * @version 
 */
public class BuzzCurve extends java.lang.Object implements java.io.Serializable {

    public double[] xPoints;
    public double[] yPoints;
    /** Creates new BuzzCurve */
    public BuzzCurve(double[] x, double[] y) {
        xPoints = x;
        yPoints = y;
    }
    
    public Polygon getPolygon() {
         int[] tempX = new int[xPoints.length];
        int[] tempY = new int[yPoints.length];
        int i;
        for (i=0; i<xPoints.length; i++) {
            tempX[i] = (int) xPoints[i];
        }
        for (i=0; i<xPoints.length; i++) {
            tempY[i] = (int) yPoints[i];
        }
        return new Polygon(tempX, tempY, xPoints.length);
    }
    public void changeLocation(int xoffset, int yoffset) {
        for (int i=0; i<xPoints.length; i++) {
            xPoints[i] += xoffset;
            yPoints[i] += yoffset;
        }    
    }
}
