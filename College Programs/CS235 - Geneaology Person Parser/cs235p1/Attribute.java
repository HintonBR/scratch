/***************************************************
Authors:     Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

ATTRIBUTE:

Responsibilities:
	Store the quality,source and value of each attribute 
	  read in from a GedCom record
	Print each attribute
	Allow each attribute to be read

Collaborators:
	gEvent
	
	
*******************************************************/



import java.awt.*;
import java.io.*;

public class Attribute{

  public int qual;
  public int source;
  public String value = new String("");
  
  
  public Attribute() {}
  
  public Attribute(Attribute a)
  {
  	qual = a.qual;
  	source = a.source;
  	value = new String(a.value);
  }
    
  public void Add_qual(int a)
  {
  	qual = a;
  }
  
  public void Add_source(int s)
  {
  	source = s;
  }
  
  public void Add_value(String v)
  {
  	value = new String(v);
  }

  public String Get_value() {
        return value;
  }
  
  public void Print_qual()
  {
  	System.out.println("Quality:  " + qual);
  }
  
  public void Print_source()
  {
  	System.out.println("Source:  " + source);
  }
  
  public void Print_value(String v)
  {
  	System.out.println(v + ":  " + value);
  }
  
}	/*  END OF ATTRIBUTE  */
