#pragma once
#include "StructData.h"
#include <string>
#include <vector>
#include <sstream>

void splitStr(const std::string &s, char delim, std::vector<std::string> &elems);

bool contains(std::vector<Image> &images, Image img);