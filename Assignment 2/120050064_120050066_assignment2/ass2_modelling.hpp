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
#define WINDOW_LEFT -20.0
#define WINDOW_RIGHT 20.0
#define WINDOW_BOTTOM -20.0
#define WINDOW_TOP 20.0
#define NEAR_PLANE -20.0
#define FAR_PLANE 20.0


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
#include <math.h>


glm::mat4 inspection_matrix = glm::mat4(1.0f);
GLfloat view_mode = 1;
std::vector<glm::mat4> scaling_matrix;


GLfloat xrot=0.0,yrot=0.0,zrot=0.0;
glm::vec4 centroid = glm::vec4(0,0,0,1);

//Number of vertices

void update_centroid();

//-----------------------------------------------------------------

std::vector<glm::vec4> v_positions[4], v_colors[4], frustum_vertex;
double sx[3], sy[3], sz[3], rx[3], ry[3], rz[3], tx[3], ty[3], tz[3];
float ex, ey, ez, lx, ly, lz, ux, uy, uz, L, R, T, B, N, F;
glm::vec3 eye, lookAt, upV, uaxis, vaxis, naxis;


//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;

//-------------------------------------------------------------------------

#endif
