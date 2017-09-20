#include <iostream>
#include "Letter.h"
#include "URLList.h"
#include <string>



bool generateHomePage(string outputDirectory, string testPath) {
    string out = outputDirectory;
    ofstream outFile(out + "\\index.html", ios:out);
    char letter = 'a';
    int loopCount;

    if (!outFile) {
        cerr << "Error in generating home page -- program exiting" << endl;
        return false;
    }
    else {
        outFile << "<html><head><title>Bryan's Web Crawler Home Page</title>" << endl;
        outFile << "<h1>Index of Words found on " + testPath + "</h1></head><body>" << endl;
        outFile << "<hr>" << endl;
        outFile << "<font size=\"18\" color=\"BLUE\">Welcome to the home page for Bryan's Web";
        outFile << " Crawler.  This page is the index to pages created by the web crawler for the";
        outFile << " site referenced above.  We hope you find the information interesting and ";
        outFile << " informative.  Have a wonderful day!</font>" << endl;
        outFile << " <hr> " << endl;
        outFile << "<h3>Index to words found by letter</h3>" << endl;
        outFile << "<br>" << endl;
        outFile << "<br>" << endl;
        outFile << "<br>" << endl;
        for(loopCount =0; loopCount <= 26; loopCount++) {
            outFile << "Letters beginning with <a href=\"" + out + '\\' + letter + ".html\">" + letter + "</a>" << endl;
            letter++;
        }
        outFile << "</body></html>" << endl;
        outFile.close();
        return true;
    }

}//end of generateHomePage method

int main(int argc, char *argv[]) {

//program constants
const string VALID_EXTENSIONS = "htm,html,HTML,HTM";
const string HOME_PAGE_PATH  = "index.html";

//program variables
int swArrayCount;
int loopCount;
char letter = 'a';
URLList *uList;
string startURL;
string testPath;
string outputDirectory;
ifstream inFile;
Letter *aLetter[26];


    if (argc != 4) {
        cerr << "Wrong number of command-line arguments" << endl;
        exit(1);
    }
    //storing commandline arguments
    startURL = argv[0];
    testPath = argv[1];
    outputDirectory = argv[2];

    //opening stopword file
    inFile.open(argv[3], ios:in);
    if (!inFile) {
        cerr << "Stopword file failed to open - exiting" << endl;
        exit(1);
    }

    //Reads every line of the file to count how many stopwords
    for(swArrayCount = 0; inFile.getLine().eof() == true; swArrayCount++) {
    }

    //declare stopWord Array to appropriate size
    string stopWord[swArrayCount];
    inFile.clear();
    //reposition File pointer to the beginning of the file
    inFile.seekg(0, ios::beg);
    for(loopCount =0; loopCount <= swArrayCount; loopCount++) {
        inFile >> stopWord[swArrayCount];
    }

    inFile.close();
    //print out stopword array if enabled
    #ifdef MainDebugging
     for(loopCount =0; loopCount <= swArrayCount; loopCount++) {
        cout << stopWord[swArrayCount];
    }
    #endif

    //create letter objects
    #ifdef MainDebugging
        cout << "Creating Letter objects" << endl;
    #endif
    for(loopCount = 0; loopCount <= 26; loopCount++) {
        aLetter[loopCount] = new Letter(letter);
        letter++;
    }


    //intialize URLList object
    uList = new URLList(startURL);

    //parse pages
    #ifdef MainDebugging
        cout << "Parsing Pages" << endl;
    #endif
    if (uList->parse(aLetter, testPath)) {
        cout << "Parsing successful" << endl;

        //generate index.html
       #ifdef MainDebugging
            cout << "Generating Pages" << endl;
        #endif

        if (!generateHomePage(outputDirectory, testPath)) {
            cerr << "An error occure in creating the Home page -- Program Exiting" << endl;
            //delete Letter object array if home page failed to be created
            for(loopCount = 0; loopCount <= 26; loopCount++) {
                delete aLetter[loopCount];
            }
            delete uList;

            //print out creations and deletions
            CS240::print();
            }
            exit(1);
        else {
            //generate Letter Pages
           #ifdef MainDebugging
                cout << "Generating Letter Pages" << endl;
            #endif
            for(loopCount = 0; loopCount <= 26; loopCount++) {
                aLetter[loopCount]->generatePage(outputDirectory);
            }
        }

    }
    else {
        cout << "An error occured in parsing the Web pages" << endl;
    }

    //delete Letter object array
    for(loopCount = 0; loopCount <= 26; loopCount++) {
        delete aLetter[loopCount];
    }
    delete uList;

    //print out creations and deletions
    CS240::print();
}//end of main
