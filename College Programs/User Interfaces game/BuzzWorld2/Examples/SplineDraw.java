package Examples;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.util.Vector;
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
public class SplineDraw extends JComponent {
	Polygon newPoly;
	Vector shapes;
	
	/** SplineDraw constructor */
	SplineDraw(){
		shapes = new Vector();
		setPreferredSize(new Dimension(500, 500));
		addMouseListener(new MouseAdapter(){
			public void mousePressed(MouseEvent e){
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
				repaint();
			}
		});
		
		//Divide all elements of the CR matrix by 2 for correct conversion and BS by 6.
		for (int i=0; i < 4; i++)
			for (int j=0; j < 4; j++){
				CR[i][j] /= 2;
				BS[i][j] /= 6;
			}
	}
	
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
	
	/**
	 * Convert a polygon to a GeneralPath.  
	 */
	private void addShape(Polygon poly){
		//Require at least 4 points in the polygon
		if (poly.npoints < 4)
			return;
			
		try {
			GeneralPath path = new GeneralPath();
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
				CubicCurve2D.Double curve = new CubicCurve2D.Double(
					xResult[0], yResult[0], 
					xResult[1], yResult[1], 
					xResult[2], yResult[2],
					xResult[3], yResult[3]
				);
				
				path.append(curve, true);
			}
			
			//Add the new spline object to the list of shapes
			shapes.add(path);
		}
		catch (MathException ex){
			ex.printStackTrace();
		}
	}
	
	/**
	 * paintComponent overridden to provide custom drawing.
	 */
	public void paintComponent(Graphics g){
		//Arbitrary colors for each spline object
		Color[] colors = new Color[]{
			Color.black, Color.red, Color.blue, Color.yellow, Color.green, Color.white
		};
		
		//Draw each shape
		for (int i=0; i < shapes.size(); i++){
			g.setColor(colors[i % colors.length]);
			Shape s = (Shape)shapes.get(i);
			//The graphics object is actually a subclass of Graphics2D, so we can cast it
			((Graphics2D)g).fill(s);
		}
		
		//Draw the new polygon in process
		g.setColor(colors[shapes.size() % colors.length]);
		if (newPoly != null)
			for (int i=0; i < newPoly.npoints-1; i++)
				g.drawLine(newPoly.xpoints[i], newPoly.ypoints[i], newPoly.xpoints[(i+1)%newPoly.npoints], newPoly.ypoints[(i+1)%newPoly.npoints]);
	}

	/**
	 * Main method for initialization of the program
	 */
	public static void main(String[] args){
		
		JFrame test = new JFrame("Spline Test");
		test.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e){
				System.exit(0);
			}
		});
		test.getContentPane().add(new SplineDraw());
		test.pack();
		test.setVisible(true);
	}
}