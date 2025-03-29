// Source code of fragment shader
#version 130

// ghost attribute
uniform int ghost;

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);

    // decide if fragment should be in grayscale
    if (ghost != 0) {
        float average = (color.r + color.g + color.b) / 3;
        color.r = color.g = color.b = average;
    }

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
