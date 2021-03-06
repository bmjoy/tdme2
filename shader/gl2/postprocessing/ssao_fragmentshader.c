#version 100

precision highp sampler2D;
precision highp float;

{$DEFINITIONS}

#if defined(HAVE_4K)
	#define SSAO_MAP_LOOKUPS		4
#else
	#define SSAO_MAP_LOOKUPS		2
#endif

// uniforms
uniform sampler2D colorBufferTextureUnit;
uniform sampler2D depthBufferTextureUnit;
uniform sampler2D temporaryColorBufferTextureUnit;
uniform sampler2D temporaryDepthBufferTextureUnit;
uniform float bufferTexturePixelWidth;
uniform float bufferTexturePixelHeight;

// passed from vertex shader
varying vec2 vsFragTextureUV;

// main
void main(void) {
	float depth = texture2D(depthBufferTextureUnit, vsFragTextureUV).r;
	vec3 originalColor = texture2D(colorBufferTextureUnit, vsFragTextureUV).rgb;

	// scale with SSAO color
	vec3 ssaoColor = vec3(0.0, 0.0, 0.0);
	for (int y = 0; y < SSAO_MAP_LOOKUPS; y++)
	for (int x = 0; x < SSAO_MAP_LOOKUPS; x++) {
		ssaoColor+= texture2D(
			temporaryColorBufferTextureUnit,
			vsFragTextureUV.xy +
				vec2(
					(float(-SSAO_MAP_LOOKUPS) / 2.0 + 0.5 + float(x)) * bufferTexturePixelWidth,
					(float(-SSAO_MAP_LOOKUPS) / 2.0 + 0.5 + float(y)) * bufferTexturePixelHeight
				)
		).rgb;
	}
	ssaoColor/= float(SSAO_MAP_LOOKUPS * SSAO_MAP_LOOKUPS);
	gl_FragColor = vec4(ssaoColor, 1.0) * vec4(originalColor, 1.0);
}
