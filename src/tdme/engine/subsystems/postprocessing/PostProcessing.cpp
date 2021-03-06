#include <map>
#include <string>

using std::map;
using std::string;

#include <tdme/engine/subsystems/postprocessing/fwd-tdme.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessing.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingProgram.h>

using tdme::engine::subsystems::postprocessing::PostProcessing;
using tdme::engine::subsystems::postprocessing::PostProcessingProgram;

PostProcessing::PostProcessing() {
	{
		auto program = new PostProcessingProgram(PostProcessingProgram::RENDERPASS_FINAL);
		program->addPostProcessingStep("depth_blur", PostProcessingProgram::FRAMEBUFFERSOURCE_SCREEN, PostProcessingProgram::FRAMEBUFFERTARGET_SCREEN);
		if (program->isSupported() == true) {
			programs["depth_blur"] = program;
		} else {
			delete program;
		}

	}
	{
		auto program = new PostProcessingProgram(PostProcessingProgram::RENDERPASS_OBJECTS);
		program->addPostProcessingStep("ssao_map", PostProcessingProgram::FRAMEBUFFERSOURCE_SCREEN, PostProcessingProgram::FRAMEBUFFERTARGET_TEMPORARY);
		program->addPostProcessingStep("ssao", PostProcessingProgram::FRAMEBUFFERSOURCE_SCREEN, PostProcessingProgram::FRAMEBUFFERTARGET_SCREEN, true);
		if (program->isSupported() == true) {
			programs["ssao"] = program;
		} else {
			delete program;
		}
	}
}

PostProcessingProgram* PostProcessing::getPostProcessingProgram(const string& programId) {
	auto programIt = programs.find(programId);
	return programIt == programs.end()?nullptr:programIt->second;
}


