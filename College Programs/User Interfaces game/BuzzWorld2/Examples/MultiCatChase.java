import java.awt.*;
import java.awt.event.*;

public class MultiCatChase
	// This program demonstrates multiple views of the same model. This
	// programs is the same as CatChase except that multiple windows are
	// created that share a model.
	
{
	private static Frame makeCatWindow(String windowName, CatModel model, int X, int Y)
	{
		Frame F = new Frame(windowName);
		F.setBounds(X,Y,300,300);
		F.addWindowListener(
			new WindowAdapter()
			{	public void windowClosing(WindowEvent evt)
				{	System.exit(0); }
			}
			);
		CatChase_Comp comp = new CatChase_Comp(model);
		F.add(comp);
		F.setVisible(true);
		return F;
	}
	
	public static void main(String args[])
	{				
		CatModel ModelA = new MultiCatModel();
		Frame View1ModelA = makeCatWindow("View 1 Model A",ModelA, 100,100);
		Frame View2ModelA = makeCatWindow("View 2 Model A",ModelA, 450,100);
		CatModel ModelB = new MultiCatModel();
		Frame View3ModelB = makeCatWindow("View 3 Model B",ModelB, 200,450);
		System.out.println("Multi Cat Chase");
		
	}
}