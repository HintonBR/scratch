/***************************************************
Authors:     Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

PERSON:

Responsibilities:
	Add each attribute in a GedCom individual record to Person
	Print each attribute
	Allow each attribute to be read

Collaborators:
	gEvent
	Attribute
	
*******************************************************/


import java.awt.*;
import java.util.*;


public class Person extends gEvent{
  char sex;		//Individual attributes found in a GedCom file
  String gedid;
  String named_after;
  String occupation;
  Attribute name;
  Attribute nation;
  Attribute title;
  gEvent baptism;
  gEvent birth;
  gEvent death;
  gEvent burial;
  gEvent christened;
  gEvent grad_info;
  gEvent marriage;
  Kin spouse;
  Kin father;
  Kin mother;
  Vector children;  
  static int child_index = 0;
  
  public Person() 
  {
    /**
      * Person Class constructor
      */
  
    gedid = new String();
    name = new Attribute();
    named_after = new String();
    occupation = new String();
    nation = new Attribute();
    title = new Attribute();
    baptism = new gEvent();
    birth = new gEvent();
    death = new gEvent();
    burial = new gEvent();
    christened = new gEvent();
    grad_info = new gEvent();
    marriage = new gEvent();
    spouse = new Kin();
    father = new Kin();
    mother = new Kin();
    children = new Vector(1);	
  };
  
  public Person(Person p)
  {
    /**
      * Person Class copy constructor with additional parameter p
      * @param p  Object of type Person used in copy constructor
      */
 
    sex = p.sex;
    named_after = new String(p.named_after); 
    occupation = new String(p.occupation); 
    gedid = new String(p.gedid);
    name = new Attribute(p.name);
    nation = new Attribute(p.nation);
    title = new Attribute(p.title);
    baptism = new gEvent(p.baptism);
    birth = new gEvent(p.birth);
    death = new gEvent(p.death);
    burial = new gEvent(p.burial);
    christened = new gEvent(p.christened);
    marriage = new gEvent(p.marriage);
    grad_info = new gEvent(p.grad_info);
    spouse = new Kin(p.spouse);
    father = new Kin(p.father);
    mother = new Kin(p.mother);
    children = (Vector)p.children.clone();
  }
  
  public void Add_sex(char c)
  {
    /**
      * Add_sex Adds an inputed character paramter into the attr. sex
      * @param c  char paramter which is the sex of the individ
      */

    sex = c;
  }

  public char Get_sex() 
    {
      /**
        * Get_sex Returns the value of the sex attribute.
        */    

    return sex;
  }

  public String Get_title()
    { 
      /**
        * Get_title Returns the value of the title attribute.
        */ 

      return title.Get_value();
    } 

  public String Get_nation()
  { 
    /**
      * Get_nation Returns the value of the nation attribute.
      */
    
    return nation.Get_value();
  } 

  public void Add_named_after(String na)
  {
    /**
      * Add_named_after Adds an inputed string param in named_after
      * @param na  String paramter which is named_after field of the individ
      */

    named_after = new String(na);
  }

  public String Get_named_after() {
    /**
      * Get_named_after Returns the value of the named_after attribute.
      */    

    return named_after;
  }
  
  public void Add_occup(String o)
  {
    /**
      * Add_occup Adds an inputed string paramter into the attr. occup.
      * @param o  String paramter which is the occup. of the individ
      */

    occupation = new String(o);
  }

  public String Get_occup() 
    {
      /**
        * Get_occup Returns the value of the occup attribute.
        */

      return occupation;
    }
  
  public void Add_name(int a, int s, String n)
  {
    /**
      * Add_name Adds an inputed string paramter into the attr. name
      * @param n  string paramter which is the name of the individ
      */
    int f = n.indexOf('/');
    int l = n.lastIndexOf('/');
    n = n.substring(0,f)+n.substring(f+1,l);
    name.Add_value(n);
  }

  public String Get_name() 
    {
      /**
        * Get_name Returns the value of the name attribute.
        */

      return name.Get_value();
    }
 
  public void Add_id(String n)
  {
    /**
      * Add_id Adds an inputed string paramter into the attr. id
      * @param n  string paramter which is the id of the individ
      */    

    gedid = new String(n);
  }

  public String Get_id()
  {
    /**
      * Get_id Returns the value of the id attribute.
      */ 
    
    return gedid;
  } 

  public void Add_nation(int a, int s, String n)
  {
    /**
      * Add_nation Adds three inputed values for the nation attribute
      * @param a  An int that is the quality level of the data
      * @param s  An int that is the source level of the data
      * @param n  A string that is the actual value of the data
      */

    nation.Add_qual(a);
    nation.Add_source(s);
    nation.Add_value(n);
  }
  
  public void Add_title(int a, int s, String t)
  {
    /**
      * Add_title Adds three inputed values for the title attribute
      * @param a  An int that is the quality level of the data
      * @param s  An int that is the source level of the data
      * @param n  A string that is the actual value of the data
      */

    title.Add_qual(a);
    title.Add_source(s);
    title.Add_value(t);
  }
  
  public void Add_baptism(String p, String d, int q, int s)
  {
    /**
      * Add_baptism Adds four inputed values for the baptism event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param q  An int that is the quality level of the data
      * @param d  A string that is the date of the event
      */

    baptism.Add_qual(q);
    baptism.Add_source(s);
    baptism.Add_place(p);
    baptism.Add_date(d);
    baptism.Add_event_value("");
  }
    
  public void Add_birth(String p, String d, int q, int s)
  {
    /**
      * Add_birth Adds four inputed values for the birth event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param q  An int that is the quality level of the data
      * @param d  A string that is the date of the event
      */

    birth.Add_qual(q);
    birth.Add_source(s);
    birth.Add_place(p);
    birth.Add_date(d);
    birth.Add_event_value("");
  }
  
  public void Add_death(String p, String d, int q, int s)
  {
    /**
      * Add_death Adds four inputed values for the death event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param q  An int that is the quality level of the data
      * @param d  A string that is the date of the event
      */

    death.Add_qual(q);
    death.Add_source(s);
    death.Add_place(p);
    death.Add_date(d);
    death.Add_event_value("");
  }
  
  public void Add_burial(String p, String d, int q, int s)
  {
    /**
      * Add_burial Adds four inputed values for the burial event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param q  An int that is the quality level of the data
      * @param d  A string that is the date of the event
      */

    burial.Add_qual(q);
    burial.Add_source(s);
    burial.Add_place(p);
    burial.Add_date(d);
    burial.Add_event_value("");
  }
  
  public void Add_christened(String p, String d, String v, int q, int s)
  {
    /**
      * Add_christened Adds five inputed values for the christened event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param q  An int that is the quality level of the data
      * @param v  A String that is the value of the event
      * @param d  A string that is the date of the event
      */

    christened.Add_qual(q);
    christened.Add_source(s);
    christened.Add_place(p);
    christened.Add_date(d);
    christened.Add_event_value(v);
  }
  
  public void Add_grad_info(String p, String d, String v, int q, int s)
  {
    /**
      * Add_grad_info Adds four inputed values for the graduation event
      * @param p  A String that is the place of the event
      * @param s  An int that is the source level of the data
      * @param v  A String that is the value of the event
      * @param q  An int that is the quality level of the data
      * @param d  A string that is the date of the event
      */

    grad_info.Add_qual(q);
    grad_info.Add_source(s);
    grad_info.Add_place(p);
    grad_info.Add_date(d);
    grad_info.Add_event_value(v);
  }

  public void Add_marriage( gEvent marry )
  {
    /**
      * Add_marriage Adds the place and date of a marriage event.
      * @param marry  An object of type gEvent that contains marriage info
      */    

    marriage.Add_place(marry.Get_place());
    marriage.Add_date(marry.Get_date());
  }

  public void Add_spouse(String n, String i)
  {
     /**
      * Add_spouse Adds the name and id for a spouse
      * @param n  A String that contains name of the spouse
      * @param i  A String that contains the id of the spouse
      */

    spouse.Add_name(n);
    spouse.Add_id(i);
  }

  public void Add_father(String n, String i)
  {
     /**
      * Add_father Adds the name and id for a father
      * @param n  A String that contains name of the father
      * @param i  A String that contains the id of the father
      */
    
    father.Add_name(n);
    father.Add_id(i);
  }

  public void Add_mother(String n, String i)
  {
     /**
      * Add_mother Adds the name and id for a mother
      * @param n  A String that contains name of the mother
      * @param i  A String that contains the id of the mother
      */
    
    mother.Add_name(n);
    mother.Add_id(i);
  }

  public void Add_child(String n, String i)
  {
     /**
      * Add_child Adds the name and id for a child
      * @param n  A String that contains name of the child
      * @param i  A String that contains the id of the child
      */

    Kin child = new Kin();
    child.Add_name(n);
    child.Add_id(i);
    children.addElement(child);
  }

  public Kin Get_child()
  {
    /**
      * Get_child gets a child object of type kin.
      */

    if (child_index < children.size())
      {
	System.out.println("Child:  " + ((Kin)children.elementAt(child_index)).Get_name());
	return (Kin)children.elementAt(child_index++);
      } else {
	child_index = 0;
	return null;
      }
  }  
  
  public String Get_birth_year()
  {
    /**
      * Get_birth_year Gets a birth year of type String.
      */    

    if ( birth.Get_date().length() == 0 ){
      return "unknown";
    } else if ( birth.Get_date().length() < 4 ){
      return birth.Get_date();
    } else {
      return birth.Get_date().substring(birth.Get_date().length() - 4);
    }
  }
  
  public void Print_sex()
  {
    /**
      * Print_sex Prints the value of sex attribute.
      */

    System.out.println("Sex:  " + sex);
  }
  
  public void Print_named_after()
  {
    /**
      * Print_named_after Prints the value of named_after attribute.
      */

    System.out.println("Named After:  " + named_after);
  }
  
  public void Print_occup()
  {
    /**
      * Print_occup Prints the value of occup attribute.
      */

    System.out.println("Occupation:  " + occupation);
  }
  
  public void Print_name()
  {
    /**
      * Print_name Prints the values of name attribute.
      */

    name.Print_qual();
    name.Print_source();
    name.Print_value("Name");
  }
  
  public void Print_nation()
  {
    /**
      * Print_nation Prints the values of nation attribute.
      */

    nation.Print_qual();
    nation.Print_source();
    nation.Print_value("Nationality");
  }
  
  public void Print_title()
  {
    /**
      * Print_title Prints the values of title attribute.
      */

    title.Print_qual();
    title.Print_source();
    title.Print_value("Title");
  }
  
  public void Print_baptism()
  {
    /**
      * Print_baptism Prints the values of baptism attribute.
      */

    System.out.println("Baptism:\n");
    baptism.Print_place();
    baptism.Print_date();
    baptism.Print_qual();
    baptism.Print_source();
  
  }
    
  public void Print_birth()
  {
    /**
      * Print_birth Prints the values of birth attribute.
      */

    System.out.println("Birth:\n");
    birth.Print_place();
    birth.Print_date();
    birth.Print_qual();
    birth.Print_source();
  
  }
  
  public void Print_death()
  {
    /**
      * Print_death Prints the values of death attribute.
      */

    System.out.println("Death:\n");
    death.Print_place();
    death.Print_date();
    death.Print_qual();
    death.Print_source();
  }
  
  public void Print_burial()
  {
    /**
      * Print_burial Prints the values of burial attribute.
      */

    System.out.println("Burial:\n");
    burial.Print_place();
    burial.Print_date();
    burial.Print_qual();
    burial.Print_source();
  }
  
  public void Print_christened()
  {
    /**
      * Print_christened Prints the values of christened attribute.
      */  	

    System.out.println("Christened:\n");
    christened.Print_place();
    christened.Print_date();
    christened.Print_event_value("Godparent");
    christened.Print_qual();
    christened.Print_source();
  }
  
  public void Print_grad_info()
  {
    /**
      * Print_grad_info Prints the values of grad_info attribute.
      */

    System.out.println("Graduation Information:\n");
    grad_info.Print_place();
    grad_info.Print_date();
    grad_info.Print_event_value("Type");
    grad_info.Print_qual();
    grad_info.Print_source();
  }

  public void Print_spouse()
  {
    /**
      * Print_spouse Prints the values of spouse attribute.
      */

    spouse.Print_name();
    spouse.Print_id();
  }
  
  public void Print_father()
  {
    /**
      * Print_father Prints the values of father attribute.
      */

    father.Print_name();
    father.Print_id();
  }
  
  public void Print_mother()
  {
    /**
      * Print_mother Prints the values of mother attribute.
      */

    mother.Print_name();
    mother.Print_id();
  }
  
  public void Print_child()
  {
    /**
      * Print_child Prints the values of child attribute.
      */

    for (int i = 0; i < children.size(); i++)
      {
	((Kin)children.elementAt(i)).Print_name();
      }
  }
  
  public void Print_Person() {
   /**
      * Print_Person Prints all the different values of a Person object.
      */     

    Print_name();
    Print_title();
    Print_sex();
    Print_named_after();
    Print_nation();
    Print_birth();
    Print_christened();
    Print_baptism();
    Print_grad_info();
    Print_death();
    Print_burial();
  }

} /*   END PERSON CLASS  */




