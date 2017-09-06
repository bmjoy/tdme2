// Generated from /tdme/src/tdme/tools/shared/controller/ParticleSystemScreenController.java
#include <tdme/tools/shared/controller/ParticleSystemScreenController_onActionPerformed_4.h>

#include <java/lang/String.h>
#include <java/lang/StringBuilder.h>
#include <tdme/gui/nodes/GUIElementNode.h>
#include <tdme/gui/nodes/GUINodeController.h>
#include <tdme/tools/shared/controller/FileDialogPath.h>
#include <tdme/tools/shared/controller/FileDialogScreenController.h>
#include <tdme/tools/shared/controller/ParticleSystemScreenController.h>
#include <tdme/tools/shared/views/PopUps.h>
#include <tdme/tools/shared/views/SharedParticleSystemView.h>
#include <tdme/utils/MutableString.h>

using tdme::tools::shared::controller::ParticleSystemScreenController_onActionPerformed_4;
using java::lang::String;
using java::lang::StringBuilder;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUINodeController;
using tdme::tools::shared::controller::FileDialogPath;
using tdme::tools::shared::controller::FileDialogScreenController;
using tdme::tools::shared::controller::ParticleSystemScreenController;
using tdme::tools::shared::views::PopUps;
using tdme::tools::shared::views::SharedParticleSystemView;
using tdme::utils::MutableString;

ParticleSystemScreenController_onActionPerformed_4::ParticleSystemScreenController_onActionPerformed_4(ParticleSystemScreenController* particleSystemScreenController)
	: particleSystemScreenController(particleSystemScreenController)
{
}

void ParticleSystemScreenController_onActionPerformed_4::performAction()
{
	particleSystemScreenController->opsModel->getController()->setValue(particleSystemScreenController->value->set(::java::lang::StringBuilder().append(particleSystemScreenController->view->getPopUpsViews()->getFileDialogScreenController()->getPathName())->append(u"/"_j)->append(particleSystemScreenController->view->getPopUpsViews()->getFileDialogScreenController()->getFileName())->toString()));
	particleSystemScreenController->modelPath->setPath(particleSystemScreenController->view->getPopUpsViews()->getFileDialogScreenController()->getPathName());
	particleSystemScreenController->view->getPopUpsViews()->getFileDialogScreenController()->close();
}
