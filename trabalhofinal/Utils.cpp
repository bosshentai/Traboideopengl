#include "Utils.h"

std::string Utils::ReadFile(const char* path) {
	std::string content;
	std::ifstream fileStream(path, std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "ERR [Utils::ReadFile]: Failed to read file" << path << std::endl;
		std::cerr << " - " << strerror(errno) << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");

	}
	fileStream.close();

	std::cout << content << std::endl;
	return content;

}
