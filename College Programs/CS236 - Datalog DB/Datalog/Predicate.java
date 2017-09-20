import Argument.*;
       public class Predicate {
           String name;
           char typeOfPredicate;
           Argument arg;
           Predicate nextPred;
           
           Predicate (String mname, char ptype) {
               name = mname;
               typeOfPredicate = ptype;
           }
                              
           void Show() {
               Argument myarg = arg;
               printSpaces(6);
                System.out.println("Predicate");
                printSpaces(9);
                System.out.println("Name:  " + name);
                printSpaces(9);
                System.out.println("Arguments");
               while (myarg != null) {
                printSpaces(12);
                myarg.Show();
                             myarg = myarg.nextarg;
                }
                
            }//end of Predicate Show method
            
            void printSpaces (int number) {
                  int counter = 1;
                   while (counter <= number) {
                   System.out.print(" ");
                   counter++;
                   }
             }//end of printSpaces method
       }//end of Predicate class
       
