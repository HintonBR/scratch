/********************************************************************
Authors:     Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

FAMILY:

Responsibilities:
	Add each attribute of a GedCom Family Record
	Print each attribute of a Family Record
	Pass attributes to PageBuilder
	
Collaborators:
	Parser
	PageBuilder
*********************************************************************/

import java.awt.*;
import java.util.*;


public class Family extends gEvent{
  String husband;	//Each attribute represents a GedCom Family record
  String wife;		//attribute
  gEvent marriage;
  Vector children;
  String notes;
  char num_children;
  
  public Family() 
    {  
      /**
        *Family Constructor--initialiazes variables
	*/

      husband  = new String();
      wife 	 = new String();
      marriage = new gEvent();
      children = new Vector(10);
      notes	 = new String();
      num_children = 0;
    }
  
  public Family(Family f)
    {
      /**
        *Family Copy Constructor--copies Family object
        *@param f  Family object read into copy constructor
        */

      husband  = f.husband;
      wife 	 = f.wife;
      marriage = f.marriage;
      children = f.children;
      notes	 = f.notes;
      num_children = f.num_children;
    }

  public void Add_husband(String h)
    {
      /**
       *Add_husband Adds a string into the attribute husband
       *@param h  String which contains the husband's name.
       */

      husband = new String(h);
    }
  
  public void Add_wife(String w)
    {
  	
      /**
        *Add_wife Adds a string into the attribute wife
	*@param w  String which contains the wife's name.
	*/

      wife = new String(w);
    }
  
  public void Add_marriage(String p, String d, int q, int s)
    {
      /**
        *Add_marriage Adds strings into the attributes for marriage
	*@param p  String which contains the marriage's place.
	*@param d  String which contains the marriage's date.
	*@param q  int which contains the quality value.
	*@param s  int which contains the source value.
	*/

      marriage.Add_qual(q);
      marriage.Add_source(s);
      marriage.Add_place(p);
      marriage.Add_date(d);
      marriage.Add_event_value("empty");
    }
  
  public void Add_child(String c)
  {
    /**
     *Add_child Adds a string into the attribute child
     *@param c  String which contains the child's name.
     */
    
    children.addElement(c);
  }
  
  
  public String Get_child()
    {
      /**
        *Get_child Returns a child string
	*/

      String temp;
      if (!children.isEmpty())
  	{
	  System.out.println("Child:  " + (String)children.elementAt(0));
	  temp = (String)children.elementAt(0); 
	  children.removeElementAt(0);
	  return temp;
  	} else {
	  return "";
  	}
    }
  
  public String Get_husband()
    {
      /**
        *Get_husband Returns a husband string
	*/

      return husband;
    }
  
  public String Get_wife()
    {
      /**
        *Get_wife Returns a wife string
	*/

      return wife;
    }
  
  public gEvent Get_marriage()
    {
      /**
        *Get_marriage Returns a marriage string
	*/

      return marriage;
    }
  
  public void Add_notes(String n)
    {
      /**
        *Add_notes Adds a string into the attribute notes
	*@param n  String which contains the notes.
	*/

      notes = new String(n);
    }
  
  public void Add_num_children(char n)
    {
      /**
        *Add_num_children Adds a string into the attribute num_of_child
	*@param n  String which contains the number of children.
	*/

      num_children = n;
    }

  public void Print_husband()
    {
      /**
        *Print_husband Prints the husband string
	*/

      System.out.println("Husband:  " + husband);
    }
  
  public void Print_wife()
    {
  	
      /**
        *Print_wife Prints the wife string
	*/
      System.out.println("Wife:  " + wife);
    }
  
  public void Print_marriage()
    {
      /**
        *Print_marriage Prints the marriage string
	*/

      System.out.println("Marriage:\n");
      marriage.Print_place();
      marriage.Print_date();
      marriage.Print_qual();
      marriage.Print_source();
  
    }

  public void Print_children()
    {
      /**
        *Print_children Prints the children string
	*/

      for (int i = 0; i < children.size(); i++)
  	{	System.out.println("Child:  " + (String) children.elementAt(i));
	children.elementAt(i);
  	}
    }
  
  public void Print_notes()
    {
      /**
        *Print_notes Prints the notes string
	*/

      System.out.println("Notes:  " + notes);
    }
  
  public void Print_num_children()
    {
      /**
        *Print_num_children Prints the num_children string
	*/

      System.out.println("Number of Children:  " + num_children);
    }
  
  public void Print_Family()  
    {     
      /**
        *Print_Family Prints the family string
	*/

      Print_husband();
      Print_wife();
      Print_marriage();
      Print_children();
      Print_notes();
      Print_num_children();
    }
  
}
