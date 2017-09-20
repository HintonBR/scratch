import java.io.*;
import java.util.*;
import readFile;
import transition;
import inputString;


class PDA {
private String currentString;
private String currentStack;
private String currentStack2;
private String currentState;
private String acceptingStates;


public static void main(String args[]) {
   PDA myOb = new PDA();
   readFile myFile = new readFile(args[0]);
   transition myTrans;
   inputString myInput = new inputString();
   String temp;
   char tempChar = 'a';
   StringTokenizer stringTok;
   
   myOb.acceptingStates = myFile.getLine();
   myTrans = new transition(myFile.getLine());
   while (myFile.eof() == false)   {
      temp = myFile.getLine();
      if (temp != null) {
         myTrans.addTransition(temp);      
      }      
   }
   while (!(myInput.dataString.trim().equals("q"))) {
   /*System.out.println("Select the mode that you would like to use to accept strings(e for empty stack or f for final state) or select q to quit");
   try {
      tempChar = (char)System.in.read();
      System.in.read();
      System.in.read();
   }  
   catch (IOException e) {
   }*/
    System.out.println("Enter the string to test for acceptance");          
     myInput.reset();
     myInput.getInputString();    
   if (!(myInput.dataString.trim().equals("q"))) {
     myOb.currentString = myInput.dataString.trim();
     myOb.currentState = "0";
     myOb.currentStack = "L";
     myOb.currentStack2 = "L";
     stringTok = myTrans.findTransition(myOb.currentState, myOb.currentString.substring(0, 1).trim(), myOb.currentStack, myOb.currentStack2);
     System.out.print(myOb.currentState + ", ");
     System.out.print(myOb.currentString + ", ");
     System.out.println(myOb.currentStack);
     System.out.println(myOb.currentStack2);
     while (stringTok != null) {
        if (myOb.currentString.length() > 1) {
         if (stringTok.nextToken().equals("l") == false) {        
         myOb.currentString = myOb.currentString.substring(1).trim();     
         }
       }
       else {
          stringTok.nextToken();
          myOb.currentString = "l";
       }
       myOb.currentState = stringTok.nextToken();
       temp = stringTok.nextToken();
       if (temp.equals("L")) {
          if (myOb.currentStack.length() > 1) {
              if (stringTok.nextToken().equals("L") == false) {
                 myOb.currentStack = myOb.currentStack.substring(1).trim();
              }
          }
          else {
            if (stringTok.nextToken().equals("L") == false) {
             myOb.currentStack = "L";
              }
             
          }
       } 
       else {
          if (myOb.currentStack.equals("L")) {
           stringTok.nextToken();
             myOb.currentStack = temp;
          }
          else {
             if (stringTok.nextToken().equals("L") == true) {
             myOb.currentStack = temp + myOb.currentStack;
             }
             else {
                myOb.currentStack = temp + myOb.currentStack.substring(1).trim();
             }
          }
       }    
       
       //This section handles code for Stack2
        temp = stringTok.nextToken();
       if (temp.equals("L")) {
          if (myOb.currentStack2.length() > 1) {
              if (stringTok.nextToken().equals("L") == false) {
                 myOb.currentStack2 = myOb.currentStack2.substring(1).trim();
              }
          }
          else {
            if (stringTok.nextToken().equals("L") == false) {
             myOb.currentStack2 = "L";
              }
          }
       } 
       else {
          if (myOb.currentStack2.equals("L")) {
           stringTok.nextToken();
             myOb.currentStack2 = temp;
          }
          else {
             if (stringTok.nextToken().equals("L") == true) {
             myOb.currentStack2 = temp + myOb.currentStack2;
             }
             else {
                myOb.currentStack2 = temp + myOb.currentStack2.substring(1).trim();
             }
          }
       }
     System.out.print(myOb.currentState + ", ");
     System.out.print(myOb.currentString + ", ");
     System.out.println(myOb.currentStack + ",");
     System.out.println(myOb.currentStack2);
        stringTok = myTrans.findTransition(myOb.currentState, myOb.currentString.substring(0, 1).trim(), myOb.currentStack.substring(0,1).trim(), myOb.currentStack2.substring(0,1).trim());
   }//end of while
   if (myOb.acceptingStates.indexOf(myOb.currentState) != -1 && myOb.currentStack.equals("L") && myOb.currentStack2.equals("L") & myOb.currentString.equals("l")) {
      System.out.println("String accepted");
   }
   else {
      System.out.println("String not accepted");
   }

   }//end if 
   else {
      System.out.println("Program Exiting");
   }
   
   }//end of while
   

}//end of main method

}//end of class PDA
