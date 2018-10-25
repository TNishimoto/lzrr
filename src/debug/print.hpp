#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <exception>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <time.h>
//#include <stdio.h>
//#include <windows.h>
//#include <Psapi.h>      // Psapi.Lib
using namespace std;

namespace stool
{

class Printer
{
  public:
	template <class X>
	static string toIntegerString(vector<X> &items)
	{
		string s = "";
		s += "[";
		int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			if(items[i] == UINT64_MAX){
			s += "-";

			}else{
			s += std::to_string(items[i]);
			}
			if (i != items.size() - 1)
				s += ", ";
		}
		s += "]";
		return s;
	}
	template <class X>
	static void toIntegerString(vector<X> &items, string &result)
	{
		result += "[";
		int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			result += std::to_string(items[i]);
			if (i != items.size() - 1)
				result += ", ";
		}
		result += "]";
	}
	template <class X>
	static void toIntegerString(vector<X> &items, string &result, uint64_t padding)
	{
		result += "[";
		int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			string s = "";

			if(items[i] == UINT64_MAX){
			s = "-";
			}else if(items[i] == UINT64_MAX-1){
			s = "*";
			}else{
			s = std::to_string(items[i]);
			}
			while(s.size() < padding){
				s += " ";
			}

			result += s;
			if (i != items.size() - 1)
				result += ",";
		}
		result += "]";
	}
	static void toIntegerString(string &items, string &result, uint64_t padding)
	{
		result += "[";
		int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			string s = "";

			if(items[i] == UINT64_MAX){
			s = "-";
			}
			else if(items[i] == UINT64_MAX-1){
			s = "*";
			}
			else{
			s = items[i];
			}
			while(s.size() < padding){
				s += " ";
			}

			result += s;
			if (i != items.size() - 1)
				result += ",";
		}
		result += "]";
	}
	


	template <class X>
	static void print(vector<X> &items)
	{
		std::cout << toIntegerString<X>(items) << std::endl;
	}

	template <class X>
	static string toString(vector<X> &items, uint64_t padding)
	{
		string s = "";
		toIntegerString<X>(items, s, padding);
		return s;
	}
	static string toString(string &str, uint64_t padding)
	{
		string s = "";
		toIntegerString(str, s, padding);
		return s;
	}

	template <class X>
	static void write(string filename, vector<X> &items)
	{
		ofstream out(filename, ios::out | ios::binary);
		if (!out)
		{
			throw - 1;
		}
		string s = toIntegerString<X>(items);
		out.write((const char *)&s[0], sizeof(char) * s.size());
	}
	static bool write(ofstream &os, std::string &text)
	{
		os.write((const char *)(&text[0]), sizeof(char) * text.size());
		return true;
	}
	static bool write(string filename, std::string &text)
	{
		ofstream out(filename, ios::out | ios::binary);
		if (!out)
			return 1;
		return write(out, text);
	}
	static bool debugWrite(std::string &text)
	{
		ofstream out("debug.txt", ios::out | ios::binary | ios::app);
		if (!out)
			return 1;
		return write(out, text);
	}
	template <class X>
	static void debugWrite(vector<X> &items)
	{
		ofstream out("debug.txt", ios::out | ios::binary | ios::app);
		if (!out)
		{
			throw - 1;
		}
		string s = toIntegerString<X>(items);
		out.write((const char *)&s[0], sizeof(char) * s.size());
	}

};
} // namespace my