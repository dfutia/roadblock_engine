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
	std::string content;
	std::string filename;
	bool enabled;

	Script(const std::string& scriptName = "Script") : Instance() {
		name = scriptName;
		filename = "Game/" + scriptName + ".lua";
		enabled = true;
		createFile();
	}

	void createFile() {
		std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
		std::ofstream file(filename);
		if (file.is_open()) {
			file << "-- " << name << " script\n\n";
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
};

#endif 
