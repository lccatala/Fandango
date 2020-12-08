#pragma once

#include <string>

namespace Fandango 
{
	class FileDialogs
	{
	public:
		// Filter is the "file type" option in Windows file dialogs
		// Returns the absolute filepath or an empty string if the dialog was canceled
		static std::string OpenFile(const char* filter);

		// Filter is the "file type" option in Windows file dialogs
		// Returns the absolute filepath or an empty string if the dialog was canceled
		static std::string SaveFile(const char* filter);
	};
}