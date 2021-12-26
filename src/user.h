#pragma once

#include<iostream>
#include<cstring>
#include<cstdio>
// #include<openssl/md5.h>
#include<map>

using namespace std; 

string MD5(const string& src){
	// MD5_CTX ctx;
	string md5_string;
	// unsigned char md[16]={0};
	// char tmp[33]={0};
	// MD5_Init(&ctx);
	// MD5_Update(&ctx,src.c_str(),src.size());
	// MD5_Final(md,&ctx);
	// for (int i=0;i<16;i++){
	// 	memset(tmp,0x00,sizeof(tmp));
	// 	sprintf(tmp,"%02X",md[i]);
	// 	md5_string += tmp;
	// }
	return md5_string;
}

class user{
	private:
		string id; // 用户名
		string passwd; // 密码的 md5 哈希值
		int group; // 用户组，0 表示管理员，1 表示用户

	public:
		user(string&);
		user(string&, string&, int&);

	friend class library;
};

// user 类的构造函数
// 参数：学工号
user::user(string &id){
	this->id = id;
	this->passwd = MD5("123456");
	this->group = 0;
}

// user 类的全参构造函数
// 参数：学工号，密码的哈希值，用户组
user::user(string &id, string &passwd_hash, int &group){
	this->id = id;
	this->passwd = passwd_hash;
	this->group = group;
}
