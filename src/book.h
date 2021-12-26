#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class book{
    private:
        string name;
        string ISBN;
        vector<string> author;
        string cate1,cate2,cate3;
        bool inlib;
    public:
        book();
    friend class library;
};

book::book(){
    name = "";
    ISBN = "";
    author.clear();
    cate1 = "";
    cate2 = "";
    cate3 = "";
    inlib = true;
}