#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/application/Application.h>
#include <tdme/gui/events/GUIActionListener.h>
#include <tdme/gui/events/GUIChangeListener.h>
#include <tdme/tools/installer/fwd-tdme.h>
#include <tdme/tools/shared/views/fwd-tdme.h>
#include <tdme/gui/events/GUIChangeListener.h>

using std::string;

using tdme::application::Application;
using tdme::engine::Engine;
using tdme::gui::events::GUIActionListener;
using tdme::gui::events::GUIActionListener_Type;
using tdme::gui::events::GUIChangeListener;
using tdme::tools::shared::views::PopUps;

/** 
 * Installer
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::tools::installer::Installer final
	: public virtual Application, public virtual GUIActionListener, public virtual GUIChangeListener
{
private:
	Engine* engine { nullptr };
	PopUps* popUps { nullptr };
	enum Screen { SCREEN_WELCOME, SCREEN_LICENSE, SCREEN_COMPONENTS, SCREEN_PATH, SCREEN_INSTALLING, SCREEN_FINISHED, SCREEN_MAX };
	Screen screen;

public:
	void initialize() override;
	void dispose() override;
	void reshape(int32_t width, int32_t height) override;
	void display() override;
	void onActionPerformed(GUIActionListener_Type* type, GUIElementNode* node) override;
	void onValueChanged(GUIElementNode* node) override;

	/** 
	 * Main
	 * @param argc argument count
	 * @param argv argument values
	 */
	static void main(int argc, char** argv);

	/**
	 * Public constructor
	 */
	Installer();
};