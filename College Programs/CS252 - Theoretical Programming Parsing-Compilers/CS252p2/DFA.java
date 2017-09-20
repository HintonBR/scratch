import java.io.*;
import java.util.*;

class DFA {

String startState;
String finalState[] = new String[5];
int numberOfFinalStates = 0;
String inputSymbols[] = new String[6];
int numberOfInputs = 0;
String grammer[][] = new String[15][7];
String finalgrammer[][] = new String[15][7];
int numberInFinalGrammer = 0;
int numberOfStates = 0;

int insert(String myString) {
   if (isIn(myString) == -1) {
   grammer[numberOfStates][0] = myString;
   numberOfStates += 1;
   return (numberOfStates -1);
   }
   return -1;

}

int isIn(String myString) {
   int counter;
   for (counter = 0; counter < numberOfStates; counter++) {
      if (grammer[counter][0].equals(myString)) {
         return counter;
      }
   }
   return -1;

}

int insertInput(String myString) {
   inputSymbols[numberOfInputs] = myString;
   numberOfInputs += 1;
   return (numberOfInputs - 1);

}

int inputIsIn(String myString) {
   int counter;
   for (counter = 0; counter < (inputSymbols.length); counter++) {
      if (inputSymbols[counter] != null) {
      if (inputSymbols[counter].equals(myString)) {
         return counter;
      }
      }
   }
   return -1;

}


int insertFinalState(String myString) {
   finalState[numberOfFinalStates] = myString;
   numberOfFinalStates += 1;
   return 1;      

}

int isFinalState(String myString) {
   int counter;
   for (counter = 0; counter < numberOfFinalStates; counter++) {
      if (finalState[counter].equals(myString)) {
         return counter;
      }
   }
   return -1;

}

String checkForLambdaInputs(int index) {
   return grammer[index][6];
}

void insertIntoFinalGrammer(String myString) {
   finalgrammer[numberInFinalGrammer][0] = myString;
   numberInFinalGrammer++;
   return;
}

boolean isInFinalGrammer(String myString) {
   int counter;
   for (counter = 0; counter < numberInFinalGrammer; counter++) {
      if (finalgrammer[counter][0].equals(myString) == true) {
         return true;
      }
   }
   return false;
}

void removeCommas() {
   int counter = 0, counter2 = 0;
   for (counter = 0; counter < numberInFinalGrammer; counter++) {
      for (counter2 = 0; counter2 <= numberOfInputs; counter2++) {
        if (finalgrammer[counter][counter2] != null) {
         finalgrammer[counter][counter2] = removeFromString(finalgrammer[counter][counter2], ",");         
       }  
      }
   }

}//end of remove Commas

void output() {
int counter;
int counter2 = 0;
System.out.println("Printing out the DFA");
System.out.println(finalgrammer[0][0]);
finalgrammer[0][6] = "T";
for (counter = 0; counter < numberInFinalGrammer; counter++) {
   while (counter2 < finalgrammer[counter][0].length()) {
      if ((counter2+1)  == finalgrammer[counter][0].length()) {
          if ((isFinalState(finalgrammer[counter][0].substring(counter2)) != -1)) {
         finalgrammer[counter][6] = "T";
          System.out.print(finalgrammer[counter][0]);
          System.out.print(" ");
          break;  
         }     
      }
      else {
      if ((isFinalState(finalgrammer[counter][0].substring(counter2, counter2 + 1)) != -1)) {
         finalgrammer[counter][6] = "T";
          System.out.print(finalgrammer[counter][0]);
          System.out.print(" ");
          break;  
      }
      }
       counter2++;
   }
   counter2 = 0;
}
System.out.println();
for (counter = 0; counter < numberInFinalGrammer; counter++) {
   if (finalgrammer[counter][6] == null) {
       System.out.print(finalgrammer[counter][0]);
       System.out.print(" ");
   }
}
System.out.print("L");
System.out.println();
for (counter = 0; counter < numberInFinalGrammer; counter++) {
   for (counter2 = 1; counter2 <= numberOfInputs; counter2++) {
      System.out.print(finalgrammer[counter][0]);
      System.out.print(inputSymbols[counter2 -1]);
      if (finalgrammer[counter][counter2].equals("") == false) {
      System.out.print(finalgrammer[counter][counter2]);
      }
      else {
         System.out.print("L");
      }
      System.out.println();
   }
}

for (counter = 0; counter < numberOfInputs; counter++) {
   System.out.print("L");
   System.out.print(inputSymbols[counter]);
   System.out.print("L");
   System.out.println();
}

}//end of output method;


String removeFromString(String myString, String stringToRemove) {
   int remove;
   String mainString = myString;
   String tempString;
   int counter = 0;
   
   int sIndex;
   
   if (mainString == null) {
      return null;
   }
   
   while (counter != -1) {
      remove = mainString.indexOf(stringToRemove);
      if (remove != -1) {
      tempString = mainString.substring(remove + stringToRemove.length());
      mainString = mainString.substring(0, remove);
      mainString = mainString.concat(tempString);
      }
      counter = mainString.indexOf(stringToRemove);
      
   }
   return mainString;
}

public static void main(String args[]) throws IOException {
	int grammerIndex;
	int index;
	String tempState;
	String finals = "";
	String temp = "";
	int positionInString = 0;
	int position = 0;
	int positionInString2 = 0;
	StringTokenizer stringTok;
	String tempSymbol;
	RandomAccessFile inFile = null;
	DFA myObject = new DFA();
	int counter = 0;
	int counter2 = 0;
	final String LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
	final String UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	myObject.inputSymbols[5] = "l";
	//open file for input
		
	
	System.out.println("We are getting the startstate");
	//get startstate from firstline in the array
	myObject.startState = inFile.readLine();
	myObject.startState = myObject.removeFromString(myObject.startState, "/r/n");
	myObject.insert(myObject.startState);
	
	System.out.println("We are parsing the final states now");
	//get final states and put them in the array
	tempState = inFile.readLine();
	tempState = myObject.removeFromString(tempState, "/r/n");
	while (counter < tempState.length()) {
                                 if ((counter + 1) < tempState.length()) {	
		while (tempState.substring(counter, counter + 1) == " ") {
                                    counter += 1;					
		}
		while (tempState.substring(counter, counter + 1) != " ") {
			finals = finals.concat(tempState.substring(counter, counter + 1));
			counter += 1;
			if (counter >= (tempState.length() -1)) {
			break;
			}
		}
		}
		else {
		   finals = finals.concat(tempState.substring(counter));
		   counter += 1;
		}
		myObject.insert(finals.trim());
		myObject.insertFinalState(finals.trim());
		finals = "";
	}
                  
                  System.out.println("WE are reading in the other states");
                 counter = 0;	
	tempState = inFile.readLine();
	tempState = myObject.removeFromString(tempState, "/r/n");
	while (counter < tempState.length()) {
	                if ((counter+1) < tempState.length()) {
		while (tempState.substring(counter, counter + 1) == " ") {
                                    counter += 1;					
		}
		while (tempState.substring(counter, counter + 1) != " ") {
			finals = finals.concat(tempState.substring(counter, counter + 1));
			counter += 1;
			if (counter >= (tempState.length() -1)) {
			break;
		               }
		}
		}
		else {
		   finals = finals.concat(tempState.substring(counter));
		   counter +=1;

		   }
		myObject.insert(finals.trim());
		finals = "";
	}
	
               counter = 0;	
               
               System.out.println("WE are parsing the transitions now");
	//Start parsing the transitions and put them in the array
	tempState = inFile.readLine();
	tempState = myObject.removeFromString(tempState, "/r/n");
	while (tempState != null) {

		//get first state
		
		while (LOWERCASE.indexOf(tempState.substring(counter, counter + 1)) == -1) {
			finals = finals.concat(tempState.substring(counter, counter + 1));
			counter += 1;
		}
		grammerIndex = myObject.isIn(finals);
               		
		//get input symbol
		tempSymbol = tempState.substring(counter, counter + 1);
		index = myObject.inputIsIn(tempSymbol);
		if (index == -1) {
			index = myObject.insertInput(tempSymbol);
		}
   		
		//increment index by one to make sure it points to the same spot
		//in the grammer array
		index += 1;
		counter++;
		

		finals = "";	
		//read in and input the state that the transition goes to
		while (counter < tempState.length()) {
			finals = finals.concat(tempState.substring(counter, counter + 1));
			counter += 1;
		}
		//grammerIndex = myObject.isIn(finals);
		
		counter = myObject.isIn(finals);
		if (myObject.grammer[grammerIndex][index] == null) {
			myObject.grammer[grammerIndex][index] = finals;
		
		}
		else {
		
			myObject.grammer[grammerIndex][index] = myObject.grammer[grammerIndex][index].concat(","); 		
			myObject.grammer[grammerIndex][index] = myObject.grammer[grammerIndex][index].concat(finals.trim());
		}
               tempState = inFile.readLine();
	tempState = myObject.removeFromString(tempState, "/r/n");
	counter = 0;
	finals = "";	
	}
	

               System.out.println("We finished reading in the states here -- Doing Lambda closure!!");	
                
            //Begin revising here to finish program
               finals = "";
	//begin processing array - lambda closure
	//begin processing array - lambda closure
	for (counter = 0; counter < myObject.numberOfStates; counter++) {
	   for (counter2 = 6; counter2 == 6; counter2++) {
                      if (myObject.grammer[counter][counter2] != null)   {
                         
                        while (positionInString != -1) {
                            positionInString2 = 0;                     
                            positionInString = myObject.grammer[counter][counter2].indexOf(",");
                         while (positionInString != -1) {
                            temp =  myObject.checkForLambdaInputs(myObject.isIn(myObject.grammer[counter][counter2].substring(positionInString2, positionInString)));
                            if (temp != null) {
                               
                               if (finals == "") {
                                  finals = finals.concat(temp);
                               }
                               else {
                                  finals = finals.concat(",");
                                  finals = finals.concat(temp);
                               }
                            }
                          
                            positionInString2 = positionInString + 1;
                            positionInString = myObject.grammer[counter][counter2].indexOf(",",  positionInString2);
                            }//end of while
                            temp = myObject.checkForLambdaInputs(myObject.isIn(myObject.grammer[counter][counter2].substring(positionInString2)));
                            positionInString = myObject.grammer[counter][counter2].length() - 1;
                            if (temp != null) {
                               if (finals == "") {
                                  finals = finals.concat(temp);
                               }
                               else {
                                  finals = finals.concat(",");
                                  finals = finals.concat(temp);
                               }
                            }
                            if (finals != "") {
                            position = 0;
                            positionInString2 = finals.indexOf(",");
                            while (positionInString2 != -1) {
                               temp = finals.substring(position, positionInString2);
                               index = myObject.grammer[counter][counter2].indexOf(temp);
                               if (index == -1) {
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(",");
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(temp);
                               }   
                               position = positionInString2 +1;
                               positionInString2 += 1;
                               positionInString2 = finals.indexOf(",", positionInString2);
                            }   
                            temp = finals.substring(position);
                               index = myObject.grammer[counter][counter2].indexOf(temp);
                               if (index == -1) {
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(",");
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(temp);
                               }   
                               finals = "";
                            if (myObject.grammer[counter][counter2].length() != (positionInString +1)) {
                              positionInString2 = positionInString + 1;                            
                            }
                            else {
                               positionInString = -1;
                            }
                            }//end of if
                            else {
                               positionInString = -1;
                            }
                            
                        }//end of outer while    
                        positionInString = 0;
                        }// end of if
                        }//end of inner for
                        }//end of outer for
                        
                 
                 finals = "";      
	for (counter = 0; counter < myObject.numberOfStates; counter++) {
	   for (counter2 = 1; counter2 <= myObject.numberOfInputs; counter2++) {
                      if (myObject.grammer[counter][counter2] != null)   {
                         
                        while (positionInString != -1) {
                            positionInString2 = 0;                     
                            positionInString = myObject.grammer[counter][counter2].indexOf(",");
                         while (positionInString != -1) {
                            temp =  myObject.checkForLambdaInputs(myObject.isIn(myObject.grammer[counter][counter2].substring(positionInString2, positionInString)));
                            if (temp != null) {
                               if (finals == "") {
                                  finals = finals.concat(temp);
                               }
                               else {
                                  finals = finals.concat(",");
                                  finals = finals.concat(temp);
                               }
                            }
                          
                            positionInString2 = positionInString + 1;
                            positionInString = myObject.grammer[counter][counter2].indexOf(",",  positionInString2);
                            }//end of while
                            temp = myObject.checkForLambdaInputs(myObject.isIn(myObject.grammer[counter][counter2].substring(positionInString2)));
                            positionInString = myObject.grammer[counter][counter2].length() - 1;
                            if (temp != null) {
                               if (finals == "") {
                                  finals = finals.concat(temp);
                               }
                               else {
                                  finals = finals.concat(",");
                                  finals = finals.concat(temp);
                               }
                            }
                            if (finals != "") {
                            position = 0;
                            positionInString2 = finals.indexOf(",");
                            while (positionInString2 != -1) {
                               temp = finals.substring(position, positionInString2);
                               index = myObject.grammer[counter][counter2].indexOf(temp);
                               if (index == -1) {
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(",");
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(temp);
                               }   
                               position = positionInString2 +1;
                               positionInString2 += 1;
                               positionInString2 = finals.indexOf(",", positionInString2);
                            }   
                            temp = finals.substring(position);
                               index = myObject.grammer[counter][counter2].indexOf(temp);
                               if (index == -1) {
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(",");
                                  myObject.grammer[counter][counter2] = myObject.grammer[counter][counter2].concat(temp);
                               }   
                               finals = "";
                            if (myObject.grammer[counter][counter2].length() != (positionInString +1)) {
                              positionInString2 = positionInString + 1;                            
                            }
                            else {
                               positionInString = -1;
                            }
                            }//end of if
                            else {
                               positionInString = -1;
                            }
                            
                        }//end of outer while    
                        positionInString = 0;
                        }// end of if
                        }//end of inner for
                        }//end of outer for
                         

                        
                 System.out.println("We finished doing the lambda closure here");
                            for (counter = 0; counter < myObject.numberOfStates; counter++) {
                           for (counter2 = 1; counter2 <= myObject.numberOfInputs; counter2++) {
                              System.out.print(myObject.grammer[counter][0]);
                              System.out.print(myObject.inputSymbols[counter2 -1]);
                              if (myObject.grammer[counter][counter2] != null) {
                                 System.out.println(myObject.grammer[counter][counter2]);
                              }
                              else {
                                 System.out.println("L");
                              }
                           }
                        }
                      
                       
                       System.out.println("Starting to create the final grammer");
                        if (myObject.grammer[0][6] != null) {
                        myObject.startState = myObject.startState.concat(",");
                         myObject.startState = myObject.startState.concat(myObject.grammer[0][6]);
                        }
                        counter = 0;
                        myObject.finalgrammer[0][0] = myObject.startState;
                        myObject.numberInFinalGrammer++;
                        while (myObject.finalgrammer[counter][0] != null) {
                           positionInString2 = 0;
                           positionInString = myObject.finalgrammer[counter][0].indexOf(",");
                           while (positionInString != -1) {
                              index = myObject.isIn(myObject.finalgrammer[counter][0].substring(positionInString2, positionInString));
                              for (counter2 = 1; counter2 <= myObject.numberOfInputs; counter2++) {
                                 if (myObject.grammer[index][counter2] != null) {
                                 stringTok = new StringTokenizer(myObject.grammer[index][counter2], ",");
                                 if (stringTok.countTokens() < 2) {
                                    temp = stringTok.nextToken();
                                    if (myObject.finalgrammer[counter][counter2] != null && myObject.finalgrammer[counter][counter2] != "") {
                                    if (myObject.finalgrammer[counter][counter2].indexOf(temp) == -1) {
                                      temp = ",".concat(temp);
                                       myObject.finalgrammer[counter][counter2] = myObject.finalgrammer[counter][counter2].concat(temp);
                                    }
                                    }
                                    else {                                    
                                    myObject.finalgrammer[counter][counter2]= myObject.grammer[index][counter2];
                                    }
                                 }
                                 else {
                                    while (stringTok.hasMoreTokens() == true) {
                                     try {
                                        temp =  stringTok.nextToken();    
                                    }
                                    catch (NoSuchElementException e) {
                                    
                                    }
                                    if (myObject.finalgrammer[counter][counter2] != null && myObject.finalgrammer[counter][counter2] != "") {
                                    if (myObject.finalgrammer[counter][counter2].indexOf(temp) == -1) {
                                      temp = ",".concat(temp);
                                       myObject.finalgrammer[counter][counter2] = myObject.finalgrammer[counter][counter2].concat(temp);
                                    }
                                    }
                                    else {
                                       myObject.finalgrammer[counter][counter2] = temp;
                                    }
                                    }//end of while
                                 }//end of else
                                 }
                                 else {
                                    if (myObject.finalgrammer[counter][counter2] == null) {
                                     myObject.finalgrammer[counter][counter2] = "";        
                                     }
                                 }
                              }//end of for
                           
                              positionInString2 = positionInString + 1;
                              positionInString = myObject.finalgrammer[counter][0].indexOf(",", positionInString2);
                           }//end of outer while
                           index = myObject.isIn(myObject.finalgrammer[counter][0].substring(positionInString2));
                              for (counter2 = 1; counter2 <= myObject.numberOfInputs; counter2++) {
                                 if (myObject.grammer[index][counter2] != null) {
                                 stringTok = new StringTokenizer(myObject.grammer[index][counter2], ",");
                                
                                 if (stringTok.countTokens() < 2) {
                                   temp = stringTok.nextToken();
                                    if (myObject.finalgrammer[counter][counter2] != null && myObject.finalgrammer[counter][counter2] != "") {
                                    if (myObject.finalgrammer[counter][counter2].indexOf(temp) == -1) {
                                      temp = ",".concat(temp);
                                       myObject.finalgrammer[counter][counter2] = myObject.finalgrammer[counter][counter2].concat(temp);
                                    }
                                    }
                                    else {                                    
                                    myObject.finalgrammer[counter][counter2]= myObject.grammer[index][counter2];
                                    }
                                 }
                                 else {
                                    while (stringTok.hasMoreTokens() == true) {
                                     try {
                                        temp =  stringTok.nextToken();    
                                    }
                                    catch (NoSuchElementException e) {
                                    
                                    }
                                    if (myObject.finalgrammer[counter][counter2] != null && myObject.finalgrammer[counter][counter2] != "") {
                                    if (myObject.finalgrammer[counter][counter2].indexOf(temp) == -1) {
                                      temp = ",".concat(temp);
                                       myObject.finalgrammer[counter][counter2] = myObject.finalgrammer[counter][counter2].concat(temp);
                                    }
                                    }
                                    else {
                                    
                                       myObject.finalgrammer[counter][counter2] = temp;
                                    }
                                    }//end of while
                                 }//end of else
                                 }
                                 else {
                                  if (myObject.finalgrammer[counter][counter2] == null) {
                                    myObject.finalgrammer[counter][counter2] = "";
                                 }
                                 }
                              }//end of for
                              
                          for (counter2 = 1; counter2 <= myObject.numberOfInputs; counter2++) {
                             if (myObject.finalgrammer[counter][counter2] != null) {
                             if (myObject.finalgrammer[counter][counter2] != "" && myObject.isInFinalGrammer(myObject.finalgrammer[counter][counter2]) == false) {
                                myObject.insertIntoFinalGrammer(myObject.finalgrammer[counter][counter2]);
                             }
                             }
                          }
                        
                        
                       counter++;  
                        }

                myObject.removeCommas();
	myObject.output();	
	

} //end of main

}//end of DFA class
