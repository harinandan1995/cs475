#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,xcord,ycord,zcord,num_vertices,xtran,ytran,ztran;
extern glm::mat4 inspection_matrix, ortho_matrix;
extern glm::vec4 centroid;
extern glm::mat4 wcsTOvcs();
extern glm::mat4 vcsTOccs();
extern GLfloat view_mode;

extern void update_centroid();
extern glm::mat4 translate(double, double,double);
extern glm::mat4 rotate(double, double,double);

namespace csX75
{
  int shift_held = false;
  std::string mode = "M";
  

  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {	
  	//std::cout << key << std::endl;
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == NUMERICAL1 && action == GLFW_PRESS && view_mode == 1){
    	std:: cout << "WCS to VCS" << std::endl;
    	inspection_matrix = wcsTOvcs();
    	view_mode = 2;
    }
    else if (key == NUMERICAL2 && action == GLFW_PRESS && view_mode == 2){
    	std:: cout << "VCS to CCS" << std::endl;
    	inspection_matrix = vcsTOccs() * wcsTOvcs();
    	view_mode = 3;
    }
    else if (key == NUMERICAL3 && action == GLFW_PRESS && view_mode == 3){
    	std:: cout << "CCS to NDCS" << std::endl;
    	inspection_matrix = vcsTOccs() * wcsTOvcs();
    	view_mode = 4;
    }
    else if (key == NUMERICAL4 && action == GLFW_PRESS && view_mode == 4){
    	std:: cout << "NDCS to DCS" << std::endl;
    	inspection_matrix = vcsTOccs() * wcsTOvcs();
    	view_mode = 5;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) // Left arrow key -> clockwise rotation about y axis
      {
      	yrot = -3.14/18;
      	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z) * inspection_matrix;
				inspection_matrix = rotate(0,yrot,0) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z) * inspection_matrix;
      }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) // Right arrow key -> anticlockwise rotation about y axis
      {
      	yrot = 3.14/18;
      	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
				inspection_matrix = rotate(0,yrot,0) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z)*inspection_matrix;
      }

    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) // Up arrow key -> anticlockwise rotation about x axis
      {
		  	xrot = 3.14/18;
		  	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
				inspection_matrix = rotate(xrot,0,0) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z)*inspection_matrix;
      }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) // Down arrow key -> clockwise rotation about x axis
      {
		  	xrot = -3.14/18;
		  	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
				inspection_matrix = rotate(xrot,0,0) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z)*inspection_matrix;
      }
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) // Page Up key -> anticlockwise rotation about z axis
      {
		  	zrot = 3.14/18;
		  	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
				inspection_matrix = rotate(0,0,zrot) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z)*inspection_matrix;
      }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) // Page Down key -> clockwise rotation about z axis
      {
		  	zrot = -3.14/18;
		  	update_centroid();
      	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
				inspection_matrix = rotate(0,0,zrot) * inspection_matrix;
				inspection_matrix = translate(centroid.x,centroid.y,centroid.z)*inspection_matrix;
      }
    else if (key == R_KEY_CODE && action == GLFW_PRESS) // move the origin to centroid of the model
      {
		  	update_centroid();
		  	inspection_matrix = translate(-centroid.x,-centroid.y,-centroid.z)*inspection_matrix;
      }
    else if (key == W_KEY_CODE && action == GLFW_PRESS) { // translate along y axis by 0.2 units
		  inspection_matrix = translate(0.0,0.2,0.0)*inspection_matrix;
    }
    else if (key == A_KEY_CODE && action == GLFW_PRESS) { // translate along y axis by -0.2 units
		  inspection_matrix = translate(0.0,-0.2,0.0)*inspection_matrix;
    }
    else if (key == S_KEY_CODE && action == GLFW_PRESS) { // translate along x axis by 0.2 units
		  inspection_matrix = translate(0.2,0.0,0.0)*inspection_matrix;
    }
    else if (key == D_KEY_CODE && action == GLFW_PRESS) { // translate along x axis by -0.2 units
		  inspection_matrix = translate(-0.2,0.0,0.0)*inspection_matrix;
    }
    else if (key == Z_KEY_CODE && action == GLFW_PRESS) { // translate along z axis by 0.2 units
		  inspection_matrix = translate(0.0,0.0,0.2)*inspection_matrix;
    }
    else if (key == X_KEY_CODE && action == GLFW_PRESS) { // translate along z axis by -0.2 units
		  inspection_matrix = translate(0,0,-0.2)*inspection_matrix;
    }
  }
  
  //! To get the coordinates of the cursor (cursor position callback)
  void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
  {

  }

  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {

  }
};  
	
