#version 110
uniform vec3 color;
uniform sampler2D tex0;
varying float textureorcolor;
varying vec3 outcolor;
varying vec2 texcoords;
void main(){
   vec4 texcolor = texture2D(tex0, texcoords);
   if(texcolor.a < 0.1 && textureorcolor == 1.0){
      discard;
   }
   gl_FragColor = vec4(outcolor, 1.0) * (1.0 - textureorcolor) + texcolor * textureorcolor;
}
