import java.io.*;
import java.util.*;

class transition {
   private String currentState;
   private String  inputSymbol;
   private String  topOfStack;
   private String  nextState;
   private String  nextStackSymbol;
   private transition nextTransition;
   
   public transition(String inputString) {
         StringTokenizer stringTok = new StringTokenizer(inputString, ",");
         currentState = stringTok.nextToken().trim();
         inputSymbol = stringTok.nextToken().trim();
         topOfStack = stringTok.nextToken().trim();
         nextState = stringTok.nextToken().trim();
         nextStackSymbol = stringTok.nextToken().trim();
         nextTransition = null;
   }
   
   public void addTransition(String inputString) {
      transition temp = this;
      while (temp.nextTransition != null) {
         temp = temp.nextTransition;
      }
      temp.nextTransition = new transition(inputString);
   }//end of addTransition method
   
   
   public StringTokenizer findTransition(String state, String input, String stack) {
      transition temp = this;
      String tempString = "";
      while (temp != null) {
         if (state.equals(temp.currentState)) {
            if (input.equals(temp.inputSymbol) || temp.inputSymbol.equals("l")) {
               if (stack.equals(temp.topOfStack) || temp.topOfStack.equals("L")) {
                  tempString = tempString.concat(temp.inputSymbol);
                  tempString = tempString.concat(",");
                  tempString = tempString.concat(temp.nextState);
                  tempString = tempString.concat(",");
                  tempString = tempString.concat(temp.nextStackSymbol);
                  tempString = tempString.concat(",");
                  tempString = tempString.concat(temp.topOfStack);
                  return new StringTokenizer(tempString, ",");
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