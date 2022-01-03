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
        int number; // 图书馆拥有该书的数量
        int inlib; // 在馆数量

    public:
        // book 类的无参构造函数
        // 参数：无
        book();
        // book 类的全参构造函数
        // 参数：书名，ISBN，作者列表，一级分类，二级分类，三级分类，在馆数量
        book(const string&, const string&, vector<string>, const string&, const string&, const string&, int);

        // 取信息部分
        // 取出各种私有成员

        string get_name();
        string get_isbn();
        vector<string>* get_authors();
        string get_cate1();
        string get_cate2();
        string get_cate3();
        int get_number();
        int get_inlib();

        // 操作部分

        // 被借一本
        // 参数：无
        // 返回：是否成功
        bool borrow();
        // 被还一本
        // 参数：无
        // 返回：是否成功
        bool returnn();
        // 数量增加
        // 参数：增加的数量
        // 返回：是否成功
        bool add(int);

        // 格式化地输出一本书的信息
        // 参数：无
        void print_info();
};
