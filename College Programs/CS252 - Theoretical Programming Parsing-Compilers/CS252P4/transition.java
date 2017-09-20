import java.io.*;
import java.util.*;

class transition {
   private String currentState;
   private String  inputSymbol;
   private String  topOfStack1;
   private String topOfStack2;
   private String  nextState;
   private String  nextStack1Symbol;
   private String nextStack2Symbol;
   private transition nextTransition;
   
   public transition(String inputString) {
         StringTokenizer stringTok = new StringTokenizer(inputString, ",");
         currentState = stringTok.nextToken().trim();
         inputSymbol = stringTok.nextToken().trim();
         topOfStack1 = stringTok.nextToken().trim();
         topOfStack2 = stringTok.nextToken().trim();
         nextState = stringTok.nextToken().trim();
         nextStack1Symbol = stringTok.nextToken().trim();
         nextStack2Symbol = stringTok.nextToken().trim();
         nextTransition = null;
   }
   
   public void addTransition(String inputString) {
      transition temp = this;
      while (temp.nextTransition != null) {
         temp = temp.nextTransition;
      }
      temp.nextTransition = new transition(inputString);
   }//end of addTransition method
   
   
   public StringTokenizer findTransition(String state, String input, String stack, String stack2) {
      transition temp = this;
      String tempString = "";
      while (temp != null) {
         if (state.equals(temp.currentState)) {
            if (input.equals(temp.inputSymbol)) {
               if (stack.equals(temp.topOfStack1) || temp.topOfStack1.equals("L")) {
                              if (stack2.equals(temp.topOfStack2) || temp.topOfStack2.equals("L")) {
                                 tempString = tempString.concat(temp.inputSymbol);
                                 tempString = tempString.concat(",");
                                 tempString = tempString.concat(temp.nextState);
                                 tempString = tempString.concat(",");
                                 tempString = tempString.concat(temp.nextStack1Symbol);
                                 tempString = tempString.concat(",");
                                 tempString = tempString.concat(temp.topOfStack1);
                                 tempString = tempString.concat(",");
                                 tempString = tempString.concat(temp.nextStack2Symbol);
                                 tempString = tempString.concat(",");
                                 tempString = tempString.concat(temp.topOfStack2);
                                 return new StringTokenizer(tempString, ",");
                            }     
               }      
            }
         }
         temp = temp.nextTransition;
      }
      return null;   
       }//end of findTranstion
       
       public void output() {
          transition temp = this;
          while (temp !=null) {
             System.out.println(temp.currentState + "," + temp.inputSymbol);
             temp = temp.nextTransition;
          }
       
       }

}//end of transition class
