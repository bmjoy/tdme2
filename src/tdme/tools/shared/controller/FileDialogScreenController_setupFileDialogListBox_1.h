
#pragma once

#include <string>

#include <tdme/tools/shared/controller/fwd-tdme.h>
#include <tdme/utils/fwd-tdme.h>
#include <tdme/utils/FilenameFilter.h>

using std::wstring;

using tdme::utils::FilenameFilter;

using tdme::tools::shared::controller::FileDialogScreenController;

class tdme::tools::shared::controller::FileDialogScreenController_setupFileDialogListBox_1
	: public virtual FilenameFilter
{
	friend class FileDialogScreenController;

public:
	bool accept(const wstring& pathName, const wstring& fileName) override;

	// Generated
	FileDialogScreenController_setupFileDialogListBox_1(FileDialogScreenController* fileDialogScreenController);

private:
	FileDialogScreenController* fileDialogScreenController;
};
