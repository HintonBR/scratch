#include "family.h"

Family::Family() {
  id = "";
  father = "";
  mother = "";
  history = NULL;
}
Family::~Family() {
 list<Event*>::iterator eIter;
 list<Child*>::iterator cIter;

 if (history != NULL) {
		delete history;
	}
	for (eIter = events.begin(); eIter != events.end(); eIter++) {
        delete (*eIter);
  }
  for (cIter = children.begin(); cIter != children.end(); cIter++) {
        delete (*cIter);
  }

}


string Family::parse(string data) {

		string tempstr = "";
        string ddata;
        int temp = 0;
		int temp2 = 0;
		list<Event*>::iterator eIter;
        list<Child*>::iterator cIter;

         ddata.assign(data);
         //find id attribute
         temp = ddata.find("d");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        id = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        id = "";
    }

    //find father attribute
    temp = 0;
    temp2 = 0;
    temp = ddata.find("father");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        father = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        father = "";
    }

    //find mother attribute
    temp = 0;
    temp2 = 0;
    temp = ddata.find("mother");
    if (temp != string::npos) {
        temp++;
        temp2 = ddata.find("\"", temp);
        temp = ddata.find("\"", temp2 + 1) - 1;
        mother = ddata.substr(temp2 + 1, temp - temp2);
    }
    else {
        mother = "";
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
    cout << "tempstr is " << tempstr << endl;
    if (tempstr != "") {
          history = new History();
           history->parse(tempstr);
	}


 //find children
		temp = 0;
    temp2 = 0;
    temp = data.find("<child");
    while (temp != string::npos) {
    	temp2 = data.find(">", temp);
    	temp2++;
        children.push_front(new Child());
        cIter = children.begin();
        (*cIter)->parse(data.substr(temp, temp2 - temp));
  		data = data.substr(0, temp) + " " + data.substr(temp2, data.length());
  		temp = 0;
	    temp2 = 0;
     temp = data.find("<child");
		}		
		

           cout << "Finished Parsing Family object" << endl;
}//end of parse method

string Family::parseTag(string data, string mystr) {
	

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

void Family::generateWebPage(list<Person*> *pList, list<Family*> *fList, string out){
string path = out + id + ".html";
ofstream output(path.c_str(), ios::out);
list<Person*>::iterator pIter;
list<Child*>::iterator cIter;
list<Person*> pcList;
list<Event*>::iterator eIter;

if (!output) {
	cout << "File failed to open" << endl;
}
else {
output << "<html><head><meta http-equiv=\"Content-Type\"content=\"text/html; charset=iso-8859-1\">";
output << "<meta name=\"GENERATOR\" content=\"Microsoft FrontPage Express 2.0\"><title>Family Page</title>";
output << "</head><body bgcolor=\"#FFFFFF\"><p align=\"center\"><font color=\"#00FF40\" size=\"7\">Family Page</font></p>";
output << "<p><a href=\"index.htm\">Link to index page</a></p><hr><p>&nbsp;</p>";
output << "<p>Father: " << (father != "" ? getLink(pList, father) : "");
output << "<p>Mother: " << (mother != "" ? getLink(pList, mother) : "");
output << "<p>Important Life Events:</p>";

events.sort(compareDate);
for (eIter = events.begin(); eIter != events.end(); eIter++) {
	output << (*eIter)->getHTML();
}


output << "<p>Children:</p>";

for (cIter = children.begin(); cIter != children.end(); cIter++) {
	for (pIter = pList->begin(); pIter != pList->end(); pIter++) {
		if ((*cIter)->getID() == (*pIter)->getID()) {
			pcList.push_front((*pIter));
		}
	} 	
}
pcList.sort(childCompare);

for (pIter = pcList.begin(); pIter != pcList.end(); pIter++) {
	output << (*pIter)->getChildLink();
} 


output << "<p>History: " << (history != NULL ? history->getHTML() : "") << "</p>";
output << "</body></html>";
output.close();
}
}

string getLink(list<Person*>* mList, string tID) {
     list<Person*>::iterator pIter = mList->begin();
     while ((*pIter)->getID() != tID) {pIter++;}
           return ("<a href=\"" + (*pIter)->getID() + ".html\">" + (*pIter)->getSurname() + ", " + (*pIter)->getGivenNames() + "&nbsp;&nbsp;&nbsp;&nbsp;Birth Year: " + (*pIter)->getBirthDate() + "</a>");
}
Event* Family::getMarriageDate() {
	list<Event*>::iterator eIter;
	for (eIter = events.begin(); eIter != events.end(); eIter++) {
		if ((*eIter)->getType() == "MARRIAGE") {
			return (*eIter);
		}
	}
  return NULL;
}//end of getMarriageDate


