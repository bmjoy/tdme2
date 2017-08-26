// Generated from /tdme/src/tdme/gui/nodes/GUIVerticalScrollbarInternalNode.java
#include <tdme/gui/nodes/GUIVerticalScrollbarInternalNode.h>

#include <array>

#include <java/lang/String.h>
#include <tdme/gui/GUI.h>
#include <tdme/gui/nodes/GUIColor.h>
#include <tdme/gui/nodes/GUINode_Border.h>
#include <tdme/gui/nodes/GUINode_ComputedConstraints.h>
#include <tdme/gui/nodes/GUINodeController.h>
#include <tdme/gui/nodes/GUIVerticalScrollbarInternalController_State.h>
#include <tdme/gui/nodes/GUIVerticalScrollbarInternalController.h>
#include <tdme/gui/renderer/GUIRenderer.h>
#include <Array.h>

using std::array;

using tdme::gui::nodes::GUIVerticalScrollbarInternalNode;
using java::lang::String;
using tdme::gui::GUI;
using tdme::gui::nodes::GUIColor;
using tdme::gui::nodes::GUINode_Border;
using tdme::gui::nodes::GUINode_ComputedConstraints;
using tdme::gui::nodes::GUINodeController;
using tdme::gui::nodes::GUIVerticalScrollbarInternalController_State;
using tdme::gui::nodes::GUIVerticalScrollbarInternalController;
using tdme::gui::renderer::GUIRenderer;

template<typename T, typename U>
static T java_cast(U* u)
{
    if (!u) return static_cast<T>(nullptr);
    auto t = dynamic_cast<T>(u);
    return t;
}

GUIVerticalScrollbarInternalNode::GUIVerticalScrollbarInternalNode(const ::default_init_tag&)
	: super(*static_cast< ::default_init_tag* >(0))
{
	clinit();
}

GUIVerticalScrollbarInternalNode::GUIVerticalScrollbarInternalNode(GUIScreenNode* screenNode, GUIParentNode* parentNode, String* id, GUINode_Flow* flow, GUINode_Alignments* alignments, GUINode_RequestedConstraints* requestedConstraints, GUIColor* backgroundColor, GUINode_Border* border, GUINode_Padding* padding, GUINodeConditions* showOn, GUINodeConditions* hideOn, GUIColor* barColorNone, GUIColor* barColorMouseOver, GUIColor* barColorDragging) 
	: GUIVerticalScrollbarInternalNode(*static_cast< ::default_init_tag* >(0))
{
	ctor(screenNode,parentNode,id,flow,alignments,requestedConstraints,backgroundColor,border,padding,showOn,hideOn,barColorNone,barColorMouseOver,barColorDragging);
}

void GUIVerticalScrollbarInternalNode::ctor(GUIScreenNode* screenNode, GUIParentNode* parentNode, String* id, GUINode_Flow* flow, GUINode_Alignments* alignments, GUINode_RequestedConstraints* requestedConstraints, GUIColor* backgroundColor, GUINode_Border* border, GUINode_Padding* padding, GUINodeConditions* showOn, GUINodeConditions* hideOn, GUIColor* barColorNone, GUIColor* barColorMouseOver, GUIColor* barColorDragging)
{
	super::ctor(screenNode, parentNode, id, flow, alignments, requestedConstraints, backgroundColor, border, padding, showOn, hideOn);
	this->controller = new GUIVerticalScrollbarInternalController(this);
	this->barColorNone = barColorNone;
	this->barColorMouseOver = barColorMouseOver;
	this->barColorDragging = barColorDragging;
}

String* GUIVerticalScrollbarInternalNode::getNodeType()
{
	return u"scrollbar"_j;
}

bool GUIVerticalScrollbarInternalNode::isContentNode()
{
	return false;
}

int32_t GUIVerticalScrollbarInternalNode::getContentWidth()
{
	return computedConstraints->width;
}

int32_t GUIVerticalScrollbarInternalNode::getContentHeight()
{
	return computedConstraints->height;
}

void GUIVerticalScrollbarInternalNode::render(GUIRenderer* guiRenderer, vector<GUINode*>* floatingNodes)
{
	if (conditionsMet == false)
		return;

	super::render(guiRenderer, floatingNodes);
	float screenWidth = guiRenderer->getGUI()->getWidth();
	float screenHeight = guiRenderer->getGUI()->getHeight();
	auto controller = java_cast< GUIVerticalScrollbarInternalController* >(this->controller);
	auto barHeight = controller->getBarHeight();
	auto barTop = controller->getBarTop();
	float left = computedConstraints->left + computedConstraints->alignmentLeft + border->left;
	auto top = barTop;
	float width = computedConstraints->width - border->left - border->right;
	auto height = barHeight;
	array<float, 4>* barColorArray = nullptr;
	{
		auto v = controller->getState();
		if ((v == GUIVerticalScrollbarInternalController_State::NONE)) {
			barColorArray = barColorNone->getArray();
			goto end_switch0;;
		}
		if ((v == GUIVerticalScrollbarInternalController_State::MOUSEOVER)) {
			barColorArray = barColorMouseOver->getArray();
			goto end_switch0;;
		}
		if ((v == GUIVerticalScrollbarInternalController_State::DRAGGING)) {
			barColorArray = barColorDragging->getArray();
			goto end_switch0;;
		}
		end_switch0:;
	}

	guiRenderer->bindTexture(0);
	guiRenderer->addQuad(((left) / (screenWidth / 2.0f)) - 1.0f, ((screenHeight - top) / (screenHeight / 2.0f)) - 1.0f, (*barColorArray)[0], (*barColorArray)[1], (*barColorArray)[2], (*barColorArray)[3], 0.0f, 1.0f, ((left + width) / (screenWidth / 2.0f)) - 1.0f, ((screenHeight - top) / (screenHeight / 2.0f)) - 1.0f, (*barColorArray)[0], (*barColorArray)[1], (*barColorArray)[2], (*barColorArray)[3], 1.0f, 1.0f, ((left + width) / (screenWidth / 2.0f)) - 1.0f, ((screenHeight - top - height) / (screenHeight / 2.0f)) - 1.0f, (*barColorArray)[0], (*barColorArray)[1], (*barColorArray)[2], (*barColorArray)[3], 1.0f, 0.0f, ((left) / (screenWidth / 2.0f)) - 1.0f, ((screenHeight - top - height) / (screenHeight / 2.0f)) - 1.0f, (*barColorArray)[0], (*barColorArray)[1], (*barColorArray)[2], (*barColorArray)[3], 0.0f, 0.0f);
	guiRenderer->render();
}

extern java::lang::Class* class_(const char16_t* c, int n);

java::lang::Class* GUIVerticalScrollbarInternalNode::class_()
{
    static ::java::lang::Class* c = ::class_(u"tdme.gui.nodes.GUIVerticalScrollbarInternalNode", 47);
    return c;
}

java::lang::Class* GUIVerticalScrollbarInternalNode::getClass0()
{
	return class_();
}

