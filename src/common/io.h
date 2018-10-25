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
using namespace std;
namespace stool
{
class IO
{
  public:
	template <typename T>
	static void load(std::ifstream &stream, vector<T> &vec, bool allReading)
	{
		if (!stream)
		{
			std::cerr << "error reading file " << endl;
			throw - 1;
		}
		uint64_t len;
		if (allReading)
		{
			stream.seekg(0, ios::end);
			uint64_t n = (unsigned long)stream.tellg();
			stream.seekg(0, ios::beg);
			len = n / sizeof(T);
		}
		else
		{
			stream.read((char *)(&len), sizeof(uint64_t));
		}
		vec.resize(len);
		stream.read((char *)&(vec)[0], len * sizeof(T));
	}

	template <typename T>
	static void load(std::ifstream &stream, vector<T> &vec)
	{
		load(stream, vec, true);
	}
	template <typename T>
	static void load(string &filename, vector<T> &vec)
	{
		ifstream inputStream1;
		inputStream1.open(filename, ios::binary);
		load(inputStream1, vec);

		inputStream1.close();
		inputStream1.clear();
	}

	template <typename T>
	static bool write(ofstream &out, vector<T> &text, bool allWriting)
	{
		if (!out)
			return 1;

		if (allWriting)
		{
			out.write(reinterpret_cast<const char *>(&text[0]), text.size() * sizeof(T));
		}
		else
		{
			uint64_t len = text.size();
			out.write(reinterpret_cast<const char *>(&len), sizeof(uint64_t));
			out.write(reinterpret_cast<const char *>(&text[0]), text.size() * sizeof(T));
		}
		return true;
	}
	template <typename T>
	static bool write(ofstream &out, vector<T> &text)
	{
		write(out, text, true);
		return true;
	}
	template <typename T>
	static bool write(string &filename, vector<T> &text)
	{
		ofstream out(filename, ios::out | ios::binary);
		write<T>(out, text);
		out.close();
		return true;
	}
	static bool load(std::ifstream &file, string &output)
	{

		if (!file)
		{
			std::cerr << "error reading file " << endl;
			return false;
		}
		file.seekg(0, ios::end);
		uint64_t n = (unsigned long)file.tellg();
		file.seekg(0, ios::beg);

		output.resize(n / sizeof(char));

		file.read((char *)&(output)[0], n);
		file.close();
		file.clear();

		return true;
	}
	static void load(string filename, string &output)
	{
		ifstream inputStream;
		inputStream.open(filename, ios::binary);
		load(inputStream, output);
	}
	//static bool write(string &filename, vector<int32_t> &text);
	static bool write(ofstream &os, std::string &text)
	{
		os.write((const char *)(&text[0]), sizeof(char) * text.size());
		return true;
	}
	static bool write(string &filename, std::string &text)
	{
		ofstream out(filename, ios::out | ios::binary);
		if (!out)
			return 1;
		return write(out, text);
	}
};
class FileReader
{
  public:
	static bool read(ifstream &file, vector<char> &output, uint64_t bufferSize, uint64_t textSize)
	{
		if (file.eof())
		{
			throw - 1;
		}
		uint64_t i = file.tellg();
		if (i == textSize)
			return false;

		uint64_t tmpBufferSize = min(textSize - i, bufferSize);
		output.resize(tmpBufferSize);
		file.read((char *)&(output)[0], tmpBufferSize * sizeof(char));

		return true;
	}
	static uint64_t getTextSize(ifstream &file)
	{
		file.seekg(0, ios::end);
		uint64_t textSize = (uint64_t)file.tellg() / sizeof(char);
		file.seekg(0, ios::beg);
		return textSize;
	}
	static uint64_t getTextSize(string filename)
	{

		ifstream inputStream;
		inputStream.open(filename, ios::binary);
		uint64_t p = getTextSize(inputStream);
		inputStream.close();
		return p;
	}
	static bool read(ifstream &file, vector<char> &output)
	{
		file.seekg(0, ios::end);
		uint64_t textSize = (uint64_t)file.tellg() / sizeof(char);
		file.seekg(0, ios::beg);

		uint64_t i = file.tellg();
		if (i == textSize)
			return false;

		output.resize(textSize);
		file.read((char *)&(output)[0], textSize * sizeof(char));

		return true;
	}

	static std::pair<bool, uint64_t> equalCheck(string filename, string filename2, uint64_t bufferSize)
	{
		ifstream stream, stream2;
		stream.open(filename, ios::binary);
		stream2.open(filename2, ios::binary);

		if (!stream)
		{
			std::cerr << "error reading file " << endl;
			throw - 1;
		}

		stream.seekg(0, ios::end);
		uint64_t textSize = (uint64_t)stream.tellg() / sizeof(char);
		stream.seekg(0, ios::beg);

		stream2.seekg(0, ios::end);
		uint64_t textSize2 = (uint64_t)stream2.tellg() / sizeof(char);
		stream2.seekg(0, ios::beg);

		if (textSize != textSize2)
		{
			stream.close();
			stream2.close();

			return std::pair<bool, uint64_t>(false, UINT64_MAX);
		}

		vector<char> tmp1, tmp2;
		uint64_t sum = 0;

		while (true)
		{
			bool b1 = FileReader::read(stream, tmp1, bufferSize, textSize);
			bool b2 = FileReader::read(stream2, tmp2, bufferSize, textSize2);

			for (uint64_t i = 0; i < tmp1.size(); i++)
			{
				if (tmp1[i] != tmp2[i])
				{
					stream.close();
					stream2.close();

					return std::pair<bool, uint64_t>(false, sum);
				}
				else
				{
					sum++;
				}
			}
			if (!b1)
				break;
		}
		stream.close();
		stream2.close();
		return std::pair<bool, uint64_t>(true, textSize);
	}
};
} // namespace stool