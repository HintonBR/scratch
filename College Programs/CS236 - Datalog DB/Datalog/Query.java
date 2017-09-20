import Predicate.*;
import java.io.*;

       public class Query {
       
          Predicate queryPredicate;
          Query nextQuery;
          
          Query (Predicate myPredicate) {
             queryPredicate = myPredicate;
          }
         
          void Show() {
              Predicate tempPred;
               Query tempQuery = this;
               int number = 1;
               System.out.println("Queries");
               while (tempQuery != null) {
               tempPred = tempQuery.queryPredicate;
                System.out.println(number);
                printSpaces();
                tempPred.Show();
                tempQuery = tempQuery.nextQuery;
                if (tempQuery != null) {
                tempPred = tempQuery.queryPredicate;                        
                }               
                try {
               System.in.read();
               System.in.read();
               
               }
               catch (IOException ie) {
               
               }              
                number++;
               }
                            
              }//end of Show method
               
               void printSpaces ()  {
                   System.out.print(" ");
                   System.out.print(" ");
                   System.out.print(" ");                 
               }//end of printSpaces method
               

       
       }//end of Query class
