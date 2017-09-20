   import Query.*;
   
       public class Rule {
       
           Predicate head;
           Predicate body;
           int numberInBody;
           Rule nextRule;

            boolean interpretRule () {
               Relation myRelation;
               Relation tempRelation1;
               Relation tempRelation2;
               Relation finalRelation;
               Argument myarg;
               String argList = null;
               Predicate mypredicate;
               String leftop;
               String rightop;
               char optype;
               int counter = 0;
               int counter2 = 0;
               int previousTupleCount;
               String operator;
               String defAttributes[][] = new String[10][2];
              
               myRelation = db.isInList(head.name);
               if (myRelation != null) {
                  previousTupleCount = myRelation.TupleCount;
                  myarg = head.arg;
                  while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                  }
                  db.Rename(myRelation, argList, 0);
                   for (counter = 0, counter < myRelation.sch.attArray.length, counter++) {
                      defAttributes[counter][0] = myRelation.sch.attArray[counter].name;            
                   }           



                 mypredicate = body;
                 finalRelation = null;
                 while (mypredicate != null) {
                   if (mypredicate.typeOfPredicate == 'B') {
                       optype = 'v'
                       operator = mypredicate.name;
                       if (mypredicate.arg.argType == CONSTANT) {
                         rightop = mypredicate.arg.name;                   
                         optype = 'c'
                       }                 
                       else leftop = mypredicate.arg.name;
                       if (mypredicate.arg.nextarg.argType == CONSTANT) {
                         rightop = mypredicate.arg.nextarg.name;                   
                         optype = 'c'
                       }                 
                       else rightop = mypredicate.arg.nextarg.name;
                       finalRelation = db.Select(leftop, operator, rightop, optype, finalRelation);
                       mypredicate = mypredicate.nextPred;
                   }//end of if statement on mypredicate.typeOfPredicate == 'B'
                   
                   else  {
                     tempRelation1 = db.isInList(mypredicate.name);
                     tempRelation1 = db.Union(tempRelation1, tempRelation1);
                     if (tempRelation1 != null) {
                       myarg = body.arg;
                        while (myarg != null) {
                           argList = argList.concat(myarg.name);
                           argList = argList.concat(",");
                           for (counter = 0, counter < 10, counter++) {
                               if ((myarg.name == defAttributes[counter][0]) && (defAttributes[counter][1] == "")) {
                                  defAttributes[counter][1] = tempRelation1.sch.attArray[counter2].name;
                               }            
                            }            
                           counter2++;
                            myarg = myarg.nextarg;
                        }
                        tempRelation1 = db.Rename(tempRelation1, argList, 0);
                   
                     }
                     else {
                      myarg = body.arg;
                      while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                        }
                     db.addRelation(mypredicate.name, argList);
                     tempRelation1 = db.isInList(mypredicate.name);      
                     }          
                     
                                            
                      mypredicate = myPredicate.nextPred;        
                      if (finalRelation == null) {
                         finalRelation = tempRelation1
                      }
                      else {
                         finalRelation = db.Join(finalRelation, tempRelation1);
                       }  
                      
                   }//end of else
                   
                 }//end of while not end of predicate list     
               }
               else {
                  myarg = head.arg;
                  while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                  }
                  db.addRelation(head.name, argList);
                  myRelation = db.isInList(head.name);      
                  previousTupleCount = myRelation.TupleCount;
               }
               for (counter = 0, counter < myRelation.sch.attArray.length, counter++) {
                      defAttributes[counter][0] = myRelation.sch.attArray[counter].name;            
                }      
                mypredicate = body;
                 finalRelation = null;
                 while (mypredicate != null) {
                   if (mypredicate.typeOfPredicate == 'B') {
                       optype = 'v'
                       operator = mypredicate.name;
                       if (mypredicate.arg.argType == CONSTANT) {
                         rightop = mypredicate.arg.name;                   
                         optype = 'c'
                       }                 
                       else leftop = mypredicate.arg.name;
                       if (mypredicate.arg.nextarg.argType == CONSTANT) {
                         rightop = mypredicate.arg.nextarg.name;                   
                         optype = 'c'
                       }                 
                       else rightop = mypredicate.arg.nextarg.name;
                       finalRelation = db.Select(leftop, operator, rightop, optype, finalRelation);
                       mypredicate = mypredicate.nextPred;
                   }//end of if statement on mypredicate.typeOfPredicate == 'B'
                   
                   else  {
                     tempRelation1 = db.isInList(mypredicate.name);
                     tempRelation1 = db.Union(tempRelation1, tempRelation1);
                     if (tempRelation1 != null) {
                       myarg = body.arg;
                        while (myarg != null) {
                           argList = argList.concat(myarg.name);
                           argList = argList.concat(",");
                           for (counter = 0, counter < 10, counter++) {
                               if ((myarg.name == defAttributes[counter][0]) && (defAttributes[counter][1] == "")) {
                                  defAttributes[counter][1] = tempRelation1.sch.attArray[counter2].name;
                               }            
                            }            
                           counter2++;
                            myarg = myarg.nextarg;
                        }
                        tempRelation1 = db.Rename(tempRelation1, argList, 0);
                   
                     }
                     else {
                      myarg = body.arg;
                      while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                        }
                     db.addRelation(mypredicate.name, argList);
                     tempRelation1 = db.isInList(mypredicate.name);      
                     }          
                     
                                            
                      mypredicate = myPredicate.nextPred;        
                      if (finalRelation == null) {
                         finalRelation = tempRelation1
                      }
                      else {
                         finalRelation = db.Join(finalRelation, tempRelation1);
                       }  
                      
                   }//end of else
                   
                 }//end of while not end of predicate list     
                 finalRelation = db.Project(myRelation.sch.scheme, finalRelation);
                 myRelation = db.Union(myRelation, finalRelation);
                 argList = ""
                 for (counter = 0, counter < 10, counter++) {
                    if (defAttributes[counter][1] != "") {
                       argList = argList.concat(defAttributes[counter][1];
                       argList = argList.concat(",");
                    }
                    elseif (defAttributes[counter][0] != "" {
                       argList = argList.concat(defAttributes[counter][0];
                       argList = argList.concat(",");   
                    }
                    else {
                       break;
                 } 
                  argList = argList.substring(0, argList.length() - 1);
                  myRelation = db.Rename(myRelation, argList, 0);
                  if (myRelation.TupleCount == previousTupleCount) {
                     return false;
                  }
                  else {
                     return true;
                  }
            
            }//end of interpretRule                                                      
           void Show() {
              Predicate tempPred;
               Rule tempRule = this;
               int number = 1;
               System.out.println("Rules");
               while (tempRule != null) {
               tempPred = tempRule.body;
                System.out.println(number);
                   printSpaces();
                   System.out.println("Head");
                   tempRule.head.Show();
                   printSpaces();
                   System.out.println("Body");
                   while (tempPred != null) {
                       tempPred.Show();
                       tempPred = tempPred.nextPred;                        
                   }
                    try {
               System.in.read();
               System.in.read();
               
               }
               catch (IOException ie) {
               
               }              
                   tempRule = tempRule.nextRule;
                   number++;
               }
                            
              }//end of Show method
               
               void printSpaces ()  {
                   System.out.print(" ");
                   System.out.print(" ");
                   System.out.print(" ");                 
               }//end of printSpaces method
           
       
       }//end of Rule class
