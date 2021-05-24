#include "stdafx.h"
#include "LexerParser.h"

CLexerParser::CLexerParser()
{

}

CLexerParser::~CLexerParser()
{

}

CLexerParser* CLexerParser::GetInstance(void)
{
    static CLexerParser instance;
    return &instance;
}

bool    CLexerParser::IsValidLineHeader(const std::string& line) const
{
    if(line.size() == 0)
        return false;
    if(line.find("//") != string::npos || line.find("#ifndef") != string::npos || line.find("#endif") != string::npos || line.find("#pragma once") != string::npos)
        return false;
    if(line.find("#define") == string::npos)
        return false;
    bool    isFullSpace = true;
    for(auto it : line)
        if(it != ' ' && it != '\t')
            isFullSpace = false;
    if(isFullSpace)
        return false;
    return true;
}


void    CLexerParser::RemoveEmpty(std::string& line)
{
    std::string tmp;
    size_t i = 0;
    size_t size = line.size();
    while(i < size && line[i])
    {
        tmp += line[i];
        if(line[i] == ' ' || line[i] == '\t')
        {
            tmp += ' ';
            while((line[i] == ' ' || line[i] == '\t') && line[i])
                ++i;
        }
        else
            ++i;
    }
    if(tmp[tmp.size() - 1] == ' ')
        tmp[tmp.size() - 1] = '\0';
    line.clear();
    line.assign(tmp);
}

bool    CLexerParser::LoadHeader(const char* pszFileName)
{
    if(!pszFileName)
        return false;

    OUTPUTDEBUGSTRING("%s: %s", __FUNCTION__, pszFileName);
    ifstream fd;
    fd.open(pszFileName);
    if(!fd.is_open())
        return false;

    std::string line;
    while(std::getline(fd, line))
    {
        if(!IsValidLineHeader(line))
            continue;
        RemoveEmpty(line);
        std::istringstream ss(line);
        std::string tmp;
        std::vector<std::string> segments;
        while(std::getline(ss, tmp, ' '))
        {
            tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
            tmp.erase(std::remove(tmp.begin(), tmp.end(), '\t'), tmp.end());
            if(tmp.size() > 0)
                segments.push_back(std::string(tmp));
        }
        if(segments.size() < 3)
            continue;
        std::istringstream sid(segments[2].c_str());
        unsigned int uid;
        sid >> uid;
        segments[1].erase(std::remove(segments[1].begin(), segments[1].end(), ' '), segments[1].end());
        segments[1].erase(std::remove(segments[1].begin(), segments[1].end(), '\t'), segments[1].end());
        if(segments[1].size() == 0)
            continue;
        m_defines[segments[1]] = static_cast<unsigned int>(uid);
    }
    fd.close();
    return true;
}

unsigned int    CLexerParser::GetHeader(const string& szHeader) const
{
    auto it = m_defines.find(szHeader);
    if(it != m_defines.end())
        return it->second;
    return 0;
}

void CLexerParser::LoadJson(picojson::value& v, const std::string& szFileJson)
{
    OUTPUTDEBUGSTRING("%s : loading %s", __FUNCTION__, szFileJson.c_str());
    stringstream ss;
    ifstream f;
    f.open(szFileJson);
    if(!f.is_open())
        throw std::exception("failed open");
    ss << f.rdbuf();
    f.close();
    ss >> v;
    if(!picojson::get_last_error().empty())
        throw std::exception(picojson::get_last_error().c_str());
}


BOOL    CLexerParser::WriteJson(picojson::value& v, const std::string& szFileName)
{
    ofstream myfile;
    myfile.open(szFileName);
    if(!myfile.is_open())
    {
        OUTPUTDEBUGSTRING("%s : not open %s", __FUNCTION__, szFileName);
        return FALSE;
    }
    myfile << picojson::value(v).serialize(true);
    myfile.close();
    return TRUE;
}
