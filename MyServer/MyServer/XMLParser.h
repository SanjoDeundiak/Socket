#pragma once

#include <string>
#include <fstream>

class XMLParser
{
	public:
		void setPath(const std::string &path);
		XMLParser();
		void Add(std::string tagname, std::string tagvalue);
		void Set(std::string tagname, std::string tagvalue);
		std::string Get(std::string tagname);
		void Remove(std::string ragname);
	private:
		std::string m_path;
};