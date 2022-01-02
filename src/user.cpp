#pragma once

#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include "user.h"
using namespace std;

user::user(const string &id){
	this->id = id;
	this->passwd = "123456";
	this->group = 0;
}

user::user(const string &id, const string &passwd, int group){
	this->id = id;
	this->passwd = passwd;
	this->group = group;
}

int user::get_group(){
	return this->group;
}

string user::get_id(){
	return this->id;
}

string user::get_passwd(){
	return this->passwd;
}

vector<book*>* user::get_books(){
	return &books;
}

bool user::check_password(const string& pass){
	return this->passwd == pass;
}

bool user::change_password(const string& old_pass, const string& new_pass){
    if (old_pass == this->passwd) {this->passwd = new_pass; return true;}
    else return false;
}

bool user::change_password(const string& new_pass){
	this->passwd = new_pass;
	return true;
}

bool user::reset_password(){
	this->passwd = "123456";
	return true;
}
