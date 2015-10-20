#ifndef _GL_FRAMEWORK_HPP_
#define _GL_FRAMEWORK_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <vector>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define SHIFT_KEY_CODE 340
#define W_KEY_CODE 87
#define S_KEY_CODE 83
#define A_KEY_CODE 65
#define D_KEY_CODE 68
#define Z_KEY_CODE 90
#define X_KEY_CODE 88
#define R_KEY_CODE 82

#define N_KEY_CODE 78

#define L_KEY_CODE 76
#define I_KEY_CODE 73
#define M_KEY_CODE 77
#define K_KEY_CODE 75
#define LSQUARE_KEY_CODE 91
#define RSQUARE_KEY_CODE 93

#define NUMERICAL1 49
#define NUMERICAL2 50
#define NUMERICAL3 51
#define NUMERICAL4 52



namespace csX75
{
  //! Initialize GL State
  void initGL(void);
 
  //!GLFW Error Callback
  void error_callback(int error, const char* description);
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  //!GLFW cursor position callback
  void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
  //!GLFW mouse click callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

#endif
