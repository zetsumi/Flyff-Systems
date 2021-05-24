#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "debug.h"
#include "picojson.h"


class CLexerParser
{
    std::unordered_map<string, unsigned int>    m_defines;
    CLexerParser();

    bool    IsValidLineHeader(const std::string& line) const;
    void    RemoveEmpty(std::string& line);

public:
    ~CLexerParser();

    static CLexerParser* GetInstance(void);
    bool    LoadHeader(const char* pszFileName);
    unsigned int    GetHeader(const string& szHeader) const;
    void    LoadJson(picojson::value& v, const std::string& szFileJson);
    BOOL    WriteJson(picojson::value& v, const std::string& szFileName);
};