#include <tdme/engine/subsystems/earlyzrejection/EZRShaderPreDefaultImplementation.h>

#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>

using tdme::engine::subsystems::earlyzrejection::EZRShaderPreDefaultImplementation;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;

bool EZRShaderPreDefaultImplementation::isSupported(Renderer* renderer) {
	return true;
}

EZRShaderPreDefaultImplementation::EZRShaderPreDefaultImplementation(Renderer* renderer): EZRShaderPreBaseImplementation(renderer)
{
}

EZRShaderPreDefaultImplementation::~EZRShaderPreDefaultImplementation() {
}

void EZRShaderPreDefaultImplementation::initialize()
{
	auto shaderVersion = renderer->getShaderVersion();

	// load shadow mapping shaders
	//	pre render
	vertexShaderId = renderer->loadShader(
		renderer->SHADER_VERTEX_SHADER,
		"shader/" + shaderVersion + "/earlyzrejection",
		"pre_vertexshader.c"
	);
	if (vertexShaderId == 0) return;
	fragmentShaderId = renderer->loadShader(
		renderer->SHADER_FRAGMENT_SHADER,
		"shader/" + shaderVersion + "/earlyzrejection",
		"pre_fragmentshader.c"
	);
	if (fragmentShaderId == 0) return;

	// create shadow mapping render program
	//	pre
	programId = renderer->createProgram(renderer->PROGRAM_OBJECTS);
	renderer->attachShaderToProgram(programId, vertexShaderId);
	renderer->attachShaderToProgram(programId, fragmentShaderId);

	//
	EZRShaderPreBaseImplementation::initialize();
}

