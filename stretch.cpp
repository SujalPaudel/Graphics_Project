#include <vector>
#include <stdio.h>


#include "./include/glfw_app.hpp"
#include "./include/gl_macros.hpp"
#include "./include/gl_shader.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "glut.h"
#include <math.h>
using namespace lynda;

int main(){
const char * vert  = GLSL(120, 

  attribute vec4 position; // 4 * 4 matrices
  attribute vec4 color;

  varying vec4 dstColor;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main(){
    dstColor = color;
    gl_Position = projection * view * model * position;
  }
);

const char * frag = GLSL(120, 

  varying vec4 dstColor;

  void main(){
    gl_FragColor = dstColor;
  }
);

struct Vertex
{
  glm::vec2 position; //buitin glm vec2 and vec4
  glm::vec4 color;
};

struct MyApp : public App
{
  Shader * shader;
  GLuint positionID, colorID;
  GLuint bufferID;
  GLuint arrayID;
  GLuint modelID, viewID, projectionID;


  MyApp() : App() { init(); }

  void init(){

    Vertex triangle[] = {
      {glm::vec2(-1, 0), glm::vec4(1,0,0,1) },
      {glm::vec2(0, 1), glm::vec4(0,1,0,1) },
      {glm::vec2(1, 0), glm::vec4(0,0,1,1) },

    };

    shader = new Shader(vert, frag);

    positionID = glGetAttribLocation(shader->id(), "position");
    colorID = glGetAttribLocation(shader->id(), "color");

    modelID = glGetUniformLocation(shader->id(), "model");
    viewID = glGetUniformLocation(shader->id(), "view");
    projectionID = glGetUniformLocation(shader->id(), "projection");

    GENVERTEXARRAYS(1, &arrayID);
    BINDVERTEXARRAY(arrayID);

    glGenBuffers(1, &bufferID);

    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), triangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionID);
    glEnableVertexAttribArray(colorID);

    glVertexAttribPointer(positionID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec2));

    BINDVERTEXARRAY(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

  }

  void onDraw(){
    static float time = 0.0;
    time += .01;

    glUseProgram(shader->id());

    BINDVERTEXARRAY(arrayID);

    glm::mat4 view = glm::lookAt(glm::vec3(0,0,2), glm::vec3(0,0,0), glm::vec3(0,1,0) );

    glm::mat4 proj = glm::perspective(PI/3.0f, (float)window().ratio(), 0.1f, -10.0f);

    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(proj));

    for(int i = 0; i < 100; ++i){
      glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(sin(time), 0, (float)i/100));
      glm::mat4 rotate = glm::rotate(glm::mat4(), time * PI * i/100, glm::vec3(0,0,1));
      glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f- (float)i/100));

      glm::mat4 model = translate * rotate * scale;

    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    }
  }
};
// };
}