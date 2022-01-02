#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
using namespace std;

class book{
    private:
        string name; // 书的名称
        string isbn; // 书的 ISBN
        vector<string> author; // 该书作者的列表
        string cate1,cate2,cate3; // 三级分类
        bool inlib; // 是否在图书馆中

    public:
        // book 类的无参构造函数
        // 参数：无
        book();
        // book 类的全参构造函数
        // 参数：书名，ISBN，作者列表，一级分类，二级分类，三级分类
        book(string&, string&, vector<string>, string&, string&, string&);

        // 取信息部分
        // 取出各种私有成员

        string get_name();
        string get_isbn();
        vector<string>* get_authors();
        string get_cate1();
        string get_cate2();
        string get_cate3();
        bool is_inlib();

        // 操作部分

        // 被借
        // 参数：无
        // 返回：是否成功
        bool borrow();
        // 被还
        // 参数：无
        // 返回：是否成功
        bool returnn();

        // 格式化地输出一本书的信息
        // 参数：无
        void print_info();
};
