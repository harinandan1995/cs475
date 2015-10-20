#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,xcord,ycord,zcord,num_vertices,xtran,ytran,ztran;
extern glm::mat4 inspection_matrix, ortho_matrix;
extern glm::vec4 centroid;
extern void reload_buffer();
extern void save_buffer();
extern void add_vertex();
extern void remove_vertex();
extern void update_centroid();
extern void show_new_window();

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
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && mode == "I") // Left arrow key -> clockwise rotation about y axis
      {
      	yrot = -3.14/18;
      	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
				inspection_matrix = glm::rotate(glm::mat4(1.0f), yrot, glm::vec3(0.0f,1.0f,0.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
      }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && mode == "I") // Right arrow key -> anticlockwise rotation about y axis
      {
      	yrot = 3.14/18;
      	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
				inspection_matrix = glm::rotate(glm::mat4(1.0f), yrot, glm::vec3(0.0f,1.0f,0.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
      }

    else if (key == GLFW_KEY_UP && action == GLFW_PRESS && mode == "I") // Up arrow key -> anticlockwise rotation about x axis
      {
		  	xrot = 3.14/18;
		  	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
				inspection_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
      }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS  && mode == "I") // Down arrow key -> clockwise rotation about x axis
      {
		  	xrot = -3.14/18;
		  	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
				inspection_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
      }
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS  && mode == "I") // Page Up key -> anticlockwise rotation about z axis
      {
		  	zrot = 3.14/18;
		  	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
		  	inspection_matrix = glm::rotate(glm::mat4(1.0f), zrot, glm::vec3(0.0f,0.0f,1.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
      }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS && mode == "I") // Page Down key -> clockwise rotation about z axis
      {
		  	zrot = -3.14/18;
		  	update_centroid();
      	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
		  	inspection_matrix = glm::rotate(glm::mat4(1.0f), zrot, glm::vec3(0.0f,0.0f,1.0f)) * inspection_matrix;
				inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(centroid.x,centroid.y,centroid.z))*inspection_matrix;
		  	
      }
    else if (key == R_KEY_CODE && action == GLFW_PRESS && mode == "I") // move the origin to centroid of the model
      {
		  	update_centroid();
		  	inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-centroid.x,-centroid.y,-centroid.z))*inspection_matrix;
      }
    else if (key == SHIFT_KEY_CODE && action == GLFW_PRESS && mode == "M") //Shift key is pressed
      shift_held = true;
    else if (key == SHIFT_KEY_CODE && action == GLFW_RELEASE && mode == "M") // shift key is released
      shift_held = false;
    else if (key == L_KEY_CODE && action == GLFW_PRESS && mode == "M") { // Load a model 
      reload_buffer();
    }
    else if (key == K_KEY_CODE && action == GLFW_PRESS && mode == "M") { // Save the model
      save_buffer();
    }
    else if (key == M_KEY_CODE && action == GLFW_PRESS && mode == "I") { // change the mode to Modelling
      mode = "M";
      std::cout<<"Current mode : Modelling"<<std::endl;
    }
    else if (key == I_KEY_CODE && action == GLFW_PRESS && mode == "M") { // Change the mode to Inspection
      mode = "I";
      std::cout<<"Current mode : Inspection"<<std::endl;
    }
    else if (key == LSQUARE_KEY_CODE && action == GLFW_PRESS && mode == "M") { // decrease z coordinate while modelling
    	zcord -= 0.1;
    	std::cout << "current zcord : " << zcord << std::endl;
    }
    else if (key == RSQUARE_KEY_CODE && action == GLFW_PRESS && mode == "M") { // increase z coordinate while modelling
    	zcord += 0.1;
    	std::cout << "current zcord : " << zcord << std::endl;
    }
    else if (key == W_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along y axis by 0.2 units
      ytran+=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.2f,0.0f))*inspection_matrix;
    }
    else if (key == A_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along y axis by -0.2 units
      ytran-=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.2f,0.0f))*inspection_matrix;
    }
    else if (key == S_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along x axis by 0.2 units
      xtran+=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.2f,0.0f,0.0f))*inspection_matrix;
    }
    else if (key == D_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along x axis by -0.2 units
      xtran-=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-0.2f,0.0f,0.0f))*inspection_matrix;
    }
    else if (key == Z_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along z axis by 0.2 units
      ztran+=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.2f))*inspection_matrix;
    }
    else if (key == X_KEY_CODE && action == GLFW_PRESS && mode == "I") { // translate along z axis by -0.2 units
      ztran-=0.2;
		  inspection_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-0.2f))*inspection_matrix;
    }
    else if (key == N_KEY_CODE && action == GLFW_PRESS){ // to show a new window so as to draw new object
    	show_new_window();
    	mode = "M";
    }
    
  }
  
  //! To get the coordinates of the cursor (cursor position callback)
  void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
  {
    xcord = xpos;
    ycord = ypos;
  }

  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == "M"){
      if(!shift_held) {
      	add_vertex();
      }
      else {
      	remove_vertex();
      }
    }
  }
};  
	
