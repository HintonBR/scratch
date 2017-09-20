import java.util.*;
import java.awt.*;

public class MultiCatModel extends CatModel
	// This class functions exactly like CatModel except that it will remember
	// multiple views rather than just one. When it does notification, it also
	// notifies all of the views.
{
	Vector views;
	
	public MultiCatModel()
	{	views = new Vector();
	}
	
	public void addCatViewListener(CatView newView)
		// Each new view is added to the list of views.
	{	
		views.add(newView); 
	}
	
	public void catMoved(int newX, int newY)
	{	int n=views.size();
		for (int i=0;i<n;i++)
		{
			CatView V = (CatView) views.get(i);
			V.catMoved(newX,newY);
		}
		catLoc=new Point(newX, newY);
	}
}