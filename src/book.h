#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
using namespace std;

class book{
    private:
        string name;
        string isbn;
        vector<string> author; // 作者的列表
        string cate1,cate2,cate3; // 三级分类
        bool inlib; // 是否在图书馆中
    public:
        // book 类的无参构造函数
        book();
        // book 类的全参构造函数
        book(string&, string&, vector<string>*, string&, string&, string&);
        // 格式化地输出一本书的信息
        void print_info();

    friend class library;
};
