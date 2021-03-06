#include <string>
#include <vector>

#include <tdme/application/Application.h>
#include <tdme/os/filesystem/FileNameFilter.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>
#include <tdme/utils/Console.h>
#include <tdme/utils/Exception.h>
#include <tdme/utils/StringTokenizer.h>
#include <tdme/utils/StringUtils.h>

using std::string;
using std::to_string;
using std::vector;

using tdme::application::Application;
using tdme::os::filesystem::FileNameFilter;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;
using tdme::utils::Console;
using tdme::utils::Exception;
using tdme::utils::StringTokenizer;
using tdme::utils::StringUtils;

void scanDir(const string& folder, vector<string>& totalFiles) {
	class ListFilter : public virtual FileNameFilter {
		public:
			virtual ~ListFilter() {}

			bool accept(const string& pathName, const string& fileName) override {
				if (fileName == ".") return false;
				if (fileName == "..") return false;
				if (FileSystem::getInstance()->isPath(pathName + "/" + fileName) == true) return true;
				if (fileName == "LICENSE") return true;
				return false;
			}
	};

	ListFilter listFilter;
	vector<string> files;

	FileSystem::getInstance()->list(folder, files, &listFilter);

	for (auto fileName: files) {
		if (fileName == "LICENSE") {
			totalFiles.push_back(folder + "/" + fileName);
		} else {
			scanDir(folder + "/" + fileName, totalFiles);
		}
	}
}

void processFile(const string& indent, const string& fileName) {
	auto _fileName = StringUtils::startsWith(fileName, "./") == true?StringUtils::substring(fileName, 2, fileName.size()):fileName;
	vector<string> lines;
	FileSystem::getInstance()->getContentAsStringArray(".", fileName, lines);
	Console::println(indent + _fileName);
	Console::print(indent);
	for (auto i = 0; i < _fileName.size() + 2; i++) Console::print("-");
	Console::println();
	Console::println();
	for (auto& line: lines) {
		if (StringUtils::trim(line).size() == 0) {
			Console::println();
		} else {
			Console::println(indent + "\t" + line);
		}
	}
	Console::println();
	Console::println();
}

int main(int argc, char** argv)
{
	Console::println(string("generate licenses 1.9.9"));
	Console::println(string("Programmed 2018 by Andreas Drewke, drewke.net."));
	Console::println();

	auto pathToHeaders = "."; // we do search in pwd
	auto indent = argc > 1?string(argv[1]):string();

	if (argc > 2 || (argc == 2 && indent.empty() == false && indent != "indent")) {
		Console::println("Usage: generatelicenses [indent]");
		Application::exit(1);
	}

	if (indent == "indent") indent = "\t";

	Console::println("Scanning files");
	vector<string> files;
	scanDir(pathToHeaders, files);

	Console::println("Processing files");
	Console::println("------------------");
	Console::println();
	for (auto fileName: files) {
		if (fileName == "./LICENSE") continue; // ignore own project license
		processFile(indent, fileName);
	}
}
