import java.io.*;
import inputString;

public class Acceptor {

   final static String DIGIT = "0123456789";
   final static String PERIOD = ".";
   final static String PLUSMINUS = "+-";
   final static String VALIDINPUT = "0123456789.+-";
   
   char nextChar;
   inputString myObject;
   
   private boolean IsDigit(char c) {
      if (DIGIT.indexOf(c) != -1) {
         return true;
      }
      else {
         return false;
      }
   }//end of IsDigit method
   
   private boolean IsPeriod(char c) {
      if (PERIOD.indexOf(c) != -1) {
         return true;
      }
      else {
         return false;
      }
   }//end of IsPeriod method
   
   private boolean IsPlusMinus(char c) {
      if (PLUSMINUS.indexOf(c) != -1) {
         return true;
      }
      else {
         return false;
      }
   }//end of IsPlusMinus method
   
  private void StartState() {
     System.out.println("We are in the Start State");
     if (IsDigit(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State3();
     }   
     else if (IsPlusMinus(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State2();
     } 
     else if (IsPeriod(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State1();
     } 
     else {
        System.out.println("We read a " + nextChar);
        State5();
     }
  }//end of Start State method
  
   private void State1() {
      System.out.println("We are in State 1");
      if (IsDigit(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State4();
     }         
     else {
        System.out.println("We read a " + nextChar);
        State5();
     }
   
   }//end of State1
   
 private void State2() {
      System.out.println("We are in State 2");   
    if (IsDigit(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State3();
     }         
     else if (IsPeriod(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State1();
     } 
     else {
        System.out.println("We read a " + nextChar);
        State5();
     }
   }//end of State2
   
    private void State4() {
    System.out.println("We are in State 4");   
     if (IsDigit(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State4();
     }           
     else if (nextChar == 'E') {
        System.out.println("Valid String");
     }
    else {
        System.out.println("We read a " + nextChar);
        State5();
     }

   }//end of State4

 private void State5() {
      System.out.println("We are in State 5");   
      System.out.println("Invalid String");
   }//end of State5
 private void State3() {
    System.out.println("We are in State 3");
     if (IsDigit(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State3();
     }               
    else if (IsPeriod(nextChar)) {
        System.out.println("We read a " + nextChar);
        nextChar = myObject.getNextCharacter();
        State4();
     } 

     else if (nextChar == 'E') {
        System.out.println("Valid String");
     }
    else {
        System.out.println("We read a " + nextChar);
        State5();
     }

 }//end of State3  

   
   public static void main(String args[]) {
     int i = 0;
     Acceptor myAcceptor = new Acceptor();
      myAcceptor.myObject = new inputString();
     while (i != 1) {    
        myAcceptor.myObject.getInputString(VALIDINPUT);
        myAcceptor.nextChar = myAcceptor.myObject.getNextCharacter(); 
        if (myAcceptor.nextChar == 'q') {
           i = 1;
        }
        else {
           myAcceptor.StartState();     
           myAcceptor.myObject.reset();
        }
     }

   }//end of main method

}//end of Acceptor class
