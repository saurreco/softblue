#version 330 core

// Task 16: Create a UV coordinate in variable
in vec2 uvFrag;
// Task 8: Add a sampler2D uniform
uniform sampler2D tex;


// Task 29: Add a bool on whether or not to filter the texture
uniform int isBussin;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    fragColor = texture(tex, uvFrag);
   // if (isBussin == 1)
    //       fragColor = 1.0f - fragColor;
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    // Task 33: Invert fragColor's r, g, and b color channels if your bool is true

}
