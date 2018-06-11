/*
// header - edit "Data/yourJavaHeader" to customize
// contents - edit "EventHandlers/onCreateJavaFile" to customize
//
*/
import byu.*;
import java.io.*;
import Parser;

  //startof class PersonList
 class PersonList {  //start of class PersonList


	//Person Node declaration

	Person person;
	PersonList next;



	//PersonList constructor
	PersonList() {
		person = null;
		next = null;
	}//end of PersonList constructor


	//general variables
	static String QUERYFILE;
	static String INPUTFILE;
	static String OUTPUTFILE = "F:\\cs235p2\\OUTPUT.TXT";
	static String NumberOfRecords;
	static int Accesses = 0;
	static RandomAccessFile the_file;
	static PersonList headperson = new PersonList();
	//static PersonList current = new PersonList();
	//static PersonList next1 = new PersonList();
	static Integer numberofrecord;
	static int numberofrecords;
  static PersonList current;
  static int compares = 0;

	//Start of class that will search for People in GEDCOM file

	 public static class bryan  {

	public static Person search()		{
	int status;


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
				 if(status == 2)		 {
						return person;
				 }//end of if
		} //end of while loop
	person = null;
	return person;
	}//end of search method

} //end of bryan class


		public static void addperson(PersonList current)  {
        //PersonList next1;

				current.next = new PersonList();
        current = current.next;

		}//end of addperson

		public static int resetaccesscount()  {
				return 0;
		}//end of method resetaccesscount

		public static String GetName(String gedid, PersonList current) {

			while (current.person != null) {
        Accesses += 1;
				if (gedid.compareTo(current.person.gedid) == 0) {
						return current.person.name.value;
				}
				else current = current.next;

			}//end of while
			return null;
			}//end of method GetName




    		public static PersonList SelSort(PersonList headperson) {
               PersonList node1;
               PersonList node2;
               PersonList temp;
               PersonList small;
               PersonList head;
               PersonList previous;
               Person temp1;

               temp = headperson;
               head = headperson;
               node1 = headperson;
               int test = 1;
               for (node1 = headperson; node1.next != null; node1 = node1.next ) {
                   small = node1;
                   previous = node1;
                   for (node2 = node1.next; node2 != null; node2 = node2.next) {
                        compares += 1;
                        if (node2.person.gedid.compareTo(small.person.gedid) < 0) {
                           previous = small;
                           small = node2;
                        }//end of if
                        }//end of for loop
                        temp1 = node1.person;
                        node1.person = small.person;
                        small.person = temp1;
                          // if (test == 1) {
                           //   previous.next = small.next;
                            //  head = small;
                             // temp = small;
                            //  temp = temp.next;
                            //  test += 1;
                         //  }//end of if
                         //  if (previous == small) {
                          // previous = small.next;
                           //temp = small;
                          // temp = temp.next;
                          // }//end of if

                          // else {
                          // previous.next = small.next;
                           //temp = small;
                          // temp = temp.next;

                            //}//end of else



               }//end of outer for loop





      return head;
	 	}//end of SelSort method




		static class MergeSorter {

			public static PersonList merge(PersonList node1, PersonList node2)  {

				if (node1 == null) return node2;
				else if (node2 == null) return node1;
					else if (node1.person.gedid.compareTo(node2.person.gedid) <= 0) {
						compares += 1;
						node1.next = merge(node1.next, node2);
						return node1;
					}
					else {
						compares += 1;
						node2.next = merge(node1, node2.next);
						return node2;
					     }
			}//end of merge method


			public static PersonList split(PersonList node1)  {

				PersonList node2;
				if (node1 == null) return null;
				else if (node1.next == null) return null;
					else {
						node2 = node1.next;
						node1.next = node2.next;
						node2.next = split(node2.next);
						return node2;
					}
			}//end of split method


			public static PersonList MergeSort(PersonList node1)  {

				PersonList node2;
        PersonList current1 = new PersonList();

        //System.out.println("we made it to here - mergesort");
        //Console.in.readInt();
				if (node1 == null) return null;
				if (node1.next == null) return node1;
				node2 = split(node1);
        //System.out.println("we made it outside of the split list");
        //Console.in.readInt();
        TextOutputStream out = new TextOutputStream("f:\\cs235p3\\out.txt");
        current1 = node1;
        while (current1 != null) {
        out.println(current1.person.name.value);
        current1 = current1.next;
        }
        current1 = node2;
        while (current1 != null) {
        out.println(current1.person.name.value);
        current1 = current1.next;
        }
        out.close();
        //System.out.println("we made it outside of the split list");
        //Console.in.readInt();
				return merge(MergeSort(node1), MergeSort(node2));
			}//end of MergeSort method


		}//end of MergerSorter


		public static void main(String args[])  {


    INPUTFILE = args[0];
			QUERYFILE = args[1];
			NumberOfRecords = args[2];
			numberofrecord = Integer.valueOf(NumberOfRecords);
			numberofrecords = numberofrecord.intValue();

      PersonList next1 = new PersonList();

			/* open the gedcom file */
	try{
			the_file = new RandomAccessFile(INPUTFILE, "rw");
	}
	catch(Exception e){
			System.out.println("Error: problem initializing file: " + args[0]);
			return;
	}

			//searches through file to build linked list
      headperson.person = bryan.search();
      headperson.next = new PersonList();
      current = headperson.next;
			for (int count = 2; count <= numberofrecords; count++) {
			current.person = bryan.search();
			if (current != null)  {
						current.next = new PersonList();
            current = current.next;



			}//end of if

			}	// end of for

			//Console.in.readInt();


     // TextOutputStream out = new TextOutputStream(OUTPUTFILE);
      //current = headperson;
     // while (current != null) {
     // out.println(current.person.name.value);
     // current = current.next;
     // }//end of while
				/* close the file */
	try{
  			the_file.close();
	}
	catch(Exception e){
			System.out.println("Error: problem closing file");
			return;
	}
  next1 = headperson;
  while ((next1.next.person != null) & (next1 != null)) {
        next1 = next1.next;
  }//end of while
  if (next1.next.person == null)  next1.next = null;

  System.out.println("we made it to here");
  int value = Console.in.readInt();
  TextOutputStream out = new TextOutputStream(OUTPUTFILE);
  if (value == 1) {
  MergeSorter sort = new MergeSorter();
  headperson = sort.MergeSort(headperson);
  }//end of if
  else {
      headperson = SelSort(headperson);
  }//end of else
  current = headperson;
  while (current != null) {
        out.println(current.person.name.value + " " + current.person.gedid);
        current =current.next;
  }//end of while
  System.out.println(compares);
  Console.in.readInt();
  out.close();
     // System.out.println("closed the file");
     // Console.in.readInt();
		//	TextInputStream inf = new TextInputStream(QUERYFILE);
	//	TextOutputStream out = new TextOutputStream(OUTPUTFILE);
		//	String readinfo = inf.readLine();
		//	int totalaccesses = 0;
		//	int query = 0;
		//	while (readinfo != null) {
		//		Accesses =  resetaccesscount();
		//		current = headperson;
		 //		String name = GetName(readinfo, current);
			//	if (name != null)   {
		//			totalaccesses += Accesses;
		//			query += 1;
		//			out.println(name);

		//		}//end of if statement
		//		readinfo = inf.readLine();
		//	} //end of while loop

		//	inf.close();
		//	out.close();
		//	if (totalaccesses != 0) {
		//				System.out.println("The number of nodes accessed per successful query was " + totalaccesses/query);
		//	}
		//	else {System.out.println("No nodes found in list of queries");
		//	}
		//	Console.in.readInt();
		}//end of main



	} //End of PersonList class
