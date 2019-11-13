#version 330 core

layout (location = 0) in vec3 attrPosition;
layout (location = 1) in vec3 attrCol;
layout (location = 2) in vec3 attrInstPosition;
layout (location = 3) in vec3 attrInstVelocity;

out vec3 fragPos_worldspace;
out vec3 fragCol;

uniform mat4 model_mat = mat4(1.0);
uniform mat4 view_mat  = mat4(1.0);
uniform mat4 proj_mat  = mat4(1.0);



void main()
{
    //set vertex position (correctly translated via MVP)
    gl_Position = proj_mat * view_mat * (model_mat * vec4(attrPosition, 1.0) + vec4(attrInstPosition, 0.0));

    //shove color to fragment shader
    fragCol = attrInstVelocity;
}