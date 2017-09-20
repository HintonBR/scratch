    import java.io.*;       
     
     
       
                    
   public class Argument {
               String name;
               char argType;
               Argument nextarg;
               
                     public final static char CONSTANT = 'v';
      public final static char VARIABLE = 'a';  
               
               Argument(String mname, char margType) {
                   name = mname;
                   argType = margType;
               }//end of Argument constructor
               
               void Show() {
                   String tempString;
                   if (argType == CONSTANT) {
                      tempString = "Constant";
                   }
                   else {
                      tempString = "Variable";
                   }
                   System.out.println("Argument  " + name + "  Type  " + tempString);
                }//end of Show method
     }//end of Argument class
