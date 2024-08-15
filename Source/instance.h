#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>

class Instance {
public:
	std::string name;
	Instance* parent;
	std::vector<Instance*> children;

	virtual ~Instance();

	virtual void setParent(Instance* parent);

	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
};

class Model : public Instance {
public:
	Model() { name = "Model"; }
};

class Part : public Instance {
public:
	Part() { name = "Part"; }
};

class Script : public Instance {
public:
	std::string content;
	std::string filename;

	Script(const std::string& scriptName) : Instance() { 
		name = scriptName;
		filename = "Game/" + scriptName + ".lua";
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
