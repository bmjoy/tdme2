#version 110

#define MAP_LOOKUPS_NEAR		4
#define MAP_LOOKUPS_FAR		8

// uniforms
uniform sampler2D colorBufferTextureUnit;
uniform sampler2D depthBufferTextureUnit;
uniform float bufferTexturePixelWidth;
uniform float bufferTexturePixelHeight;

// passed from vertex shader
varying vec2 vsFragTextureUV;

// main
void main (void) {
	float depth = texture2D(depthBufferTextureUnit, vsFragTextureUV).z;
	vec3 originalColor = texture2D(colorBufferTextureUnit, vsFragTextureUV).xyz;
	vec3 blurredColor;
	gl_FragColor = vec4(originalColor, 1.0);
	if (depth > 0.96) {
		float intensity = clamp((depth - 0.96) * 1.0 / (0.98 - 0.96), 0.0, 1.0);
		for (int y = 0; y < MAP_LOOKUPS_NEAR; y++)
		for (int x = 0; x < MAP_LOOKUPS_NEAR; x++) {
			blurredColor+= texture2D(
				colorBufferTextureUnit,
				vsFragTextureUV.xy +
					vec2(
						(-float(MAP_LOOKUPS_NEAR) / 2.0 + 0.5 + float(x)) * bufferTexturePixelWidth,
						(-float(MAP_LOOKUPS_NEAR) / 2.0 + 0.5 + float(y)) * bufferTexturePixelHeight
					)
			).rgb;
		}
		blurredColor/= float(MAP_LOOKUPS_NEAR * MAP_LOOKUPS_NEAR);
		blurredColor*= intensity;
		blurredColor+= originalColor * (1.0 - intensity);
		gl_FragColor = vec4(blurredColor, 1.0);
	}
	if (depth > 0.98) {
		originalColor = blurredColor;
		blurredColor = vec3(0.0, 0.0, 0.0);
		float intensity = clamp((depth - 0.98) * 1.0 / (0.985 - 0.98), 0.0, 1.0);
		for (int y = 0; y < MAP_LOOKUPS_FAR; y++)
		for (int x = 0; x < MAP_LOOKUPS_FAR; x++) {
			blurredColor+= texture2D(
				colorBufferTextureUnit,
				vsFragTextureUV.xy +
					vec2(
						(-float(MAP_LOOKUPS_FAR) / 2.0 + 0.5 + float(x)) * bufferTexturePixelWidth,
						(-float(MAP_LOOKUPS_FAR) / 2.0 + 0.5 + float(y)) * bufferTexturePixelHeight
					)
			).rgb;
		}
		blurredColor/= float(MAP_LOOKUPS_FAR * MAP_LOOKUPS_FAR);
		blurredColor*= intensity;
		blurredColor+= originalColor * (1.0 - intensity);
		gl_FragColor = vec4(blurredColor, 1.0);
	}
	gl_FragDepth = depth;
}
