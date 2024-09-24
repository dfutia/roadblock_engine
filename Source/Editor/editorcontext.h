#pragma once
#ifndef EDITOR_CONTEXT_H

#include <string>

class Instance;

enum class EditorAction {
	ADD_INSTANCE,
	NONE
};

struct EditorContext {
	Instance* selected = nullptr;
	Instance* targetInstance = nullptr;
	EditorAction action = EditorAction::NONE;
	std::string instanceTypeToAdd;
};

#endif 
