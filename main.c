#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<stdbool.h>
#include<string.h>
#include<stb/stb_image.h>
#include<cglm/cglm.h>

const unsigned int width = 1054;
const unsigned int height = 1057;
//float lastx = (float)width/2;
//float lasty = (float)height/2;
float xoffset;
float yoffset;
float jumpacceleration = 1.0f;
float gravity = -0.05f;
GLint uniColor;
GLint xploc;
GLint yploc;
GLint modelloc;
GLint tex0uni;
GLuint left = 0;
GLuint right = 0;
GLuint rotateleft = 0;
GLuint rotateright = 0;
GLuint reset = 0;
GLuint jumppressed = 0;
GLuint jump = 0;
GLuint falling = 0;
GLuint leftend = 0;
GLuint rightend = 0;
void key_callback();

int main(int argc, char *argv[]){
   int i;
   for(i = 1; i < argc; i++){
      if(strcmp(argv[i], "hello") == 0){
         printf("Hi!\n");
      }
   }
   if(!glfwInit()){
      printf("Failed to initialize glfw!\n");
      return 1;
   }else{
      printf("Successfully initialized glfw!\n");
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
   GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
   if(window == NULL){
      printf("Window creation failed!\n");
      return 1;
   }else{
      printf("Window created successfully!\n");
   }
   glfwSetKeyCallback(window, key_callback);
   glfwMakeContextCurrent(window);
   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK){
      printf("Failed to initialize glew!\n");
      return 1;
   }else{
      printf("Successfully initialized glew!\n");
   } 
   glfwSwapInterval(1);
   float Vertices[] = {
     -0.1f, -0.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f,
     -0.1f, -0.2f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 1.0f, 1.0f,
      0.1f, -0.2f, 0.0f, 0.0f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f,
      0.1f, -0.5f, 0.0f, 0.0f, 0.1f, 0.1f, 1.0f, 0.0f, 1.0f,
   };

   float floor[] = {
     -2.0f, -1.0f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f,
     -2.0f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
      2.0f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 1.0f, 0.0f,
      2.0f, -1.0f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f
   };

   float cloud[] = {
     -0.5f,  0.3f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f,
     -0.5f,  0.7f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 1.0f, 1.0f,
      0.5f,  0.7f, 0.0f, 0.0f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f,
      0.5f,  0.3f, 0.0f, 0.0f, 0.1f, 0.1f, 1.0f, 0.0f, 1.0f,
   };

   GLuint indices[] = {
      0, 1, 2,
      0, 2, 3
   };

   printf("GLfloat variable called successfully\n"); 
   GLuint ShaderProgram = glCreateProgram();
   GLuint ShaderObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint ShaderObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* pvs[1];
   const GLchar* pfs[1];
   const char* pVertexShaderText = "#version 110\n"
   "attribute vec3 Position;\n"
   "attribute vec3 incolor;\n"
   "attribute vec2 Tex;\n"
   "attribute float intextureorcolor;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "varying float textureorcolor;\n"
   "uniform mat4 model;\n"
   "void main(){\n"
   "   gl_Position = model * vec4(Position, 1.0);\n"
   "   outcolor = incolor;\n"
   "   texcoords = Tex;\n"
   "   textureorcolor = intextureorcolor;\n"
   "}";
   const char* pFragmentShaderText = "#version 110\n"
   "uniform vec3 color;\n"
   "uniform sampler2D tex0;\n"
   "varying float textureorcolor;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "void main(){\n"
   "   vec4 texcolor = texture2D(tex0, texcoords);\n"
   "   if(textureorcolor == 1.0){\n"
   "      if(texcolor.a < 0.1){\n"
   "         discard;\n"
   "      }\n"
   "      gl_FragColor = texcolor;\n"
   "   }else{\n"
   "      gl_FragColor = vec4(outcolor, 1.0);\n"
   "   }\n"
   "}";
   pvs[0] = pVertexShaderText;
   pfs[0] = pFragmentShaderText;
   GLint LenghtsVertex[1];
   GLint LenghtsFragment[1];
   LenghtsVertex[0] = strlen(pVertexShaderText);
   LenghtsFragment[0] = strlen(pFragmentShaderText);
   glShaderSource(ShaderObjv, 1, pvs, LenghtsVertex);
   glShaderSource(ShaderObjf, 1, pfs, LenghtsFragment);
   glCompileShader(ShaderObjv);
   glCompileShader(ShaderObjf);
   GLint successvs;
   glGetShaderiv(ShaderObjv, GL_COMPILE_STATUS, &successvs);
   if(!successvs){
       GLchar InfoLog1[1024];
       glGetShaderInfoLog(ShaderObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog1);
       return 1;
   }
   GLint successfs;
   glGetShaderiv(ShaderObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       GLchar InfoLog2[1024];
       glGetShaderInfoLog(ShaderObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog2);
       return 1;
   }
   glAttachShader(ShaderProgram, ShaderObjv);
   glAttachShader(ShaderProgram, ShaderObjf);
   glLinkProgram(ShaderProgram);
   GLint success;
   glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
   if(!success){
      GLchar InfoLog3[1024];
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking shader program: '%s'\n", InfoLog3);
      return 1;
   }
   glValidateProgram(ShaderProgram);
   GLint successl;
   glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &successl);
   if(!successl){
      GLchar InfoLog4[1024];
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog4), NULL, InfoLog4);
      fprintf(stderr, "Error validating shader program: '%s'\n", InfoLog4);
      return 1;
   }
   glUseProgram(ShaderProgram);
   mat4 model = GLM_MAT4_IDENTITY_INIT;
   mat4 floormodel = GLM_MAT4_IDENTITY_INIT;
   mat4 cloudmodel = GLM_MAT4_IDENTITY_INIT;
   versor rotation = GLM_QUAT_IDENTITY_INIT;
   glm_quat_init(rotation, 0.0f, 0.0f, 0.0f, 1.0f);
   vec3 position;
   vec3 floorposition;
   float currentframe = glfwGetTime();
   float lastframe = 0.0f;
   float deltatime;
   float angle = glm_rad(0.0f);
   float currentrotation = 0.0f;
   GLenum err;
   GLint uniColor = glGetUniformLocation(ShaderProgram, "color");
   GLint tex0uni = glGetUniformLocation(ShaderProgram, "tex0");
   GLint modelloc = glGetUniformLocation(ShaderProgram, "model");
   glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   glUniform1i(tex0uni, 0); 
   glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
   glClearColor(0.53f, 0.71f, 0.92f, 1.0f);
   GLuint VBO;
   GLuint VAO = 0;
   GLuint EBO;
   glGenBuffers(1, &VBO);
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   printf("Buffer created successfully\n");
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   glEnableVertexAttribArray(3);
   printf("Enabling buffer successfull\n");
   int texwidth, texheight, texnum, tex2width, tex2height, tex2num, tex3width, tex3height, tex3num, tex4width, tex4height, tex4num, tex5width, tex5height, tex5num;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* bytes = stbi_load("idle2.png", &texwidth, &texheight, &texnum, 0);
   unsigned char* bytes2 = stbi_load("grass.jpg", &tex2width, &tex2height, &tex2num, 0);
   unsigned char* bytes3 = stbi_load("moving_left2.png", &tex3width, &tex3height, &tex3num, 0);
   unsigned char* bytes4 = stbi_load("moving_right2.png", &tex4width, &tex4height, &tex4num, 0);
   unsigned char* bytes5 = stbi_load("cloud2.png", &tex5width, &tex5height, &tex5num, 0);
   GLuint texture[5];
   glGenTextures(5, texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float flatcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   printf("Set parameters successfully\n");
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
   glGenerateMipmap(GL_TEXTURE_2D); 
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2width, tex2height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes2);
   glGenerateMipmap(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex3width, tex3height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes3);
   glGenerateMipmap(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex4width, tex4height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes4);
   glGenerateMipmap(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE4);
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex5width, tex5height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes5);
   glGenerateMipmap(GL_TEXTURE_2D);
   GLuint positionattriblocation = glGetAttribLocation(ShaderProgram, "Position");
   GLuint colorattriblocation = glGetAttribLocation(ShaderProgram, "incolor");
   GLuint textureattriblocation = glGetAttribLocation(ShaderProgram, "Tex");
   GLuint textureorcolorattriblocation = glGetAttribLocation(ShaderProgram, "intextureorcolor");
   printf("Texture set successfully\n");
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   glVertexAttribPointer(textureorcolorattriblocation, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");

   while(!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT); 
      currentframe = glfwGetTime();
      deltatime = currentframe - lastframe;
      lastframe = currentframe;
      glm_vec3_copy(model[3], position);
      glm_vec3_copy(floormodel[3], floorposition);
      /*
      if(positionleftup[0] >= -1.0f && positionleftdown[0] >= -1.0f && positionleftup[0] <= 1.0f && positionleftdown[0] <= 1.0f){
         if(left == 1){
	    //glm_translate(model, (vec3){-1.0f * deltatime, 0.0f, 0.0f});
	    glm_quatv(rotation, currentrotation, (vec3){0.0f, 0.0f, 1.0f});
	    glm_quat_rotate(model, rotation, model);
	    glm_translate_x(floormodel, 1.0f * deltatime);
	    glm_quatv(rotation, -currentrotation, (vec3){0.0f, 0.0f, 1.0f});
	    glm_quat_rotate(model, rotation, model);
	 }
      }
      if(positionrightup[0] <= 1.0f && positionrightdown[0] <= 1.0f && positionrightup[0] >= -1.0f && positionrightdown[0] >= -1.0f){
         if(right == 1){
            //glm_translate(model, (vec3){1.0f * deltatime, 0.0f, 0.0f});
	    glm_quatv(rotation, currentrotation, (vec3){0.0f, 0.0f, 1.0f});
            glm_quat_rotate(model, rotation, model);
            glm_translate_x(floormodel, -1.0f * deltatime);
            glm_quatv(rotation, -currentrotation, (vec3){0.0f, 0.0f, 1.0f});
            glm_quat_rotate(model, rotation, model);
	 }
      }
      */
      if(left == 1 && right == 0 && falling == 0 && leftend == 0 && rightend == 0){
         glm_translate_x(floormodel, 1.0f * deltatime);
	 glm_translate_x(cloudmodel, 1.0f * deltatime);
	 glUniform1i(tex0uni, 2);
      }
      if(right == 1 && left == 0 && falling == 0 && rightend == 0 && leftend == 0){
         glm_translate_x(floormodel, -1.0f * deltatime);
	 glm_translate_x(cloudmodel, -1.0f * deltatime);
	 glUniform1i(tex0uni, 3);
      }
      if(left == 1 && right == 0 && falling == 0 && leftend == 1 || left == 1 && right == 0 && falling == 0 && rightend == 1){
         glm_translate_x(model, -1.0f * deltatime);
	 glUniform1i(tex0uni, 2);
      }
      if(right == 1 && left == 0 && falling == 0 && rightend == 1 || right == 1 && left == 0 && falling == 0 && leftend == 1){
         glm_translate_x(model, 1.0f * deltatime);
	 glUniform1i(tex0uni, 3);
      }
      if(left == 1 && right == 0 && falling == 1){
         glUniform1i(tex0uni, 2);
      }
      if(right == 1 && left == 0 && falling == 1){
         glUniform1i(tex0uni, 3);
      }
      if(left == 1 && right == 1){
         glUniform1i(tex0uni, 0);
      }
      if(right == 0 && left == 0){
         glUniform1i(tex0uni, 0);
      }
      glm_vec3_copy(model[3], position);
      glm_vec3_copy(floormodel[3], floorposition);
      if(floorposition[0] - 2.0f > -0.5f && leftend == 0){
         leftend = 1;
      }
      if(floorposition[0] + 2.0f < 0.5f && rightend == 0){
         rightend = 1;
      }
      if(position[0] > 0.0f && leftend == 1){
         leftend = 0;
         glm_translate_x(model, -position[0]);
      }
      if(position[0] < 0.0f && rightend == 1){
         rightend = 0;
	 glm_translate_x(model, -position[0]);
      }
      if(floorposition[0] + 2.0f < -0.05f && leftend == 0 || floorposition[0] - 2.0f > 0.05f && rightend == 0){
         falling = 1;
      }
      if(position[0] + 0.05f < -0.5f && leftend == 1 || position[0] - 0.05f > 0.5f && rightend == 1){
         falling = 1;
      }
      if(falling == 1){
         glm_translate_y(model, -1.0f * deltatime);
	 //glUniform1i(tex0uni, 0);
      }
      /*
      if(rotateleft == 1){
         angle = glm_rad(60.0f * deltatime);
	 currentrotation -= angle;
         //glm_rotate(model, glm_rad(60.0f * deltatime), (vec3){0.0f, 0.0f, 1.0f});
      }
      if(rotateright == 1){
         angle = glm_rad(-60.0f * deltatime);
	 currentrotation -= angle;
         //glm_rotate(model, glm_rad(60.0f * deltatime), (vec3){0.0f, 0.0f, -1.0f});
      }
      if(currentrotation >= glm_rad(360.0f)){
         currentrotation -= glm_rad(360.0f);
      }
      if(currentrotation < glm_rad(0.0f)){
         currentrotation += glm_rad(360.0f);
      } 
      glm_quatv(rotation, angle, (vec3){0.0f, 0.0f, 1.0f});
      glm_quat_rotate(model, rotation, model);
      angle = glm_rad(0.0f);
      */
      if(jumppressed == 1){
         jump = 1;
      }
      if(jump == 1 && falling == 0){
         glm_translate_y(model, jumpacceleration * deltatime);
	 jumpacceleration += gravity;
	 glm_vec3_copy(model[3], position);
	 if(position[1] <= 0.0f){
            jump = 0;
	    jumpacceleration = 1.0f;
	    glm_translate_y(model, -position[1]);
	 }
      }
      if(position[1] - 0.5f < -1.0f){
         reset = 1;
      }
      if(reset == 1){
         glm_mat4_identity(model);
	 glm_mat4_identity(floormodel);
	 glm_mat4_identity(cloudmodel);
	 jump = 0;
	 jumpacceleration = 1.0f;
	 falling = 0;
	 leftend = 0;
	 rightend = 0;
	 reset = 0;
      }
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &floormodel[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glUniform1i(tex0uni, 4);
      glm_translate_x(cloudmodel, -1.2f);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &cloudmodel[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cloud), cloud, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glm_translate_x(cloudmodel, 1.2f);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &cloudmodel[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cloud), cloud, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glm_translate_x(cloudmodel, 1.2f);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &cloudmodel[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cloud), cloud, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glm_translate_x(cloudmodel, -1.2f);
      if((err = glGetError()) != GL_NO_ERROR){
         printf("OpenGL error: %d\n", err);
      }
      glfwSwapBuffers(window);
   } 
   return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if(key == GLFW_KEY_R && action == GLFW_PRESS){
      glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   }else if(key == GLFW_KEY_G && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);
   }else if(key == GLFW_KEY_B && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);
   }else if(key == GLFW_KEY_A && action == GLFW_PRESS){
      left = 1;
   }else if(key == GLFW_KEY_A && action == GLFW_RELEASE){
      left = 0;
   }else if(key == GLFW_KEY_D && action == GLFW_PRESS){
      right = 1;
   }else if(key == GLFW_KEY_D && action == GLFW_RELEASE){
      right = 0;
   }else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
      rotateleft = 1;
   }else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE){
      rotateleft = 0;
   }else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
      rotateright = 1;
   }else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE){
      rotateright = 0;
   }else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
      jumppressed = 1;
   }else if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
      jumppressed = 0;
   }
}
