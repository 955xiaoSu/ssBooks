#pragma once

#include<cstdio>
#include<cstring>
#include<iostream>
#include "library.h"
using namespace std;

library s;

void who(){
	cout<<((s.get_user())?((s.get_user()->get_group())?"User > ":"Admin > "):"Guest > ");
}

void opt_help(){
	cout<<endl<<"Welcome to ssBooks system! :)"<<endl;
	cout<<" HNU homework by SBK & SkyWT. "<<endl;
	cout<<" A tiny but delicate library manager. "<<endl<<endl;

	cout<<"Global Commands: "<<endl;
	cout<<"  search\tSearch for a book"<<endl;
	cout<<"  login\tLogin as a user/admin"<<endl;

	cout<<"User Commands: "<<endl;
	cout<<"  borrow\tBorrow a book"<<endl;
	cout<<"  return\tReturn a book"<<endl;
	cout<<"  change_pass\tChange your password"<<endl;
	cout<<"  list_records\tList your records"<<endl;
	cout<<"  logout\tLogout your account"<<endl;

	cout<<"Admin Commands: "<<endl;
	cout<<"  add_book\tAdd a book"<<endl;
	cout<<"  delete_book\tDelete a book"<<endl;
	cout<<"  add_user\tAdd a user"<<endl;
	cout<<"  delete_user\tDelete a user"<<endl;
	cout<<"  list_users\tList all users"<<endl;
	cout<<"  change_user_pass\tChange a user's password"<<endl;
	cout<<"  reset_user_pass\tReset a user's password to '123456'"<<endl;
}

void opt_search();
void opt_login();

void opt_borrow();
void opt_return();
void opt_change_pass();
void opt_list_records();
void opt_logout();

void opt_add_book();
void opt_delete_book();
void opt_add_user();
void opt_delete_user();
void opt_list_users();
void opt_change_user_pass();
void opt_reset_user_pass();

int main(){
    cout<<"Welcome to ssBooks system! "<<endl;
	cout<<"Type 'help' for help. "<<endl;

	string opt;
	for (;;){
		who(); cin>>opt;
		if (opt=="help") opt_help(); else

		if (opt=="search") opt_search(); else
		if (opt=="login") opt_login(); else

		if (opt=="borrow") opt_borrow(); else
		if (opt=="return") opt_return(); else
		if (opt=="change_pass") opt_change_pass(); else
		if (opt=="list_records") opt_list_records(); else
		if (opt=="logout") opt_logout(); else

		if (opt=="add_book") opt_add_book(); else
		if (opt=="delete_book") opt_delete_book(); else
		if (opt=="add_user") opt_add_user(); else
		if (opt=="delete_user") opt_delete_user(); else
		if (opt=="list_users") opt_list_users(); else
		if (opt=="change_user_pass") opt_change_user_pass(); else
		if (opt=="reset_user_pass") opt_reset_user_pass(); else

		cout<<"Invalid command! "<<endl;
		cout<<endl;
	}
}

bool check_login(){
	if (!s.get_user()){
		cout<<"ERROR: You don't have logged in. Please login first. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

bool check_nologin(){
	if (s.get_user()){
		cout<<"ERROR: You have already logged in. Please logout first. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

bool check_admin(){
	if (s.get_user()->get_group()){
		cout<<"ERROR: Access denied. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

void opt_search(){
	string opt;
	cout<<"What do you want to search by? (name/isbn/author/catelogy)"<<endl;
	who(); cin>>opt;
	if (opt=="name"){
		cout<<"Please input the name of the book: "<<endl;
		who(); cin>>opt;
		book* now = s.search_book_by_name(opt);
		if (now) now->print_info();
	} else if (opt=="isbn"){
		cout<<"Please input the ISBN of the book: "<<endl;
		who(); cin>>opt;
		book* now = s.search_book_by_isbn(opt);
		if (now) now->print_info();
	} else if (opt=="author"){
		cout<<"Please input the name of the author: "<<endl;
		who(); cin>>opt;
		s.list_books_by_author(opt);
	} else if (opt=="catelogy"){
		cout<<"Please input the catelogy (1~3 chars): "<<endl;
		who(); cin>>opt;
		// s.list_books_by_cate(opt);
	} else {
		cout<<"Invalid input. Exit."<<endl;
	}
}

void opt_login(){
	if (!check_nologin()) return;
	string username,password;
	cout<<"Username: "; cin>>username;
	cout<<"Password: "; cin>>password;
	if (s.user_login(username,password)) cout<<"Welcome back, "<<s.get_user()->get_id()<<"! :) "<<endl;
	else cout<<"Login failed. "<<endl; 
}

void opt_borrow(){
	if (!check_login) return;
	cout<<"Please input the name of the book you want to borrow: "<<endl;
	string name;
	who(); cin>>name;
	if (s.borrow_book(s.search_book_by_name(name))) cout<<"Success! You borrowed the book. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_return(){
	if (!check_login) return;
	cout<<"Please input the name of the book you want to return: "<<endl;
	string name;
	who(); cin>>name;
	if (s.return_book(s.search_book_by_name(name))) cout<<"Success! The book is returned. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_change_pass(){
	if (!check_login) return;
	string old_pass,new_pass,tmp;
	cout<<"Please enter your old password: "<<endl;
	who(); cin>>old_pass;
	cout<<"Please enter your new password: "<<endl;
	who(); cin>>new_pass;
	cout<<"Verify your password: "<<endl;
	who(); cin>>tmp;
	if (tmp != new_pass) {cout<<"Different password. "<<endl<<"Failed"<<endl; return;}
	if (s.get_user()->change_password(old_pass,new_pass)) cout<<"Success! Your password has been changed successfully. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_list_records(){
	if (!check_login) return;
	s.list_records();
}
void opt_logout(){
	if (!check_login) return;
	s.user_logout();
}

void opt_add_book(){
	if (!check_login() || !check_admin()) return;
	string name,isbn;
    vector<string> author; // 作者的列表
    string cate1,cate2,cate3; // 三级分类
	string confirm;
	cout<<"Please input the name of the book: "<<endl;
	who(); cin>>name;
	cout<<"Please input the ISBN of the book: "<<endl;
	who(); cin>>isbn;
	cout<<"Please input the author(s) of the book: "<<endl;
	who(); //!!!
	cout<<"Please input hte category of the book: "<<endl;
	who(); //!!!
	cout<<"Is that all right? Enter 'yes' to confirm. "<<endl;
	who(); cin>>confirm;
	if (confirm == "yes") s.add_book(book(name,isbn,&author,cate1,cate2,cate3));
	else cout<<"Not confirmed. Exit. "<<endl;
}
void opt_delete_book(){
	if (!check_login() || !check_admin()) return;
}
void opt_add_user(){
	if (!check_login() || !check_admin()) return;
}
void opt_delete_user(){
	if (!check_login() || !check_admin()) return;
}
void opt_list_users(){
	if (!check_login() || !check_admin()) return;
}
void opt_change_user_pass(){
	if (!check_login() || !check_admin()) return;
}
void opt_reset_user_pass(){
	if (!check_login() || !check_admin()) return;
}