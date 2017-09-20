#include "person.h"
#include "family.h"

bool compareDate(Event*, Event*);
bool childCompare(Person *p1, Person *p2) {
	Event* e1;
	Event* e2;

	e1 = p1->getBirthEvent();
	e2 = p2->getBirthEvent();

	if (e2 == NULL) {
		return true;
	}
	if (e1 == NULL) {
		return false;
	}

  if (compareDate(e1, e2)) {
		return true;			
	}
	return false;

}

Person::Person(){
	id = "";
	surname = "";
	givenNames = "";
    gender = ' ';
	parents = NULL;
	occupation = NULL;
	history = NULL;
}


Person::~Person(){
	list<Event*>::iterator eIter;
	list<Marriage*>::iterator mIter;
	if (occupation != NULL) {
		delete occupation;
	}
	if (parents != NULL) {
		delete parents;
	}
	if (history != NULL) {
		delete history;
	}
	for (eIter = events.begin(); eIter != events.end(); eIter++) {
        delete (*eIter);
  }
  for (mIter = marriages.begin(); mIter != marriages.end(); mIter++) {
        delete (*mIter);
  }
}//end of destructor

string Person::getLink() {
       string spaces = "";
       for (int count = 0; count <= 5; count++) {
           spaces += "&nbsp;";
       }
       string mgender = (tolower(gender) == 'm' ? "male" : (tolower(gender) == 'f' ? "female" : ""));
       return (surname + "," + givenNames + spaces + "Gender: " + mgender + spaces + " Birthdate: " + getBirthDate());
}
string Person::parse(string data) {
		
		string tempstr = "";
        string ddata;
        int temp = 0;
		int temp2 = 0;
		list<Event*>::iterator eIter;
		list<Marriage*>::iterator mIter;

         ddata.assign(data);
         //find id attribute
         temp = ddata.find("id");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        id = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        id = "";
    }

    //find surname attribute
    temp = 0;
    temp2 = 0;
    temp = ddata.find("surname");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        surname = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        surname = "";
    }

    //find givenNames attribute
    temp = 0;
    temp2 = 0;
    temp = ddata.find("givenNames");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        givenNames = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        givenNames = "";
    }

    //find gender attribute
    temp = 0;
    temp2 = 0;
    temp = ddata.find("gender");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        gender = ddata.substr(temp2 + 1, temp - temp2)[0];
    }
    else {
         gender = ' ';
    }


    //find events
		temp = 0;
    temp2 = 0;
    temp = data.find("<event");
    while (temp != string::npos) {
    	temp2 = data.find(">", temp);
    	if (data[temp2 - 1] != '/') {
    		temp2 = data.find("</event", temp2);
    		temp2 = data.find(">", temp2);		
    	}
    	temp2++;
    	events.push_front(new Event());
  		eIter = events.begin();
  		(*eIter)->parse(data.substr(temp, temp2 - temp));  	
  		data = data.substr(0, temp) + " " + data.substr(temp2, data.length());
  		temp = 0;
	    temp2 = 0;
  	  temp = data.find("<event");
  }//end of while

    //find history tag
    tempstr = parseTag(data, "history");
    if (tempstr != "") {
          history = new History();
           history->parse(tempstr);
	}
  		//find occupation tag
    tempstr = parseTag(data, "occupation");
    if (tempstr != "") {
    occupation = new Occupation();
      occupation->parse(tempstr);
		}
		
		//find parents
		tempstr = parseTag(data, "parents");
    if (tempstr != "") {
			parents = new Parents();
      parents->parse(tempstr);
		}
		
 //find marriages
		temp = 0;
    temp2 = 0;
    temp = data.find("<marriage");
    while (temp != string::npos) {
    	temp2 = data.find(">", temp);
    	temp2++;
    	marriages.push_front(new Marriage());
  		mIter = marriages.begin();
  		(*mIter)->parse(data.substr(temp, temp2 - temp));  	
  		data = data.substr(0, temp) + " " + data.substr(temp2, data.length());
  		temp = 0;
	    temp2 = 0;
  	  temp = data.find("<marriage");
		}		
		
          cout << "Finished Parsing Person object" << endl;
}//end of parse method

string Person::parseTag(string data, string mystr) {
	

    int temp = 0;
    int temp2 = 0;
    temp = data.find("<" + mystr);
    if (temp != string::npos) {
       temp2 = data.find(">", temp);
    if (data[temp2 - 1] != '/') {
        temp2++;
        temp2 = data.find("</" + mystr, temp2);
        temp2 = data.find(">", temp2);
        temp2++;
        return data.substr(temp, temp2 - temp);
    	}
        temp2++;
        return data.substr(temp, temp2 - temp);
         }
	return "";
}//end of parseTag

bool Person::compareID(Person *data) {
	return (id == data->getID());
}
string Person::getBirthDate() {
	list<Event*>::iterator eIter;
	for (eIter = events.begin(); eIter != events.end(); eIter++) {
        if ((*eIter)->getType() == "BIRTH") {
           return (*eIter)->getDate();
        }
  }
  return "";
}

void Person::printEventList() {
     list<Event*>::iterator eIter;
     for (eIter = events.begin(); eIter != events.end(); eIter++) {
         cout << "The event is " << (*eIter)->getType() << " and the date is " << (*eIter)->getDate() << endl;
     }

}

void Person::generateWebPage(list<Person*>* pList, list<Family*>* fList, string out){
     string path = out + id + ".html";
     string temp = "";
     list<Event*>::iterator eIter;
     list<Marriage*>::iterator mIter;
     list<Person*>::iterator pIter;
     list<Family*>::iterator fIter;
     ofstream output(path.c_str(), ios::out);
     if (!output) {
        cout << "File failed to open -- exiting function" << endl;

     }
     else {
          char surn = tolower(surname[0]);
          cout << "Outputting a new file" << endl;
          output << "<html><head><meta http-equiv=\"Content-Type\"content=\"text/html; rset=iso-8859-1\"><meta name=\"GENERATOR\" content=\"Microsoft FrontPage Express 2.0\">";
          output << "<title>Individual Page for " << surname << "," << givenNames << "</title></head><body bgcolor=\"#FFFFFF\"><p align=\"center\"><font size=\"5\">Individual Page for " << surname << "," << givenNames << "</font></p><p align=\"center\"><font size=\"5\"></font>&nbsp;</p>";
          output << "<p align=\"center\"><font size=\"5\"></font>&nbsp;</p><p>To return to the index page - Click <a href=\"index.htm\">here</a></p><p>To return to the surname page - Click <a href=\"" << surn << ".htm\">here</a><hr><p>&nbsp;</p>";
          output << "<p>Surname: " << surname << "</p>";
          output << "<p>Given Names: " << givenNames << "</p>";
          output << "<p>Birthdate: " << getBirthDate() << "</p> ";
          output << "<p>Gender: " << gender << "</p>";
          output << "<p>Important Life Events:</p>";
          events.sort(compareDate);
          for (eIter = events.begin(); eIter != events.end(); eIter++) {
             output << (*eIter)->getHTML();
          }
          output << "<p>Parents: " << (parents != NULL ? getParentLink(pList, fList, parents->getID()) : "") << "</p>";
          output << "<p>Marriages:</p>";
          //getMarriageDates(fList);
          //marriages.sort(marriagesort);
          int count = 1;
          for (mIter = marriages.begin(); mIter != marriages.end(); mIter++) {
              cout << "Marriage found was: " << (*mIter)->getID() << endl;
              fIter = fList->begin();
              while ((*fIter)->getID() != (*mIter)->getID()) {
                    fIter++;
              }
              if (tolower(gender) == 'm') {
                 temp = (*fIter)->getMother();
              }
              else {
                   temp = (*fIter)->getFather();
              }
              pIter = pList->begin();
              if (temp != "") {
              while ((*pIter)->getID() != temp) {
                    pIter++;
              }
              }
              output << "<br>Marriage #" << count << ":&nbsp;&nbsp;&nbsp;<a href=\"" << (*mIter)->getID() << ".html\">" << (temp != "" ? (*pIter)->getSurname() : "") << ", " << (temp != "" ? (*pIter)->getGivenNames() : "") << "</a>";
              count++;
          }
          output << "<p>Occupation: " << (occupation != NULL ? occupation->getDesc() : "")  << "</p>";
          output << "<p>History: " << (history != NULL ? history->getHTML() : "") << "</p>";
          output << "</body></html>";
          output.close();
     }//end of else
}
string Person::getParentLink(list<Person*> *mpList, list<Family*> *mfList, string id) {
	string father = "";
	string mother = "";
	list<Person*>::iterator pIter;
	list<Family*>::iterator fIter;
    list<Person*> pList = (*mpList);
    list<Family*> fList = (*mfList);
			
	for (fIter = fList.begin(); fIter != fList.end(); fIter++) {
		if (id == (*fIter)->getID()) {
			father = (*fIter)->getFather();
			mother = (*fIter)->getMother();			
		}		

	}
	
     for (pIter = pList.begin(); pIter != pList.end(); pIter++) {
		if (father == (*pIter)->getID()) {
			father = (*pIter)->getSurname() + ", " + (*pIter)->getGivenNames(); 			
		}
		if (mother == (*pIter)->getID()) {
			mother = (*pIter)->getSurname() + ", " + (*pIter)->getGivenNames(); 			
		}
 	}

 return ("<a href=\"" + id + ".html\"> Father: " + father + "  Mother: " + mother + "</a>");
}//end of getParentLink method

void Person::getMarriageDates(list<Family*> *fList) {
     list<Family*>::iterator fIter = fList->begin();
	list<Marriage*>::iterator mIter;
	Event* e;
        cout << "We got into the entering the getdate method" << endl;
	for (mIter = marriages.begin(); mIter != marriages.end(); mIter++) {
		while ((*fIter)->getID() != (*mIter)->getID()) {
			fIter++;
		}
		e = (*fIter)->getMarriageDate();

		if (e != NULL) {
            (*mIter)->setYear(e->getYear());
            (*mIter)->setMonth(e->getMonth());
            (*mIter)->setDay(e->getDay());
		}
		else {
			(*mIter)->setYear(0);
			(*mIter)->setMonth(0);
			(*mIter)->setDay(0);
		}
	}
         cout << "We got into the entering the getdate method" << endl;
}//end of method getMarriageDates


bool marriagesort(Marriage* t1, Marriage* temp) {
cout << "we are in marriagesort" << endl;
if (t1->getYear() < temp->getYear()) {
        return true;
    }
    else if (t1->getYear() == temp->getYear()) {
         if (t1->getMonth() < temp->getMonth()) {
            return true;
        }
        else if (t1->getMonth() == temp->getMonth()) {
             if (t1->getDay() < temp->getDay()) {
                return true;
            }
        }
    }
    return false;


}//end of marriagesort method


string Person::getChildLink() {

	string result = "";
	result = "<a href=\"" + id + ".html\">" + surname + ", " + givenNames + "&nbsp;&nbsp;&nbsp;&nbsp;Gender: " + (tolower(gender) == 'm' ? "male" : (tolower(gender) == 'f' ? "female" : "")) + "&nbsp;&nbsp;&nbsp;&nbsp;BirthDate: " + getBirthDate() + "&nbsp;&nbsp;&nbsp;&nbsp;Birthplace: " + getBirthPlace() + "</a><br>";
	return result;
}

string Person::getBirthPlace() {
	list<Event*>::iterator eIter;
	for (eIter = events.begin(); eIter != events.end(); eIter++) {
		if ((*eIter)->getType() == "BIRTH") {
if ((*eIter)->getPlace() != "") {
				return (*eIter)->getPlace();
			}
		}
	}
	return "";


}//end of getBirthPlace method

Event* Person::getBirthEvent() {
	list<Event*>::iterator eIter;

	for (eIter = events.begin(); eIter != events.end(); eIter++) {
		if ((*eIter)->getType() == "BIRTH") {
			return (*eIter);
		}
	}
	return NULL;
}//end of get birthEvent method
