#pragma once

#include<iostream>
#include<cstring>
#include<map>
#include<set>
#include "user.h"
#include "book.h"

using namespace std;

class library{
	private:
		string name; // 图书馆名
		user* this_user; // 当前用户

		vector<pair<user*, book* > > records_borrow; // 借阅记录
		vector<pair<user*, book* > > records_return; // 归还记录

		//定义多个映射，所有查找都可以实现 log 级别的响应
		map<string, user*> user_link;
		map<string, book*> book_name_link;
		map<string, book*> book_isbn_link;
		map<string, set<string> > author_link;

	public:
		// 初始化，包含文件读入等
		library(string&);
		// 返回当前图书馆名
		string get_name();
		// 返回当前用户指针
		user* get_user();

	// 图书查找部分
	// 用户组：所有

		// 根据书名查找图书
		book* search_book_by_name(string&);
		// 根据 ISBN 查找图书
		book* search_book_by_isbn(string&);
		// 模糊查找，根据作者列出图书
		void list_books_by_author(string&);
		// 模糊查找，根据分类列出图书
		// 参数：一级分类、二级分类、三级分类、页码（每页 50 本书）
		// 对应分类如果不提供，用空字符串代替
		void list_books_by_cate(string&, string&, string&, int);

	// 图书借阅部分
	// 用户组：用户

		// 借阅图书
		// 参数：要借图书的指针
		// 返回：是否成功
		bool borrow_book(book*);
		// 归还图书
		// 参数：要还图书的指针
		// 返回：是否成功
		bool return_book(book*);

	// 图书管理部分
	// 用户组：管理员

		// 增加图书
		// 参数：要增加的图书对象
		// 返回：是否成功（ISBN 重复时不成功）
		bool add_book(book);
		// 删除图书
		// 参数：指向要删除图书的指针
		bool del_book(book*);

	// 用户操作部分
	// 用户组：用户（除了 login）

		// 用户登录
		// 参数：用户名，密码
		// 返回：是否成功
		bool user_login(string&, string&);
		// 用户退出
		bool user_logout();
		// 用户修改密码
		// 参数：用户名，原密码，新密码
		// 返回：是否成功
		bool user_change_password(string&, string&, string&);
		// 用户查询自己的借阅记录
		bool list_records();

	// 用户管理部分
	// 权限：管理员

		// 查询用户
		// 参数：用户名
		// 返回：指向该用户的指针
		user* search_user(string&);
		// 修改用户密码
		// 参数：用户指针，新的密码
		bool change_user_password(user*, string&);
		// 重设密码为 123456
		// 参数：用户指针
		bool reset_user_password(user*);
		// 列出所有用户
		// 返回：是否成功
		bool list_users();
		// 增加用户
		// 参数：要增加的用户对象
		// 返回：是否成功（用户名重复的情况下不成功）
		bool add_user(user);
		// 删除用户
		// 参数：指向要删除用户的指针
		// 返回：是否成功
		bool del_user(user*);
};