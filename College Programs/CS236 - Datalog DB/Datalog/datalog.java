import java.io.*;
import LexicalAnalyzer.*;
import Token.*;
import RDMS.*;
import Query.*;


public class Datalog {

       public Datalog( String filename) {
            lex = new LexicalAnalyzer(filename);
            tok = lex.currentToken();
            db = new RDMS();
            ruleList = new Rule();
            queryList = new Query(null);
                   }
      //Enumeration of possible token types
      public final static int COLON       = 0;
      public final static int COLON_DASH  = 1;
      public final static int COMMA       = 2;
      public final static int LEFT_PAREN  = 3;
      public final static int PERIOD      = 4;
      public final static int Q_MARK      = 5;
      public final static int RIGHT_PAREN = 6;
      public final static int EQ          = 7;
      public final static int NE          = 8;
      public final static int GT          = 9;
      public final static int GE          = 10;
      public final static int LT          = 11;
      public final static int LE          = 12;
      public final static int STRING      = 13;
      public final static int SCHEMES     = 14;
      public final static int FACTS       = 15;
      public final static int RULES       = 16;
      public final static int QUERIES     = 17;
      public final static int ID          = 18;
      public final static int EOF         = 19;
      public final static int UNDEFINED   = 20;
      public final static String COMPARATORSTRING = "<<==!=>=>";
      public final static char SIMPLEPREDICATE = 'S';
      public final static char BUILTINPREDICATE = 'B';
      public final static char CONSTANT = 'v';
      public final static char VARIABLE = 'a';
      
      //Declaration of variables
      LexicalAnalyzer lex;
      Token tok;
       RDMS db;
       Rule ruleList;
       Query queryList;
       

   
       public class Rule {
       
           Predicate head;
           Predicate body;
           int numberInBody;
           Rule nextRule;

            boolean interpretRule () {
               Relation myRelation;
               RelationList pointToRelation;
               RelationList fake;
               Relation tempRelation1;
               Relation tempRelation2;
               Relation finalRelation;
               Argument myarg;
                              String argList = "";
               String myString = ""               ;
               Predicate mypredicate;
               String leftop = "";
               String rightop = "";
               char optype;
		   int attcounter = 0; 
               int counter = 0;
               int counter2 = 0;
               int previousTupleCount;
               String operator;
               String defAttributes[][] = new String[10][2];

               pointToRelation = db.isInList(head.name);
               if (pointToRelation != null) {
                  myRelation = pointToRelation.rel;
               }
               else {
               myRelation = null;
               }
               if (myRelation != null) {
                  previousTupleCount = myRelation.TupleCount;
                  myarg = head.arg;
                 while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                  }
                  argList = argList.substring(0, argList.length() - 1);
                  myRelation = db.Rename(myRelation, argList, 0);
                  argList = "";
                   for (counter = 0; counter < myRelation.sch.attArray.length; counter++) {
                      defAttributes[counter][0] = myRelation.sch.attArray[counter].name;            
                   }           

                 mypredicate = body;
                 finalRelation = null;
                 while (mypredicate != null) {
                   if (mypredicate.typeOfPredicate == 'B') {
                       optype = VARIABLE;
                       operator = mypredicate.name;
                       if (mypredicate.arg.argType == CONSTANT) {
                         rightop = mypredicate.arg.name;                   
                         optype = CONSTANT;
                       }                 
                       else leftop = mypredicate.arg.name;
                         if (mypredicate.arg.nextarg.argType == CONSTANT) {
                         rightop = mypredicate.arg.nextarg.name;                   
                         optype = CONSTANT;
                       }                 
                       else if (leftop == "") {
                          leftop = mypredicate.arg.nextarg.name;
                       }
                       else {
                          rightop = mypredicate.arg.nextarg.name;
                       }
                       finalRelation = db.Select(leftop, operator, rightop, optype, finalRelation);
                       mypredicate = mypredicate.nextPred;
                   }//end of if statement on mypredicate.typeOfPredicate == 'B'
                   
                   else  {
                   fake = db.isInList(mypredicate.name);                     
                   if (fake != null) {
                   tempRelation1 = fake.rel;
                   try {
                     tempRelation1 = db.Union(tempRelation1, tempRelation1);
                     }
                     catch (IOException e) {
                     }
                     }
                     else {
                        tempRelation1 = null;
                     }
                     if (tempRelation1 != null) {
                       myarg = mypredicate.arg;
                        while (myarg != null) {
				   if (myarg.argType == CONSTANT) {
                              myString = tempRelation1.sch.attArray[attcounter].name;
                              tempRelation1 = db.Select(myString, "=", myarg.name, CONSTANT, tempRelation1);
                           }                           
                           
                           attcounter++;	
				   if (myarg.argType == CONSTANT) {
					argList = argList.concat(myString);
				   }	
                           else {
                           argList = argList.concat(myarg.name);
			         }		
                           argList = argList.concat(",");
                          
                                     for (counter = 0; counter < 10; counter++) {
                               if ((myarg.name == defAttributes[counter][0]) && (defAttributes[counter][1] == "")) {
                                  defAttributes[counter][1] = tempRelation1.sch.attArray[counter2].name;
                               }            
                            }            
                           counter2++;
                            myarg = myarg.nextarg;
                        }
                         argList = argList.substring(0, argList.length() - 1);
                        tempRelation1 = db.Rename(tempRelation1, argList, 0);
                         argList = "";
                   
                     }
                     else {
                      myarg = mypredicate.arg;
                      while (myarg != null) {
if (myarg.argType == CONSTANT) {
                              myString = tempRelation1.sch.attArray[attcounter].name;
                              tempRelation1 = db.Select(myString, "=", myarg.name, CONSTANT, tempRelation1);
                           }                           
                           
                           attcounter++;	
				   if (myarg.argType == CONSTANT) {
					argList = argList.concat(myString);
				   }	
                           else {
                           argList = argList.concat(myarg.name);
			         }		
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                        }
                        argList = argList.substring(0, argList.length() - 1);
                     db.addRelation(mypredicate.name, argList);
                     argList = "";
                      fake = db.isInList(mypredicate.name);                     
                   tempRelation1 = fake.rel;
                     }          
		                     
            	    
                      mypredicate = mypredicate.nextPred;        
                      if (finalRelation == null) {
                         finalRelation = tempRelation1;
                      }
                      else {
                      try {
                         finalRelation = db.Join(finalRelation, tempRelation1);
                      }
                      catch (IOException e) {
                      
                      }
                       }  
                      
                   }//end of else
			attcounter = 0;	                                	
                 }//end of while not end of predicate list     
               }
               else {
                   myarg = head.arg;
                  while (myarg != null) {
                     argList = argList.concat(myarg.name);
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                  }
                  argList = argList.substring(0, argList.length() - 1);
                  db.addRelation(head.name, argList);
                  argList = "";
                  pointToRelation = db.isInList(head.name);      
                  myRelation = pointToRelation.rel;
                  previousTupleCount = myRelation.TupleCount;
                  
               }
               for (counter = 0; counter < myRelation.sch.attArray.length; counter++) {
                      defAttributes[counter][0] = myRelation.sch.attArray[counter].name;            
                }      
                mypredicate = body;
                 finalRelation = null;
                 while (mypredicate != null) {
                   if (mypredicate.typeOfPredicate == 'B') {
                       optype = VARIABLE;
                       operator = mypredicate.name;
                       if (mypredicate.arg.argType == CONSTANT) {
                         rightop = mypredicate.arg.name;                   
                         optype = CONSTANT;
                       }                 
                       else leftop = mypredicate.arg.name;
                       if (mypredicate.arg.nextarg.argType == CONSTANT) {
                         rightop = mypredicate.arg.nextarg.name;                   
                         optype = CONSTANT                         ;
                       }                 
                       else if (leftop == "") {
                          leftop = mypredicate.arg.nextarg.name;
                       }
                       else {
                          rightop = mypredicate.arg.nextarg.name;
      
												}
                       finalRelation = db.Select(leftop, operator, rightop, optype, finalRelation);
                       mypredicate = mypredicate.nextPred;
                   }//end of if statement on mypredicate.typeOfPredicate == 'B'
                   
                   else  {
                     fake = db.isInList(mypredicate.name);
                      if (fake != null) {
                   tempRelation1 = fake.rel;
                   try {
                     tempRelation1 = db.Union(tempRelation1, tempRelation1);
                     }
                     catch (IOException e) {
                     }
                     }
                     else {
                        tempRelation1 = null;
                     }                    
                     if (tempRelation1 != null) {
                       myarg = mypredicate.arg;
                        while (myarg != null) {
if (myarg.argType == CONSTANT) {
                              myString = tempRelation1.sch.attArray[attcounter].name;
                              tempRelation1 = db.Select(myString, "=", myarg.name, CONSTANT, tempRelation1);
                           }                           
                           
                           attcounter++;	
				   if (myarg.argType == CONSTANT) {
					argList = argList.concat(myString);
				   }	
                           else {
                           argList = argList.concat(myarg.name);
			         }		
                           argList = argList.concat(",");
                           for (counter = 0; counter < 10; counter++) {
                               if ((myarg.name == defAttributes[counter][0]) && (defAttributes[counter][1] == "")) {
                                  defAttributes[counter][1] = tempRelation1.sch.attArray[counter2].name;
                               }            
                            }            
                           counter2++;
                            myarg = myarg.nextarg;
                        }
                        argList = argList.substring(0, argList.length() - 1);
                        tempRelation1 = db.Rename(tempRelation1, argList, 0);
                        argList = "";            
                     }
                     else {
                      myarg = mypredicate.arg;
                      while (myarg != null) {
if (myarg.argType == CONSTANT) {
                              myString = tempRelation1.sch.attArray[attcounter].name;
                              tempRelation1 = db.Select(myString, "=", myarg.name, CONSTANT, tempRelation1);
                           }                           
                           
                           attcounter++;	
				   if (myarg.argType == CONSTANT) {
					argList = argList.concat(myString);
				   }	
                           else {
                           argList = argList.concat(myarg.name);
			         }		
                     argList = argList.concat(",");
                     myarg = myarg.nextarg;
                        }
                        argList = argList.substring(0, argList.length() - 1);
                     db.addRelation(mypredicate.name, argList);
                     argList = "";
                     fake = db.isInList(mypredicate.name);
                     tempRelation1 = fake.rel;
                     }          
                     
			    	                                            
                      mypredicate = mypredicate.nextPred;        
                      if (finalRelation == null) {
                         finalRelation = tempRelation1;
                      }
                      else {
                      try {
                         finalRelation = db.Join(finalRelation, tempRelation1);
 
                         }
                         catch (IOException e) {
                         }
                       }  
                      
                   }//end of else
                  attcounter = 0;
                 }//end of while not end of predicate list     
                 finalRelation = db.Project(myRelation.sch.scheme, finalRelation);
                 try {
                 myRelation = db.Union(myRelation, finalRelation);
                 }
                 catch (IOException e) {
                 }
                 argList = "";
                  for (counter = 0; counter < 10; counter++) {
                  if ((defAttributes[counter][1] != "") && (defAttributes[counter][1] != null)) {
                      argList = argList.concat(defAttributes[counter][1]);
                       argList = argList.concat(",");
                  }
                    else if ((defAttributes[counter][0] != "") && (defAttributes[counter][0] != null)) {
                       argList = argList.concat(defAttributes[counter][0]);
                       argList = argList.concat(",");   
                      }
                    else {
                       break;
                 } 
                 }
                  argList = argList.substring(0, argList.length() - 1);
                  myRelation = db.Rename(myRelation, argList, 0);
                  myRelation.nameOfRelation = head.name;
                  pointToRelation.rel = myRelation;                  
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
       

       
void buildRules() {
   boolean Changes;
   Changes = true;
   while (Changes == true) { 
   Changes = false;
   Rule myRule = ruleList;
   while (myRule != null) {
      Changes = myRule.interpretRule();
      myRule = myRule.nextRule;
   }
   }
}//end of buildRules method

   void answerQueries() {

                  RelationList pointToRelation;
                  Predicate mypredicate;
                  Relation  myRelation; 
                  Argument myarg;
                  String myString;
                  Query myquery = queryList;
                  int counter = 0;

                
                   System.out.println("**************************************");
                   System.out.println("Query Result");                  
                  while (myquery != null) {   
                  mypredicate = myquery.queryPredicate;
                  pointToRelation = db.isInList(mypredicate.name);
                   if (pointToRelation != null) {
                       myRelation = pointToRelation.rel;
                        myarg = mypredicate.arg;
                        while (myarg != null) {
                           if (myarg.argType == CONSTANT) {
                              myString = myRelation.sch.attArray[counter].name;
                              myRelation = db.Select(myString, "=", myarg.name, CONSTANT, myRelation);
                           }                           
                           myarg = myarg.nextarg;
                           counter++;
                        }                               
                   }
                  else {
                      System.out.println("Query is on a relation that does not exist");
                      myRelation = null;
                      
                   }
                  
                   myRelation.Show();
			counter=0;         
                   myquery = myquery.nextQuery;
                   }
                   System.out.println("**************************************");
}//end of answerQueries method
       
//Beginning of collection of methods that parses a DatalogProgram - makes sure that it is syntatically correct - build relations from facts and defines Queries and Rules from the program                       
        private boolean isDatalogProgram() {
            String tempString;
            tempString = tok.getValue();
            if (tempString.equals("Schemes"))  {
                lex.nextToken();
                tok = lex.currentToken();
                tempString = tok.getValue();
                if (tempString.equals(":")) {
                    lex.nextToken();
                    tok = lex.currentToken();                
                    if (isSchemeList()) {
                        tempString = tok.getValue();
                        if (tempString.equals("Facts")) {
                            lex.nextToken();
                            tok = lex.currentToken();
                            tempString = tok.getValue();
                            if (tempString.equals(":")) {
                                lex.nextToken();
                                tok = lex.currentToken();
                                if (isFactList()) {
                                    tempString = tok.getValue();
                                    if (tempString.equals("Rules")) {
                                        lex.nextToken();
                                        tok = lex.currentToken();
                                        tempString = tok.getValue();
                                        if (tempString.equals(":")) {
                                            lex.nextToken();
                                            tok = lex.currentToken();
                                            if (isRuleList()) {
                                                tempString = tok.getValue();
                                                if (tempString.equals("Queries")) {
                                                    lex.nextToken();
                                                    tok = lex.currentToken();
                                                    tempString = tok.getValue();
                                                    if (tempString.equals(":")) {
                                                        lex.nextToken();
                                                        tok = lex.currentToken();
                                                        if (isQueryList()) {
                                                            if (tok.getType() == EOF) {
                                                                return true;
                                                            }
                                                            else {
                                                                return false;
                                                            }
                                                        }
                                                        else {
                                                            return false;
                                                        }
                                                    }
                                                    else {
                                                        return false;
                                                    }
                                                }
                                                else {
                                                    return false;
                                                }
                                            }
                                            else {
                                                return false;
                                            }
                                        }
                                        else {
                                            return false;
                                        }
                                    }
                                    else {
                                        return false;
                                    }
                                    
                                }
                                else {
                                    return false;
                                }
                            }
                            else {
                                return false;
                            }
                        }
                        else {
                            return false;
                        }
                    
                    }
                    else {
                        return false;
                     }
                }
                else{
                    return false;
                 }   
            }
            else {
                return false;
            }
        }//end of isDatalogProgram

        private boolean isSchemeList() {
            if (isScheme()) {
                if (isSchemeListTail()) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }//end of isSchemelist

        private boolean isSchemeListTail() {
        
            if (tok.getType() == FACTS) {
                return true;
            }        
            else if (isSchemeList()) {
                return true;
            }
            else {
                return false;
            }

        }//end of isSchemeListTail

        private boolean isScheme() {
            String tempString2;
            String tempString;
            String nameOfRelation;
            String scheme;
            if (tok.getType() == ID) {
                nameOfRelation = tok.getValue();
                lex.nextToken();
                tok = lex.currentToken();
                tempString = tok.getValue();
                if (tempString.equals("(")) {
                    lex.nextToken();
                    tok = lex.currentToken();
                    tempString2 = isAttributeList();
                    if (tempString2 != null) {
                        scheme = tempString2;
                        tempString = tok.getValue();
                        if (tempString.equals(")")) {
                            lex.nextToken();
                            tok = lex.currentToken();
                            db.addRelation(nameOfRelation, tempString2);
                            return true;    
                        }
                        else {
                            return false;
                        }
                    }
                                        else {
                        return false;
                    }
                }
                else {
                    return false;
                }
                
            }
            else {
                return false;
            }
        }//end of isScheme


        private String isAttributeList() {
           String tempString2;
           String tempString = isAttribute();
            if (tempString != null) {
                tempString2  = isAttributeListTail();
                if (tempString2 != null) {
                  if (tempString2.equals(")") == false) {
                   tempString = tempString.concat(tempString2);
                  } 
                  return tempString;
                }
                else {
                    return null;
                }
            }
                   return null;            
                           }//end of isAttributeList


        private String isAttributeListTail() {
            String tempString2;
            String tempString = tok.getValue();
            if (tempString.equals(",")) {
                lex.nextToken();
                tok = lex.currentToken();
                tempString2 = isAttributeList();
                if (tempString2 != null) {
                   tempString = tempString.concat(tempString2);
                   return tempString;
                }
                else {
                    return null;
                }
            }
            else {
                tempString = tok.getValue();
                return tempString;
            }
            

        }//end of isAttributeListTail


        private String isAttribute() {
            String tempString ;
            if (tok.getType() == ID) {
                tempString = tok.getValue();               
                lex.nextToken();
                tok = lex.currentToken();
                return tempString;
            }
            else {
                return null;
            }               
        }//end of isAttribute

        private boolean isFactList() {
          if (isFact()) {
                if (isFactListTail()) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;            
            }
            
        }//end of isFactList

        private boolean isFactListTail() {
            if (tok.getType() == RULES) {
                return true;
            }
            else if (isFactList()) {
                return true;
            }
            else {
                 return false;   
            }
        }//end of isFactListTail

        private boolean isFact(){
            String tempString;
            String nameOfRelation;
            String factList;
            
            if (tok.getType() == ID) {
                nameOfRelation = tok.getValue();
                lex.nextToken();
                tok = lex.currentToken();
                tempString = tok.getValue();
                if (tempString.equals("(")) {                
                    lex.nextToken();
                    tok = lex.currentToken();
                    factList = isConstantList();
                    if (db.addFact(nameOfRelation, factList)) {
                        tempString = tok.getValue();
                        if (tempString.equals(")")) {
                            lex.nextToken();
                            tok = lex.currentToken();
                            tempString = tok.getValue();
                            if (tempString.equals(".")) {
                                lex.nextToken();
                                tok = lex.currentToken();
                                return true;
                            }
                            else {
                                return false;
                            }    
                        }
                        else {
                            return false;
                        }
                    }
                    else {
                        return false;
                    }
                }    
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }//end of isFact

        private String isConstantList() {
            String tempString;
            String tempString2;            
            if (tok.getType() == STRING) {
                tempString = tok.getValue();
                lex.nextToken();
                tok = lex.currentToken();
                tempString2 = isConstantListTail();
                if (tempString2 != null) {
                     if (tempString2.equals(")") == false) {                
                        tempString = tempString.concat(tempString2);
                    }
                    return tempString;
                }
                else {
                    return null;
                }
            }
            else {
                return null;
            }
        }//end of isConstantList


        private String isConstantListTail(){    
            String tempString2;
            String tempString = tok.getValue();
            if (tempString.equals(",")) {
                lex.nextToken();
                tok = lex.currentToken();
                tempString2 = tok.getValue();
                if (tempString2.equals(")")) {
                    return null;
                }
                tempString2 = isConstantList();
                if (tempString2 != null) {
                    tempString = tempString.concat(tempString2);
                    return tempString;
                }
                else {
                    return null;
                }
            }
            else {
                return tempString;
            }

        }//end of isConstantListTail


        private boolean isRuleList() {
            if (isRule()) {
                if (isRuleListTail()) {
                     return true;   
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }//end of isRuleList


        private boolean isRuleListTail() {
           
           if (tok.getType() == QUERIES) {
               return true;
           } 
           else if (isRuleList()) {
                return true;
            }
            else {
                return false;
            }
        }//end of isRuleListTail

        private boolean isRule() {
            String tempString;
            Rule tempRule = ruleList;
            if (tempRule.head != null) {
            while (tempRule.nextRule != null) {
                tempRule = tempRule.nextRule;
            }
            tempRule.nextRule = new Rule();
            tempRule = tempRule.nextRule;
            }
            if (isHead(tempRule)) {
                tempString = tok.getValue();
                if (tempString.equals(":-")) {
                    lex.nextToken();
                    tok = lex.currentToken();
                    if (isBody(tempRule)) {
                        tempString = tok.getValue();
                        if (tempString.equals(".")) {
                            lex.nextToken();
                            tok = lex.currentToken();
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    else {
                        return false;
                    }         
                }
                else {
                    return false;
                }                
            }
            else {
                return false;
            }
        }//end of isRule

        private boolean isHead(Rule tempRule) {
           Predicate myPredicate = isSimplePredicate();
           if (myPredicate != null) {
                tempRule.head = myPredicate;
                return true;
            }
            else {
                return false;
            }
        }//end of isHead

        private Predicate isSimplePredicate() {
            String tempString;
            String nameOfPredicate;
            Predicate myPredicate;
            
            if (tok.getType() == ID) {
                nameOfPredicate = tok.getValue();
                lex.nextToken();
                tok = lex.currentToken();
                tempString = tok.getValue();
                if (tempString.equals("(")) {
                    lex.nextToken();
                    tok = lex.currentToken();
                    myPredicate  = new Predicate(nameOfPredicate, SIMPLEPREDICATE);
                    if (isArgumentList(myPredicate)) {
                        tempString = tok.getValue();
                        if (tempString.equals(")")) {
                            lex.nextToken();
                            tok = lex.currentToken();
                            return myPredicate;
                        }
                        else {
                            return null;
                        }
                    }
                    else {
                        return null;
                    }
                }
                else {
                    return null;
                }
            }
            else {
                return null;
            }
        }//end of isSimplePredicate
        
            private boolean isArgumentList(Predicate tempPredicate) {
        
            if (isArgument(tempPredicate)) {
                if (isArgumentListTail(tempPredicate)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }

        }//end of isArgumentList


        private boolean isArgumentListTail(Predicate tempPredicate) {
           String tempString = tok.getValue();
            if (tempString.equals(",")) {
                lex.nextToken();
                tok = lex.currentToken();
                if (isArgumentList(tempPredicate)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return true;
            }
            
        }//end of isArgumentListTail


        private boolean isArgument(Predicate tempPredicate) {
            String tempString;
            Argument newArg;
             Argument myArg = null             ;
             if (tempPredicate.arg != null) {
               myArg = tempPredicate.arg;
             while (myArg.nextarg != null) {
                myArg = myArg.nextarg;
             }
             }
             if (tok.getType() == STRING) {
                newArg = new Argument(tok.getValue(), CONSTANT);       
                 if (myArg == null) {
                   tempPredicate.arg = newArg;
                }
                else {
                myArg.nextarg = newArg;
                }
                lex.nextToken();
                tok = lex.currentToken();
                return true;
            }
            else {
                newArg = isVariable();
                if (newArg != null) {
                    if (myArg == null) {
                       tempPredicate.arg = newArg;
                    }
                    else { 
                    myArg.nextarg = newArg;
                    }                   
                    return true;
                }
                else {
                    return false;
                }
            }
            
        }//end of isArgument


        private Argument isVariable() {
           Argument myArgument;
            if (tok.getType() == ID) {
               myArgument = new Argument(tok.getValue(), VARIABLE);
                lex.nextToken();
                tok = lex.currentToken();
                return myArgument;
            }
            else {
                return null;
            }
        }//end of isVariable

        private boolean isBody(Rule tempRule) {
            if (isPredicateList(tempRule)) {
                return true;
            }
            else {
                return false;
            } 
        }//end of isBody

        private boolean isPredicateList(Rule tempRule) {
            if (isPredicate(tempRule)) {
                if (isPredicateListTail(tempRule)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
                            }
        }//end of isPredicateList

        private boolean isPredicateListTail(Rule tempRule) {
           String tempString = tok.getValue();
            if (tempString.equals(",")) {
                lex.nextToken();
                tok = lex.currentToken();
                if (isPredicateList(tempRule)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return true;
            }

        }//end of isPredicateListTail

        private boolean isPredicate(Rule tempRule) {
            Predicate followPredicate = null            ;
            Predicate myPredicate = tempRule.body;
            if (myPredicate != null) {
            while (myPredicate.nextPred != null) {
                myPredicate = myPredicate.nextPred;
            }
            myPredicate.nextPred = isSimplePredicate();
            followPredicate = myPredicate;
            myPredicate = myPredicate.nextPred;
            }
            else {
               tempRule.body = isSimplePredicate();
               myPredicate = tempRule.body;
            }
            if (myPredicate != null) {
                tempRule.numberInBody++;    
                return true;
            }
            else {
                if (tempRule.body != null) {
                   myPredicate = followPredicate;
                   myPredicate.nextPred = isBuiltInPredicate();
                }
                else {
                   tempRule.body = isBuiltInPredicate();
                   myPredicate = tempRule.body;
                }
                
                              if (myPredicate != null) {
                    tempRule.numberInBody++;
                    return true;
                }
                else {
                    return false;
                }
             }
            
        }//end of isPredicate

        private Predicate isBuiltInPredicate() {
            String tempString;
            Predicate myPredicate;
            tempString = isComparator();
            if (tempString != null) {
                myPredicate = new Predicate(tempString, BUILTINPREDICATE);
                          tempString = tok.getValue();
               if (tempString.equals("(")) {
                   lex.nextToken();
                   tok = lex.currentToken();
                   if (isArgument(myPredicate)) {
                       tempString = tok.getValue();
                       if (tempString.equals(",")) {
                           lex.nextToken();
                           tok = lex.currentToken();
                           if (isArgument(myPredicate)) {
                               tempString = tok.getValue();
                               if (tempString.equals(")")) {
                                   lex.nextToken();
                                   tok = lex.currentToken();
                                   return myPredicate;
                               }
                               else {
                                   return null;
                               }
                           }
                           else {
                               return null;
                           }
                       }
                       else {
                           return null;
                       }
                   }
                   else {
                       return null;
                   }
               }
               else {
                   return null;
               }     
            }
            else {
                return null;
            }
        }//end of isBuiltInPredicate


        private String isComparator() {
        String tempString = tok.getValue();
        if (COMPARATORSTRING.indexOf(tempString) != -1) {
               lex.nextToken();
               tok = lex.currentToken();
               return tempString;  
        }
         else {
	return null;		
            }	
        }//end of isComparator


        private boolean isQueryList() {
        
            if (isQuery()) {
                if (isQueryListTail()) {
                    return true;    
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }

        }//end of isQueryList

        private boolean isQueryListTail() {
        
            if (tok.getType() == EOF) {
                return true;
            }
            else if (isQueryList()) {
                return true;
            }
            else {
                return false;
            }
        }//end of isQueryListTail


        private boolean isQuery() {
            String tempString;    
            Query myQuery = queryList;
            Predicate myPredicate = isSimplePredicate();        
            if (myPredicate != null) {
                 if (myQuery.queryPredicate  != null) {
                    while (myQuery.nextQuery != null) {
                       myQuery = myQuery.nextQuery;
                    }
                    myQuery.nextQuery = new Query(myPredicate);
                 }
                 else {
                    queryList = new Query(myPredicate);
                 }
                 tempString = tok.getValue();
                 if (tempString.equals("?")) {
                     lex.nextToken();
                     tok = lex.currentToken();
                     return true;
                 }
                 else {
                     return false;
                 }                
            }
            else {
                return false;
            }
        }//end of isQuery
        
        void Show() {
          db.showRelationList(); 
          ruleList.Show();
          queryList.Show();            
        }


        


        public static void main(String args[]) throws IOException  {
            Datalog dg;
            boolean result;
            try {  
            dg = new Datalog(args[0]);
            }
            catch (Exception e) {
                System.out.println("You have either forgotten to enter in a filename or the filename is invalid");
                return;
            }

            result = dg.isDatalogProgram();
            if (result == true) {
                System.out.println("Success");            
                System.in.read();
                System.in.read();
                dg.buildRules();
                dg.Show();                
                dg.answerQueries();
                 }
            else {
                System.out.println("Failure");
            }
                        
}//end of main method

}//end of Datalog class
