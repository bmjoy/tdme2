// Based on:
//	of some code of 3Dlabs Inc. Ltd.
//	and http://stackoverflow.com/questions/11365399/opengl-shader-a-spotlight-and-a-directional-light?answertab=active#tab-top
/************************************************************************
*                                                                       *                                                                       
*                                                                       *
*        Copyright (C) 2002-2004  3Dlabs Inc. Ltd.                      *
*                                                                       *
*                        All rights reserved.                           *
*                                                                       *
* Redistribution and use in source and binary forms, with or without    *
* modification, are permitted provided that the following conditions    *
* are met:                                                              *
*                                                                       *
*     Redistributions of source code must retain the above copyright    *
*     notice, this list of conditions and the following disclaimer.     *
*                                                                       *
*     Redistributions in binary form must reproduce the above           *
*     copyright notice, this list of conditions and the following       *
*     disclaimer in the documentation and/or other materials provided   *
*     with the distribution.                                            *
*                                                                       *
*     Neither the name of 3Dlabs Inc. Ltd. nor the names of its         *
*     contributors may be used to endorse or promote products derived   *
*     from this software without specific prior written permission.     *
*                                                                       *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, *
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                           *
*                                                                       *
************************************************************************/

#version 330

#define FALSE		0
#define MAX_LIGHTS	8

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
};

struct Light {
	int enabled;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec3 spotDirection;
	float spotExponent;
	float spotCosCutoff;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform Material material;
uniform Light lights[MAX_LIGHTS];

#if defined(HAVE_SOLID_SHADING)
#else
	uniform sampler2D specularTextureUnit;
	uniform int specularTextureAvailable;

	uniform sampler2D normalTextureUnit;
	uniform int normalTextureAvailable;

	// material shininess
	float materialShininess;
#endif

// passed from geometry shader
in vec2 vsFragTextureUV;
in vec3 vsNormal;
in vec3 vsPosition;
in vec3 vsTangent;
in vec3 vsBitangent;
in vec4 vsEffectColorMul;
in vec4 vsEffectColorAdd;
in vec3 vsEyeDirection;

// out
out vec4 outColor;

vec4 fragColor;

{$DEFINITIONS}

#if defined(HAVE_TERRAIN_SHADER)
	#define TERRAIN_UV_SCALE			0.2
	#define TERRAIN_LEVEL_0				-4.0
	#define TERRAIN_LEVEL_1				10.0
	#define TERRAIN_HEIGHT_BLEND			4.0
	#define TERRAIN_SLOPE_BLEND			5.0

	in vec3 vertex;
	in vec3 normal;
	in float height;
	in float slope;
	uniform sampler2D grasTextureUnit;
	uniform sampler2D dirtTextureUnit;
	uniform sampler2D stoneTextureUnit;
	uniform sampler2D snowTextureUnit;

	vec4 readTerrainTextureGras(vec3 coords, vec3 blending, float scale) {
		// see: https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		vec4 xAxis = texture(grasTextureUnit, coords.yz * scale);
		vec4 yAxis = texture(grasTextureUnit, coords.xz * scale);
		vec4 zAxis = texture(grasTextureUnit, coords.xy * scale);
		vec4 result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
		return result;
	}

	vec4 readTerrainTextureDirt(vec3 coords, vec3 blending, float scale) {
		// see: https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		vec4 xAxis = texture(dirtTextureUnit, coords.yz * scale);
		vec4 yAxis = texture(dirtTextureUnit, coords.xz * scale);
		vec4 zAxis = texture(dirtTextureUnit, coords.xy * scale);
		vec4 result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
		return result;
	}

	vec4 readTerrainTextureStone(vec3 coords, vec3 blending, float scale) {
		// see: https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		vec4 xAxis = texture(stoneTextureUnit, coords.yz * scale);
		vec4 yAxis = texture(stoneTextureUnit, coords.xz * scale);
		vec4 zAxis = texture(stoneTextureUnit, coords.xy * scale);
		vec4 result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
		return result;
	}

	vec4 readTerrainTextureSnow(vec3 coords, vec3 blending, float scale) {
		// see: https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		vec4 xAxis = texture(snowTextureUnit, coords.yz * scale);
		vec4 yAxis = texture(snowTextureUnit, coords.xz * scale);
		vec4 zAxis = texture(snowTextureUnit, coords.xy * scale);
		vec4 result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
		return result;
	}

#elif defined(HAVE_WATER_SHADER)
	in float height;
#else
	uniform sampler2D diffuseTextureUnit;
	uniform int diffuseTextureAvailable;
	uniform int diffuseTextureMaskedTransparency;
	uniform float diffuseTextureMaskedTransparencyThreshold;
#endif

#if defined(HAVE_DEPTH_FOG)
	#define FOG_DISTANCE_NEAR			100.0
	#define FOG_DISTANCE_MAX				250.0
	#define FOG_RED						(255.0 / 255.0)
	#define FOG_GREEN					(255.0 / 255.0)
	#define FOG_BLUE						(255.0 / 255.0)
	in float fragDepth;
#endif

#if defined(HAVE_SOLID_SHADING)
#else
	void computeLight(in int i, in vec3 normal, in vec3 position) {
		vec3 lightDirection = lights[i].position.xyz - position.xyz;
		float lightDistance = length(lightDirection);
		lightDirection = normalize(lightDirection);
		vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));

		// compute attenuation
		float lightAttenuation =
			1.0 /
			(
				lights[i].constantAttenuation +
				lights[i].linearAttenuation * lightDistance +
				lights[i].quadraticAttenuation * lightDistance * lightDistance
			);

		// see if point on surface is inside cone of illumination
		float lightSpotDot = dot(-lightDirection, normalize(lights[i].spotDirection));
		float lightSpotAttenuation = 0.0;
		if (lightSpotDot >= lights[i].spotCosCutoff) {
			lightSpotAttenuation = pow(lightSpotDot, lights[i].spotExponent);
		}

		// Combine the spotlight and distance attenuation.
		lightAttenuation *= lightSpotAttenuation;

		// add color components to fragment color
		fragColor+=
			clamp(lights[i].ambient * material.ambient, 0.0, 1.0) +
			clamp(lights[i].diffuse * material.diffuse * max(dot(normal, lightDirection), 0.0) * lightAttenuation, 0.0, 1.0) +
			clamp(lights[i].specular * material.specular * pow(max(dot(reflectionDirection, vsEyeDirection), 0.0), 0.3 * materialShininess) * lightAttenuation, 0.0, 1.0);
	}

	void computeLights(in vec3 normal, in vec3 position) {
		// process each light
		for (int i = 0; i < MAX_LIGHTS; i++) {
			// skip on disabled lights
			if (lights[i].enabled == FALSE) continue;

			// compute light
			computeLight(i, normal, position);
		}
	}
#endif

void main(void) {
	#if defined(HAVE_TERRAIN_SHADER)
		// see: https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		vec3 uvMappingBlending = abs(normal);
		uvMappingBlending = normalize(max(uvMappingBlending, 0.00001)); // Force weights to sum to 1.0
		float b = (uvMappingBlending.x + uvMappingBlending.y + uvMappingBlending.z);
		uvMappingBlending /= vec3(b, b, b);
	#elif defined(HAVE_WATER_SHADER)
	#else
		vec4 diffuseTextureColor;
		if (diffuseTextureAvailable == 1) {
			// fetch from texture
			diffuseTextureColor = texture(diffuseTextureUnit, vsFragTextureUV);
			// check if to handle diffuse texture masked transparency
			if (diffuseTextureMaskedTransparency == 1) {
				// discard if beeing transparent
				if (diffuseTextureColor.a < diffuseTextureMaskedTransparencyThreshold) discard;
				// set to opqaue
				diffuseTextureColor.a = 1.0;
			}
		}
	#endif

	//
	fragColor = vec4(0.0, 0.0, 0.0, 0.0);
	fragColor+= clamp(material.emission, 0.0, 1.0);
	fragColor = fragColor * vsEffectColorMul;

	#if defined(HAVE_SOLID_SHADING)
		fragColor+= material.ambient;
		// no op
	#else
		vec3 normal = vsNormal;

		// specular
		materialShininess = material.shininess;
		if (specularTextureAvailable == 1) {
			vec3 specularTextureValue = texture(specularTextureUnit, vsFragTextureUV).rgb;
			materialShininess =
				((0.33 * specularTextureValue.r) +
				(0.33 * specularTextureValue.g) +
				(0.33 * specularTextureValue.b)) * 255.0;
		}

		// compute normal
		if (normalTextureAvailable == 1) {
			vec3 normalVector = normalize(texture(normalTextureUnit, vsFragTextureUV).rgb * 2.0 - 1.0);
			normal = vec3(0.0, 0.0, 0.0);
			normal+= vsTangent * normalVector.x;
			normal+= vsBitangent * normalVector.y;
			normal+= vsNormal * normalVector.z;
		}

		// compute lights
		computeLights(normal, vsPosition);
	#endif

		// take effect colors into account
	fragColor.a = material.diffuse.a * vsEffectColorMul.a;

	#if defined(HAVE_DEPTH_FOG)
		float fogStrength = 0.0;
		if (fragDepth > FOG_DISTANCE_NEAR) {
			fogStrength = (clamp(fragDepth, FOG_DISTANCE_NEAR, FOG_DISTANCE_MAX) - FOG_DISTANCE_NEAR) * 1.0 / (FOG_DISTANCE_MAX - FOG_DISTANCE_NEAR);
		}
	#endif

	//
	#if defined(HAVE_TERRAIN_SHADER)
		if (fogStrength < 1.0) {
			vec4 terrainBlending = vec4(0.0, 0.0, 0.0, 0.0); // gras, dirt, stone, snow

			// height
			if (height > TERRAIN_LEVEL_1) {
				float blendFactorHeight = clamp((height - TERRAIN_LEVEL_1) / TERRAIN_HEIGHT_BLEND, 0.0, 1.0);
				if (slope >= 45.0) {
					terrainBlending[2]+= blendFactorHeight; // stone
				} else
				if (slope >= 45.0 - TERRAIN_SLOPE_BLEND) {
					terrainBlending[2]+= blendFactorHeight * ((slope - (45.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // stone
					terrainBlending[3]+= blendFactorHeight * (1.0 - (slope - (45.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // snow
				} else {
					terrainBlending[3]+= blendFactorHeight; // snow
				}
			}
			if (height >= TERRAIN_LEVEL_0 && height < TERRAIN_LEVEL_1 + TERRAIN_HEIGHT_BLEND) {
				float blendFactorHeight = 1.0;
				if (height > TERRAIN_LEVEL_1) {
					blendFactorHeight = 1.0 - clamp((height - TERRAIN_LEVEL_1) / TERRAIN_HEIGHT_BLEND, 0.0, 1.0);
				} else
				if (height < TERRAIN_LEVEL_0 + TERRAIN_HEIGHT_BLEND) {
					blendFactorHeight = clamp((height - TERRAIN_LEVEL_0) / TERRAIN_HEIGHT_BLEND, 0.0, 1.0);
				}

				if (slope >= 45.0) {
					terrainBlending[2]+= blendFactorHeight; // stone
				} else
				if (slope >= 45.0 - TERRAIN_SLOPE_BLEND) {
					terrainBlending[2]+= blendFactorHeight * ((slope - (45.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // stone
					terrainBlending[1]+= blendFactorHeight * (1.0 - (slope - (45.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // dirt
				} else
				if (slope >= 26.0) {
					terrainBlending[1]+= blendFactorHeight; // dirt
				} else
				if (slope >= 26.0 - TERRAIN_SLOPE_BLEND) {
					terrainBlending[1]+= blendFactorHeight * ((slope - (26.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // dirt
					terrainBlending[0]+= blendFactorHeight * (1.0 - (slope - (26.0 - TERRAIN_SLOPE_BLEND)) / TERRAIN_SLOPE_BLEND); // gras
				} else {
					terrainBlending[0]+= blendFactorHeight; // gras
				}
			}
			if (height < TERRAIN_LEVEL_0 + TERRAIN_HEIGHT_BLEND) {
				float blendFactorHeight = 1.0;
				if (height > TERRAIN_LEVEL_0) {
					blendFactorHeight = 1.0 - clamp((height - TERRAIN_LEVEL_0) / TERRAIN_HEIGHT_BLEND, 0.0, 1.0);
				}
				// 0- meter
				terrainBlending[1]+= blendFactorHeight; // dirt
			}

			//
			outColor = vsEffectColorAdd;
			if (terrainBlending[0] > 0.001) outColor+= readTerrainTextureGras(vertex, uvMappingBlending, TERRAIN_UV_SCALE) * terrainBlending[0];
			if (terrainBlending[1] > 0.001) outColor+= readTerrainTextureDirt(vertex, uvMappingBlending, TERRAIN_UV_SCALE) * terrainBlending[1];
			if (terrainBlending[2] > 0.001) outColor+= readTerrainTextureStone(vertex, uvMappingBlending, TERRAIN_UV_SCALE) * terrainBlending[2];
			if (terrainBlending[3] > 0.001) outColor+= readTerrainTextureSnow(vertex, uvMappingBlending, TERRAIN_UV_SCALE) * terrainBlending[3];
			outColor*= fragColor;
			outColor = clamp(outColor, 0.0, 1.0);
			if (fogStrength > 0.0) {
				outColor = vec4(
					(outColor.rgb * (1.0 - fogStrength)) +
					vec3(FOG_RED, FOG_GREEN, FOG_BLUE) * fogStrength,
					1.0
				);
			}
		} else {
			outColor = vec4(
				vec3(FOG_RED, FOG_GREEN, FOG_BLUE) * fogStrength,
				1.0
			);
		}
	#elif defined(HAVE_WATER_SHADER)
		//
		outColor = vec4(0.25, 0.25, 0.8, 0.5);
		outColor+= vsEffectColorAdd;
		outColor*= fragColor;
		outColor = clamp(outColor, 0.0, 1.0);
		if (fogStrength > 0.0) {
			outColor = vec4(
				(outColor.rgb * (1.0 - fogStrength)) +
				vec3(FOG_RED, FOG_GREEN, FOG_BLUE) * fogStrength,
				1.0
			);
		}
	#else
		if (diffuseTextureAvailable == 1) {
			outColor = clamp((vsEffectColorAdd + diffuseTextureColor) * fragColor, 0.0, 1.0);
		} else {
			outColor = clamp(vsEffectColorAdd + fragColor, 0.0, 1.0);
		}
		if (outColor.a < 0.0001) discard;
		#if defined(HAVE_BACK)
			gl_FragDepth = 1.0;
		#endif
		#if defined(HAVE_FRONT)
			gl_FragDepth = 0.0;
		#endif
		#if defined(HAVE_DEPTH_FOG)
			if (fogStrength > 0.0) {
				outColor = vec4(
					(outColor.rgb * (1.0 - fogStrength)) +
					vec3(FOG_RED, FOG_GREEN, FOG_BLUE) * fogStrength,
					1.0
				);
			}
		#endif
	#endif
}
