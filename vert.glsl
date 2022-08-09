#version 110
attribute vec3 Position;
attribute vec3 incolor;
attribute vec2 Tex;
attribute float intextureorcolor;
varying vec3 outcolor;
varying vec2 texcoords;
varying float textureorcolor;
uniform mat4 model;
void main(){
   gl_Position = model * vec4(Position, 1.0);
   outcolor = incolor;
   texcoords = Tex;
   textureorcolor = intextureorcolor;
}
