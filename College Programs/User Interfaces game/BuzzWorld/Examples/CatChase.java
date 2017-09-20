import java.awt.*;
import java.awt.event.*;

public class CatChase
	// This program demonstrates basic mouse events. The user can chase the image
	// of a cat around the window by using the mouse.
	//
	// This program uses the Model-View-Controller architecture. The cat
	// model is in the class CatModel. The view of the cat is in the
	// class CatChase_Comp. The controller is also implemented in the
	// CatChase_Comp class. The interface that CatModel uses to inform
	// the view is defined in CatView.
	//
	// The CatChase_Comp class also demonstrates the following techniques
	//		Enabling and receiving mouse movement events
	//		Use of repaint to get the old and new locations of the cat
	//			redrawn.
{
	public static void main(String args[])
	{	System.out.println("Cat Chase");
	
		Frame F = new Frame("Use the mouse to chase the cat");	
		F.setBounds(100,100,300,400);			
		F.addWindowListener(
			new WindowAdapter()					
			{	public void windowClosing(WindowEvent evt)
				{	System.exit(0);	}
			}
			);

		CatModel model = new CatModel();		// Create a new cat model
		CatChase_Comp comp = new CatChase_Comp(model);
												// Create the cat view and
													// give it the model that the
													// view should use 
		F.add(comp); // Add our component to the frame
		F.setVisible(true);
													
	}
}