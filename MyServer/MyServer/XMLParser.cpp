#include "XMLParser.h"
#include <stdio.h>

XMLParser::XMLParser()	
{
}

void XMLParser::setPath(const std::string &path)
{
	m_path = path;
}

void XMLParser::Remove(std::string tagname)
{
	std::fstream file;
	std::string newName = m_path + ".temp";
	std::ofstream copy(newName);
	tagname = "<" + tagname + ">";
	std::string line;
	file.open(m_path, std::ios_base::in);
	while (file.good())
	{
		std::getline(file, line);		
		if (line.find(tagname) == std::string::npos)
		{
			copy << line << std::endl;
		}
	}

	file.close();
	copy.close();
	std::remove(m_path.c_str());
	std::rename(newName.c_str(), m_path.c_str());
}

void XMLParser::Set(std::string tagname, std::string tagvalue)
{
	Remove(tagname);
	Add(tagname, tagvalue);
}

std::string XMLParser::Get(std::string tagname)
{
	std::fstream file;
	std::string result;
	std::string revtagname = "</" + tagname + ">";
	tagname = "<" + tagname + ">";
	std::string line;
	file.open(m_path, std::ios_base::in);
	while (file.good())
	{
		std::getline(file, line);		
		if (line.find(tagname) == 0)
		{
			size_t f = line.find(revtagname);
			if (f != std::string::npos)
				result = line.substr(tagname.length(), f - tagname.length());			
		}
	}

	file.close();
	return result;
}

void XMLParser::Add(std::string tagname, std::string tagvalue)
{
	if (Get(tagname) != "")
		throw std::logic_error("Tag " + tagname + " already exists");
	std::fstream file;
	file.open(m_path, std::ios_base::app);
	if (file.good())
		file << '<' << tagname << '>' << tagvalue << "</" << tagname << '>' << std::endl;
	else
		throw std::runtime_error("File error");
	file.close();
}