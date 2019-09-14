#include "dsstring.h"

DSString::DSString(){
    //cout << "in default constructor" << endl;
    this->data = nullptr;
}

DSString::DSString(const char* originalChar){                               // 'originalChar' is a constant, can not change
    this->data = new char[strlen(originalChar) + 1];                        // create new 'data' array size of originalChar + 1('\0')
    strcpy(this->data,originalChar);                                        // c-copy 'originalChar' into 'data' array
    //cout << "in constructor: " << this->data << endl;
}

DSString::DSString(const DSString& originaldsString){                       // Copy constructor, DSString parameter is a const,
    char * tempoData = this->data;
    this->data = new char[strlen(originaldsString.data) + 1];               // that goes by another name (&)
    strcpy(this->data,originaldsString.data);

    delete [] tempoData;
}

DSString::~DSString(){                                                      // destructor
    delete [] this->data;
}

DSString& DSString::operator=(const char* originalChar){                    // returns DSString& = originalChar
    char * tempData = this->data;

    this->data = new char[strlen(originalChar) + 1];
    strcpy(this->data,originalChar);                                        // copies originalChar to this->data

    delete [] tempData;
    return *this;                                                           // *this returns a reference to the object
}

DSString& DSString::operator=(const DSString& originalString){              // returns DSString& = originalString
    char * tempData = this->data;
    this->data = new char[strlen(originalString.data) + 1];
    strcpy(this->data,originalString.data);                                 // copies originalString.data to this->data

    delete [] tempData;

    return *this;                                                           // return DSString by reference
}

DSString DSString::operator+(const DSString& originalString){
    DSString newString = this->data;
    strcat(newString.data,originalString.data);

    return newString;                                                       // return DSString by value
}

DSString& DSString::operator+=(const DSString& originaldsString){
    char * tempChar = this->data;                                           // temporarily store this->data
    this->data = new char[strlen(tempChar) + strlen(originaldsString.data) + 1];    // empty out this->data
    strcpy(this->data,tempChar);                                            // copy tempChar to this->data
    strcat(this->data,originaldsString.data);                               // concatenate orginialString.data to this->data

    delete [] tempChar;

    return *this;                                                           // return a reference to invoking address
}

bool DSString::operator==(const char* parameterData){
    if(this->data == parameterData){
        return true;
    }

    return false;
}

bool DSString::operator==(const DSString& parameterString){
    if(this->data == parameterString.data){
        return true;
    }
    return false;
}

bool DSString::operator<(const char * parameterChar){
    if(this->data < parameterChar){
        return true;
    }
    return false;
}


bool DSString::operator<(const DSString& parameterString){
    if(this->data < parameterString.data){
        return true;
    }
    return false;
}

char& DSString::operator[](const int indexSize){
    return *(this->data + indexSize);
}

int DSString::charToNum(char aValue){
    switch(aValue){
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case '0':
            return 0;
        default:
            return -1;
    }
}

int DSString::size(){                                                       // return by value
    return int(strlen(this->data));
}

/*  Parameter a represents the starting position
 *  Parameter b represents the # of characters to copy into the substring that is returned.
 *  If b is (+), count forward from position a.
 *  If b is (-), count backward from position a.
 */
DSString DSString::substring(int a, int b){
    char * tempData = this->data;
    int c = 0;
    if(b > 0){
        c = b;
        this->data = new char[c];
        for(int i = 0; i < c ; i++){
            this->data[i] = tempData[a + (i + 1)];
        }

        delete [] tempData;

        return this->data;
    }
    else if(b < 0){
        this->data = new char[-b + 1];
        for(int i = 0; i < a ; i++){
            this->data[i] = tempData[i];
        }

        delete [] tempData;

        return this->data;
    }
    else{
        return this->data;
    }
}

char* DSString::c_str(){                                                    // returns by pointer
    return this->data;
}

ostream& operator<<(ostream& out, const DSString& theString){
    out << theString.data;

    return out;
}

istream& operator>>(istream& stream,DSString& theString){
    char * tempChar = theString.data;

    theString.data = new char[100];

    if(stream.good()){
        while(!stream.eof()){
            stream >> *(theString.data);
        }
    }

    delete [] tempChar;

    return stream;
}
