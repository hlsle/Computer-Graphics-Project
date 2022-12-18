#version 330
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1
layout (location = 0) in vec3 in_Position; 
layout (location = 1) in vec3 in_Color;

out vec3 color;
uniform mat4 trans;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(void)
{
gl_Position = projectionTransform  * viewTransform * trans * vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);
color = in_Color;
}