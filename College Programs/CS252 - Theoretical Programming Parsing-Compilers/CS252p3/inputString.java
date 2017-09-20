import java.io.*;

public class inputString {

   public String dataString;
   int stringIndex;
   
   inputString() {
      dataString = "";
      stringIndex = 0;
   }
   
   private boolean validInput(char c, String inputStr) {
      if (inputStr.indexOf(c) != -1) {
         return true;
      }
      else if ((int) c == 13) {
            return true;
         }
         else { 
            return false;
         }
      
   }//end of validInput function       
   
   public char getNextCharacter() {
      if ((stringIndex == -1) || (stringIndex == dataString.length())) {
         return 'E';      
      }
      else {
         return dataString.charAt(stringIndex++); 
      }
   }//end of getNextCharacter
   
   public void reset() {
      stringIndex = 0;
      dataString = "";
   }//end of reset function
   
   public void getInputString() {
      char inputChar = ' ';
       try {
      do {
         if (inputChar != ' ') {
         dataString = dataString.concat(String.valueOf(inputChar)); 
         }
        
         inputChar = (char) System.in.read();
         
                           
      } while (inputChar != '\n');
      }
      catch (IOException e) {
         
         }
   }//end of getInputString function

}//end of inputString class
