#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include<map>
#include "library.h"
using namespace std;

library::library(){};

library::library(string& name){
	string tmp = name + "_books.db";
	FILE *p = freopen(tmp.c_str() , "r", stdin);
	string now; cin>>now;
	while (now[0] != EOF){
		string passwd_hash; cin>>passwd_hash;
		int group; cin>>group;
		user_link[now] = new user(now,passwd_hash,group);
		cin>>now;
	}
	fclose(p);

	tmp = name + "_users.db";
	p = freopen(tmp.c_str(), "r", stdin);

	tmp = name + "_records.db";
	p = freopen(tmp.c_str(), "r", stdin);
}

string library::get_name(){
	return this->name;
}

user* library::get_user(){
	return this->this_user;
}

// 图书查找部分
// 用户组：所有

book* library::search_book_by_name(string& book_name){
	if (!book_name_link[book_name]){
		cout<<"ERROR: Book doesn't exist."<<endl;
		return NULL;
	}
	return book_name_link[book_name];
}

book* library::search_book_by_isbn(string& book_isbn){
	if (!book_isbn_link[book_isbn]){
		cout<<"ERROR: Book doesn't exist."<<endl;
		return NULL;
	}
	return book_isbn_link[book_isbn];
}

void library::list_books_by_author(string& author_name){
	vector<book*>& s = author_link[author_name];
	for (vector<book*>::iterator it=s.begin(); it!=s.end(); it++)
		(*it)->print_info();
}

void library::list_books_by_cate(string& cate1, string& cate2, string& cate3, int page){
	map<string, book*>::iterator it;
	// for (it=book_name_link.begin(); it!=book_name_link.end(); it++){
	// 	book* now = it->second;
	// 	cout<<now->name<<endl;
	// }
}

// 图书借阅部分
// 用户组：用户

bool library::borrow_book(book* now){
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	if (this_user == NULL) {cout<<"ERROR: Please log in first."<<endl; return false;}
	if (!now->inlib){
		cout<<"ERROR: Book already borrowed."<<endl;
		return false;
	}
	this_user->books.push_back(now);
	now->inlib = false;
	records_borrow.push_back(make_pair(this_user, now));
	return true;
}

bool library::return_book(book* now){
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	if (this_user == NULL) {cout<<"ERROR: Please log in first."<<endl; return false;}
	if (now->inlib){
		cout<<"ERROR: Book not borrowed."<<endl;
		return false;
	}
	vector<book*>& vec = this_user->books;
	for (vector<book*>::iterator it=vec.begin(); it!=vec.end(); it++)
		if ((*it) == now) {vec.erase(it); now->inlib = true; break;}
	if (!now->inlib){
		cout<<"ERROR: Book not borrowed by you."<<endl;
		return false;
	} else records_return.push_back(make_pair(this_user, now));
	return true;
}

// 图书管理部分
// 用户组：管理员

bool library::add_book(book now_obj){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (book_isbn_link[now_obj.isbn]){
		cout<<"ERROR: Book already exist."<<endl;
		return false;
	}
	book* now = new book(now_obj);
	book_name_link[now->name] = now;
	book_isbn_link[now->isbn] = now;
	for (int i=0; i<(int)now->author.size(); i++)
		author_link[now->author[i]].push_back(now);
	return true;
}

bool library::del_book(book* now){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	book_name_link.erase(now->name);
	book_isbn_link.erase(now->isbn);
	for (int i=0; i<(int)now->author.size(); i++){
		vector<book*>& vec = author_link[now->author[i]];
		for (vector<book*>::iterator it=vec.begin();it!=vec.end();it++)
			if ((*it)->name == now->name) vec.erase(it);
	}
	delete now;
	return true;
}

// 用户操作部分
// 用户组：用户（除了 login）

bool library::user_login(string& username, string& password){
	if (this_user) {cout<<"ERROR: You have already logged in. Please logout first. "<<endl; return false;}
    user* now = user_link[username];
    if (now->check_password(password)) {this_user = now; return true;}
	cout<<"ERROR: Invalid username or password. "<<endl;
    return false;
}

bool library::user_logout(){
	if (this_user == NULL) {cout<<"ERROR: Please log in first. "<<endl; return false;}
    this_user = NULL;
	return true;
}

#define v1 records_borrow
#define v2 records_return
bool library::list_records(){
	if (this_user == NULL) {cout<<"ERROR: Please log in first."<<endl; return false;}
	cout<<"Borrowed: "<<endl;
	for (int i=0; i<(int)v1.size(); i++)
		if (v1[i].first == this_user) cout<<v1[i].second->name<<endl;
	cout<<"Returned: "<<endl;
	for (int i=0; i<(int)v2.size(); i++)
		if (v2[i].first == this_user) cout<<v2[i].second->name<<endl;
	return true;
}
#undef v1
#undef v2

// 用户管理部分
// 权限：管理员

user* library::search_user(string& username){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return NULL;}
	return user_link[username];
}

bool library::change_user_password(user* now, string& new_password){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	now->change_password(new_password);
	return true;
}

bool library::reset_user_password(user* now){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	now->reset_password();
	return true;
}

bool library::list_users(){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	for (map<string, user* >::iterator it=user_link.begin(); it!=user_link.end(); it++)
		cout<<it->first<<endl;
	return true;
}

bool library::add_user(user now_obj){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (user_link[now_obj.get_id()]) {cout<<"ERROR: User already exist."<<endl; return false;}
	user* now = new user(now_obj);
	user_link[now->get_id()] = now;
	return true;
}

bool library::del_user(user* now){
	if (this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	user_link.erase(now->get_id());
	delete now;
	return true;
}