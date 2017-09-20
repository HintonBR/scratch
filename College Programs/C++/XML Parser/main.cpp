#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <ctype.h>
#include "family.h"
#include "xmltag.h"

bool comparePeople(Person *data1, Person *data) {
     if ((data1->getSurname() + data1->getGivenNames()) < (data->getSurname() + data->getGivenNames())) {
		return true;
	}
    return false;
}

void generateLetterPage(char name, char *file, list<Person*> *mainList) {
    string mfile = file;
    mfile += name;
    mfile += ".htm";
    ofstream output(mfile.c_str(), ios::out);
    list<Person*>::iterator pIter = mainList->begin();
    if (!output) {
       cout << "Output file could not be opened" << endl;
       exit(1);
    }
    mfile = toupper(name);
    output << "<html><head><meta http-equiv=\"Content-Type\"content=\"text/html; charset=iso-8859-1\"><meta name=\"GENERATOR\" content=\"Microsoft FrontPage Express 2.0\"><title>This is the surname page for each surname that begins with the letter</title>";
    output << "</head><body bgcolor=\"#FFFFFF\"><p align=\"center\"><font color=\"#FF0000\" size=\"5\"><strong>This is the surname page for each surname that begins with the letter " << mfile << "</strong></font></p>";
    output << "<p align=\"center\">&nbsp;</p><p>To return to the index page - Click <a href=\"index.htm\">here</a></p><p>Individuals</p><hr>";
    while (pIter != mainList->end()) {
          if (tolower(((*pIter)->getSurname())[0]) == name) {
             output << "<strong><a href=\"" << (*pIter)->getID() << ".html" << "\">" << (*pIter)->getLink() << "</a><strong><br>";
          }
          pIter++;
    }
    output << "</body></html>";
    output.close();
}

void main(int argc, char *argv[]){
    string fileInput = "";
    XMLTag *myTag = NULL;
    XMLTag *newTag = NULL;
    char tempChar;

    list<Person*> pList;
    list<Family*> fList;
    list<Person*>::iterator pIter;
    list<Family*>::iterator fIter;
    if (argc != 3) {
        cout << "Wrong number of commandline arguments - Program Exiting" << endl;
        exit (1);
    }

    //Store command-line parameters.
    ifstream inFile(argv[1], ios::in);
    if (!inFile) {
        cout << "Input file failed to open - Program exiting!!" << endl;
        exit(1);
    }
		tempChar = inFile.get();
    while (inFile && inFile.eof() == false) {
    	  if (fileInput[fileInput.length() - 1] == '<') {
    			if ((isalpha(tempChar)) || (tempChar == '/')) {
    				fileInput += tempChar;
    			}
    	  }
    	  else {
	        fileInput += tempChar;
	      }
        tempChar = inFile.get();
    }
    cout << "Finished reading in from the file" << endl;
    if (fileInput == "") {
        cout << "Input file was empty - Program exiting!!" << endl;
        exit(1);
    }
    myTag = new XMLTag();
    while (fileInput.find("<") != string::npos) {
        fileInput = myTag->parse(fileInput);
        if (myTag->getType() == "Person") {
            pList.push_front(new Person);
            pIter = pList.begin();
            (*pIter)->parse(myTag->getValue());

        }
        else {
            fList.push_front(new Family());
            fIter = fList.begin();
            (*fIter)->parse(myTag->getValue());
        }
    }//end of while
    pList.sort(comparePeople);

    /*for (pIter = pList.begin(); pIter != pList.end(); pIter++) {
        cout << (*pIter)->getSurname() << "," << (*pIter)->getGivenNames() << " ID = " << (*pIter)->getID() << endl;
        (*pIter)->printEventList();
    }  */
    inFile.close();
    fileInput = "";
    fileInput = argv[2];
    fileInput += "index.htm";
    ofstream output(fileInput.c_str(), ios::out);
    if (!output) {
       cout << "Output file could not be opened" << endl;
       exit(1);
    }
    output << "<html><head><meta http-equiv=\"Content-Type\"content=\"text/html; charset=iso-8859-1\"><meta name=\"GENERATOR\" content=\"Microsoft FrontPage Express 2.0\"><title>Welcome to Bryan Hinton'</title></head><body bgcolor=\"#FFFFFF\"><p align=\"center\"><font color=\"#000040\" size=\"6\"><strong>Welcome to Bryan Hinton's genealogy page!!</strong></font></p> <p align=\"center\">&nbsp;</p> <p><font color=\"#400000\" size=\"4\"><strong><u>Letter List that indexes surnames</u></strong></font></p> <table border=\"0\"> <tr> <td><a href=\"a.htm\">A</a></td><td><a href=\"b.htm\">B</a></td><td><a href=\"c.htm\">C</a></td><td><a href=\"d.htm\">D</a></td><td><a href=\"e.htm\">E</a></td><td><a href=\"f.htm\">F</a></td><td><a href=\"g.htm\">G</a></td><td><a href=\"h.htm\">H</a></td><td><a href=\"i.htm\">I</a></td><td><a href=\"j.htm\">J</a></td><td><a href=\"k.htm\">K</a></td><td><a href=\"l.htm\">L</a></td>";
    output << "<td><a href=\"m.htm\">M</a></td><td><a href=\"n.htm\">N</a></td><td><a href=\"o.htm\">O</a></td><td><a href=\"p.htm\">P</a></td><td><a href=\"q.htm\">Q</a></td><td><a href=\"r.htm\">R</a></td><td><a href=\"s.htm\">S</a></td><td><a href=\"t.htm\">T</a></td><td><a href=\"u.htm\">U</a></td><td><a href=\"v.htm\">V</a></td><td><a href=\"w.htm\">W</a></td><td><a href=\"x.htm\">X</a></td><td><a href=\"y.htm\">Y</a></td><td><a href=\"z.htm\">Z</a></td></tr></table></body></html>";
    output.close();
    cout << "Created index file" << endl;

    fileInput = argv[2];
    for (tempChar = 'a'; tempChar <= 'z'; tempChar++) {
        generateLetterPage(tempChar, argv[2], &pList);
    }
    cout << "Created Letter Pages" << endl;

    for (pIter = pList.begin(); pIter != pList.end(); pIter++) {
        (*pIter)->generateWebPage(&pList, &fList, fileInput);
   }

   for (fIter = fList.begin(); fIter != fList.end(); fIter++) {
        (*fIter)->generateWebPage(&pList, &fList, fileInput);
   }
//Generate the Family pages


    for (pIter = pList.begin(); pIter != pList.end(); pIter++) {
        delete *pIter;
    }
    for (fIter = fList.begin(); fIter != fList.end(); fIter++) {
        delete *fIter;
    }
    delete myTag;
    CS240::print();
}


