#include <tdme/engine/model/PBRMaterialProperties.h>

#include <string>

#include <tdme/engine/fileio/textures/Texture.h>
#include <tdme/engine/fileio/textures/TextureReader.h>

using std::string;

using tdme::engine::model::PBRMaterialProperties;
using tdme::engine::fileio::textures::Texture;
using tdme::engine::fileio::textures::TextureReader;

PBRMaterialProperties::PBRMaterialProperties()
{
	baseColorTexture = nullptr;
	metallicRoughnessTexture = nullptr;
	normalTexture = nullptr;
	baseColorTextureTransparency = false;
	baseColorTextureMaskedTransparency = false;
	baseColorTextureMaskedTransparencyThreshold = 0.1f;
}

PBRMaterialProperties::~PBRMaterialProperties() {
	if (baseColorTexture != nullptr) baseColorTexture->releaseReference();
	if (metallicRoughnessTexture != nullptr) metallicRoughnessTexture->releaseReference();
	if (normalTexture != nullptr) normalTexture->releaseReference();
}

void PBRMaterialProperties::setBaseColorTexture(const string& pathName, const string& fileName)
{
	baseColorTexturePathName = pathName;
	baseColorTextureFileName = fileName;
	baseColorTexture = TextureReader::read(pathName, fileName);
	checkBaseColorTextureTransparency();
}

void PBRMaterialProperties::setMetallicRoughnessTexture(const string& pathName, const string& fileName)
{
	metallicRoughnessTexturePathName = pathName;
	metallicRoughnessTextureFileName = fileName;
	metallicRoughnessTexture = TextureReader::read(pathName, fileName);
}

void PBRMaterialProperties::setNormalTexture(const string& pathName, const string& fileName)
{
	normalTexturePathName = pathName;
	normalTextureFileName = fileName;
	normalTexture = TextureReader::read(pathName, fileName);
}

void PBRMaterialProperties::checkBaseColorTextureTransparency()
{
	baseColorTextureTransparency = false;
	auto baseColorTextureMaskedTransparencyTest = true;
	if (baseColorTexture != nullptr && baseColorTexture->getDepth() == 32) {
		auto textureData = baseColorTexture->getTextureData();
		for (auto i = 0; i < baseColorTexture->getTextureWidth() * baseColorTexture->getTextureHeight(); i++) {
			auto alpha = textureData->get(i * 4 + 3);
			if (alpha != 255) baseColorTextureTransparency = true;
			if (alpha > 5 || alpha < 250) baseColorTextureMaskedTransparencyTest = false;
		}
	}
	if (baseColorTextureMaskedTransparency == false) baseColorTextureMaskedTransparency = baseColorTextureMaskedTransparencyTest;
}
