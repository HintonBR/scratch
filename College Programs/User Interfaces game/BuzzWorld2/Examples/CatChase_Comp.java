import java.awt.*;
import java.awt.event.*;

/**
 * This class implements both the view and the controller for the
 * cat chasing application. Pay particular attention to
 * How the model is registered with this class so that the
 * controller code can notify the model about the mouse and
 * so that the model can notify the view of cat movements
 * How repaint is used to notify the windowing system of the
 * portion of the view that must be redrawn
 * How mouse movement events are enabled.
 * How mouse movement events are received.
 */
public class CatChase_Comp extends Component implements CatView {

	/** This contains the loaded image of the cat. */
	Image cat;
	
	/** This is a pointer to the model for the cat. This object should
	 *  be notified every time the mouse moves.
	 */
	CatModel theModel;
	
	/** This constructor has the model that this view should use. */
	public CatChase_Comp(CatModel newModel){
		theModel = newModel;	//Remember the model
		theModel.addCatViewListener(this); //The view is registering itself with
			// the model so that it will inform
			// the view whenever the cat moves.
		cat = getImage("c:\\bryan's files\\programs\\java\\BuzzWorld\\examples\\Cat.gif");		
		enableEvents(AWTEvent.MOUSE_MOTION_EVENT_MASK);
			// This tells Java that this component
			// should receive Mouse Motion events
	}
	
	private Image getImage(String imageName){
		Toolkit TK = Toolkit.getDefaultToolkit(); 	
		Image im = TK.getImage(imageName);			
		MediaTracker track = new MediaTracker(this);
		track.addImage(im,0);		
		try {	track.waitForID(0); }	
		catch(Exception e) {}
		return im;
	}
	
	/**
	 * This method is required by the CatView interface. This is the method
	 * that the model calls whenever the cat moves. The model stores the old
	 * cat location. The new cat location is at (X,Y). 
	 * 
	 * Note that there is no drawing performed in this method. This method 
	 * 		simply informs the windowing system of those parts of the view
	 * 		that must be repainted. All drawing is in the paint method.
	 * 
	 * Note that the location of the cat is at its center. Drawing the cat image,
	 * however, requires the top left position.
	 */
	public void catMoved(int X, int Y){	
		Point oldLoc = theModel.getCatLoc();
		int catWidth = cat.getWidth(this);
		int catHeight = cat.getHeight(this);
		int catX = oldLoc.x-catWidth/2;
		int catY = oldLoc.y-catHeight/2;
		
		repaint(catX,catY,catWidth,catHeight);	// This tells the windowing 
			// system that the old location of the cat must be repainted
			// because the cat has moved.

		catX = X-catWidth/2;
		catY = Y-catHeight/2;
		repaint(catX,catY,catWidth,catHeight);	// This tells the windowing system
				// that the new location of the cat must be repainted.
	}
	
	/** This method draws the cat in its new location. 
	 *  This method will first retrieve the location of the cat from the
	 *  cat model.
	 */
	public void paint(Graphics g) {
		Point catLoc = theModel.getCatLoc();
		g.drawImage(cat,					// image to be drawn
			catLoc.x-cat.getWidth(this)/2,	// left side of the image
			catLoc.y-cat.getHeight(this)/2,	// top of the image
			this); // image observer to be used if this
			// image was not loaded previously.
			// Note that if the image is not already loaded, then the width
			// and height will be wrong. The getImage method makes sure that
			// the image is completely loaded before continuing.
	}
		
	/** This method is the only controller code found in this application. 
	 *  Java AWT will call this method whenever the mouse is moved inside of
	 *  this component. This method is only called, however, if mouse motion
	 *  events have been enabled for this component. See the constructor for
	 *  this class to see how such events are enabled.
	 */
	public void processMouseMotionEvent(MouseEvent evnt) {	
		Point mouseLoc = evnt.getPoint();
		theModel.mousePlace(mouseLoc.x,mouseLoc.y);
	}
	
}