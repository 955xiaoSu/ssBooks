#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include "book.h"
using namespace std;

book::book(){
    name = isbn = cate1 = cate2 = cate3 = "";
    author.clear();
    number = inlib = 0;
}

book::book(const string& names, const string& isbns, vector<string> vec, const string& c1, const string& c2, const string& c3, int numbers){
	name = names;
	isbn = isbns;
	author = vec;
	cate1 = c1; cate2 = c2; cate3 = c3;
	number = inlib = numbers;
}

string book::get_name(){return name;}
string book::get_isbn(){return isbn;}
vector<string>* book::get_authors(){return &author;}
string book::get_cate1(){return cate1;}
string book::get_cate2(){return cate2;}
string book::get_cate3(){return cate3;}
int book::get_number(){return number;}
int book::get_inlib(){return inlib;}

bool book::borrow(){
	if (!inlib){
		cout<<"ERROR: Book(s) not in library! "<<endl;
		return false;
	}
	inlib--;
	return true;
}

bool book::returnn(){
	if (inlib+1 > number){
		cout<<"ERROR: Book(s) too many in library! "<<endl;
		return false;
	}
	inlib++;
	return true;
}

bool book::add(int num){
	number += num;
	inlib += num;
	return true;
}

void book::print_info(){
	cout<<endl;
	cout<<"Name:\t"<<name<<endl;
	cout<<"ISDB:\t"<<isbn<<endl;
	cout<<"Author:\t";
	for (int i=0;i<(int)(author.size());i++) cout<<author[i]<<" ";
	cout<<endl;
	cout<<"Type:\t"<<cate1<<"/"<<cate2<<"/"<<cate3<<endl;
	cout<<"In library:\t"<<inlib<<endl;
	cout<<"Borrowed:\t"<<number-inlib<<endl;
	cout<<endl;
}
