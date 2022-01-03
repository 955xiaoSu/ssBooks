#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include<map>
#include "user.h"
#include "book.h"
using namespace std;

class library{
	private:
		string name; // 图书馆名
		user* this_user; // 当前用户指针

		vector<pair<user*, book* > > records_borrow; // 总借阅记录
		vector<pair<user*, book* > > records_return; // 总归还记录

		map<string, user*> user_link; // 用户 ID 到用户指针的映射
		map<string, book*> book_name_link; // 书名到书的指针的映射
		map<string, book*> book_isbn_link; // 书 ISBN 到书的指针的映射
		map<string, vector<book*> > author_link; // 作者名到 指向作者所著书的集合的指针 的映射

		bool loading; // 是否处于加载模式，加载模式下忽略一切权限限制

	public:
		// 初始化，无参构造函数，不实际使用
		// 参数：无
		library();
		// 初始化，包含文件读入等
		// 参数：图书馆的名称，也是文件名
		library(const string&);
		// 析构函数，包含文件保存等
		~library();
		// 取当前图书馆名
		// 参数：无
		// 返回：当前图书馆名
		string get_name();
		// 取当前用户指针
		// 参数：无
		// 返回：当前用户指针
		user* get_user();

		// 图书查找部分
		// 用户组：所有

		// 根据书名查找图书
		// 参数：图书名
		// 返回：书的指针
		book* search_book_by_name(const string&);
		// 根据 ISBN 查找图书
		// 参数：图书 ISBN
		// 返回：书的指针
		book* search_book_by_isbn(const string&);
		// 模糊查找，根据作者列出图书
		// 参数：作者名称
		// 返回：是否有结果
		bool list_books_by_author(const string&);
		// 模糊查找，根据分类列出图书
		// 参数：一级分类、二级分类、三级分类、页码（每页 page_size 本书，常量在 library.cpp 中定义）
		//   对应分类如果不提供，用空字符串代替
		// 返回：是否有结果
		bool list_books_by_cate(const string&, const string&, const string&, const int);

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
		// 返回：指向书的指针，失败返回 NULL
		book* add_book(book);
		// 删除图书
		// 参数：指向要删除图书的指针
		// 返回：是否成功
		bool del_book(book*);

		// 用户操作部分
		// 用户组：用户（除了 login）

		// 用户登录
		// 参数：用户名，密码
		// 返回：是否成功
		bool user_login(const string&, const string&);
		// 用户退出
		// 参数：无
		// 返回：是否成功
		bool user_logout();
		// 用户查询自己的借阅记录
		// 参数：无
		// 返回：是否成功
		bool list_records();

		// 用户管理部分
		// 权限：管理员

		// 查询用户
		// 参数：用户名
		// 返回：指向该用户的指针
		user* search_user(const string&);
		// 修改用户密码
		// 参数：用户指针，新的密码
		// 返回：是否成功
		bool change_user_password(user*, const string&);
		// 重设密码为 123456
		// 参数：用户指针
		// 返回：是否成功
		bool reset_user_password(user*);
		// 列出所有用户
		// 参数：无
		// 返回：是否成功
		bool list_users();
		// 增加用户
		// 参数：要增加的用户对象
		// 返回：指向用户的指针，失败返回 NULL
		user* add_user(user);
		// 删除用户
		// 参数：指向要删除用户的指针
		// 返回：是否成功
		bool del_user(user*);
};
