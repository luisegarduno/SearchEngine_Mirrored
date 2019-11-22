#include "parser.h"

Parser::Parser() : totNumFiles(0), totNumWordAppears(0), totNumOfApperances(0){
}

Parser::Parser(char* argv[]) : totNumFiles(0), totNumWordAppears(0), totNumOfApperances(0){
    string file_name;
    file_name = argv[1];

    // If no file was selected, display warning message
    if(file_name == ""){
        string noFile = "Nothing was selected!\n";
        string tryAgain = "Please select valid path to a folder";
        cout << "Error: " << noFile + tryAgain << endl;
    }
    else{
        // Information window is displayed containing selected folder
        cout << "File Selected: " <<  file_name << endl;

        allFileLocations = setFileLocations(file_name);
    }
}

list<string> Parser::setFileLocations(string fileName){
    list<string> allFileLocations;

    filesystem::directory_iterator end;
    for(filesystem::directory_iterator theIterator(fileName) ; theIterator!= end; ++theIterator){
        // directory iterator is first converted to a path
        filesystem::path dirToPath = *theIterator;

        // path directory is converted to a string
        string pathToString = dirToPath.string();

        parseJSON(pathToString);

        // string is appended to end of vector
        allFileLocations.push_back(pathToString);

        // string is parsed to filename, and added to vector
        setFileNames(pathToString);
        totNumFiles++;
    }

    return allFileLocations;
}

void Parser::parseJSON(string pathString){
        FILE* fp = fopen(pathString.c_str(), "rb"); // non-Windows use "r"

        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        Document document;
        document.ParseStream(is);
        fclose(fp);

        int documentID = document["id"].GetInt();
        string author = document["author_str"].GetString();
        string htmlString = stripHTML(document["html"].GetString());
        string htmlLawbox = stripHTML(document["html_lawbox"].GetString());
        string plainString = stripHTML(document["plain_text"].GetString());

        cout << "\nFile[" << totNumFiles + 1 << "]********************************************************************" << endl;
        cout << "Document ID: " << documentID << endl;
        cout << "Author: " << author << endl;
        cout << "HTML: " <<  htmlString << endl;
        cout << "HTML_Lawbox: " << htmlLawbox << endl;
        cout << "Plain_Text: " << plainString << endl;
}

string Parser::stripHTML(string htmlString){

    for(unsigned int start = 0; start < htmlString.size(); start++){
        if(htmlString[start] == '<'){
            unsigned int end = start;

            while(htmlString[end] != '>' && end < htmlString.size()){
                end++;
            }
            htmlString.erase(start, end - start + 1);
            start--;
        }
    }
    return htmlString;
}


void Parser::setFileNames(string fileName){
    fileName.erase(0,133);

    fileNamesOnly.push_back(fileName);
}

// returns vector of strings containing file locations
list<string> Parser::getFileNamesOnly(){
    return fileNamesOnly;
}

// returns vector of strings containing file locations
list<string> Parser::getFileLocations(){
    return allFileLocations;
}

size_t Parser::getSizeOfFolder(){

    if(totNumFiles > 0){
        return totNumFiles;
    }

    return 0;
}

void Parser::printFileNames(){
    int fileCounter = 0;
    for(auto const &v : fileNamesOnly){
        cout << "File[" << ++fileCounter << "]: " << v << "\n" ;
    }
}
