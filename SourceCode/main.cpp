//
//  main.cpp
//  Lexical Analysis
//
//  Created by 陈子源 on 2019/5/08.
//  Copyright © 2019 陈子源. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "LexicalAnalysis.h"

string nowTime() {
    time_t rawtime;
    struct tm *timeInfo;
    string out = "";
    time(&rawtime);
    timeInfo = localtime(&rawtime);
    out += to_string(timeInfo -> tm_year + 1900) + "/" + to_string(timeInfo -> tm_mon + 1) + "/" +
           to_string(timeInfo -> tm_mday) + " " + to_string(timeInfo -> tm_hour) + ":" +
           to_string(timeInfo -> tm_min) + ":" + to_string(timeInfo -> tm_sec);
    return out;
}

void buildUpEnvironment(string fileName) {
    ofstream fileOut ("tokenOut.txt");
    string out = "THREAD COMPILATION: " + fileName + "  " + nowTime();
    out += "\nEOF";
    fileOut << out << endl;
    fileOut.close();
}

void cleanEnvironment() {
    writeFile("\nEOF");
    string out = "";
    if (errorTimes > 2) {
        out += "DETECTED ERROR: " + to_string(errorTimes) + " errors.";
    } else {
        out += "DETECTED ERROR: " + to_string(errorTimes) + " error.";
    }
    cout << out << endl;
    writeFile(out);
    closeFile();
}

void lexicalAnalysis (string fileName) {
    fileIn.open(fileName);
    if (fileIn.fail()) {
        cout << "Open File Error!\nPlease put the target file in the lexical analysis code folder!" << endl;
        exit(1);
    }
    fileIn.close();
    buildUpEnvironment(fileName);
    openFile(fileName);
    start();
    cleanEnvironment();
}

int main (int argc, const char * argv[]) {
    string fileName = argv[1];
    lexicalAnalysis(fileName);
    return 0;
}
