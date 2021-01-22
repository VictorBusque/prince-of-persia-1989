#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform vec2 minTexCoord, maxTexCoord;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Use minTexCoord and maxTexCoord to determine the sample textures coordinates
	// inside the font texture atlas. Use that texel to determine transparency
	// combining it with the incoming color.
	vec2 texCoord = texCoordFrag * (maxTexCoord - minTexCoord) + minTexCoord;
	outColor = color * vec4(1, 1, 1, texture(tex, texCoord).r);
}

