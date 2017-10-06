
#pragma once

#include <tdme/tools/shared/controller/fwd-tdme.h>
#include <tdme/gui/events/Action.h>

using tdme::gui::events::Action;
using tdme::tools::shared::controller::ModelViewerScreenController;

class tdme::tools::shared::controller::ModelViewerScreenController_onModelSave_3
	: public virtual Action
{
	friend class ModelViewerScreenController;
	friend class ModelViewerScreenController_ModelViewerScreenController_1;
	friend class ModelViewerScreenController_onModelLoad_2;

public:
	void performAction() override;

	// Generated
	ModelViewerScreenController_onModelSave_3(ModelViewerScreenController* modelViewerScreenController);

private:
	ModelViewerScreenController* modelViewerScreenController;
};
