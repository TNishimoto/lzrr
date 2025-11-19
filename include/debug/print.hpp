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

namespace stool
{

    namespace lzrr
    {
class Printer
{
  public:
	template <class X>
	static std::string toIntegerString(std::vector<X> &items)
	{
		std::string s = "";
		s += "[";
		//int k = items.size();
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
	static void toIntegerString(std::vector<X> &items, std::string &result)
	{
		result += "[";
		//int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			result += std::to_string(items[i]);
			if (i != items.size() - 1)
				result += ", ";
		}
		result += "]";
	}
	template <class X>
	static void toIntegerString(std::vector<X> &items, std::string &result, uint64_t padding)
	{
		result += "[";
		//int k = items.size();
		for (int i = 0; i < (int)items.size(); i++)
		{
			std::string s = "";

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
			if (i != (int)(items.size() - 1))
				result += ",";
		}
		result += "]";
	}
	static void toIntegerString(std::string &items, std::string &result, uint64_t padding)
	{
		result += "[";
		//int k = items.size();
		for (int i = 0; i < (int)items.size(); i++)
		{
			std::string s = "";

			s = items[i];
			while(s.size() < padding){
				s += " ";
			}

			result += s;
			if (i != (int)items.size() - 1)
				result += ",";
		}
		result += "]";
	}
	


	template <class X>
	static void print(std::vector<X> &items)
	{
		std::cout << toIntegerString<X>(items) << std::endl;
	}

	template <class X>
	static std::string toString(std::vector<X> &items, uint64_t padding)
	{
		std::string s = "";
		toIntegerString<X>(items, s, padding);
		return s;
	}
	static std::string toString(std::string &str, uint64_t padding)
	{
		std::string s = "";
		toIntegerString(str, s, padding);
		return s;
	}

	template <class X>
	static void write(std::string filename, std::vector<X> &items)
	{
		std::ofstream out(filename, std::ios::out | std::ios::binary);
		if (!out)
		{
			throw - 1;
		}
		std::string s = toIntegerString<X>(items);
		out.write((const char *)&s[0], sizeof(char) * s.size());
	}
	static bool write(std::ofstream &os, std::string &text)
	{
		os.write((const char *)(&text[0]), sizeof(char) * text.size());
		return true;
	}
	static bool write(std::string filename, std::string &text)
	{
		std::ofstream out(filename, std::ios::out | std::ios::binary);
		if (!out)
			return 1;
		return write(out, text);
	}
	static bool debugWrite(std::string &text)
	{
		std::ofstream out("debug.txt", std::ios::out | std::ios::binary | std::ios::app);
		if (!out)
			return 1;
		return write(out, text);
	}
	template <class X>
	static void debugWrite(std::vector<X> &items)
	{
		std::ofstream out("debug.txt", std::ios::out | std::ios::binary | std::ios::app);
		if (!out)
		{
			throw - 1;
		}
		std::string s = toIntegerString<X>(items);
		out.write((const char *)&s[0], sizeof(char) * s.size());
	}

};
} // namespace my
}