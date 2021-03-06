#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/gui/events/fwd-tdme.h>
#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/tools/leveleditor/controller/fwd-tdme.h>
#include <tdme/tools/shared/views/fwd-tdme.h>
#include <tdme/tools/shared/controller/ScreenController.h>
#include <tdme/gui/events/GUIActionListener.h>
#include <tdme/gui/events/GUIChangeListener.h>

using std::string;

using tdme::tools::shared::controller::ScreenController;
using tdme::gui::events::GUIActionListener;
using tdme::gui::events::GUIChangeListener;
using tdme::gui::events::GUIActionListener_Type;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUIScreenNode;
using tdme::tools::shared::views::PopUps;

/** 
 * Level editor model library screen controller
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::tools::leveleditor::controller::LevelEditorEntityLibraryScreenController
	: public ScreenController
	, public virtual GUIActionListener
	, public virtual GUIChangeListener
{
	friend class LevelEditorEntityLibraryScreenController_onValueChanged_1;

private:
	GUIScreenNode* screenNode {  };
	GUIElementNode* entityLibraryListBox {  };
	GUIElementNode* buttonEntityPlace {  };
	GUIElementNode* buttonLevelEdit {  };
	PopUps* popUps {  };
	string modelPath {  };

public:
	GUIScreenNode* getScreenNode() override;

	/** 
	 * @return model path
	 */
	virtual const string& getModelPath();

	/** 
	 * Set model path
	 * @param modelPath model path
	 */
	virtual void setModelPath(const string& modelPath);
	void initialize() override;
	void dispose() override;

	/** 
	 * Select entity by entity id
	 * @param entityId entity id
	 */
	virtual void selectEntity(int32_t entityId);

	/** 
	 * Set up complete entity library
	 */
	virtual void setEntityLibrary();

	/** 
	 * On entity selection changed
	 */
	virtual void onEntitySelectionChanged();

	/** 
	 * On edit entity
	 */
	virtual void onEditEntity();

	/** 
	 * On edit level
	 */
	virtual void onEditLevel();

	/** 
	 * place object button clicked
	 */
	virtual void onPlaceEntity();

	/** 
	 * place model entity clicked
	 */
	virtual void onDeleteEntity();

	/**
	 * partition entity
	 */
	virtual void onPartitionEntity();

	// overriden methods
	void onValueChanged(GUIElementNode* node) override;
	void onActionPerformed(GUIActionListener_Type* type, GUIElementNode* node) override;

	/**
	 * Public constructor
	 */
	LevelEditorEntityLibraryScreenController(PopUps* popUps);
};
