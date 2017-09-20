import java.awt.*;
import java.lang.Math;

public class CatModel
	// This is the model for managing the position of the cat.
	// It is notified by the controller whenever the mouse moves.
	// It then checks to see if the mouse is too close to the cat. If
	// the mouse is too close then it moves the location of the cat to
	// get away. If the mouse moves, this class will notify the view
	// that the cat has moved so that the view can be updated.
{
	Point catLoc;
		// This saves the current location of the cat.
		
	CatView theView;
		// This is the view that must be notified whenever the cat has
		// moved. This model does not know anything more about the
		// view than the interface defined by CatView.
	
	static final double FEAR_DISTANCE = 35.0;
		// When the mouse is within this distance the cat gets scared
		// and jumps away.
	
	public CatModel()
	{	catLoc = new Point(100,100);
		theView = null;
	}
	

// This is the method that the controller uses to notify the model
// when the mouse moves.
	
	public void mousePlace(int mouseX, int mouseY)
		// This is called by the controller whenever the mouse moves. This
		// checks to see if the mouse is too close to the cat and if 
		// necessary it moves the cat.
	{	
		double dx = catLoc.x-mouseX;
		double dy = catLoc.y-mouseY;
		double dist = Math.sqrt(dx*dx+dy*dy);	// compute the distance
													// between the mouse and
													// the cat
		if (dist>FEAR_DISTANCE)
		{	return; }							// if the mouse far away then
													// do nothing
		int newX = (int)(catLoc.x+dx*FEAR_DISTANCE/dist);
		int newY = (int)(catLoc.y+dy*FEAR_DISTANCE/dist);
		if (newX<FEAR_DISTANCE)
		{	newX=100; }
		if (newY<FEAR_DISTANCE)
		{	newY=100; }
		catMoved(newX,newY);					// notify the view that the
													// cat has moved. 
	}

//	The following methods handle the registration and notification
//	of the view when the cat moves
//
	public void addCatViewListener(CatView newView)
		// This method is used to register a view with this model
		// so that the view can be notified whenever the cat moves.
	{	theView = newView; 
	}

	public void catMoved(int newX, int newY)
		// This will notify the view that the cat has moved.
		// The view is notified before the location is changed so that
		// the view can find both the old and the new location of the
		// cat
	{	if (theView != null)
		{ 	theView.catMoved(newX,newY); }
		catLoc = new Point(newX,newY);
	}
	
	public Point getCatLoc()
	{	return catLoc; }
}