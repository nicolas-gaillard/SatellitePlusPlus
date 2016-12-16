#include "Utils.h"

void splitStr(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

/*
 * Check if images contains img
 * Return true if, false otherwise
 */
bool contains(std::vector<Image> &images, Image img) {
    std::vector<Image>::iterator it;
    for(it=images.begin() ; it < images.end(); it++) {
        if (((*it).la == img.la)&&((*it).lo == img.lo)) {
            return true;
        }
    }
    return false;
}