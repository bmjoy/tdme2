#include <tdme/gui/elements/GUITab.h>

#include <map>
#include <string>

#include <tdme/gui/elements/GUITabController.h>
#include <tdme/gui/nodes/GUIScreenNode.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemException.h>
#include <tdme/os/filesystem/FileSystemInterface.h>

using std::map;
using std::wstring;

using tdme::gui::elements::GUITab;
using tdme::gui::elements::GUITabController;
using tdme::gui::nodes::GUIScreenNode;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemException;
using tdme::os::filesystem::FileSystemInterface;

GUITab::GUITab() throw (FileSystemException)
{
	template_ = FileSystem::getInstance()->getContentAsString(L"resources/gui/definitions/elements", L"tab.xml");
}

wstring GUITab::NAME = L"tab";

const wstring& GUITab::getName()
{
	return NAME;
}

const wstring& GUITab::getTemplate()
{
	return template_;
}

map<wstring, wstring>* GUITab::getAttributes(GUIScreenNode* screenNode)
{
	attributes.clear();
	attributes[L"id"] = screenNode->allocateNodeId();
	return &attributes;
}

GUINodeController* GUITab::createController(GUINode* node)
{
	return new GUITabController(node);
}

