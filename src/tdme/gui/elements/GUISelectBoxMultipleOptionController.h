#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/gui/elements/fwd-tdme.h>
#include <tdme/gui/events/fwd-tdme.h>
#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/utils/fwd-tdme.h>
#include <tdme/gui/nodes/GUINodeController.h>

using std::string;

using tdme::gui::nodes::GUINodeController;
using tdme::gui::events::GUIKeyboardEvent;
using tdme::gui::events::GUIMouseEvent;
using tdme::gui::nodes::GUINode;
using tdme::gui::nodes::GUIParentNode;
using tdme::utils::MutableString;

/** 
 * GUI select box multiple option controller
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::gui::elements::GUISelectBoxMultipleOptionController final
	: public GUINodeController
{
	friend class GUISelectBoxMultipleOption;
	friend class GUISelectBoxMultipleController;

private:
	static string CONDITION_SELECTED;
	static string CONDITION_UNSELECTED;
	static string CONDITION_FOCUSSED;
	static string CONDITION_UNFOCUSSED;
	static string CONDITION_DISABLED;
	static string CONDITION_ENABLED;
	GUIParentNode* selectBoxMultipleNode {  };
	bool selected {  };
	bool focussed {  };

	/** 
	 * @return is selected
	 */
	bool isSelected();

	/** 
	 * Select
	 */
	void select();

	/** 
	 * Unselect
	 * @param checked
	 */
	void unselect();

	/** 
	 * Toggle selection
	 */
	void toggle();


	/** 
	 * Focus
	 */
	void focus();

	/** 
	 * Unfocus
	 * @param checked
	 */
	void unfocus();

	/**
	 * Private constructor
	 * @param node
	 */
	GUISelectBoxMultipleOptionController(GUINode* node);

public:
	bool isDisabled() override;
	void setDisabled(bool disabled) override;

	/**
	 * @return is focussed
	 */
	bool isFocussed();

	// overriden methods
	void initialize() override;
	void dispose() override;
	void postLayout() override;
	void handleMouseEvent(GUINode* node, GUIMouseEvent* event) override;
	void handleKeyboardEvent(GUINode* node, GUIKeyboardEvent* event) override;
	void tick() override;
	void onFocusGained() override;
	void onFocusLost() override;
	bool hasValue() override;
	MutableString* getValue() override;
	void setValue(MutableString* value) override;

};
