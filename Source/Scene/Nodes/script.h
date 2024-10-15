#pragma once
#ifndef SCRIPT_H
#define SCRIPT_H

#include "Scene/Nodes/instance.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

class Script : public Instance {
public:
	Script(const std::string& name = "Script") : Instance(name) {
		filename = "Game/" + name + ".lua";
		enabled = true;
		createFile();
		std::cout << "script created" << std::endl;
	}

	~Script() {
		std::cout << "script destroyed" << std::endl;
	}

	std::string getTypeName() const override {
		return "Script";
	}

	void createFile() {
		std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
		std::ofstream file(filename);
		if (file.is_open()) {
			file << "-- " << getName() << " script\n\n";
			file.close();
		}
		else {
			// Handle error (couldn't create file)
			std::cerr << "Failed to create script file: " << filename << std::endl;
		}
	}

	void loadContent() {
		std::ifstream file(filename);
		if (file.is_open()) {
			content.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
		}
		else {
			// Handle error (couldn't open file)
			std::cerr << "Failed to open script file: " << filename << std::endl;
		}
	}

	void saveContent() {
		std::ofstream file(filename);
		if (file.is_open()) {
			file << content;
			file.close();
		}
		else {
			// Handle error (couldn't open file for writing)
			std::cerr << "Failed to save script file: " << filename << std::endl;
		}
	}

	std::string content;
	std::string filename;
	bool enabled;
};

#endif 
