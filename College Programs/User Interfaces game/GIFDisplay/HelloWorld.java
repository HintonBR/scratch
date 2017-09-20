
import java.awt.*; // get access to all of the AWT classes
import java.awt.event.*; // get access to the classes for monitoring AWT
 // events
import javax.swing.*;// get access to the Swing widgets
public class HelloWorld extends JComponent {
 // This class defines a new custom AWT component that will draw the
 // string "Hello World" centered in the middle of the component and
 // underlined by a single blue line. Resizing this window will move
 // the text around to keep it centered no matter what the size of the
 // component.
 //
 // Because this component only draws static information, it only
 // overrides the "paint" method of Component.
 //
 // This class also provides a static "main" method which is the starting
 // place for this application.

 String msg;
 public HelloWorld()
 { msg = "Hello World";
 }

 public void paint(Graphics g)
 // This method is called each time this component needs
 // to be redrawn. There are many situations which might
 // cause this method to be called. All drawing for this
// component should be applied to the Graphics object g which
// is the display interface. All drawing for this component
// should be done in this method or called from this method.
{
g.setColor(Color.black); // Set the color so that all
// future drawing will use the
// color black. See the Color
// class for information on how
// to create colors
Font timesFont = new Font(
"Times New Roman",// name of the font to be used
Font.BOLD,// constant that indicates that 
// the text for this font should
// be bold. See the Font class
// for other constants to set
// the style of this font
25); // The point size for the font.
g.setFont(timesFont); // Set to font so that all future text
// will be drawn using this font
Point center = new Point(
getWidth()/2,// Half the width of this component
getHeight()/2); // Half the height of this component
FontMetrics fm = g.getFontMetrics();// An object that will tell us 
// sizes of text drawn using the
// current font.
int textHeight= fm.getAscent(); // The distance between the baseline
// of the text and the top of the
// tallest character in this font
int textWidth =
fm.stringWidth(msg); // the number of pixels wide that 
// drawing the string in msg will
// take

int left = center.x-(textWidth/2);
int bottom = center.y+(textHeight/2);

g.drawString(msg,left,bottom);// draw the string at (left,bottom)

g.setColor(new Color(0,0,255));// set the drawing color to the RGB
// value (0,0,255) which is a 
// a bright blue.
g.drawLine(left,bottom+3,
left+textWidth,bottom+3);// Draw the blue line 3 pixels below
// message
}
public static void main(String args[])
// This method is called to start the program.
 {
JFrame F = new JFrame("Demo 1");// Creates a new frame object that
// provides the window in which
 // our component will be displayed

F.setSize(300,200); // Set the width and height of our
 // new window
 F.setLocation(100,100); // Set the location of the window
 // on the screen
 F.addWindowListener( new WindowAdapter()
 { public void windowClosing(WindowEvent evt)
{
System.exit(0);
}// Set up a WindowListener that will
 // exit the program whenever the
 // close box on this frame is clicked
 });
 F.getContentPane().add( new HelloWorld()); // Adds an instance of our component 
 // to the frame
 // as its only component
 F.getContentPane().add( new HelloWorldBean("this is my life"));
F.setVisible(true); // Makes the frame visible by displaying
// the window
}
}

