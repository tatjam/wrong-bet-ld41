#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <streambuf>

std::vector<std::string> splitByDelimiter(std::string& data, char delim);

std::string loadStringFromFile(std::string path);