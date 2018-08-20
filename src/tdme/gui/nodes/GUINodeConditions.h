#pragma once

#include <string>
#include <vector>

#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/utils/fwd-tdme.h>

using std::vector;
using std::string;

using tdme::gui::nodes::GUIElementNode;

/** 
 * GUI node conditions
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::gui::nodes::GUINodeConditions final
{
	friend class GUINode;

private:
	GUIElementNode* elementNode {  };
	vector<string> conditions {  };

	/**
	 * Update node
	 * @param node node
	 */
	void updateNode(GUINode* node);

	/**
	 * Update element node
	 */
	void updateElementNode();

public:

	/** 
	 * @return conditions
	 */
	vector<string>& getConditions();

	/** 
	 * Add a condition
	 * @param condition condition
	 * @return condition changed
	 */
	bool add(const string& condition);

	/** 
	 * Remove a condition
	 * @param condition condition
	 * @return condition changed
	 */
	bool remove(const string& condition);

	/** 
	 * Remove all
	 * @return condition changed
	 */
	bool removeAll();

	/**
	 * Public constructor
	 * @param node node
	 */
	GUINodeConditions(GUIElementNode* node = nullptr);
};
