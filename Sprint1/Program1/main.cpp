#include <iostream>
#include "lasertag.h"

using namespace std;

int main(int argc,char * argv[]){

    if(argc != 6){
        cout << "Error, missing files" << endl;
        return -1;
    }

    LaserTag newGame(argv);
    cout << endl;

    return 0;
}