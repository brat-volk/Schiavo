#include "functions.h"

namespace interpreter {

	std::wstring InterpretTask(std::wstring task) {
		MessageBoxW(NULL,task.c_str(),L"",MB_OK);
		return task;
	}
}