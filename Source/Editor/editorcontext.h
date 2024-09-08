#pragma once
#ifndef EDITOR_CONTEXT_H

class Instance;

enum class EditorAction {
	ADD_SCRIPT,
	ADD_PART,
	ADD_MODEL,
	NONE
};

struct EditorContext {
	Instance* selected = nullptr;
	Instance* targetInstance = nullptr;
	EditorAction action = EditorAction::NONE;
};

#endif 
