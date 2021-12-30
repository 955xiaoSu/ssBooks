#pragma once

#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include "book.h"
using namespace std;
class user{
	private:
		string id; // 用户名
		string passwd; // 密码
		int group; // 用户组，0 表示管理员，1 表示用户

	public:
		vector<book*> books; // 已经借的书

		// user 类的构造函数
		// 参数：学工号
		user(string&);
		// user 类的全参构造函数
		// 参数：学工号，密码的哈希值，用户组
		user(string&, string&, int&);

		int get_group();
		string get_id();

		// 验证密码
		// 参数：输入的密码
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
		// 返回：是否成功（总是成功）
		bool reset_password();
};