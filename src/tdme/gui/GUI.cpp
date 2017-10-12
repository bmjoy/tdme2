#include <tdme/gui/GUI.h>

#ifdef __APPLE__
	#include <Carbon/Carbon.h>
#endif

#include <map>
#include <string>
#include <vector>

#include <tdme/engine/Engine.h>
#include <tdme/engine/fileio/textures/Texture.h>
#include <tdme/engine/fileio/textures/TextureLoader.h>
#include <tdme/gui/GUI_1.h>
#include <tdme/gui/GUI_2.h>
#include <tdme/gui/GUIParserException.h>
#include <tdme/gui/events/GUIInputEventHandler.h>
#include <tdme/gui/events/GUIKeyboardEvent_Type.h>
#include <tdme/gui/events/GUIKeyboardEvent.h>
#include <tdme/gui/events/GUIMouseEvent_Type.h>
#include <tdme/gui/events/GUIMouseEvent.h>
#include <tdme/gui/nodes/GUIColor.h>
#include <tdme/gui/nodes/GUIElementNode.h>
#include <tdme/gui/nodes/GUINode_Border.h>
#include <tdme/gui/nodes/GUINode.h>
#include <tdme/gui/nodes/GUINodeConditions.h>
#include <tdme/gui/nodes/GUINodeController.h>
#include <tdme/gui/nodes/GUIScreenNode.h>
#include <tdme/gui/renderer/GUIFont.h>
#include <tdme/gui/renderer/GUIRenderer.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>
#include <tdme/utils/Pool.h>
#include <tdme/utils/StringConverter.h>
#include <tdme/utils/Time.h>
#include <tdme/utils/Console.h>
#include <tdme/utils/Exception.h>

using std::map;
using std::vector;
using std::wstring;
using std::string;
using std::to_string;
using std::to_wstring;

using tdme::gui::GUI;
using tdme::engine::Engine;
using tdme::engine::fileio::textures::Texture;
using tdme::engine::fileio::textures::TextureLoader;
using tdme::gui::GUI_1;
using tdme::gui::GUI_2;
using tdme::gui::GUIParserException;
using tdme::gui::events::GUIInputEventHandler;
using tdme::gui::events::GUIKeyboardEvent_Type;
using tdme::gui::events::GUIKeyboardEvent;
using tdme::gui::events::GUIMouseEvent_Type;
using tdme::gui::events::GUIMouseEvent;
using tdme::gui::nodes::GUIColor;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUINode_Border;
using tdme::gui::nodes::GUINode;
using tdme::gui::nodes::GUINodeConditions;
using tdme::gui::nodes::GUINodeController;
using tdme::gui::nodes::GUIScreenNode;
using tdme::gui::renderer::GUIFont;
using tdme::gui::renderer::GUIRenderer;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;
using tdme::utils::Pool;
using tdme::utils::StringConverter;
using tdme::utils::Time;
using tdme::utils::Console;
using tdme::utils::Exception;

map<wstring, GUIFont*> GUI::fontCache;
map<wstring, Texture*> GUI::imageCache;

GUI::GUI(Engine* engine, GUIRenderer* guiRenderer)
{
	init();
	this->engine = engine;
	this->guiRenderer = guiRenderer;
	this->width = 0;
	this->height = 0;
	try {
		this->foccussedBorderColor = new GUIColor(L"#8080FF");
	} catch (Exception& exception) {
		Console::print(string("GUI(): An error occurred: "));
		Console::println(string(exception.what()));
	}
}

void GUI::init()
{
	foccussedBorderColor = nullptr;
	unfocussedNodeBorderLeftColor = nullptr;
	unfocussedNodeBorderRightColor = nullptr;
	unfocussedNodeBorderTopColor = nullptr;
	unfocussedNodeBorderBottomColor = nullptr;
	mouseEventsPool = new GUI_1(this);
	keyboardEventsPool = new GUI_2(this);
	mouseButtonLast = 0;
}

int32_t GUI::getWidth()
{
	return width;
}

int32_t GUI::getHeight()
{
	return height;
}

void GUI::initialize()
{
}

void GUI::reshape(int32_t width, int32_t height)
{
	this->width = width;
	this->height = height;
}

void GUI::dispose()
{
	reset();
}

void GUI::lockEvents()
{
}

void GUI::unlockEvents()
{
}

vector<GUIMouseEvent*>* GUI::getMouseEvents()
{
	return &mouseEvents;
}

vector<GUIKeyboardEvent*>* GUI::getKeyboardEvents()
{
	return &keyboardEvents;
}

GUIFont* GUI::getFont(const wstring& fileName)
{
	wstring canonicalFile;
	wstring path;
	wstring file;
	GUIFont* font;
	try {
		canonicalFile = FileSystem::getInstance()->getCanonicalPath(L".", fileName);
		path = FileSystem::getInstance()->getPathName(canonicalFile);
		file = FileSystem::getInstance()->getFileName(canonicalFile);
	} catch (Exception& exception) {
		Console::print(string("GUI::getFont(): An error occurred: "));
		Console::println(string(exception.what()));
		return nullptr;
	}

	auto fontCacheIt = fontCache.find(canonicalFile);
	if (fontCacheIt == fontCache.end()) {
		try {
			font = GUIFont::parse(path, file);
		} catch (Exception& exception) {
			Console::print(string("GUI::getFont(): An error occurred: "));
			Console::println(string(exception.what()));
			return nullptr;
		}

		fontCache[canonicalFile] = font;
	}
	else {
		font = fontCacheIt->second;
	}
	return font;
}

Texture* GUI::getImage(const wstring& fileName)
{
	// TODO: fix me, proper get path, filename
	wstring canonicalFile;
	wstring path;
	wstring file;
	try {
		canonicalFile = FileSystem::getInstance()->getCanonicalPath(L".", fileName);
		path = FileSystem::getInstance()->getPathName(canonicalFile);
		file = FileSystem::getInstance()->getFileName(canonicalFile);
	} catch (Exception& exception) {
		Console::print(string("GUI::getImage(): An error occurred: "));
		Console::println(string(exception.what()));
		return nullptr;
	}

	auto imageIt = imageCache.find(canonicalFile);
	auto image = imageIt != imageCache.end() ? imageIt->second : nullptr;
	if (image == nullptr) {
		try {
			image = TextureLoader::loadTexture(path, file);
		} catch (Exception& exception) {
			Console::print(string("GUI::getImage(): An error occurred: "));
			Console::println(string(exception.what()));
			return nullptr;
		}
		imageCache[canonicalFile] = image;
	}
	return image;
}

GUIScreenNode* GUI::getScreen(const wstring& id)
{
	auto screensIt = screens.find(id);
	if (screensIt == screens.end()) {
		return nullptr;
	}
	return screensIt->second;
}

void GUI::addScreen(const wstring& id, GUIScreenNode* screen)
{
	screens.emplace(id, screen);
}

void GUI::removeScreen(const wstring* id)
{
	auto screensIt = screens.find(*id);

	if (screensIt != screens.end()) {
		screensIt->second->dispose();
		screens.erase(*id);
	}
}

void GUI::reset()
{
	vector<wstring> entitiesToRemove;
	for (auto screenKeysIt: screens) {
		entitiesToRemove.push_back(screenKeysIt.first);
	}
	for (auto i = 0; i < entitiesToRemove.size(); i++) {
		removeScreen(&(entitiesToRemove.at(i)));
	}
	fontCache.clear();
	imageCache.clear();
}

void GUI::resetRenderScreens()
{
	for (auto i = 0; i < renderScreens.size(); i++) {
		renderScreens.at(i)->setGUI(nullptr);
	}
	renderScreens.clear();
}

void GUI::addRenderScreen(const wstring& screenId)
{
	auto screenIt = screens.find(screenId);
	if (screenIt == screens.end())
		return;

	screenIt->second->setGUI(this);
	renderScreens.push_back(screenIt->second);
}

GUIColor* GUI::getFoccussedBorderColor()
{
	return foccussedBorderColor;
}

void GUI::invalidateFocussedNode()
{
	unfocusNode();
	focussedNode = nullptr;
}

void GUI::determineFocussedNodes()
{
	focusableNodes.clear();
	focusableScreenNodes.clear();
	for (int32_t i = renderScreens.size() - 1; i >= 0; i--) {
		auto screen = renderScreens.at(i);
		if (screen->isVisible() == false)
			continue;

		focusableScreenNodes.push_back(screen);
		if (screen->isPopUp() == true)
			break;

	}
	for (int32_t i = focusableScreenNodes.size() - 1; i >= 0; i--) {
		auto screen = focusableScreenNodes.at(i);
		screen->determineFocussedNodes(screen, &focusableNodes);
	}
}

GUIElementNode* GUI::getFocussedNode()
{
	return this->focussedNode;
}

void GUI::unfocusNode()
{
	if (focussedNode != nullptr) {
		focussedNode->getActiveConditions()->remove(GUIElementNode::CONDITION_FOCUS);
		focussedNode->getBorder()->topColor = unfocussedNodeBorderTopColor;
		focussedNode->getBorder()->leftColor = unfocussedNodeBorderLeftColor;
		focussedNode->getBorder()->bottomColor = unfocussedNodeBorderBottomColor;
		focussedNode->getBorder()->rightColor = unfocussedNodeBorderRightColor;
		if (focussedNode->getController() != nullptr)
			focussedNode->getController()->onFocusLost();

	}
}

void GUI::focusNode()
{
	if (focussedNode != nullptr) {
		focussedNode->getActiveConditions()->add(GUIElementNode::CONDITION_FOCUS);
		unfocussedNodeBorderTopColor = focussedNode->getBorder()->topColor;
		unfocussedNodeBorderLeftColor = focussedNode->getBorder()->leftColor;
		unfocussedNodeBorderBottomColor = focussedNode->getBorder()->bottomColor;
		unfocussedNodeBorderRightColor = focussedNode->getBorder()->rightColor;
		focussedNode->getBorder()->topColor = foccussedBorderColor;
		focussedNode->getBorder()->leftColor = foccussedBorderColor;
		focussedNode->getBorder()->bottomColor = foccussedBorderColor;
		focussedNode->getBorder()->rightColor = foccussedBorderColor;
		if (focussedNode->getController() != nullptr)
			focussedNode->getController()->onFocusGained();

	}
}

void GUI::setFoccussedNode(GUIElementNode* newFoccussedNode)
{
	if (this->focussedNode == newFoccussedNode) {
		return;
	}
	unfocusNode();
	this->focussedNode = newFoccussedNode;
	focusNode();
	determineFocussedNodes();
}

void GUI::focusNextNode()
{
	determineFocussedNodes();
	unfocusNode();
	if (focusableNodes.size() > 0) {
		auto focussedNodeIdx = -1;
		for (auto i = 0; i < focusableNodes.size(); i++) {
			if (focussedNode == focusableNodes.at(i)) {
				focussedNodeIdx = i;
			}
		}
		auto focussedNextNodeIdx = (focussedNodeIdx + 1) % focusableNodes.size();
		focussedNode = focusableNodes.at(focussedNextNodeIdx);
		focusNode();
		focussedNode->scrollToNodeX();
		focussedNode->scrollToNodeY();
	}
}

void GUI::focusPreviousNode()
{
	determineFocussedNodes();
	unfocusNode();
	if (focusableNodes.size() > 0) {
		auto focussedNodeIdx = -1;
		for (auto i = 0; i < focusableNodes.size(); i++) {
			if (focussedNode == focusableNodes.at(i)) {
				focussedNodeIdx = i;
			}
		}
		auto focussedPreviousNodeIdx = (focussedNodeIdx - 1) % focusableNodes.size();
		if (focussedPreviousNodeIdx < 0)
			focussedPreviousNodeIdx += focusableNodes.size();

		focussedNode = focusableNodes.at(focussedPreviousNodeIdx);
		focusNode();
		focussedNode->scrollToNodeX();
		focussedNode->scrollToNodeY();
	}
}

void GUI::render()
{
	if (renderScreens.empty() == true)
		return;

	if (focussedNode == nullptr) {
		focusNextNode();
	}
	guiRenderer->setGUI(this);
	engine->initGUIMode();
	guiRenderer->initRendering();
	for (auto i = 0; i < renderScreens.size(); i++) {
		auto screen = renderScreens.at(i);
		if (screen->isVisible() == false)
			continue;

		if (screen->getScreenWidth() != width || screen->getScreenHeight() != height) {
			screen->setScreenSize(width, height);
			screen->layout();
		}
		screen->setConditionsMet();
		screen->tick();
		screen->render(guiRenderer);
	}
	for (auto i = 0; i < renderScreens.size(); i++) {
		auto screen = renderScreens.at(i);
		if (screen->isVisible() == false)
			continue;

		screen->renderFloatingNodes(guiRenderer);
	}
	guiRenderer->doneRendering();
	engine->doneGUIMode();
}

void GUI::handleEvents(GUINode* node)
{
	for (auto i = 0; i < mouseEvents.size(); i++) {
		auto event = mouseEvents.at(i);
		if (event->isProcessed() == true)
			continue;

		event->setX(event->getX() + node->getScreenNode()->getGUIEffectOffsetX());
		event->setY(event->getY() + node->getScreenNode()->getGUIEffectOffsetY());
		node->handleMouseEvent(event);
	}
	for (auto i = 0; i < keyboardEvents.size(); i++) {
		auto event = keyboardEvents.at(i);
		if (event->isProcessed() == true)
			continue;

		switch (event->getKeyCode()) {
			case (GUIKeyboardEvent::KEYCODE_TAB):
				{
					if (event->getType() == GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_PRESSED) {
						focusNextNode();
					}
					event->setProcessed(true);
					break;
				}
			case (GUIKeyboardEvent::KEYCODE_TAB_SHIFT):
				{
					if (event->getType() == GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_PRESSED) {
						focusPreviousNode();
					}
					event->setProcessed(true);
					break;
				}
			default:
				{
					break;
				}
		}

		if (event->isProcessed() == true) {
			continue;
		}
		if (focussedNode != nullptr) {
			focussedNode->handleKeyboardEvent(event);
		}
	}
}

void GUI::handleEvents()
{
	lockEvents();
	for (int32_t i = renderScreens.size() - 1; i >= 0; i--) {
		auto screen = renderScreens.at(i);
		if (screen->isVisible() == false)
			continue;

		vector<GUINode*>* floatingNodes = screen->getFloatingNodes();
		for (auto j = 0; j < floatingNodes->size(); j++) {
			auto floatingNode = floatingNodes->at(j);
			handleEvents(floatingNode);
		}
		if (screen->isPopUp() == true)
			break;

	}
	for (int32_t i = renderScreens.size() - 1; i >= 0; i--) {
		auto screen = renderScreens.at(i);
		if (screen->isVisible() == false)
			continue;

		handleEvents(screen);
		if (screen->getInputEventHandler() != nullptr) {
			screen->getInputEventHandler()->handleInputEvents();
		}
		if (screen->isPopUp() == true)
			break;

	}
	mouseEvents.clear();
	mouseEventsPool->reset();
	keyboardEvents.clear();
	keyboardEventsPool->reset();
	unlockEvents();
}

void GUI::onKeyDown (unsigned char key, int x, int y) {
	fakeMouseMovedEvent();
	lockEvents();
	auto guiKeyboardEvent = keyboardEventsPool->allocate();
	guiKeyboardEvent->setTime(Time::getCurrentMillis());
	guiKeyboardEvent->setType(GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_PRESSED);
	guiKeyboardEvent->setKeyCode(GUIKeyboardEvent::getKeyCodeFromChar(key));
	guiKeyboardEvent->setKeyChar(key);
	guiKeyboardEvent->setMetaDown(false);
	guiKeyboardEvent->setControlDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_CTRL) == KEYBOARD_MODIFIER_CTRL);
	guiKeyboardEvent->setAltDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_ALT) == KEYBOARD_MODIFIER_ALT);
	guiKeyboardEvent->setShiftDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_SHIFT) == KEYBOARD_MODIFIER_SHIFT);
	guiKeyboardEvent->setProcessed(false);
	keyboardEvents.push_back(guiKeyboardEvent);
	unlockEvents();
}

void GUI::onKeyUp(unsigned char key, int x, int y) {
	fakeMouseMovedEvent();
	lockEvents();
	auto guiKeyboardEvent = keyboardEventsPool->allocate();
	guiKeyboardEvent->setTime(Time::getCurrentMillis());
	guiKeyboardEvent->setType(GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_RELEASED);
	guiKeyboardEvent->setKeyCode(GUIKeyboardEvent::getKeyCodeFromChar(key));
	guiKeyboardEvent->setKeyChar(key);
	guiKeyboardEvent->setMetaDown(false);
	guiKeyboardEvent->setControlDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_CTRL) == KEYBOARD_MODIFIER_CTRL);
	guiKeyboardEvent->setAltDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_ALT) == KEYBOARD_MODIFIER_ALT);
	guiKeyboardEvent->setShiftDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_SHIFT) == KEYBOARD_MODIFIER_SHIFT);
	guiKeyboardEvent->setProcessed(false);
	keyboardEvents.push_back(guiKeyboardEvent);
	unlockEvents();
}

void GUI::onSpecialKeyDown (int key, int x, int y) {
	fakeMouseMovedEvent();
	lockEvents();
	auto guiKeyboardEvent = keyboardEventsPool->allocate();
	guiKeyboardEvent->setTime(Time::getCurrentMillis());
	guiKeyboardEvent->setType(GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_PRESSED);
	guiKeyboardEvent->setKeyCode(key);
	guiKeyboardEvent->setKeyChar(-1);
	guiKeyboardEvent->setMetaDown(false);
	guiKeyboardEvent->setControlDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_CTRL) == KEYBOARD_MODIFIER_CTRL);
	guiKeyboardEvent->setAltDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_ALT) == KEYBOARD_MODIFIER_ALT);
	guiKeyboardEvent->setShiftDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_SHIFT) == KEYBOARD_MODIFIER_SHIFT);
	guiKeyboardEvent->setProcessed(false);
	keyboardEvents.push_back(guiKeyboardEvent);
	unlockEvents();
}

void GUI::onSpecialKeyUp(int key, int x, int y) {
	fakeMouseMovedEvent();
	lockEvents();
	auto guiKeyboardEvent = keyboardEventsPool->allocate();
	guiKeyboardEvent->setTime(Time::getCurrentMillis());
	guiKeyboardEvent->setType(GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_RELEASED);
	guiKeyboardEvent->setKeyCode(key);
	guiKeyboardEvent->setKeyChar(-1);
	guiKeyboardEvent->setMetaDown(false);
	guiKeyboardEvent->setControlDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_CTRL) == KEYBOARD_MODIFIER_CTRL);
	guiKeyboardEvent->setAltDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_ALT) == KEYBOARD_MODIFIER_ALT);
	guiKeyboardEvent->setShiftDown((ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_SHIFT) == KEYBOARD_MODIFIER_SHIFT);
	guiKeyboardEvent->setProcessed(false);
	keyboardEvents.push_back(guiKeyboardEvent);
	unlockEvents();
}

void GUI::onMouseDragged(int x, int y) {
	fakeKeyboardModifierEvent();

	lockEvents();
	auto guiMouseEvent = mouseEventsPool->allocate();
	guiMouseEvent->setTime(Time::getCurrentMillis());
	guiMouseEvent->setType(GUIMouseEvent_Type::MOUSEEVENT_DRAGGED);
	guiMouseEvent->setX(x);
	guiMouseEvent->setY(y);
	guiMouseEvent->setButton(mouseButtonLast);
	/*
	guiMouseEvent->setWheelX((*event->getRotation())[0] * event->getRotationScale());
	guiMouseEvent->setWheelY((*event->getRotation())[1] * event->getRotationScale());
	guiMouseEvent->setWheelZ((*event->getRotation())[2] * event->getRotationScale());
	*/
	guiMouseEvent->setProcessed(false);
	mouseEvents.push_back(guiMouseEvent);
	unlockEvents();
}

void GUI::onMouseMoved(int x, int y) {
	fakeKeyboardModifierEvent();

	lockEvents();
	auto guiMouseEvent = mouseEventsPool->allocate();
	guiMouseEvent->setTime(Time::getCurrentMillis());
	guiMouseEvent->setType(GUIMouseEvent_Type::MOUSEEVENT_MOVED);
	guiMouseEvent->setX(x);
	guiMouseEvent->setY(y);
	guiMouseEvent->setButton(0);
	/*
	guiMouseEvent->setWheelX((*event->getRotation())[0] * event->getRotationScale());
	guiMouseEvent->setWheelY((*event->getRotation())[1] * event->getRotationScale());
	guiMouseEvent->setWheelZ((*event->getRotation())[2] * event->getRotationScale());
	*/
	guiMouseEvent->setProcessed(false);
	mouseEvents.push_back(guiMouseEvent);
	unlockEvents();
}

void GUI::onMouseButton(int button, int state, int x, int y) {
	fakeKeyboardModifierEvent();

	lockEvents();
	mouseButtonLast = button + 1;
	auto guiMouseEvent = mouseEventsPool->allocate();
	guiMouseEvent->setTime(Time::getCurrentMillis());
	guiMouseEvent->setType(state == MOUSE_BUTTON_DOWN?GUIMouseEvent_Type::MOUSEEVENT_PRESSED:GUIMouseEvent_Type::MOUSEEVENT_RELEASED);
	guiMouseEvent->setX(x);
	guiMouseEvent->setY(y);
	guiMouseEvent->setButton(mouseButtonLast);
	/*
	guiMouseEvent->setWheelX((*event->getRotation())[0] * event->getRotationScale());
	guiMouseEvent->setWheelY((*event->getRotation())[1] * event->getRotationScale());
	guiMouseEvent->setWheelZ((*event->getRotation())[2] * event->getRotationScale());
	*/
	guiMouseEvent->setProcessed(false);
	mouseEvents.push_back(guiMouseEvent);
	unlockEvents();
}

/*
void GUI::mouseWheelMoved(MouseEvent* event)
{
	lockEvents();
	auto guiMouseEvent = java_cast< GUIMouseEvent* >(mouseEventsPool->allocate());
	guiMouseEvent->setTime(Time::getCurrentMillis());
	guiMouseEvent->setType(GUIMouseEvent_Type::MOUSEEVENT_WHEEL_MOVED);
	guiMouseEvent->setX(event->getX());
	guiMouseEvent->setY(event->getY());
	guiMouseEvent->setButton(0);
	guiMouseEvent->setWheelX((*event->getRotation())[0] * event->getRotationScale());
	guiMouseEvent->setWheelY((*event->getRotation())[1] * event->getRotationScale());
	guiMouseEvent->setWheelZ((*event->getRotation())[2] * event->getRotationScale());
	guiMouseEvent->setProcessed(false);
	mouseEvents->add(guiMouseEvent);
	unlockEvents();
}
*/

void GUI::fakeMouseMovedEvent()
{
	lockEvents();
	auto guiMouseEvent = mouseEventsPool->allocate();
	guiMouseEvent->setTime(Time::getCurrentMillis());
	guiMouseEvent->setType(GUIMouseEvent_Type::MOUSEEVENT_MOVED);
	guiMouseEvent->setX(-10000);
	guiMouseEvent->setY(-10000);
	guiMouseEvent->setButton(0);
	guiMouseEvent->setWheelX(0.0f);
	guiMouseEvent->setWheelY(0.0f);
	guiMouseEvent->setWheelZ(0.0f);
	guiMouseEvent->setProcessed(false);
	mouseEvents.push_back(guiMouseEvent);
	unlockEvents();
}

void GUI::fakeKeyboardModifierEvent() {
	bool isControlDown = false;
	bool isAltDown = false;
	bool isShiftDown = false;
	#ifdef __linux__
		// TODO: check if this works with FreeGlut
		isControlDown = (ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_CTRL) == KEYBOARD_MODIFIER_CTRL;
		isAltDown = (ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_ALT) == KEYBOARD_MODIFIER_ALT;
		isShiftDown = (ApplicationInputEventsHandler::getKeyboardModifiers() &  KEYBOARD_MODIFIER_SHIFT) == KEYBOARD_MODIFIER_SHIFT;
	#elif __APPLE__
		KeyMap keys;
		GetKeys(keys);
		#define IS_KEY_DOWN(key, var) \
		{ \
			uint8_t index = key / 32 ; \
			uint8_t shift = key % 32 ; \
			var = keys[index].bigEndianValue & (1 << shift); \
		}
		IS_KEY_DOWN(kVK_Command, isControlDown);
		IS_KEY_DOWN(kVK_Option, isAltDown);
		IS_KEY_DOWN(kVK_Shift, isShiftDown);
	#endif

	if (isControlDown == false &&
		isAltDown == false &&
		isShiftDown == false) {
		return;
	}

	lockEvents();
	auto guiKeyboardEvent = keyboardEventsPool->allocate();
	guiKeyboardEvent->setTime(Time::getCurrentMillis());
	guiKeyboardEvent->setType(GUIKeyboardEvent_Type::KEYBOARDEVENT_KEY_PRESSED);
	guiKeyboardEvent->setKeyCode(-1);
	guiKeyboardEvent->setKeyChar(-1);
	guiKeyboardEvent->setMetaDown(false);
	guiKeyboardEvent->setControlDown(isControlDown);
	guiKeyboardEvent->setAltDown(isAltDown);
	guiKeyboardEvent->setShiftDown(isShiftDown);
	guiKeyboardEvent->setProcessed(false);
	keyboardEvents.push_back(guiKeyboardEvent);
	unlockEvents();
}

