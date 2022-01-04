#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
#include "library.h"
using namespace std;

library::library(){
	name = "";
	this_user = NULL;
	records_borrow.clear();
	records_return.clear();
	user_link.clear();
	book_name_link.clear();
	book_isbn_link.clear();
	author_link.clear();
}

// 擦除字符串函数，删除 s1 中的 s2 子串
// 参数：待操作的字符串（引用），要擦除的子串
void string_erase(string& s1, const string& s2){
	int pos = s1.find(s2);
	if (pos != (int)string::npos) s1.erase(pos, s2.length());
}

// “清洗”字符串函数
// 关于换行符问题：Windows 中一个换行有两个字符，分别是换行符 10 和回车符 13
//   而 Unix 系统中换行只有换行符 10，也认为 \n 就是 10
//   给出的数据是 Windows 中生成的，存在回车符处于 getline 得到的字符串尾部
//   所以需要 wash 一下 :(
// 参数：待清洗的字符串（引用）
void wash(string& str){
	if (str[(int)str.length()-1] == 13) str.erase((int)str.length()-1, 1);
}

// 数据库文件格式的说明
// [books] - 书的信息，每条记录格式为：
//   书名 ISBN 作者列表 分类列表
// [users] - 用户信息，每条记录格式为：
//   用户ID 密码 用户组编号 借阅列表
// [records] - 借阅记录，每条记录格式为：
//   用户ID 书IDBN
// [records_return] - 隶属于借阅记录部分，归还记录，每条记录格式为：
//   用户ID 书ISBN
library::library(const string& s){
	name = s;
	this_user = NULL;
	records_borrow.clear();
	records_return.clear();
	user_link.clear();
	book_name_link.clear();
	book_isbn_link.clear();
	author_link.clear();
	loading = true;

	fstream fin;
	string name_tmp = name + ".db";
	fin.open(name_tmp.c_str(), ios::in);
	if (!fin){
		cout<<"No database file found. "<<endl;
		cout<<"New library " + name + " created. "<<endl;
		cout<<endl;
		cout<<"Default admin user: "<<endl;
		cout<<" ID: admin"<<endl;
		cout<<" Password: 123456"<<endl;
		string s1="admin",s2="123456";
		add_user(user(s1,s2,0));
		return;
	} else {
		cout<<"Loading file " + name_tmp<<" ... "<<endl;
	}

	cout<<"Loading books..."<<endl;
	string tmp;

	getline(fin, tmp); wash(tmp);
	if (tmp != "[books]"){
		cout<<"ERROR: broken file! "<<endl;
		exit(1);
	}
	getline(fin, tmp); wash(tmp);
	while (tmp != "[users]" && !fin.eof()){
		string name, isbn, authors, cates, num;
		stringstream ss(tmp);
		getline(ss, name, ' ');
		getline(ss, isbn, ' ');
		getline(ss, authors, ' ');
		getline(ss, cates, ' ');
		getline(ss, num, ' ');
		// stoi(str) 将 str 转换成 int
		add_book(name, isbn, authors, cates, stoi(num));

		getline(fin, tmp); wash(tmp);
	}

	cout<<"Loading users..."<<endl;
	if (!fin.eof() && tmp=="[users]"){
		getline(fin, tmp); wash(tmp);
		while (tmp!="[records]"){
			// 最后一行如果是空行， 会 getline 进一个空字符串而不是换行符！
			if (tmp == "") break;
			stringstream ss(tmp);
			string id, passwd, group, user_books;
			getline(ss, id, ' ');
			getline(ss, passwd, ' ');
			getline(ss, group, ' ');
			user* now_user = add_user(user(id, passwd, (int)(group=="1")));
			user_link[now_user->get_id()] = now_user;

			// 处理这个用户借阅的书籍
			getline(ss, user_books, '@');
			if (user_books != "" && user_books != " "){
				ss = stringstream(user_books);
				while (!ss.eof()){
					string now_book_isbn;
					getline(ss, now_book_isbn, ',');
					book* now_book = search_book_by_isbn(now_book_isbn);
					now_book->borrow();
					now_user->get_books()->push_back(now_book);
				}
			}

			// 文件尾 EOF 一定要在这里判断！！！
			if (fin.eof()) break;
			getline(fin, tmp); wash(tmp);
		}
	}

	cout<<"Loading records..."<<endl;
	if (!fin.eof() && tmp=="[records]"){
		getline(fin, tmp); wash(tmp);
		bool flag=false;
		while (true){
			if (tmp == "") break;
			if (tmp == "[records_return]") {flag=true;if (fin.eof()) break;getline(fin, tmp);wash(tmp);continue;}
			stringstream ss(tmp);
			string user,isbn;
			getline(ss, user, ' ');
			getline(ss, isbn, ' ');
			if (!flag) records_borrow.push_back(make_pair(search_user(user), search_book_by_isbn(isbn)));
			else records_return.push_back(make_pair(search_user(user), search_book_by_isbn(isbn)));

			if (fin.eof()) break;
			getline(fin, tmp); wash(tmp);
		}
	}

	fin.close();
	cout<<"Loading finish. "<<endl;

	this_user = NULL;
	loading = false;
}

library::~library(){
	cout<<"Saving your file... "<<endl;
	fstream fout;
	string name_tmp = name + ".db";
	fout.open(name_tmp.c_str(), ios::out | ios::trunc);
	if (!fout){
		cout<<"No database fin found. "<<endl;
		cout<<"New library " + name + " created. "<<endl;
	} else {
		cout<<"Saving to file " + name_tmp<<". "<<endl;
	}

	cout<<"Saving books... "<<endl;
	fout<<"[books]"<<endl;
	for (map<string, book*>::iterator it=book_name_link.begin(); it!=book_name_link.end(); it++){
		fout << it->first << " " << it->second->get_isbn() << " ";
		vector<string>& vec = *(it->second->get_authors());
		for (int i=0; i<(int)vec.size(); i++){
			fout<<vec[i];
			if (i!=(int)vec.size()-1) fout<<",";
		}
		fout << " " << it->second->get_cate1() << it->second->get_cate2() << "/" << it->second->get_cate3() << " " << it->second->get_number() << endl;
	}

	cout<<"Saving users... "<<endl;
	fout<<"[users]"<<endl;
	for (map<string, user*>::iterator it=user_link.begin(); it!=user_link.end(); it++){
		fout << it->first << " " << it->second->get_passwd() << " " << it->second->get_group() << " ";
		vector<book*>& vec = *(it->second->get_books());
		for (int i=0; i<(int)vec.size(); i++){
			fout << vec[i]->get_isbn();
			if (i != (int)vec.size()-1) fout<<",";
		}
		fout<<endl;
	}

	cout<<"Saving records... "<<endl;
	fout<<"[records]"<<endl;
	for (vector<pair<user*, book* > >::iterator it=records_borrow.begin(); it!=records_borrow.end(); it++)
		fout << it->first->get_id() << " " << it->second->get_isbn() << endl;
	fout<<"[records_return]"<<endl;
	for (vector<pair<user*, book* > >::iterator it=records_return.begin(); it!=records_return.end(); it++)
		fout << it->first->get_id() << " "<< it->second->get_isbn() << endl;

	cout<<"Cleaning up..."<<endl;
	for (map<string, book*>::iterator it=book_name_link.begin(); it!=book_name_link.end(); it++)
		delete (it->second);
	for (map<string, user*>::iterator it=user_link.begin(); it!=user_link.end(); it++)
		delete it->second;

	cout<<"Successfully saved the database. "<<endl;
}

string library::get_name(){
	return this->name;
}

user* library::get_user(){
	return this->this_user;
}

// 图书查找部分
// 用户组：所有

book* library::search_book_by_name(const string& book_name){
	if (!book_name_link[book_name]){
		cout<<"ERROR: Book "<<book_name<<" doesn't exist."<<endl;
		return NULL;
	}
	return book_name_link[book_name];
}

book* library::search_book_by_isbn(const string& book_isbn){
	if (!book_isbn_link[book_isbn]){
		cout<<"ERROR: Book "<<book_isbn<<" doesn't exist."<<endl;
		return NULL;
	}
	return book_isbn_link[book_isbn];
}

bool library::list_books_by_author(const string& author_name){
	bool ret=false;
	vector<book*>& s = author_link[author_name];
	for (vector<book*>::iterator it=s.begin(); it!=s.end(); it++)
		(*it)->print_info(),ret=true;
	return ret;
}

// 每页的显示数量可以以此配置
const int page_size = 50;

bool library::list_books_by_cate(const string& cate1, const string& cate2, const string& cate3, const int page){
	bool ret=false;
	map<string, book*>::iterator it;
	int count = 0;
	for (it=book_name_link.begin(); it!=book_name_link.end(); it++){
		book* now_book = it->second;
		if (now_book->get_cate1() == cate1 && (cate2=="" || now_book->get_cate2()==cate2) && (cate3=="" || now_book->get_cate3()==cate3)){
			count++;
			if (page*page_size < count && count <= (page+1)*page_size) now_book->print_info(),ret=true;
		}
		if (count > (page+1)*page_size) break;
	}
	return ret;
}

// 图书借阅部分
// 用户组：用户

#define vb_it vector<book*>::iterator

bool library::borrow_book(book* now_book){
	if (now_book == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	if (this_user == NULL) {cout<<"ERROR: Please log in first."<<endl; return false;}
	if (!now_book->borrow()){
		cout<<"ERROR: Book already borrowed."<<endl;
		return false;
	}
	this_user->get_books()->push_back(now_book);
	records_borrow.push_back(make_pair(this_user, now_book));
	return true;
}

bool library::return_book(book* now_book){
	if (now_book == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	if (this_user == NULL) {cout<<"ERROR: Please log in first."<<endl; return false;}
	bool flag = false;
	vector<book*>& vec = *(this_user->get_books());
	for (vb_it it=vec.begin(); it!=vec.end(); it++)
		if ((*it) == now_book) {vec.erase(it); now_book->returnn(); flag=true; break;}
	if (!flag){
		cout<<"ERROR: Book not borrowed by you."<<endl;
		return false;
	} else records_return.push_back(make_pair(this_user, now_book));
	return true;
}

#undef vb_it

// 图书管理部分
// 用户组：管理员

#define vs_it vector<string>::iterator
#define vb_it vector<book*>::iterator

book* library::add_book(const string& the_name, const string& the_isbn, const string& authors, const string& cates, int the_number){
	if (book_isbn_link[the_isbn]){
		book* now_book = book_isbn_link[the_isbn];
		now_book->add(the_number);
		return now_book;
	}

	stringstream ss;
	vector<string> author; author.clear(); // 作者的列表
    string cate1,cate2,cate3; // 三级分类

	// 处理作者
	// 一个作者集合字符串分为两种情况：翻译书和国内书
	// 对于翻译书，考虑 作者,作者,作者;译者,译者 的格式
	// 对于国内书，考虑 作者,作者,作者 的格式
	ss = stringstream(authors);
	if (authors.find(';') != string::npos){ // 分为国外作者和国内译者
		string writers,transers;
		getline(ss, writers, ';');
		getline(ss, transers, '@');

		string now_name;
		ss = stringstream(writers);
		while (!ss.eof() && getline(ss, now_name, ',')) author.push_back(now_name);
		ss = stringstream(transers);
		while (!ss.eof() && getline(ss, now_name, ',')) author.push_back(now_name);

		// 处理乱七八糟的格式问题
		// 末尾可能有 译 等译 [等]译 等后缀
		string tmp_chinese;
		string& last_name = author[(int)author.size()-1];
		tmp_chinese = "等译";
		string_erase(last_name, tmp_chinese);
		tmp_chinese = "[等]译";
		string_erase(last_name, tmp_chinese);
		tmp_chinese = "译";
		string_erase(last_name, tmp_chinese);
		// 开头可能有 作者 编者 主编 等前缀
		string& first_name = author[0];
		tmp_chinese = "作者";
		string_erase(first_name, tmp_chinese);
		tmp_chinese = "编者";
		string_erase(first_name, tmp_chinese);
		tmp_chinese = "主编";
		string_erase(first_name, tmp_chinese);
	} else { // 只有作者没有译者
		string now_name;
		while (getline(ss, now_name, ',')) author.push_back(now_name);
	}

	// 处理分类
	// 格式比较混乱
	// 考虑第一个大写字母为一级分类，之后到 / 符号之前为二级分类，之后为三级分类
	string n_cates = cates;
	cate1 = cates[0]; n_cates.erase(0, 1);
	ss = stringstream(n_cates);
	getline(ss, cate2, '/');
	getline(ss, cate3, '@');

	return add_book(book(the_name, the_isbn, author, cate1, cate2, cate3, the_number));
}

book* library::add_book(book now_book_obj){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return NULL;}
	if (book_isbn_link[now_book_obj.get_isbn()]){
		cout<<"ERROR: Book "<<now_book_obj.get_isbn()<<" already exist."<<endl;
		return NULL;
	}
	book* now_book = new book(now_book_obj);
	book_name_link[now_book->get_name()] = now_book;
	book_isbn_link[now_book->get_isbn()] = now_book;
	vector<string>& vec = *(now_book->get_authors());
	for (vs_it it=vec.begin(); it!=vec.end(); it++)
		author_link[*it].push_back(now_book);
	return now_book;
}

bool library::del_book(book* now_book){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now_book == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	book_name_link.erase(now_book->get_name());
	book_isbn_link.erase(now_book->get_isbn());
	vector<string>& vec_a = *(now_book->get_authors());
	for (vs_it it=vec_a.begin(); it!=vec_a.end(); it++){
		vector<book*>& vec = author_link[*it];
		for (vb_it itt=vec.begin(); itt!=vec.end(); itt++){
			cout<<"!"<<*itt<<" ";
			// vecotr 的 iterator 在删除元素之后会失效！所以这里 erase 之后必须要 break
			if ((*itt) == now_book){vec.erase(itt);break;}
		}
	}
	delete now_book;
	return true;
}

#undef vs_it
#undef vb_it

// 用户操作部分
// 用户组：用户（除了 login）

bool library::user_login(const string& username, const string& password){
	if (this_user) {cout<<"ERROR: You have already logged in. Please logout first. "<<endl; return false;}
    user* now = user_link[username];
	if (!now){cout<<"ERROR: No such user. "<<endl; return false;}
    if (now->check_password(password)) {this_user = now; return true;}
	cout<<"ERROR: Wrong password. "<<endl;
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
		if (v1[i].first == this_user) cout<<v1[i].second->get_name()<<endl;
	cout<<"Returned: "<<endl;
	for (int i=0; i<(int)v2.size(); i++)
		if (v2[i].first == this_user) cout<<v2[i].second->get_name()<<endl;
	return true;
}
#undef v1
#undef v2

// 用户管理部分
// 权限：管理员

user* library::search_user(const string& username){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return NULL;}
	return user_link[username];
}

bool library::change_user_password(user* now_user, const string& new_password){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now_user == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	now_user->change_password(new_password);
	return true;
}

bool library::reset_user_password(user* now_user){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now_user == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	now_user->reset_password();
	return true;
}

bool library::list_users(){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	for (map<string, user* >::iterator it=user_link.begin(); it!=user_link.end(); it++)
		cout<<" "<<it->first<<endl;
	return true;
}

user* library::add_user(user now_user_obj){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return NULL;}
	if (user_link[now_user_obj.get_id()]) {cout<<"ERROR: User already exist."<<endl; return NULL;}
	user* now_user = new user(now_user_obj);
	user_link[now_user->get_id()] = now_user;
	return now_user;
}

bool library::del_user(user* now_user){
	if (!loading && this_user->get_group()) {cout<<"ERROR: Access denied."<<endl; return false;}
	if (now_user == NULL) {cout<<"ERROR: Empty ptr."<<endl; return false;}
	user_link.erase(now_user->get_id());
	delete now_user;
	return true;
}
