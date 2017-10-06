// Generated from /tdme/src/tdme/gui/nodes/GUIInputInternalController.java

#pragma once

#include <string>

#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/utils/Enum.h>

using std::wstring;

using tdme::utils::Enum;
using tdme::gui::nodes::GUIInputInternalController;
using tdme::gui::nodes::GUIInputInternalController_CursorMode;

class tdme::gui::nodes::GUIInputInternalController_CursorMode final
	: public Enum
{
	friend class GUIInputInternalController;

public:
	static GUIInputInternalController_CursorMode *SHOW;
	static GUIInputInternalController_CursorMode *HIDE;
	GUIInputInternalController_CursorMode(const wstring& name, int ordinal);
	static GUIInputInternalController_CursorMode* valueOf(const wstring& a0);
};
