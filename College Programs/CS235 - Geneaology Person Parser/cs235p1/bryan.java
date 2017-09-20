/**********************

Program Description: This is a simple program which demonstrates use of the
Parser class for parsing gedcom files.  The program takes a single argument
(the name of a gedcom file) and prints our the name associated with the
first record.



***********************/


import byu.*;
import Parser;
import java.io.*;

public class bryan
{

    public static void main(String[] args)
    {
	int status;

	RandomAccessFile the_file;


	/* check to see if a gedcom filename was given */
        if(args.length != 2)
	    {
		System.out.println("Usage: Sample <gedcom filename>");
		return;
	    }



	/* open the gedcom file */
	try{
	    the_file = new RandomAccessFile(args[0], "rw");
	}
	catch(Exception e){
	    System.out.println("Error: problem initializing file: " + args[0]);
	    return;
	}


	/* declare a parser and variables to hold results */
	Parser parser = new Parser();
	Person person = new Person();
	Family family = new Family();
  String name1 = new String();
  int result1;

	/* read the next person or family record from the_file */
	/* status == 0 means no record found
	 * status == 1 means family record was found
	 * status == 2 means person record was found
	 */

  status = 1;
  while (status != 0) {
	status = parser.parseFile(the_file, person, family);


	       if(status == 2)
	       {
                name1 = person.name.Get_value();
                name1 = name1.toLowerCase();
                result1 = name1.indexOf(args[1], 0);

                    if(result1 != -1) {
                                          System.out.println(name1);
                    }//end of if
           }//end of if
  } //end of while loop

	/* close the file */
	try{
	    the_file.close();
	}
	catch(Exception e){
	    System.out.println("Error: problem closing file");
	    return;
	}
      Console.in.readInt();
    }//end of main
}













