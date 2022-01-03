#include<cstdio>
#include<cstring>
#include<iostream>
#include<sstream>
#include "library.h"
using namespace std;

library* s;

void who(){
	cout<<(s->get_user()?s->get_user()->get_id():"Guest")<<" > ";
}

void opt_help(){
	cout<<endl<<"Welcome to ssBooks system! :)"<<endl;
	cout<<" HNU homework by SBK & SkyWT. "<<endl;
	cout<<" A tiny but delicate library manager. "<<endl<<endl;

	cout<<"Global Commands: "<<endl;
	cout<<"  search\tSearch for a book"<<endl;
	cout<<"  login\tLogin as a user/admin"<<endl;
	cout<<"  quit\tQuit the program"<<endl;

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

	cout<<"Please enter your library name: "<<endl;
	string s_name;
	cout<<" > "; getline(cin, s_name);
	// 如果直接这么写，会产生一个 library 的临时变量，导致先调用一次读文件写文件，出现错误
	// s = library(s_name);
	s = new library(s_name);

	cout<<endl<<"Welcome to "<<s->get_name()<<" ! "<<endl;
	cout<<" Type 'help' for help. "<<endl;

	string opt;
	for (;;){
		who(); getline(cin, opt);
		if (opt=="help") opt_help(); else

		if (opt=="search") opt_search(); else
		if (opt=="login") opt_login(); else
		if (opt=="quit"){
			cout<<"Bye~"<<endl;
			delete s;
			return 0;
		} else

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

		if (opt=="" || opt=="\n") continue; else
		cout<<"Invalid command '"<<opt<<"'! :( "<<endl;
		cout<<endl;
	}
}

bool check_login(){
	if (!s->get_user()){
		cout<<"ERROR: You don't have logged in. Please login first. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

bool check_nologin(){
	if (s->get_user()){
		cout<<"ERROR: You have already logged in. Please logout first. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

bool check_admin(){
	if (s->get_user()->get_group()){
		cout<<"ERROR: Access denied. "<<endl<<"Failed. "<<endl;
		return false;
	}
	return true;
}

void opt_search(){
	string opt;
	cout<<" > What do you want to search by? (name/isbn/author/catelogy)"<<endl;
	who(); getline(cin, opt);
	if (opt=="name"){
		cout<<" > Please input the name of the book: "<<endl;
		who(); getline(cin, opt);
		book* now = s->search_book_by_name(opt);
		if (now) now->print_info();
	} else if (opt=="isbn"){
		cout<<" > Please input the ISBN of the book: "<<endl;
		who(); getline(cin, opt);
		book* now = s->search_book_by_isbn(opt);
		if (now) now->print_info();
	} else if (opt=="author"){
		cout<<" > Please input the name of the author: "<<endl;
		who(); getline(cin, opt);
		s->list_books_by_author(opt);
	} else if (opt=="catelogy"){
		string cate1,cate2,cate3;
		cout<<" > Please input level1 catelogy: "<<endl;
		who(); getline(cin, cate1);
		cout<<" > Please input level2 catelogy, leave empty if you don't want: "<<endl;
		who(); getline(cin, cate2);
		cout<<" > Please input level3 catelogy, leave empty if you don't want: "<<endl;
		who(); getline(cin, cate3);

		int now_page = 0;
		for (;;){
			bool now_have = s->list_books_by_cate(cate1, cate2, cate3, now_page);
			if (!now_have) cout<<"No more books! "<<endl;
			cout<<endl<<" > Showing page "<<now_page+1<<". Input 'next'/'prev' or 'quit': "<<endl;
			who(); getline(cin, opt);

			bool quit=false;
			for (;;){
				if (opt=="next"){
					if (!now_have) cout<<"Already the last page! "<<endl;
					else {now_page++;break;}
				} else if (opt=="prev"){
					if (!now_page) cout<<"Already the first page! "<<endl;
					else {now_page--;break;}
				} else if (opt=="quit"){quit=true;break;}

				cout<<endl<<" > Showing page "<<now_page+1<<". Input 'next'/'prev' or 'quit': "<<endl;
				who(); getline(cin, opt);
			}
			if (quit){
				cout<<"Stop searching. "<<endl;
				break;
			}
		}
	} else {
		cout<<"Invalid input. Exit."<<endl;
	}
}

void opt_login(){
	if (!check_nologin()) return;
	string username,password;
	cout<<" > Username: "; getline(cin, username);
	cout<<" > Password: "; getline(cin, password);
	if (s->user_login(username,password)) cout<<"Welcome back, "<<s->get_user()->get_id()<<"! :) "<<endl;
	else cout<<"Login failed. "<<endl; 
}

void opt_borrow(){
	if (!check_login()) return;
	cout<<" > Please input the name of the book you want to borrow: "<<endl;
	string name;
	who(); getline(cin, name);
	if (s->borrow_book(s->search_book_by_name(name))) cout<<"Success! You borrowed the book. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_return(){
	if (!check_login()) return;
	cout<<" > Please input the name of the book you want to return: "<<endl;
	string name;
	who(); getline(cin, name);
	if (s->return_book(s->search_book_by_name(name))) cout<<"Success! The book is returned. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_change_pass(){
	if (!check_login()) return;
	string old_pass,new_pass,tmp;
	cout<<" > Please enter your old password: "<<endl;
	who(); getline(cin, old_pass);
	cout<<" > Please enter your new password: "<<endl;
	who(); getline(cin, new_pass);
	cout<<" > Verify your password: "<<endl;
	who(); getline(cin, tmp);
	if (tmp != new_pass) {cout<<"Different password. "<<endl<<"Failed"<<endl; return;}
	if (s->get_user()->change_password(old_pass,new_pass)) cout<<"Success! Your password has been changed successfully. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_list_records(){
	if (!check_login()) return;
	s->list_records();
}

void opt_logout(){
	if (!check_login()) return;
	cout<<"Bye, "<<s->get_user()->get_id()<<"! "<<endl;
	s->user_logout();
}

void opt_add_book(){
	if (!check_login() || !check_admin()) return;
	string name,isbn;
    vector<string> author; author.clear(); // 作者的列表
    string cate1,cate2,cate3; // 三级分类
	string authors,cates,now_tmp;
	string confirm;
	cout<<" > Please input the name of the book: "<<endl;
	who(); getline(cin, name);
	cout<<" > Please input the ISBN of the book: "<<endl;
	who(); getline(cin, isbn);

	cout<<" > Please input the author(s) of the book, seperate by ',': "<<endl;
	who(); getline(cin, authors);
	stringstream ss(authors);
	while (!ss.eof() && getline(ss, now_tmp, ',')) author.push_back(now_tmp);

	cout<<" > Please input hte category of the book, formatted: "<<endl;
	who(); getline(cin, cates);
	cate1 = cates[0]; cates.erase(0, 1);
	ss = stringstream(cates);
	getline(ss, cate2, '/');
	getline(ss, cate3, '@');

	cout<<" > Is that all right? Enter 'yes' to confirm, or else exit. "<<endl;
	who(); getline(cin, confirm);
	if (confirm == "yes") s->add_book(book(name, isbn, author, cate1, cate2, cate3)),cout<<"Success! New book added successfully. ";
	else cout<<"Not confirmed. Exit. "<<endl;
}

void opt_delete_book(){
	if (!check_login() || !check_admin()) return;
	string name,confirm;
	cout<<" > Please input the name of the book you want to delete: "<<endl;
	who(); getline(cin, name);
	cout<<" > Are you sure you want to delete '"<<name<<"' ? Input 'yes' to confirm, or else exit. "<<endl;
	who(); getline(cin, confirm);
	if (confirm == "yes"){
		if (s->del_book(s->search_book_by_name(name))) cout<<"Success! Book deleted successfully. "<<endl;
		else cout<<"Failed. "<<endl;
	} else cout<<"Not confirmed. Exit. "<<endl;
}

void opt_add_user(){
	if (!check_login() || !check_admin()) return;
	string id;
	cout<<" > Please input the id of the new user: "<<endl;
	who(); getline(cin, id);
	if (s->add_user(user(id))) cout<<"Success! The password is '123456' by default. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_delete_user(){
	if (!check_login() || !check_admin()) return;
	string id,confirm;
	cout<<" > Please input the id of the user you want to delete: "<<endl;
	who(); getline(cin, id);
	cout<<" > Are you sure you want to delete '"<<id<<"' ? Input 'yes' to confirm, or else exit. "<<endl;
	who(); getline(cin, confirm);
	if (confirm == "yes"){
		if (s->del_user(s->search_user(id))) cout<<"Success! User deleted successfully. "<<endl;
		else cout<<"Failed. "<<endl;
	} else cout<<"Not confirmed. Exit. "<<endl;
}

void opt_list_users(){
	if (!check_login() || !check_admin()) return;
	s->list_users();
}

void opt_change_user_pass(){
	if (!check_login() || !check_admin()) return;
	string id,new_pass;
	cout<<" > Please input the id of the user you want to change: "<<endl;
	who(); getline(cin, id);
	cout<<" > Please input the new password: "<<endl;
	who(); getline(cin, new_pass);
	if (s->change_user_password(s->search_user(id),new_pass)) cout<<"Success! User password changed successfully. "<<endl;
	else cout<<"Failed. "<<endl;
}

void opt_reset_user_pass(){
	if (!check_login() || !check_admin()) return;
	string id;
	cout<<" > Please input the id of the user you want to reset: "<<endl;
	who(); getline(cin, id);
	if (s->reset_user_password(s->search_user(id))) cout<<"Success! User password reseted successfully. "<<endl;
	else cout<<"Failed. "<<endl;
}