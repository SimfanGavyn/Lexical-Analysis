//
//  LexicalAnalysis.h
//  Lexical Analysis
//
//  Created by 陈子源 on 2019/5/08.
//  Copyright © 2019 陈子源. All rights reserved.
//

#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define IDENTIFIER 1                // 定义四类不同的mod
#define DECIMAL 2
#define SIGNAL 3
#define NOMATCHING 4

int numOfLine = 1;                  // 当前读取行数
int errorTimes = 0;                 // 出错次数
char savedWord = 0;                 // 保存下来的分隔符
ifstream fileIn;                    // 所打开的文件
ifstream fileInCopy;                // 所打开文件副本 用来输出原文
string copyLine;                    // 复制当行内容
string token = "";                  // 输出的正确token 每次输出后清空
string errorToken = "";             // 输出的错误token 每次输出后清空

void openFile (string fileName);    // 打开指定文件
void copyFileLine();                // 拷贝当前所在行内容
void closeFile ();                  // 关闭文件与文件副本
void writeFile (string stringOut);  // 输入指定内容到tokenOut.txt

bool isError();                     // 判断当前状态是否含有错误
bool isLetter (char c);             // 判断该字符是否是字母
bool isDigit (char c);              // 判断该字符是否是数字
bool isThisLineSpace (char c);      // 判断该字符是否是 \r \t或空格
bool isNextLineSpace (char c);      // 判断该字符是否是 \n
bool isSpace (char c);              // 判断该字符是否是 \r \t 空格 \n 或文件终结符
bool tokenBreak (char c, int mod);  // 根据mod 来判断该字符是否是分隔符 单词和数字的分割是空白 : ; { } 符号的分割是空白 ;
bool isReserved();                  // 判断当前token是否是保留字

void out (int mod);                 // 根据mod 来输出正确的单词序列
void error (int mod);               // 根据mod 来输出错误
void directState (int mod);         // 根据mod 来执行直接输出状态中识别到other的情况
void indirectState (int mod);       // 根据mod 来执行间接输出状态中识别到other的情况
char readNext();                    // 读取下一个字符

void start();                       // 各个状态机 包含有 符号跳转 自循环 other处理
void identifier();                  // 可直接输出正确的identifier的状态
void letterOrDigit();
void formerNumeral();
void latterNumeral();
void decimal();                     // 可直接输出正确的decimal的状态
void plusOrSignal();
void signOrSignal();
void minusOrSignal();
void colonSignal();                 // 可直接输出正确的signal的状态
void biggerSignal();
void signal();                      // 可直接输出正确的signal的状态


void openFile (string fileName) {
    fileIn.open(fileName);
    if (fileIn.fail()) {
        cout << "Open File Error!\nPlease put the target file in the lexical analysis code folder!" << endl;
        exit(1);
    }
    fileInCopy.open (fileName);
    copyFileLine();
}

void closeFile () {
    fileIn.close();
    fileInCopy.close();
}

void copyFileLine() {
    getline(fileInCopy, copyLine);
    writeFile("\nLine " + to_string(numOfLine) + ":  |" + copyLine);
}

void writeFile (string stringOut) {
    ofstream fileOut("tokenOut.txt", ios::app);
    fileOut << stringOut << endl;
    fileOut.close();
}

bool isError() {
    return (errorToken != "");
}

bool isLetter (char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit (char c) {
    return (c >= '0' && c <= '9');
}

bool isThisLineSpace (char c) {
    return (c == '\r' || c == '\t' || c == ' ');
}

bool isNextLineSpace (char c) {
    if (c == '\n') {
        numOfLine++;
        copyFileLine();
        return true;
    } else {
        return false;
    }
}

bool isSpace (char c) {
    return (isNextLineSpace(c) || isThisLineSpace(c));
}

bool tokenBreak (char c, int mod) {
    switch (mod) {
        case IDENTIFIER:
            return (isSpace(c) || fileIn.eof() || c == ':' || c == ';' || c == '{' || c == '}');
            break;
        case DECIMAL:
            return (isSpace(c) || fileIn.eof() || c == ':' || c == ';' || c == '{' || c == '}');
            break;
        case SIGNAL:
            return (isSpace(c) || fileIn.eof() || isLetter(c) || c == ';');
            break;
        default:
            return (isSpace(c) || fileIn.eof() || c == ':' || c == ';' || c == '{' || c == '}');
            break;
    }
}

bool isReserved() {
    if (token == "thread") {
        return true;
    } else if (token == "features") {
        return true;
    } else if (token == "flows") {
        return true;
    } else if (token == "properties") {
        return true;
    } else if (token == "end") {
        return true;
    } else if (token == "none") {
        return true;
    } else if (token == "in") {
        return true;
    } else if (token == "out") {
        return true;
    } else if (token == "data") {
        return true;
    } else if (token == "port") {
        return true;
    } else if (token == "event") {
        return true;
    } else if (token == "parameter") {
        return true;
    } else if (token == "flow") {
        return true;
    } else if (token == "source") {
        return true;
    } else if (token == "sink") {
        return true;
    } else if (token == "path") {
        return true;
    } else if (token == "constant") {
        return true;
    } else if (token == "access") {
        return true;
    } else {
        return false;
    }
}

void out (int mod) {
    string out = "   Sub " + to_string(numOfLine) + ":  |";
    switch (mod){
        case IDENTIFIER:
            if (isReserved()) {
                out += "Reserved Word = " + token;
            } else {
                out += "Identifier, Name = " + token;
            }
            break;
        case DECIMAL:
            out += "Decimal, Value = " + token;
            break;
        case SIGNAL:
            out += "Signal = " + token;
            break;
    }
    writeFile(out);
    token = "";
    start();
}

void error (int mod) {
    errorTimes++;
    string out = "   Sub " + to_string(numOfLine) + ":  -|";
    switch (mod) {
        case NOMATCHING:
            out += "NO MATCHING ERROR = " + errorToken;
            break;
        case IDENTIFIER:
            out += "IDENTIFIER ERROR = " + errorToken;
            break;
        case DECIMAL:
            out += "DECIMAL ERROR = " + errorToken;
            break;
        case SIGNAL:
            out += "SIGNAL ERROR = " + errorToken;
            break;
    }
    writeFile(out);
    token = "";
    errorToken = "";
    start();
}

void directState (char current, int mod) {
    if (tokenBreak(current, mod)) { //可能在进来之前就有错误了
        savedWord = current;
        if (isError()) {
            errorToken += token;
            error(mod);
        } else {
            out(mod);
        }
    } else {
        errorToken += token;
        while (!tokenBreak(current, mod)) {
            errorToken += current;
            current = readNext();
        }
        savedWord = current;
        error(mod);
    }
}

void indirectState (char current, int mod) {
    if (tokenBreak(current, mod)) {
            savedWord = current;
            errorToken += token;
            error(mod);
    } else {
        errorToken += token;
        while (!tokenBreak(current, mod)) {
            errorToken += current;
            current = readNext();
        }
        savedWord = current;
        error(mod);
    }
}

char readNext() {
    char ch;
    fileIn.get(ch);
    return ch;
}

void start() {
    char current;
    if (savedWord == 0) {
        current = readNext();
    } else {
        current = savedWord;
        savedWord = 0;
    }
    if (isSpace(current)) {
        start();
    } else if (isLetter(current)) {
        token += current;
        identifier();
    } else if (isDigit(current)) {
        token += current;
        formerNumeral();
    } else if (current == '+') {
        token += current;
        plusOrSignal();
    } else if (current == '-') {
        token += current;
        minusOrSignal();
    } else if (current == ':') {
        token += current;
        colonSignal();
    } else if (current == '=') {
        token += current;
        biggerSignal();
    } else if (current == ';' || current == '{' || current == '}') {
        token += current;
        signal();
    } else if (fileIn.eof()) {
        return;
    } else {
        errorToken += current;
        current = readNext();
        savedWord = current;
        if (tokenBreak(current, IDENTIFIER)) {
            error(NOMATCHING);
        } else {
            start();
        }
    }
}

void identifier() {
    char current = readNext();
    if (isLetter(current) || isDigit(current)) {
        token += current;
        identifier();
    } else if (current == '_') {
        token += current;
        letterOrDigit();
    } else {
        directState(current, IDENTIFIER);
    }
}

void letterOrDigit() {
    char current = readNext();
    if (isLetter(current) || isDigit(current)) {
        token += current;
        identifier();
    } else {
        indirectState(current, IDENTIFIER);
    }
}

void formerNumeral() {
    char current = readNext();
    if (isDigit(current)) {
        token += current;
        formerNumeral();
    } else if (current == '.') {
        token += current;
        latterNumeral();
    } else {
        indirectState(current, DECIMAL);
    }
}

void latterNumeral() {
    char current = readNext();
    if (isDigit(current)) {
        token += current;
        decimal();
    } else {
        indirectState(current, DECIMAL);
    }
}

void decimal() {
    char current = readNext();
    if (isDigit(current)) {
        token += current;
        decimal();
    } else {
        directState(current, DECIMAL);
    }
}

void plusOrSignal() {
    char current = readNext();
    if (isDigit(current)) {
        token += current;
        formerNumeral();
    } else if (current == '=') {
        token += current;
        signOrSignal();
    } else {
        indirectState(current, SIGNAL);
    }
}

void signOrSignal() {
    char current = readNext();
    if (current == '>') {
        token += current;
        signal();
    } else {
        indirectState(current, SIGNAL);
    }
}

void minusOrSignal() {
    char current = readNext();
    if (isDigit(current)) {
        token += current;
        formerNumeral();
    } else if (current == '>') {
        token += current;
        signal();
    } else {
        indirectState(current, SIGNAL);
    }
}

void colonSignal() {
    char current = readNext();
    if (current == ':') {
        token += current;
        signal();
    } else {
        directState(current, SIGNAL);
    }
}

void biggerSignal() {
    char current = readNext();
    if (current == '>') {
        token += current;
        signal();
    } else {
        indirectState(current, SIGNAL);
    }
}

void signal() {
    char current = readNext();
    directState(current, SIGNAL);
}

#endif /* LEXICALANALYSIS_H */
