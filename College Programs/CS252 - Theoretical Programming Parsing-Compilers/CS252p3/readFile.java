import java.io.*;
import java.util.*;

class readFile {
   private RandomAccessFile myFile;
   private boolean endOfFile;
   
   public readFile(String filename) {

     	try {
		myFile = new RandomAccessFile(filename, "rw");	
	}
	catch (IOException e) {
	   System.out.println("The file failed to open properly");
	}
    
   
   }//end of readFile constructor 
   
   public boolean eof() {
      return endOfFile;
   }
   
   public void close() {
      if (myFile != null) {
      try {
         myFile.close();
      }
      catch (IOException e) {
      
      }
      }
   }//end of close method

   public String getLine() {
      String temp = null;
      if (myFile != null) {
        try {
         temp = myFile.readLine();
         }
         catch (IOException e) {
         }
         if (temp == null) {
            endOfFile = true;
         }
         return temp;
      }   
      else {
         endOfFile = true;      
         return null;
      }
   }//end of getLine method




}//end of readFile class