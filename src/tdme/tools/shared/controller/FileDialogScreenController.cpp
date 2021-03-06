#include <tdme/tools/shared/controller/FileDialogScreenController.h>

#include <string>
#include <vector>


#include <tdme/gui/GUIParser.h>
#include <tdme/gui/events/Action.h>
#include <tdme/gui/events/GUIActionListener_Type.h>
#include <tdme/gui/events/GUIChangeListener.h>
#include <tdme/gui/nodes/GUIElementNode.h>
#include <tdme/gui/nodes/GUINode.h>
#include <tdme/gui/nodes/GUINodeController.h>
#include <tdme/gui/nodes/GUIParentNode.h>
#include <tdme/gui/nodes/GUIScreenNode.h>
#include <tdme/gui/nodes/GUITextNode.h>
#include <tdme/os/filesystem/FileNameFilter.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>
#include <tdme/utils/Console.h>
#include <tdme/utils/Exception.h>
#include <tdme/utils/MutableString.h>
#include <tdme/utils/StringUtils.h>

using std::vector;
using std::string;

using tdme::tools::shared::controller::FileDialogScreenController;
using tdme::gui::GUIParser;
using tdme::gui::events::Action;
using tdme::gui::events::GUIActionListener_Type;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUINode;
using tdme::gui::nodes::GUINodeController;
using tdme::gui::nodes::GUIParentNode;
using tdme::gui::nodes::GUIScreenNode;
using tdme::gui::nodes::GUITextNode;
using tdme::os::filesystem::FileNameFilter;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;
using tdme::utils::Console;
using tdme::utils::Exception;
using tdme::utils::MutableString;
using tdme::utils::StringUtils;

FileDialogScreenController::FileDialogScreenController() 
{
	this->cwd = FileSystem::getInstance()->getCurrentWorkingPathName();
	this->applyAction = nullptr;
	this->enableFilter = false;
	this->filtered = false;
}

FileDialogScreenController::~FileDialogScreenController() {
	if (applyAction != nullptr) delete applyAction;
}

GUIScreenNode* FileDialogScreenController::getScreenNode()
{
	return screenNode;
}

const string& FileDialogScreenController::getPathName()
{
	return cwd;
}

const string FileDialogScreenController::getFileName()
{
	return fileName->getController()->getValue().getString();
}

void FileDialogScreenController::initialize()
{
	try {
		screenNode = GUIParser::parse("resources/engine/tools/shared/gui", "filedialog.xml");
		screenNode->setVisible(false);
		screenNode->addActionListener(this);
		screenNode->addChangeListener(this);
		screenNode->addFocusListener(this);
		caption = dynamic_cast< GUITextNode* >(screenNode->getNodeById("filedialog_caption"));
		files = dynamic_cast< GUIElementNode* >(screenNode->getNodeById("filedialog_files"));
		fileName = dynamic_cast< GUIElementNode* >(screenNode->getNodeById("filedialog_filename"));
	} catch (Exception& exception) {
		Console::print(string("FileDialogScreenController::initialize(): An error occurred: "));
		Console::println(string(exception.what()));
	}
}

void FileDialogScreenController::dispose()
{
	screenNode = nullptr;
}

bool FileDialogScreenController::setupFileDialogListBox()
{
	class ExtensionFilter: public virtual FileNameFilter
	{
	public:
		bool accept(const string& pathName, const string& fileName) override {
			if (FileSystem::getInstance()->isPath(pathName + "/" + fileName) == true) return true;
			for (auto& extension : fileDialogScreenController->extensions) {
				if (StringUtils::endsWith(StringUtils::toLowerCase(fileName), "." + extension) == true) return true;

			}
			return false;
		}

		/**
		 * Public constructor
		 * @param fileDialogScreenController file dialog screen controller
		 */
		ExtensionFilter(FileDialogScreenController* fileDialogScreenController): fileDialogScreenController(fileDialogScreenController) {
		}

	private:
		FileDialogScreenController* fileDialogScreenController;
	};

	auto success = true;
	auto directory = cwd;
	if (directory.length() > 50) {
		directory = "..." + StringUtils::substring(directory, directory.length() - 50 + 3);
	}

	caption->setText(MutableString(captionText).append(directory));

	fileList.clear();
	try {
		auto directory = cwd;
		ExtensionFilter extensionsFilter(this);
		FileSystem::getInstance()->list(directory, fileList, &extensionsFilter, FileSystem::getInstance()->isDrive(directory));
	} catch (Exception& exception) {
		Console::print(string("FileDialogScreenController::setupFileDialogListBox(): An error occurred: "));
		Console::println(string(exception.what()));
		success = false;
	}

	setupFileDialogListBoxFiles(fileList);
	if (enableFilter == true) fileName->getController()->setValue(MutableString("Type a string to filter the list..."));

	//
	filtered = false;

	//
	return success;
}

void FileDialogScreenController::setupFileDialogListBoxFiles(const vector<string>& fileList, const string& selectedFile) {
	auto filesInnerNode = dynamic_cast< GUIParentNode* >(files->getScreenNode()->getNodeById(files->getId() + "_inner"));
	auto idx = 1;
	string filesInnerNodeSubNodesXML = "";
	filesInnerNodeSubNodesXML =
		filesInnerNodeSubNodesXML +
		"<scrollarea width=\"100%\" height=\"100%\">\n";
	for (auto& file : fileList) {
		filesInnerNodeSubNodesXML =
			filesInnerNodeSubNodesXML +
			"<selectbox-option text=\"" +
			GUIParser::escapeQuotes(file) +
			"\" value=\"" +
			GUIParser::escapeQuotes(file) +
			"\"" +
			(selectedFile == file?" selected=\"true\"":"") +
			"/>\n";
	}
	filesInnerNodeSubNodesXML =
		filesInnerNodeSubNodesXML + "</scrollarea>\n";
	try {
		filesInnerNode->replaceSubNodes(filesInnerNodeSubNodesXML, false);
	} catch (Exception& exception) {
		Console::print(string("FileDialogScreenController::setupFileDialogListBox(): An error occurred: "));
		Console::println(string(exception.what()));
	}
}

void FileDialogScreenController::show(const string& cwd, const string& captionText, const vector<string>& extensions, const string& fileName, bool enableFilter, Action* applyAction)
{
	try {
		this->cwd = FileSystem::getInstance()->getCanonicalPath(cwd, "");
		if (FileSystem::getInstance()->isPath(this->cwd) == false) {
			this->cwd = FileSystem::getInstance()->getCurrentWorkingPathName();
		}
	} catch (Exception& exception) {
		Console::print(string("FileDialogScreenController::show(): An error occurred: "));
		Console::print(string(exception.what()));
		Console::println(": using cwd!");
		this->cwd = FileSystem::getInstance()->getCurrentWorkingPathName();
	}
	this->captionText = captionText;
	this->extensions = extensions;
	this->fileName->getController()->setValue(fileName);
	this->enableFilter = enableFilter;
	setupFileDialogListBox();
	screenNode->setVisible(true);
	if (this->applyAction != nullptr) delete this->applyAction;
	this->applyAction = applyAction;
}

void FileDialogScreenController::close()
{
	screenNode->setVisible(false);
}

void FileDialogScreenController::onValueChanged(GUIElementNode* node)
{
	try {
		if (node->getId().compare(files->getId()) == 0) {
			auto selectedFile = node->getController()->getValue().getString();
			if (FileSystem::getInstance()->isDrive(selectedFile) == true) {
				auto lastCwd = cwd;
				cwd = selectedFile;
				if (setupFileDialogListBox() == false) {
					cwd = lastCwd;
					setupFileDialogListBox();
				}
			} else
			if (FileSystem::getInstance()->isPath(cwd + "/" + selectedFile) == true) {
				auto lastCwd = cwd;
				try {
					cwd = FileSystem::getInstance()->getCanonicalPath(cwd, selectedFile);
				} catch (Exception& exception) {
					Console::print(string("FileDialogScreenController::onValueChanged(): An error occurred: "));
					Console::println(string(exception.what()));
				}
				if (setupFileDialogListBox() == false) {
					cwd = lastCwd;
					setupFileDialogListBox();
				}
			} else {
				if (filtered == true) {
					setupFileDialogListBoxFiles(fileList, selectedFile);
					filtered = false;
				}
				fileName->getController()->setValue(selectedFile);
			}
		} else
		if (node->getId() == "filedialog_filename") {
			if (enableFilter == true) {
				auto filterString = StringUtils::toLowerCase(node->getController()->getValue().getString());
				if (FileSystem::getInstance()->fileExists(cwd + "/" + filterString) == true) {
					auto selectedFile = node->getController()->getValue().getString();
					setupFileDialogListBoxFiles(fileList, selectedFile);
				} else {
					vector<string> fileListFiltered;
					for (auto file: fileList) {
						if (StringUtils::toLowerCase(file).find(filterString) != -1) fileListFiltered.push_back(file);
					}
					setupFileDialogListBoxFiles(fileListFiltered);
					filtered = true;
				}
			}
		}
	} catch (Exception& exception) {
		Console::print(string("FileDialogScreenController::onValueChanged(): An error occurred: "));
		Console::println(string(exception.what()));
		fileName->getController()->setValue(MutableString());
	}
}

void FileDialogScreenController::onActionPerformed(GUIActionListener_Type* type, GUIElementNode* node)
{
	{
		auto v = type;
		if (v == GUIActionListener_Type::PERFORMED)
		{
			if (node->getId().compare("filedialog_apply") == 0) {
				if (applyAction != nullptr) applyAction->performAction();
			} else
			if (node->getId().compare("filedialog_abort") == 0) {
				close();
			}
		}
	}
}

void FileDialogScreenController::onFocus(GUIElementNode* node) {
	if (node->getId() == "filedialog_filename") {
		if (enableFilter == true) node->getController()->setValue(MutableString(""));
	}
}

void FileDialogScreenController::onUnfocus(GUIElementNode* node) {
}

