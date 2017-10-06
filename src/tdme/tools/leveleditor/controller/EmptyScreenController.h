
#pragma once

#include <string>

#include <tdme/gui/events/fwd-tdme.h>
#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/tools/leveleditor/controller/fwd-tdme.h>
#include <tdme/tools/leveleditor/views/fwd-tdme.h>
#include <tdme/tools/shared/controller/fwd-tdme.h>
#include <tdme/utils/fwd-tdme.h>
#include <tdme/tools/shared/controller/ScreenController.h>
#include <tdme/gui/events/GUIActionListener.h>
#include <tdme/gui/events/GUIChangeListener.h>

using std::wstring;

using tdme::tools::shared::controller::ScreenController;
using tdme::gui::events::GUIActionListener;
using tdme::gui::events::GUIChangeListener;
using tdme::gui::events::GUIActionListener_Type;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUIScreenNode;
using tdme::gui::nodes::GUITextNode;
using tdme::tools::leveleditor::views::EmptyView;
using tdme::tools::shared::controller::EntityBaseSubScreenController;
using tdme::utils::MutableString;

/** 
 * Empty screen controller
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::tools::leveleditor::controller::EmptyScreenController final
	: public ScreenController
	, public GUIActionListener
	, public GUIChangeListener
{
	friend class EmptyScreenController_EmptyScreenController_1;

private:
	static MutableString* TEXT_EMPTY;
	EntityBaseSubScreenController* entityBaseSubScreenController {  };
	EmptyView* view {  };
	GUIScreenNode* screenNode {  };
	GUITextNode* screenCaption {  };

public:
	GUIScreenNode* getScreenNode() override;
	void initialize() override;
	void dispose() override;

	/** 
	 * Set screen caption
	 * @param text
	 */
	void setScreenCaption(const wstring& text);

	/** 
	 * Set up general entity data
	 * @param name
	 * @param description
	 */
	void setEntityData(const wstring& name, const wstring& description);

	/** 
	 * Unset entity data
	 */
	void unsetEntityData();

	/** 
	 * Set up entity properties
	 * @param preset id
	 * @param entity properties
	 * @param selected name
	 */
	void setEntityProperties(const wstring& presetId, const wstring& selectedName);

	/** 
	 * Unset entity properties
	 */
	void unsetEntityProperties();

	/** 
	 * On quit
	 */
	void onQuit();

	/** 
	 * Shows the error pop up
	 */
	void showErrorPopUp(const wstring& caption, const wstring& message);
	void onValueChanged(GUIElementNode* node) override;
	void onActionPerformed(GUIActionListener_Type* type, GUIElementNode* node) override;

	/**
	 * Public constructor
	 * @param view
	 */
	EmptyScreenController(EmptyView* view);
};
