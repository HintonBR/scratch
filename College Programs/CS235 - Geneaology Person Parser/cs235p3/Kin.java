/***************************************************
Authors:     Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

KIN:

Responsibilities:
	Stores Person name and id
	Print each attribute
	Allows each attribute to be read

Collaborators:
	Person	
*******************************************************/

import java.awt.*;
import java.util.*;


public class Kin {
  String name;
  String id;
  int index;
  
  public Kin() 
    {
      /**
        * Kin Class constructor
	*/
      
      name = new String();
      id = new String();
    };
  
  public Kin(Kin k)
    {
      /**
        * Kind Copy constructor that takes object Kin
	* @param k Object of type Kin used in copy constructor
	*/

      name = new String(k.name);
      id = new String(k.id);
      index = 0;
    }

  
  public void Add_id(String i)
    {
      /**
        * Add_id Adds string id i
	* @param i  String which stores the id
	*/

      id = i;
    }
  
  public String Get_id() 
    {
       /**
         * Get_id returns id i
	 */

      return id;
    }
  
  public void Add_name(String n)
    {
      /**
        * Add_name Adds string name n
	* @param n  String which stores the name
	*/

      name = n;	
    }

  public String Get_name() 
    {
       /**
         * Get_name returns String name
	 */

      return name;
    }

  public void Add_index(int i)
    {
      /**
        * Add_index Adds string index i
	* @param i  String which stores the index
	*/

      index = i;
    }

  public int Get_index()
    {
       /**
         * Get_index returns index
	 */

      return index;
    }
  
  public void Print_id()
    {
       /**
         * Print_id Prints id 
	 */

      System.out.println("ID:  " + id);
    }
  
  public void Print_name()
    {
      /**
        * Print_name Prints name 
	*/

      System.out.println("Kin Name:  " + name);
    }
  
  public void Print_index()
    {
      /**
        * Print_index Prints index 
        */

      System.out.println("Index:  " + index);
    }
  
}	/*  END OF KIN  */

