import node;
import leaf;

public class leafsum {

   node head;
   
   public leafsum() {
      head = new node("first", null, null);
      head.field2 = new node("second", null, null);
      head.field3 = new node("third", null, null);
      ((node) head.field2).field2 = new leaf(5);
      ((node) head.field2).field3 = new leaf(5);
      ((node) head.field3).field2 = new leaf(5);
      ((node) head.field3).field3 = new leaf(5);
   
   
   }
   int sum(tree test) {
     
   return 1;
   }
   public static void main(String args[]) {
      leafsum myObj = new leafsum();      
      System.out.println(Object.instanceOf(myObj.head)); 
      
   }






}
