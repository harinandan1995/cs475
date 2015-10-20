/*
  A program which opens a window and draws the "color cube."

  Use the arrow keys and PgUp,PgDn, 
  keys to make the cube move.

  Written by - 
               Parag Chaudhuri
*/
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#define WINDOW_HEIGHT 512.0
#define WINDOW_WIDTH 512.0
#define WINDOW_LEFT -10.0
#define WINDOW_RIGHT 10.0
#define WINDOW_BOTTOM -10.0
#define WINDOW_TOP 10.0
#define NEAR_PLANE -15.0
#define FAR_PLANE 15.0


#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iomanip>


// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
glm::mat4 inspection_matrix = glm::mat4(1.0f);
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;
glm::vec4 centroid = glm::vec4(0,0,0,1);
GLfloat xtran=0.0,ytran=0.0,ztran=0.0;

GLfloat xcord, ycord, zcord = 0.0;

//Number of vertices
int num_vertices;

void reload_buffer();
void save_buffer();
void add_vertex();
void remove_vertex();
void show_new_window();

//-----------------------------------------------------------------

std::vector<glm::vec4> v_positions, v_colors, v_positions_clicked, v_colors_clicked, clicked_positions, clicked_colors;


//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;

//-------------------------------------------------------------------------

#endif
