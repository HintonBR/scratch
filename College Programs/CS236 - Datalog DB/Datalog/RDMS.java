import byu.*;
import java.io.*;


public class RDMS {

    RelationList relationList;

    RDMS() {
        relationList = new RelationList(null);
    }

Relation Project(String schm, Relation rel) {

    int counter = 0;
    Tuple nextTuple;
    Tuple newTuple;
    Relation tempRelation = new Relation(schm);
    
    while (counter < tempRelation.sch.attArray.length) {
       if (rel.sch.GetIndex(tempRelation.sch.attArray[counter]) == - 1)  {
          System.out.println("Attribute not found in the scheme for this relation");
          System.out.println(schm);
          System.out.println(rel.sch.scheme);          
          return tempRelation          ;
       }
       counter++;
    }    
         
    nextTuple = rel.begTuple;
    while (nextTuple != null) {
        counter = 0;
        newTuple = new Tuple(tempRelation.sch);
        while (counter < tempRelation.sch.attArray.length) {
            newTuple.SetIthValue(nextTuple.GetIthValue(rel.sch.GetIndex(tempRelation.sch.attArray[counter])), counter);
            rel.accesscount++;
            counter++;
         }
         if (tempRelation.IsIn(newTuple) == false) {
             tempRelation.Insert(newTuple);
         }
         nextTuple = nextTuple.nextTuple;
         rel.accesscount++;
    }
    return tempRelation;
}//end of Project method

Relation Select (String leftop, String op, String rightop, char optype, Relation rel) {
	
	Relation newRelation = new Relation(rel.sch);
	Attribute attLeftOp = new Attribute(leftop);
	Attribute attRightOp = null;
	Value valLeftOp;
	Value valRightOp = null;
	Tuple r1Tuple;
	Tuple newTuple;
	String charRightOp = null;
	int leftOpIndex = 0;
	int rightOpIndex = 0;
	int typeOfOperator = 0;
	
		//Test for error conditions
	if (rel == null) {
		System.out.println("Empty Relation");
		return newRelation;
	}
	
	if (newRelation.sch.GetIndex(attLeftOp) == -1) {
		System.out.println("Attribute not in scheme of relation");
		return newRelation;
	}
	
	if ((optype != 'a') 	& (optype != 'v')) {
	                 System.out.println(optype);	                  
		System.out.println("Invalid operand type");
		return newRelation;
	}
	
	else if (optype == 'a') {
	   charRightOp = rightop;
	          attRightOp = new Attribute(charRightOp);
		if (newRelation.sch.GetIndex(attRightOp) == -1) {
			System.out.println("Attribute is not found in the scheme of the relation");
			return newRelation;
		}
	}
	
	if (op.equals("=")) {
		typeOfOperator = 1;
	}
	else if (op.equals("!=")) {
		typeOfOperator = 2;
	}
	
	else if (op.equals("<")) {
		typeOfOperator = 3;
	}
	
	else if (op.equals("<=")) {
		typeOfOperator = 4;
	}
	
	else if (op.equals(">")) {
		typeOfOperator = 5;
	}
	
	else if (op.equals(">=")) {
		typeOfOperator = 6;
	}
	
	switch(typeOfOperator) {
		
		case 1:
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.Equals(valRightOp)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.Equals(valRightOp)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
		
		case 2:
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.Equals(valRightOp) == false) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.Equals(valRightOp) == false) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
		
		case 3:
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.lessThan(valRightOp)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (valLeftOp.lessThan(valRightOp)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
			
		case 4: 
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if ((valLeftOp.Equals(valRightOp)) || (valLeftOp.lessThan(valRightOp))) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if ((valLeftOp.Equals(valRightOp)) || (valLeftOp.lessThan(valRightOp))) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
		
		case 5:
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (((valLeftOp.Equals(valRightOp)) == false) & ((valLeftOp.lessThan(valRightOp)) == false)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if (((valLeftOp.Equals(valRightOp)) == false) & ((valLeftOp.lessThan(valRightOp)) == false)) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
			
		case 6:
			r1Tuple = rel.begTuple;
			leftOpIndex = r1Tuple.sch.GetIndex(attLeftOp);
			
			if (optype == 'a') {
				rightOpIndex = r1Tuple.sch.GetIndex(attRightOp);
				while (r1Tuple != null) {
					valRightOp = r1Tuple.GetIthValue(rightOpIndex);
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if ((valLeftOp.lessThan(valRightOp)) == false) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			else {
				valRightOp = new Value(rightop);
				while (r1Tuple != null) {
					valLeftOp = r1Tuple.GetIthValue(leftOpIndex);
					if ((valLeftOp.lessThan(valRightOp)) == false) {
						newTuple = r1Tuple.Copy();
						newRelation.Insert(newTuple);
					}
					r1Tuple = r1Tuple.nextTuple;
					rel.accesscount++;
				}
			}
			break;
		
		default:
			System.out.println("Invalid Operator");
			return newRelation;
	}//end of switch
	
	return newRelation;
	
}//end of Select method

Relation Join (Relation r1, Relation r2) throws IOException {

 	int counter = 0;
	int isInScheme = 0;
	boolean haveCommonAttribute = false;
	int dontInsertTuple = 0;
	int inR1Tuple = 0;
	int inR2Tuple = 0;
	Value valR1 = null;
	Value valR2 = null;
	Tuple r1Tuple = r1.begTuple;
	Tuple r2Tuple = r2.begTuple;
	Tuple newTuple;
	Scheme newScheme = new Scheme(r1.sch.scheme);
	String tempString = newScheme.scheme;	
	while (counter < r2.sch.attArray.length) {
		isInScheme = newScheme.GetIndex(r2.sch.attArray[counter]);
		if (isInScheme == -1) {
                                                tempString = tempString.concat(",");
			tempString = tempString.concat(tempString.valueOf(r2.sch.attArray[counter].name));
		}
		else {
			haveCommonAttribute = true;
		}
		counter++;
		newScheme = new Scheme(tempString);
	}

	Relation newRelation = new Relation(newScheme);
	
	if (haveCommonAttribute == false) {
		while (r1Tuple != null) {
		  r2Tuple = r2.begTuple;
		   while (r2Tuple != null) {
				counter = 0;
				newTuple = new Tuple(newScheme);
				while (counter < newTuple.sch.attArray.length) {
					inR1Tuple = r1Tuple.sch.GetIndex(newTuple.sch.attArray[counter]);
					inR2Tuple = r1Tuple.sch.GetIndex(newTuple.sch.attArray[counter]);
					if (inR1Tuple != -1) {
						newTuple.SetIthValue(r1Tuple.GetIthValue(inR1Tuple), counter);
					}
					else if (inR2Tuple != -1) {
						newTuple.SetIthValue(r2Tuple.GetIthValue(inR2Tuple), counter);
					}
					else {
						newTuple.SetIthValue(null, counter);
					}
					counter++;
				}
				r2Tuple = r2Tuple.nextTuple;
				r2.accesscount++;
				newRelation.Insert(newTuple);
			}
			r1Tuple = r1Tuple.nextTuple;
			r1.accesscount++;
		}
	}
	else {
		int num = 0;
		int num2 = 0;
		while (r1Tuple != null) {
			r2Tuple = r2.begTuple;
			while (r2Tuple != null) {
				counter = 0;
				newTuple = new Tuple(newScheme);
				while (counter < newScheme.attArray.length) {
					inR1Tuple = r1Tuple.sch.GetIndex(newTuple.sch.attArray[counter]);
					inR2Tuple = r2Tuple.sch.GetIndex(newTuple.sch.attArray[counter]);
					if (inR1Tuple != -1) {
						valR1 = r1Tuple.GetIthValue(inR1Tuple);
						if (inR2Tuple != -1) {
                                                                                                   valR2 = r2Tuple.GetIthValue(inR2Tuple);
							if (valR1.Equals(valR2)) {
								newTuple.SetIthValue(valR1, counter);
							}
							else {
								dontInsertTuple = 1;
								break;
							}
						}
						else {
							newTuple.SetIthValue(valR1, counter);
						}
					}
					else {
						valR2 = r2Tuple.GetIthValue(inR2Tuple);
						newTuple.SetIthValue(valR2, counter);
					}
					counter++;
				}
				if (dontInsertTuple == 0) {
					if (newRelation.IsIn(newTuple) == false) {
						newRelation.Insert(newTuple);
					}
				}
				else {
					dontInsertTuple = 0;
				}
				r2Tuple = r2Tuple.nextTuple;
				r2.accesscount++;
			}
 			r1Tuple = r1Tuple.nextTuple;
			r1.accesscount++;

			}
	}
	return newRelation;
}//end of Join Method

Relation Union (Relation r1, Relation r2) throws IOException {

    Relation tempRelation = new Relation(r1.sch);
    if (r1.sch.Equals(r2.sch) == false) {
        System.out.println("The schemes of the two relation are disjoint.  Thank you for playing - Have a nice day!");
        return tempRelation;
    }
    Tuple r1Tuple = r1.begTuple;
    Tuple r2Tuple = r2.begTuple;
    
    while ((r1Tuple != null ) | (r2Tuple != null)) {
        if (r1Tuple == null)  {
            if (tempRelation.IsIn(r2Tuple) == false)  {
                tempRelation.Insert(r2Tuple);
            }
            r2Tuple = r2Tuple.nextTuple;
            r2.accesscount++;
        }
        else if (r2Tuple == null)  {
            if (tempRelation.IsIn(r1Tuple) == false)  {
                tempRelation.Insert(r1Tuple);
            }
            r1Tuple = r1Tuple.nextTuple;
            r1.accesscount++;
        }
        else {
             if (r1Tuple.valArray[0].lessThan(r2Tuple.valArray[0]))  {
                 if (tempRelation.IsIn(r1Tuple) == false)  {
                     tempRelation.Insert(r1Tuple);
                 }
                 r1Tuple = r1Tuple.nextTuple;
                 r1.accesscount++;
             }
             else {
                 if (tempRelation.IsIn(r2Tuple) == false)  {
                     tempRelation.Insert(r2Tuple);
                 }
                 r2Tuple = r2Tuple.nextTuple;
                 r2.accesscount++;
             }
        }
    }
    return tempRelation;
    
}//end of Union method

Relation Minus (Relation r1, Relation r2) {

    Relation tempRelation = new Relation(r1.sch);
    if ((r1.sch.Equals(r2.sch)) == false) {
          System.out.println("The schemes of the two relation are disjoint.  Thank you for playing - Have a nice day!");
        return tempRelation;
    }
    Tuple r1Tuple = r1.begTuple;
    while (r1Tuple != null) {
        if ((r2.IsIn(r1Tuple)) == false) {
            tempRelation.Insert(r1Tuple);
        }
        r1Tuple = r1Tuple.nextTuple;
        r1.accesscount++;
    }
    return tempRelation;    

}//end of Minus method

Relation Rename (Relation r, String schm, int index) {
    String newscheme;  
    int position = 0;
    int counter = 0;
    Scheme newScheme;
    Relation tempRelation;
    if (index != 0) {
    newScheme = new Scheme(schm);
    tempRelation = new Relation(newScheme);
    }
    else {
      while (counter < index) {
        position = r.sch.scheme.indexOf(",", position);
        counter++;
        position = position + 1;          
      }    
      newscheme = r.sch.scheme.substring(position);
      schm.concat(newscheme);
      newScheme = new Scheme(schm);
       tempRelation = new Relation(newScheme);    
    }
    if (r.sch.Length() != newScheme.Length())  {
        System.out.println("The two Schemes are of different length and so the renaming cannot occur - returning and empty relation");
        return tempRelation;
    }
    Tuple r1Tuple = r.begTuple;
    Tuple newTuple = new Tuple(newScheme);
    while (r1Tuple != null) {
      newTuple = r1Tuple.Copy(newScheme);
      tempRelation.Insert(newTuple);
      r.accesscount++;
      r1Tuple = r1Tuple.nextTuple;
    }
    return tempRelation;
}//end of Rename method

void addRelation (String name, String sch) {
                
                Relation myrelation;

                RelationList relation = relationList;
                Scheme scheme = new Scheme(sch);
                if (relation.rel == null) {
                    relation.rel = new Relation(name, scheme);
                }
                else {
                   while (relation.nextrel  != null) {
                     relation = relation.nextrel;
                    }
                    myrelation = new Relation(name, scheme);
                    relation.nextrel = new RelationList(myrelation);
                }
                
}//end of addRelation method          

RelationList isInList(String name) {
   RelationList relation = relationList;
   
   while (relation != null) {
      if (relation.rel.nameOfRelation.equals(name)) {
           return relation;
      }
      relation = relation.nextrel;
   }
   return null;
}   

Relation createRelation (String name, String sch) {
   Relation rel = new Relation(name, sch);
   return rel;
}
        
void showRelationList () {
     RelationList relat = relationList;
     while (relat != null) {
          relat.rel.Show();
          relat = relat.nextrel;
      }
}//end of showRelationList method

boolean addFact (String relationname, String fact) {
     Value myvalue;
    Tuple mytuple;
    int positionInString = 0;
    int previousPositionInString = 0;
    int counter = 0;
    RelationList relation = relationList;
    if (fact == null) {
        return false;
    }
    while (relation.rel != null) {
        if (relation.rel.nameOfRelation.equals(relationname)) {
            while (fact.indexOf(",", positionInString) != -1) {
               positionInString = fact.indexOf(",", positionInString) + 1;
               counter++;
            }
            counter++;
            mytuple = new Tuple(relation.rel.sch);
            if (counter != mytuple.valArray.length) {
                return false;
            }            
           counter = 0;
           positionInString = fact.indexOf(",", 0);
					if (positionInString != -1) {	
           myvalue = new Value(fact.substring(0, positionInString));
           mytuple.SetIthValue(myvalue, counter);
            counter++;
            while (counter < mytuple.valArray.length - 1) {
                previousPositionInString = positionInString + 1;
                positionInString = fact.indexOf(",", previousPositionInString);
                myvalue = new Value(fact.substring(previousPositionInString, positionInString));
                mytuple.SetIthValue(myvalue, counter);
                 counter++;
									
           }
					}
           myvalue = new Value(fact.substring(positionInString + 1));
           mytuple.SetIthValue(myvalue, counter);
           if (relation.rel.IsIn(mytuple)) {
               return false;
           }
          relation.rel.Insert(mytuple);
          return true;
        }
        else {
            relation = relation.nextrel;
        }
    }
    return false;
}//end of addFact method

    public class RelationList {
            
            Relation rel;
            RelationList nextrel;
                        
            RelationList(Relation relat) {
                rel = relat;
                nextrel = null;
            }
     }//end of RelationList class    

        

    
    class Relation {
    
       int TupleCount = 0;
       int accesscount = 0;
       String nameOfRelation;
       Scheme sch;
       Tuple begTuple;
       Tuple lastTuple;       
       
    
        Relation(Scheme schm) {
           nameOfRelation = schm.scheme;
           sch = schm;      
           
        } //end of Relation constructor
        
        Relation(String schm) {
           nameOfRelation = schm;
           sch = new Scheme(schm);      
           
        } //end of Relation constructor
        
        Relation(String name, Scheme schm) {
           nameOfRelation = name;
           sch = schm;      
           
        } //end of Relation constructor
        
        Relation(String name, String schm) {
          nameOfRelation = name;
          sch = new Scheme(schm); 
        }//end of Relation Constructor
        
        Scheme GetScheme () {
             return sch;  
        }//end of GetScheme method
        
        boolean IsIn(Tuple tupl) {
           Tuple next = begTuple;
           while (next != null) {
              if (next.Equals(tupl) == true) {
                 return true;
             }   
             else    {
                next = next.nextTuple;
             }
           }  
           return false;   
        }//end of IsIn method
        
        void Insert (Tuple tupl) {
           if (lastTuple == null)  {
              begTuple = tupl.Copy();
              lastTuple = begTuple;
           }
           else {
               lastTuple.nextTuple = tupl.Copy();
               lastTuple = lastTuple.nextTuple;
           }
          
           TupleCount++;
        }//end of Insert method
        
        void ResetAccessCount() {
             accesscount = 0;        
        }//end of ResetAccessCount method
        
        int GetAccessCount() {
           return accesscount;
        }//end of GetAccessCount method
        
        void Show() {
           Tuple next = begTuple;
           System.out.println("Name of Relation is : " + nameOfRelation);           
           sch.Show();
           while (next != null) {
               next.Show();
               next = next.nextTuple;
           }  
           try {       
           System.in.read();
           System.in.read();           
           }
           catch (IOException e) {
           
           }
        }//end of Show method
        
        void FillFromFile (RandomAccessFile infile) throws IOException {

           String tempString;
            String tempString2;
            int counter = 0;  
            int nextSpace =0;
            int endoffile = 0;
            int endSpace;
            
           tempString = infile.readLine();
 
           while  (endoffile != 1) {
               Tuple newTuple = new Tuple(sch);
               counter = 0; 
               nextSpace = 0;
               endSpace = 0;
               endSpace = tempString.indexOf("/r");
                      if (endSpace < tempString.indexOf("/n")) {
                         endSpace = tempString.indexOf("/n");
                      }
                      if (endSpace == -1) {
                         tempString = tempString.substring(0);
                      }
                      else {
                         tempString = tempString.substring(0, endSpace - 1);
                      }      
                      
               while (counter <  newTuple.sch.attArray.length) {
                 while (tempString.indexOf(" ") == 0) {
                      tempString = tempString.substring(1, tempString.length());
                  }
                  nextSpace = tempString.indexOf(" ");
                  if (nextSpace != -1) {
                      tempString2 = tempString.substring(0, (nextSpace));
                  }
                  else {
                      tempString2 = tempString;
                  }
                  newTuple.valArray[counter] = new Value(tempString2);
                  if (nextSpace != -1) {
                      tempString = tempString.substring(nextSpace);
                  }    
                  
                  counter++;
                }
                Insert(newTuple);
                tempString = infile.readLine();
                if (tempString == null) {
                    endoffile = 1;
                }
                
                
              }                              
        
        }//end of FillFromFile method
    
    }//end of Relation class
    

//TUPLE CLASS
    public class Tuple {
        Scheme sch;
        Value valArray[];
        Tuple nextTuple;
        
        Tuple(Scheme schm) {
           sch = schm;
           valArray = new Value[sch.attArray.length];                  
        }//end of Tuple constructor
        
        void SetIthValue (Value val, int i)  {
           valArray[i] = val;
        }//end of SetIthValue method
        
        Value GetIthValue (int i) {
           return valArray[i];
        
        }//end of GetIthValue method
        
        int Length() {
           return valArray.length;
       
        }//end of Length method
        
        Value GetValue(Scheme schm, Attribute a)  {
           if (schm.scheme.compareTo(sch.scheme) != 0)  {
               System.out.println("The Scheme that was specified doesn't match the Scheme of the Tuple!!");
               return null;
           }
           else {
               return GetIthValue(sch.GetIndex(a));
            }
                 
        }//end of GetValue method
        
        Tuple Copy() {
         int counter = 0;
            
         Tuple newTuple = new Tuple(sch);
         while (counter != valArray.length) {
             newTuple.valArray[counter] = valArray[counter];
             counter++;
         }
            
          return newTuple;
       }//end of Copy method
				
        Tuple Copy(Scheme schem) {
         int counter = 0;
            
         Tuple newTuple = new Tuple(schem);
         while (counter != valArray.length) {
            newTuple.valArray[counter] = valArray[counter];
            counter++;
         }
            
         return newTuple;
       }//end of Copy method
        
        boolean Equals (Tuple tupl) {
           int counter = 0;
           while (counter < (valArray.length)) {
              if (valArray[counter].Equals(tupl.valArray[counter]) == false) {
                 return false;
              }
              counter++;
          }
          return true;   
        
        }//end of Equals method
        
        void Show()  {
           int counter = 0;
           while (counter <= (valArray.length - 1)) {
              valArray[counter].Show();
                            counter++;
           }
           System.out.println();
        }//end of Show method
        
   }//end of Tuple class
   

//ATTRIBUTE CLASS
   public class Attribute {
   
       public String name;
       
       Attribute (String nam) {
          name = nam;
       
       }//end of Attribute constructor
       
       boolean Equals (Attribute att)  {
          if (att.name.equals(name)) {
             return true;
          }
          else {
             return false;
          }
             
       }//end of Equals method
       
       void Show () {
          System.out.print(name + "           ");
       
       }//end of Show method
       
   }//end of Attribute class        
        
    

//SCHEME CLASS
    public class Scheme {
       
        String scheme;
        Attribute attArray[];
        
         
        Scheme (String s) {
           
           int positionInString = 0;
           int previousPositionInString = 0;
           int counter = 0;
           scheme = s;
           String tempString;
           
           while (scheme.indexOf(",", positionInString) != -1) {
               positionInString = scheme.indexOf(",", positionInString) + 1;
               counter++;
            }
            counter++;            
           attArray = new Attribute[counter];
           counter = 0;
           positionInString = scheme.indexOf(",", 0);
					if (positionInString != -1) {	
           attArray[counter] = new Attribute(scheme.substring(0, positionInString));
            counter++;
            while (counter < attArray.length - 1) {
                previousPositionInString = positionInString + 1;
                positionInString = scheme.indexOf(",", previousPositionInString);
               attArray[counter] = new Attribute(scheme.substring(previousPositionInString, positionInString));
                        counter++;
           }
					}	
					
           attArray[counter] = new Attribute(scheme.substring(positionInString + 1));
         }//end of Scheme constructor         
        
        void Show ()  {
           
           int counter = 0;
             
            while (counter <=  attArray.length - 1) {
               attArray[counter].Show();
               counter++;
            }
            System.out.println();
        
        }//end of Show method
        
        String GetString() {
                   
            return scheme;
        }//end of GetString method
        
        int GetIndex (Attribute a)  {
        
            int counter= 0;
            while (counter < attArray.length) {
               if (attArray[counter].Equals(a))  {
                  return counter;
               }
              counter++;  
            }
            return -1;     
        }//end of GetIndex method
        
        String GetScheme() {
           String tempString = "";
           int counter = 0;
           
           while (counter < attArray.length) {
                  tempString = tempString.concat(attArray[counter].name);
                  counter++;                  
           }
           return tempString;
        }//end of GetScheme method
        
        int Length() {
        
           return attArray.length;
        
        }//end of Length method
        
        boolean Equals (Scheme schm) {
        
           if (scheme.compareTo(schm.scheme) == 0)  {
               return true;
           }
           else {
              return false;
           }
        
        }//end of Equals method
        
    }//end of Scheme class
    
    
    //VALUE CLASS
    public class Value {
    
      String value;
      
      Value(String val) {
         value = val;
      }
      
      boolean Equals(Value valu) {
           
          
          if ((value.equals(valu.value)) == true) {
              return true;
          }
          else {
              return false;
           }
      
      }//end of Equals method
      
      boolean lessThan(Value valu) {
     
         int compare = value.compareTo(valu.value);       
         
         if (compare < 0)  {
            return true;
         }
      else {
          return false;
      }
      }//end of lessThan method
      
      void Show()  {
         System.out.print(value);
         for(int i = value.length(); i <= 11         ; i++) {
             System.out.print(" ");
         }
               
      }//end of Show
            
    }//end of Value class


/*
public static void main(String[] args) throws Exception {

    int totaccess = 0;
    RDMS db = new RDMS();
    Relation r1 =  db. new Relation("ABC");
    RandomAccessFile f1 = new RandomAccessFile ("c:\\bryan'~1\\programs\\java\\database\\R1.txt", "rw");
    r1.FillFromFile(f1);
    Relation r2  = db. new Relation("CDE");
    RandomAccessFile f2 = new RandomAccessFile ("c:\\bryan'~1\\programs\\java\\database\\R2.txt", "rw");
    r2.FillFromFile(f2);
    r1.ResetAccessCount();
    r2.ResetAccessCount();
    //Relation temp1 = db.Join(r1, r2);
    //Relation temp2 = db.Select ('E', "=", "113", 'v', temp1);
    //Relation temp3 = db.Select ('A', "=", "25", 'v', temp2);
    //Relation temp4 = db.Project ("AD", temp3);
    //temp4.Show();
    Relation temp1;
    Relation temp2;
    Relation temp3;
    Relation temp4;
    Relation temp5;
    Relation temp;
    
    temp1 = db.Join(r1, r2);
    temp2 = db.Select('A', "<=", "C", 'a', temp1);
    temp = db.Project("CDE", temp2);
    temp3 = db.Union(temp, r2);
    temp4 = db.Join(r1, temp3);
    temp5 = db.Select('D', "<", "100", 'v', temp4);
    temp5.Show();
    totaccess = r1.GetAccessCount() + r2.GetAccessCount() + temp1.GetAccessCount() + temp2.GetAccessCount() + temp3.GetAccessCount() + temp4.GetAccessCount() + temp5.GetAccessCount() + temp.GetAccessCount();;
    System.out.println ("It took ");
    System.out.println (totaccess);
    System.out.println (" accesses");            
       
    //Now Perform Optimized Query
    
    r1.ResetAccessCount();
    r2.ResetAccessCount();
    temp.ResetAccessCount();
    temp1.ResetAccessCount();
    temp2.ResetAccessCount();
    temp3.ResetAccessCount();
    temp4.ResetAccessCount();
    temp5.ResetAccessCount();
    temp1 = db.Select ('A', "<=", "C", 'a', r1);
    temp3 = db.Join (temp1, r2);
    temp2 = db.Select ('D', "<", "100", 'v', temp3);
    temp = db.Project("CDE", temp2);
    Relation temp6 = db.Select('D', "<", "100", 'v', r2);
    temp4 = db.Union (temp, temp6);
    temp5 = db.Join(r1, temp4);
    
    temp5.Show();
    totaccess = r1.GetAccessCount() + r2.GetAccessCount() + temp1.GetAccessCount() + temp2.GetAccessCount() + temp3.GetAccessCount() + temp4.GetAccessCount() + temp5.GetAccessCount() + temp6.GetAccessCount();;
    System.out.println ("It took ");
    System.out.println (totaccess);
    System.out.println (" accesses"); 
    	
		
/*		//QUERY 1
 //-----------
 Relation snap = db.new Relation("SNAP");
 f1 = new RandomAccessFile ("f:\\snap.txt", "rw");	
 snap.FillFromFile(f1);

 Relation csg = db.new Relation("CSG");
 f2 = new RandomAccessFile ("f:\\csg.txt", "rw");	
 csg.FillFromFile(f2);

 temp1 = db.Join(snap, csg);
 temp2 = db.Project("CN", temp1);

 temp2.Show();
	
 //QUERY 2
 //------------
 Relation cp = db.new Relation("CP");
 f1 = new RandomAccessFile ("f:\\cp.txt", "rw");	
 cp.FillFromFile(f1);

 temp1 = db.Select ('C', "=", "CS206", 'v', cp);
 temp2 = db.Project("P", temp1);
 temp3 = db.Rename (temp2, "C");
 temp4 = db.Join   (temp3, cp);
 Relation temp5 = db.Project("P", temp4);

 temp5.Show();


 //QUERY 3
 //------------
 Relation cdh = db.new Relation("CDH");
 f1 = new RandomAccessFile ("f:\\cdh.txt", "rw");	
 cdh.FillFromFile(f1);

 Relation disj1 = db.Select('D', "=", "M", 'v', cdh);
 Relation disj2 = db.Select('D', "=", "W", 'v', cdh);
 Relation disj3 = db.Select('D', "=", "F", 'v', cdh);

 Relation t1 = db.Union(disj1, disj2);
 Relation t2 = db.Union(disj3, t1);

 Relation t3 = db.Project("C", t2);
 Relation t4 = db.Project("C", cdh);

 Relation t5 = db.Minus(t4, t3);

 t5.Show();



    
}//end of main method
*/	

}//end of RDMS Class
