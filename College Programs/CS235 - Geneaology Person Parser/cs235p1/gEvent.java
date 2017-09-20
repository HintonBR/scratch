/***************************************************
Authors:     Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

gEVENT:

Responsibilities:
	Stores and extends Attribute
	Adds Date, Place and a Value to a Person Record
	Print each attribute
	Allows each attribute to be read

Collaborators:
	Attribute
	Person	
*******************************************************/

import java.awt.*;
import java.util.*;

public class gEvent extends Attribute{
  String place;
  String date;
  String event_value;
  
public gEvent() 
  {
    /**
      * gEvent class constructor
      */

    place = new String();
    date = new String();
    event_value = new String();
  };
  
  public gEvent(gEvent e)
  {
    /**
      * gEvent class constructor which alternately may accept a parameter
      *        which is of type gEvent
      * @param e  An object of type gEvent used to construct a new object
      */
  
    date = new String(e.date);
    place = new String(e.place);
    event_value = new String(e.event_value); 
  }
  
  public void Add_place(String p)
  {
    /**
      * Add_place reads in a string for the attribute place
      * @param p  String which is read in as new place value
      */

    place = new String(p);
  }

  public String Get_place() 
    {
      /**
        * Get_place Returns the value of place.
	*/

      return place;
  }
  
  public void Add_date(String d)
  {
    /**
      * Add_date reads in a string for the attribute date
      * @param d  String which is read in as new date value
      */

    date = new String(d);	
  }

  public String Get_date() 
    {
      /**
        * Get_date Returns the value of date.
	*/
      
      return date;
  }

  public void Add_event_value(String v)
  {
    /**
      * Add_event_value Reads in a string for the attribute event_value
      * @param v  String which is read in as new event_value value
      */

    event_value = new String(v);	
  }

  public String Get_event_value() 
    {
      /**
        * Get_event_value Returns the value of event_value.
	*/

      return event_value;
  }
  
  public void Print_place()
  {
    /**
      * Print_place Prints the value of place.
      */

    System.out.println("Place:  " + place);
  }
  
  public void Print_date()
  {
    /**
      * Print_date Prints the value of date.
      */
    
    System.out.println("Date:  " + date);
  }
  
  public void Print_event_value(String v)
  {
    /**
      * Print_event_value Prints the value of event_value.
      */

    System.out.println(v + ":  " + event_value);
  }

}	/************  END OF GEVENT  ******************/
