#include "../include/external_data.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>

std::vector<TagType> tagTypes;

std::optional<std::string> getFileData(const std::string &filepath) {
	std::ifstream file(filepath);
	if (!file) return {};

	std::string data;
	std::string line;

	while (getline(file, line)) {
		data.append(line + "\n");
	}
	file.close();

	return std::make_optional(data);
}

void getTagTypes(const std::string &data) {
	size_t pos = 0;
	while ((pos = data.find("define", pos)) != std::string::npos) {
		TagType tagType;
		pos += 7;
		while (isalnum(data[pos])) {
			tagType.name += data[pos++];
		}

		std::string key;
		std::string value;
		while (data[++pos] != '}') {
			if (data[pos] == ',') {
				if (key.size() < 1) continue;
				tagType.keys.push_back(key);
				tagType.defaultValues.push_back("");
				key = "";

			}
			else if (data[pos] == '=') {
				tagType.keys.push_back(key);
				tagType.defaultValues.push_back("");
				key = "";

				while (data[++pos] != '"') {};
				while (data[++pos] != '"') {
					value += data[pos];
				}
				tagType.defaultValues[tagType.defaultValues.size() - 1] = value;
				value = "";
			}
			else if (isalnum(data[pos])) {
				key += data[pos];
			}
		}

		bool isDuplicate = false;
		for (size_t i = 0; i < tagTypes.size(); i++) {
			if (tagTypes[i].name == tagType.name) {
				isDuplicate = true;
			}
		}
		if (!isDuplicate) {
			tagTypes.push_back(tagType);
		}
	}
}

std::optional<std::vector<Tag>> getTags(const std::string &filepath) {
	auto result = getFileData(filepath);
	if (!result.has_value()) return {};

	std::string data = result.value();
	getTagTypes(data);

	std::vector<Tag> tags;
	size_t pos = -1;
	bool inTag = false;
	std::string key;
	std::string value;

	Tag tag;
	TagType tagType;
	while (++pos < data.size()) {
		if (data[pos] == '<') {
			inTag = true;
			while (isalnum(data[++pos])) {
				tag.type += data[pos];
			}
			for (size_t i = 0; i < tagTypes.size(); i++) {
				if (tagTypes[i].name == tag.type) {
					tagType = tagTypes[i];
				}
			}
		}
		if (data[pos] == '>') {
			for (size_t i = 0; i < tagType.keys.size(); i++) {
				bool defaultValue = true;
				for (size_t j = 0; j < tag.keys.size(); j++) {
					if (tag.keys[j] == tagType.keys[i]) defaultValue = false;
				}
				if (defaultValue) {
					tag.keys.push_back(tagType.keys[i]);
					tag.values.push_back(tagType.defaultValues[i]);
				}
			}


			inTag = false;
			tags.push_back(tag);
			tag = Tag();
		}
		if (!inTag) continue;

		if (data[pos] == '=') {
			while (data[++pos] != '"');
			while (data[++pos] != '"') {
				value += data[pos];
			}

			tag.keys.push_back(key);
			tag.values.push_back(value);
			key = "";
			value = "";
		}

		if (isalnum(data[pos])) {
			key += data[pos];
		}
	}

	return tags;
}

std::string Tag::getValue(const std::string &key) {
	for (size_t i = 0; i < keys.size(); i++) {
		if (keys[i] == key) {
			return values[i];
		}
	}
	return "";
}

std::optional<TagType> getTagType(const std::string &name) {
	for (size_t i = 0; i < tagTypes.size(); i++) {
		if (tagTypes[i].name == name) {
			return std::make_optional(tagTypes[i]);
		}
	}
	return {};
}
