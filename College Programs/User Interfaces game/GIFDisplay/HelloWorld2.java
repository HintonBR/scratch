import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class HelloWorld2
        // This is a seperate version of "Hello World" that demonstrates the use of
        // images and the use of separate classes to implement components
        // 
        // This class contains only the "main" method that sets up the frame, creates
        // the component and makes it visible. The actual component implementation is
        // in the class "HelloWorld_Comp", which is found in the file 
        // "HelloWorld_Comp.java".
{
        public static void main(String args[])
        {       
            if (args.length > 0) {   
            System.out.println("Project 1 - GIF Display");
        
                JFrame F = new JFrame("Project 1 - GIF Display"); // Create the frame with
                                                       // a title
                F.setBounds(100,100,300,400);           // set the position and size of the
                                                       // frame's window
                F.addWindowListener(
                        new WindowAdapter()             // Setup quiting on close of window
                        {       public void windowClosing(WindowEvent evt)
                                {       System.exit(0); }
                        }
                        );
        
                F.getContentPane().add(new HelloWorldBean(args[0])); // Add our component to the frame
                F.setVisible(true);
            }
            else {
               System.out.println("Must include a parameter specifying the directory to use!");   
            }
                                                                                                        
        }
}

