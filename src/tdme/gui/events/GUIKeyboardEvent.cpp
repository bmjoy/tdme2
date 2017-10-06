// Generated from /tdme/src/tdme/gui/events/GUIKeyboardEvent.java
#include <tdme/gui/events/GUIKeyboardEvent.h>

#include <java/lang/Object.h>
#include <java/lang/String.h>
#include <java/lang/StringBuilder.h>
#include <tdme/gui/events/GUIKeyboardEvent_Type.h>

using tdme::gui::events::GUIKeyboardEvent;
using java::lang::Object;
using java::lang::String;
using java::lang::StringBuilder;
using tdme::gui::events::GUIKeyboardEvent_Type;

int32_t GUIKeyboardEvent::getKeyCodeFromChar(wchar_t key) {
	switch(key) {
		case(9): return KEYCODE_TAB;
		case(25): return KEYCODE_TAB_SHIFT;
		case(27): return KEYCODE_ESCAPE;
		case(32): return KEYCODE_SPACE;
		case(95): return KEYCODE_DELETE;
		case(127): return KEYCODE_BACKSPACE;
		default: return -1;
	}
}

GUIKeyboardEvent::GUIKeyboardEvent(const ::default_init_tag&)
	: super(*static_cast< ::default_init_tag* >(0))
{
	clinit();
}

GUIKeyboardEvent::GUIKeyboardEvent() 
	: GUIKeyboardEvent(*static_cast< ::default_init_tag* >(0))
{
	ctor();
}

constexpr int32_t GUIKeyboardEvent::KEYCODE_TAB;

constexpr int32_t GUIKeyboardEvent::KEYCODE_TAB_SHIFT;

constexpr int32_t GUIKeyboardEvent::KEYCODE_BACKSPACE;

constexpr int32_t GUIKeyboardEvent::KEYCODE_SPACE;

constexpr int32_t GUIKeyboardEvent::KEYCODE_DELETE;

constexpr int32_t GUIKeyboardEvent::KEYCODE_LEFT;

constexpr int32_t GUIKeyboardEvent::KEYCODE_UP;

constexpr int32_t GUIKeyboardEvent::KEYCODE_RIGHT;

constexpr int32_t GUIKeyboardEvent::KEYCODE_DOWN;

constexpr int32_t GUIKeyboardEvent::KEYCODE_ESCAPE;

void GUIKeyboardEvent::ctor()
{
	super::ctor();
	this->time = -1LL;
	this->type = GUIKeyboardEvent_Type::NONE;
	this->keyCode = -1;
	this->keyChar = char16_t(0x0000);
	this->metaDown = false;
	this->controlDown = false;
	this->altDown = false;
	this->shiftDown = false;
	this->processed = false;
}

int64_t GUIKeyboardEvent::getTime()
{
	return time;
}

void GUIKeyboardEvent::setTime(int64_t time)
{
	this->time = time;
}

GUIKeyboardEvent_Type* GUIKeyboardEvent::getType()
{
	return type;
}

void GUIKeyboardEvent::setType(GUIKeyboardEvent_Type* type)
{
	this->type = type;
}

int32_t GUIKeyboardEvent::getKeyCode()
{
	return keyCode;
}

void GUIKeyboardEvent::setKeyCode(int32_t code)
{
	this->keyCode = code;
}

char16_t GUIKeyboardEvent::getKeyChar()
{
	return keyChar;
}

void GUIKeyboardEvent::setKeyChar(char16_t keyChar)
{
	this->keyChar = keyChar;
}

bool GUIKeyboardEvent::isMetaDown()
{
	return metaDown;
}

void GUIKeyboardEvent::setMetaDown(bool metaDown)
{
	this->metaDown = metaDown;
}

bool GUIKeyboardEvent::isControlDown()
{
	return controlDown;
}

void GUIKeyboardEvent::setControlDown(bool controlDown)
{
	this->controlDown = controlDown;
}

bool GUIKeyboardEvent::isAltDown()
{
	return altDown;
}

void GUIKeyboardEvent::setAltDown(bool altDown)
{
	this->altDown = altDown;
}

bool GUIKeyboardEvent::isShiftDown()
{
	return shiftDown;
}

void GUIKeyboardEvent::setShiftDown(bool shiftDown)
{
	this->shiftDown = shiftDown;
}

bool GUIKeyboardEvent::isProcessed()
{
	return processed;
}

void GUIKeyboardEvent::setProcessed(bool processed)
{
	this->processed = processed;
}

String* GUIKeyboardEvent::toString()
{
	return ::java::lang::StringBuilder().append(u"GUIKeyboardEvent [time="_j)->append(time)
		->append(u", type="_j)
		->append(type->getName())
		->append(u", keyCode="_j)
		->append(keyCode)
		->append(u", keyChar="_j)
		->append(keyChar)
		->append(u", metaDown="_j)
		->append(metaDown)
		->append(u", controlDown="_j)
		->append(controlDown)
		->append(u", altDown="_j)
		->append(altDown)
		->append(u", shiftDown="_j)
		->append(shiftDown)
		->append(u", processed="_j)
		->append(processed)
		->append(u"]"_j)->toString();
}

extern java::lang::Class* class_(const char16_t* c, int n);

java::lang::Class* GUIKeyboardEvent::class_()
{
    static ::java::lang::Class* c = ::class_(u"tdme.gui.events.GUIKeyboardEvent", 32);
    return c;
}

java::lang::Class* GUIKeyboardEvent::getClass0()
{
	return class_();
}

