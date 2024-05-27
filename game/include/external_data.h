#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>

class TagType {
public:
	std::string name;
	std::vector<std::string> keys;
	std::vector<std::string> defaultValues;
};

class Tag {
public:
	std::string getValue(const std::string &key);

	std::string type;
	std::vector<std::string> keys;
	std::vector<std::string> values;
};

std::optional<std::vector<Tag>> getTags(const std::string &data);
std::optional<TagType> getTagType(const std::string &name);
