/****************************************************************
Authors:  Joseph Olsen, Sean Jefferies, Russ Nelson, Judd Richards

PARSER:
Responsibilities:
                                         
    Detect family and individual record tags
    Validate family and individual record tags
    Read in family and individual record data
    Return family and individual record information
    Display error messages when bad data found
    
Collaborators:
    Person
    Family
    GedCom                                                                   
****************************************************************/

import java.awt.*;
import java.io.*;
import Person;
import Family;

public class Parser {

  TextArea historyWindow;    	//Print History to screen
  TextField statusWindow;    	//Prints current status of program to screen
  String id = "";	
  //ID used to calculate filename for 
  //each Person or Family record

    public Parser(/*TextArea a, TextField f*/)
    {
      /**
        * Parser Constructor for object Parser
	* @param a  Object of type test area used in construction of parser
	* @param f  Object of type test field used in construc of parser
	*/
	/*
      historyWindow = a;
      statusWindow = f;
	*/
    };

    /*
  public void printStatus(String status)
    {
      **
        * printStatus prints message to text area
	* @param status  String used for printing message of print status
	*
 
      historyWindow.append(status);
      if ( status.substring(0,4).equals("ERROR") )
        {
	  statusWindow.setForeground(GedCom.red);
        } else {
	  statusWindow.setForeground(GedCom.blue);
        }
      statusWindow.setText(status);
    }
    commented out by Karl */
    
  public int findNextRecord(RandomAccessFile gc) 
    {
      /**
        * findNextRecord Seeks next family or individual record.
	*                Returns a 0 if no family or individual records 
	*                are found.  Returns a 1 if a family record is found.
	*                Returns a 2 if an individual record is found.
	* @param gc  Object of type RandomAccessFile used for finding next
	*            record.
	*/

      String currLine = "";
      String tag      = "";
      int    index    = -1;  // index for a given character in a string
      int    index2   = -1;  // index to help find person id
      
      
      // Search for a valid Person or Family record tag
      // Return a 0 if a valid tag was not found
      try {
	currLine = gc.readLine();		//reads in line from file
	//System.out.println(currLine + "\n");
	if (currLine.equals("0 TRLR"))
	  {
	    return -1;
	  }
	if (currLine.charAt(0) == '0') {
	  index2 = currLine.indexOf('@');  //looks for @ symbols that
	  index = currLine.lastIndexOf('@');//denote new GedCom record
	}
	if (index > -1) {
	  id  = new String(currLine.substring((index2 + 1), index));
	  //System.out.println("DEBUG: ID="+id);
	  tag = currLine.substring(index);
	  tag = tag.trim();  // trim off the white space around the tag
	  //System.out.println("id:  " + id);
	  //System.out.println("tag: " + tag);        
	}
	
	//if tag is not individual or family record keep looking 
	while (!(tag.equals("@ INDI")) && !(tag.equals("@ FAM"))) {
	  tag = "";
	  index = -1;
	  index2 = -1;
	  currLine = gc.readLine();
	  //System.out.println(currLine + "\n");
	  if (currLine.charAt(0) == '0'){
	    index2 = currLine.indexOf('@');
	    index = currLine.lastIndexOf('@');
	  }
	  if (index > -1) {
	    id  = new String(currLine.substring((index2 + 1), index));
	    tag = currLine.substring(index);
                    tag = tag.trim();  // trim off the white space around the tag
                    //System.out.println("id:  " + id);
                    //System.out.println("tag: " + tag);
	  }
	}
	if (tag.equals("@ FAM")) {
	    //printStatus("Found a new Family\n");
	    	return 1;
	} 
	else if (tag.equals("@ INDI")) {
	    //printStatus("Found a new Person\n");
	  return 2;
	} else { return 0;}
      } catch (NullPointerException e) {
	// Must not have found a valid Person Info tag
	//printStatus("ERROR: Unable to find a valid Person tag\n");
	//  System.out.println("ERROR finding a valid Person tag\n" + e);
	    return 0;	
      } catch (IOException e) {
	  //printStatus("ERROR: Parsing error\n");
	System.out.println("ERROR parsing\n" + e);
	return 0;
      }
    } /* --- END OF findNextRecord METHOD --- */

  public int parseFile(RandomAccessFile gc, Person p, Family f)
    {    
    /**
      * parseFile Calls findNextRecord to see if any Person or Family 
      *           records exist calls appropriate parse procedure 
      *           depending on the value returned from findNextRecord. 
      *           0 means no record found, 1 a family record was found, 
      *           2 a person record was found.
      * @param gc  Object of time RandomAccessFile used to parse the file
      * @param p  Person object used for parsing the file
      * @param f  Family object used for parsing the file.
      */
    
      int recordCheck = 0;
      
      recordCheck = findNextRecord(gc);
      if (recordCheck == 1) {
	  //printStatus("Parsing a family record.\n");
	if (parseFamily(gc, f)) {
	  return 1;
	} else 	return 0;
      } else if (recordCheck == 2) {
	  //printStatus("Parsing an individual record.\n");
	if (parsePerson(gc, p)) {
	  return 2;
	} else  return 0;
        } else return 0;
    }
    
  public boolean parsePerson(RandomAccessFile gc, Person p) 
    {
      /**
        * parsePerson Accepts a file stream gc and returns a Person if a 
	*             valid Person record is found.
	* @param gc  Object of type RandomAccessFile used to parse the file
	* @param p  Person object used for parsing a person
	*/
    
      String currLine = "";
      String info     = "";
      String tag      = "";  // Low-level tags (DATE, PLAC)
      String keyTag   = "";  // High-level tags (BIRT, DEAT, SEX)
      int    level    = -1;  // level of GedCom info in the current line
      int    index    = -1;  // index for a given character in a string
      Person person   = p;
      long   fPointer = 0;
      
      // If the method has made it this far, 
      // a valid Person record tag must have been found.
      // So parse that person's record.
      try {
	while (level != 0 ) {
	  // Reset the strings
	  tag = "";
	  info = "";
	  currLine = "";
	  // Mark the current line so that it can be put back if the eof is read in
	  // or if the next person's INDI tag is found.
	  //gc.mark(1024);  
	  fPointer = gc.getFilePointer();
	  // read in the current line
	  currLine = gc.readLine();
	  index = currLine.indexOf(" ");
	  //System.out.println("DEBUG[parse person]: ID="+id);
	  person.Add_id(id);	//add the id string to person
	  //used in the HTML filename.
	  if (index > -1) {
	    // parse off and store the current level
	    level = Integer.parseInt(currLine.substring(0, index));
	    // keep everything after the level
	    currLine = currLine.substring(index + 1);
	    //System.out.println("*****************");
	    //System.out.println("Level: " + level);
	  }
	  // If the level = 0, stop parsing -
	  // you've reached the end of the person's record.
	  // If the level is greater than 0, keep parsing.
	  if (level > 0) {
	    tag = new String(currLine);
	    index = tag.indexOf(" ");
	    if (index > -1) {      
	      // If there's a space, copy everything after it
	      // into the information string.
	      info = tag.substring(index + 1);
	      // and keep the tag
	      tag = tag.substring(0,index);
	    }

	    // If the level is a one, the tag is a key tag
                    if (level == 1)
                      keyTag = new String(tag);
		    
		    // Key into the current keyTag to fill the appropriate
		    // Person attribute
                    if (keyTag.equals("BIRT")) {
		      // All birth information is stored in level 2 tags
		      if (level == 2) {
			if (tag.equals("DATE"))
			  person.birth.Add_date(info);
			else if (tag.equals("PLAC"))
			  person.birth.Add_place(info);
			else if (tag.equals("QUAL"))
			  person.birth.Add_qual(Integer.parseInt(info));
			//System.out.println(keyTag + " " + tag + ": " + info);
		      }
                    }
                    else if (keyTag.equals("SEX")) {
                      person.Add_sex(info.charAt(0));
                      //System.out.println(keyTag + ": " + info);
		    }
                    else if (keyTag.equals("GRAD")) {
		      // All graduation info is stored in level 2 tags
		      if (level == 2) {
                            if (tag.equals("DATE"))
			      person.grad_info.Add_date(info);
                            else if (tag.equals("PLAC"))
			      person.grad_info.Add_place(info);
                            else if (tag.equals("QUAL"))
			      person.grad_info.Add_qual(Integer.parseInt(info));
                            //System.out.println(keyTag + " " + tag + ": " + info);
		      }
                    }
                    else if (keyTag.equals("NAME")) {
			//printStatus("Name:  " + info + "\n");
		         //person.name.Add_value(info);
		      person.Add_name(0,0,info);
                    }
                    else if (keyTag.equals("NOTE")) {
		      //System.out.println(info); 
                    }
                    else if (keyTag.equals("DEAT")) {
		      // All death information is stored in level 2 tags
		        if (level == 2) {
			  if (tag.equals("DATE"))
			    (person.death).Add_date(info);
			  else if (tag.equals("PLAC"))
			    (person.death).Add_place(info);
			  else if (tag.equals("QUAL"))
			    (person.death).Add_qual(Integer.parseInt(info));
			  //System.out.println(keyTag + " " + tag + ": " + info);
                        }
                    }
                    else if (keyTag.equals("BURI")) {
		      // All burial information is stored in level 2 tags
		      if (level == 2) {
			if (tag.equals("DATE"))
			  (person.burial).Add_date(info);
			else if (tag.equals("PLAC"))
			  (person.burial).Add_place(info);
			else if (tag.equals("QUAL"))
			  (person.burial).Add_qual(Integer.parseInt(info));
			//System.out.println(keyTag + " " + tag + ": " + info);
		      }
                    }
                    else {      
		      //System.out.println("tag: " + tag);
		      //System.out.println("info: " + info);
                    }
		    
                    //System.out.println("*******************");
		    
	  } // Finished parsing the current line
	  
	} // End of While loop through the current person's record.
	
	// Must have found the next person's INDI tag, so put it back
	//gc.reset();
	gc.seek(fPointer);
	//System.out.println("reached the end of this person's info");
	return true;
	
      } catch (NullPointerException e) {
	// Must have reached the end of the file.
	// Put back (reset) the end of file marker, so that
	// The next run through this method will return 
	// a false boolean value and end the loop through
	// the GedCom file.
	try {
	  //gc.reset();
	  gc.seek(fPointer);
	  return true;	
	} catch (IOException error) {
	    //printStatus("Unable to replace the current line "+error.toString()+"\n");
	  System.out.println("Unable to replace the current line\n" + error);
	  return false;
	}
      } catch (IOException e) {
	  //printStatus("ERROR: Parsing error\n");
	System.out.println("Error parsing\n" + e);
	return false;
      }
      
    } /* --- END OF parsePerson METHOD --- */	

  public boolean parseFamily(RandomAccessFile gc, Family f) 
    {
      /**
        * parseFamily Accepts a file stream gc and returns a Family if a 
	*             valid Family record is found.
	* @param gc  Object of type RandomAccessFile used for parsing family
	* @param f  Family object used in parsing family
	*/
	
      String currLine = "";
      String info     = "";
      String tag      = "";  // Low-level tags (DATE, PLAC)
      String keyTag   = "";  // High-level tags (MARR)
      int    level    = -1;  // level of GedCom info in the current line
      int    index    = -1;  // index for a given character in a string
      Family family   = f;
      long   fPointer = 0;
      String tempId;
      int idIndex = -1;
      int idIndex2 = -1;
      
      
      // If the method has made it this far, 
      // a valid Family record tag must have been found.
      // So parse that family record.
      try {
	while (level != 0 ) {
	  // Reset the strings
	  tag = "";
	  info = "";
	  currLine = "";
	  // Mark the current line so that it can be put back if the eof is read in
	  // or if the next RECORD tag is found.
	  //gc.mark(1024);  
	  fPointer = gc.getFilePointer();
	  // read in the current line
	  currLine = gc.readLine();
	  index = currLine.indexOf(" ");
	  //family.Add_id(id);  //Not ready for this yet.
	  if (index > -1) {
	    // parse off and store the current level
	    level = Integer.parseInt(currLine.substring(0, index));
	    // keep everything after the level
	    currLine = currLine.substring(index + 1);
	    //System.out.println("*****************");
	    //System.out.println("Level: " + level);
	  }
	  // If the level = 0, stop parsing -
	  // you've reached the end of the family record.
	  // If the level is greater than 0, keep parsing.
	  if (level > 0) {
	    tag = new String(currLine);
	    index = tag.indexOf(" ");
	    if (index > -1) {      
	      // If there's a space, copy everything after it
	      // into the information string.
	      info = tag.substring(index + 1);
	      // and keep the tag
	      tag = tag.substring(0,index);
	    }
	    
	    // If the level is a one, the tag is a key tag
	    if (level == 1)
	      keyTag = new String(tag);
	    
	    // Key into the current keyTag to fill the appropriate
	    // Family attribute
	    if (keyTag.equals("HUSB")) {
	      idIndex = -1;
	      idIndex2 = -1;
	      idIndex2 = info.indexOf('@');
	      idIndex = info.lastIndexOf('@');
	      tempId = new String(info.substring((idIndex2 + 1), idIndex));
	      family.Add_husband(tempId);
	      //System.out.println("Info: " + info);
	      //System.out.println(keyTag + ": " + tempId);
	    }
	    else if (keyTag.equals("WIFE")) {
	      idIndex = -1;
	      idIndex2 = -1;
	      idIndex2 = info.indexOf('@');
	      idIndex = info.lastIndexOf('@');
	      tempId = new String(info.substring((idIndex2 + 1), idIndex));
	      family.Add_wife(tempId);
	      //System.out.println(keyTag + ": " + tempId);
	      //System.out.println(keyTag + ": " + info);
	    }
	    else if (keyTag.equals("MARR")) {
	      // All marriage info is stored in level 2 tags
	      if (level == 2) {
		if (tag.equals("DATE"))
		  family.marriage.Add_date(info);
		else if (tag.equals("PLAC"))
		  family.marriage.Add_place(info);
		else if (tag.equals("QUAL"))
		  family.marriage.Add_qual(Integer.parseInt(info));
                                //System.out.println(keyTag + " " + tag + ": " + info);
	      }
	    }
	    else if (keyTag.equals("CHIL")) {
	      idIndex = -1;
	      idIndex2 = -1;
	      idIndex2 = info.indexOf('@');
	      idIndex = info.lastIndexOf('@');
	      tempId = new String(info.substring((idIndex2 + 1), idIndex));
	      family.Add_child(tempId);
	      //System.out.println(keyTag + ": " + tempId);
	      //System.out.println(keyTag + ": " + info);
	    }
	    else if (keyTag.equals("NOTE")) {
	      //System.out.println(info); 
	    }
	    else if (keyTag.equals("NCHI")) {
	      family.Add_num_children(info.charAt(0));
	      //System.out.println(keyTag + ": " + info);
	    }		        
	    else {      
	      //System.out.println("tag: " + tag);
	      //System.out.println("info: " + info);
	    }
	    
	    //System.out.println("*******************");
	    
	  } // Finished parsing the current line
	  
	} // End of While loop through the current family's record.
	
	// Must have found another RECORD tag, so put it back
	//gc.reset();
	gc.seek(fPointer);
	//System.out.println("reached the end of this family's info");
	return true;
	
      } catch (NullPointerException e) {
	// Must have reached the end of the file.
	// Put back (reset) the end of file marker, so that
	// The next run through this method will return 
	// a false boolean value and end the loop through
	// the GedCom file.
	try {
	  //gc.reset();
	  gc.seek(fPointer);
	  return true;	
	} catch (IOException error) {
	  System.out.println("Unable to replace the current line\n" + error);
	  return false;
	}
      } catch (IOException e) {
	System.out.println("Error parsing\n" + e);
	return false;
      }
      
    } /* --- END OF parseFamily METHOD --- */	
  
  
} /* *---* END OF Parser CLASS *---* */






