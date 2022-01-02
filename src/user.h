#pragma once

#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include "book.h"
using namespace std;
class user{
	private:
		string id; // 用户 ID（用户名/学工号）
		string passwd; // 密码
		int group; // 用户组，0 表示管理员，1 表示用户
		vector<book*> books; // 已借阅、未归还的书的集合

	// 由于文件包含顺序问题，无法如此声明友元
	// friend library::~library();

	public:
		// user 类的构造函数
		// 参数：ID
		user(string&);
		// user 类的全参构造函数
		// 参数：ID，密码，用户组
		user(string&, string&, int);

		// 取用户组
		// 参数：无
		// 返回：用户组编号
		int get_group();
		// 取用户 ID
		// 参数：无
		// 返回：用户 ID
		string get_id();
		// 取密码，只在保存数据库时使用！
		// 参数：无
		// 返回：用户密码
		string get_passwd();
		// 取已借阅书籍
		// 参数：无
		// 返回：指向借阅记录列表的指针
		vector<book*>* get_books();

		// 验证密码
		// 参数：输入的密码
		// 返回：是否验证通过
		bool check_password(string&);

		// 用户修改密码
		// 参数：原密码，新密码
		// 返回：是否成功
		bool change_password(string&, string&);

		// 被管理员修改密码
		// 参数：新密码
		// 返回：是否成功（总是成功）
		bool change_password(string&);
		// 被管理员重设密码
		// 参数：无
		// 返回：是否成功（总是成功）
		bool reset_password();
};