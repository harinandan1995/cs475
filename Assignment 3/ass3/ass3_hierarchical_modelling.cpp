/*
  CSX75 Tutorial 3

  Use the arrow keys and PgUp,PgDn, 
  keys to make the arms move.

  Use the keys 1,2 and 3 to switch between arms.

  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by - 
               Harshavardhan Kode
*/


#include "ass3_hierarchical_modelling.hpp"
#include "texture.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;

GLuint uModelViewMatrix;
const int num_vertices = 36;


//-----------------------------------------------------------------

//Eight vertices in homogenous coordinates
//elongated cuboid , basic arm in the hierarchy;
glm::vec4 positions[8] = {
  glm::vec4(0.0, -0.25, 0.25, 1.0),
  glm::vec4(0.0, 0.25, 0.25, 1.0),
  glm::vec4(2.0, 0.25, 0.25, 1.0),
  glm::vec4(2.0, -0.25, 0.25, 1.0),
  glm::vec4(0.0, -0.25, -0.25, 1.0),
  glm::vec4(0.0, 0.25, -0.25, 1.0),
  glm::vec4(2.0, 0.25, -0.25, 1.0),
  glm::vec4(2.0, -0.25, -0.25, 1.0)
};

//RGBA colors
glm::vec4 colors[8] = {
  glm::vec4(0.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(0.0, 1.0, 1.0, 1.0)
};

int tri_idx=0;
std::vector<std::vector<glm::vec4> > v_positions;
std::vector<std::vector<glm::vec4> > v_colors;
std::vector<std::vector<glm::vec3> > v_normals;
std::vector<std::vector<glm::vec2> > tex_coords;


void load_file(std::string file){
  std::cout << "Loading file : "<< file << std::endl;
  std::vector<glm::vec4> v_positions_temp,v_colors_temp;
  std::vector<glm::vec3> v_normals_temp;
  std::vector<glm::vec2> tex_coords_temp;
  std::string line;
  std::ifstream myfile (file.c_str());
  if (myfile.is_open()){
    while ( getline (myfile,line) ){
      std::stringstream ss(line);
      std::string item;
      double help[11];
      int count = 0;
      while(getline(ss,item,' ')){
        help[count] = stod(item);
        count++;
      }
      glm::vec4 position, color;
      glm::vec3 normal;
      glm::vec2 tex_coord;
      position.x = help[0];
      position.y = help[1];
      position.z = help[2];
      position.w = 1.0;
      color.x = help[3];
      color.y = help[4];
      color.z = help[5];
      color.w = 1.0;
      tex_coord.x = help[6];
      tex_coord.y = help[7];
      normal.x = help[8];
      normal.y = help[9];
      normal.z = help[10];
      normal = glm::normalize(normal);
      
      //std::cout << tex_coord.x << std::endl;
      v_positions_temp.push_back(position);
      v_colors_temp.push_back(color);
      v_normals_temp.push_back(normal);
      tex_coords_temp.push_back(tex_coord);
    }
    myfile.close();
    v_positions.push_back(v_positions_temp);
    v_colors.push_back(v_colors_temp);
    v_normals.push_back(v_normals_temp);
    tex_coords.push_back(tex_coords_temp);
  }
  else std::cout << "Unable to open file"<< std::endl;
}

void load_scene(){
    int num_objects;
    std::cin >> num_objects;
    for(int i=0;i<num_objects;i++){
      std::string file;
      double sx,sy,sz,rx,ry,rz,tx,ty,tz;
      int parent_index;
      std::cin >> file;
      load_file(file);
      std::cin >> parent_index;
      std::cin >> sx >> sy >> sz >> rx >> ry >> rz >> tx >> ty >> tz;
      //glm::mat4(sx,0,0,0, 0,sy,0,0, 0,0,sz,0, 0,0,0,1);
      std::cout << "Loading scene....."<< std::endl;
      csX75::HNode* node;
      if(parent_index == -1) node = new csX75::HNode(NULL,v_positions[i].size(),v_positions[i],v_colors[i],v_normals[i],tex_coords[i],v_positions[i].size()*sizeof(glm::vec4),v_colors[i].size()*sizeof(glm::vec4),v_normals[i].size()*sizeof(glm::vec3),tex_coords[i].size()*sizeof(glm::vec2));
      else node = new csX75::HNode(node_list[parent_index],v_positions[i].size(),v_positions[i],v_colors[i],v_normals[i],tex_coords[i],v_positions[i].size()*sizeof(glm::vec4),v_colors[i].size()*sizeof(glm::vec4),v_normals[i].size()*sizeof(glm::vec3),tex_coords[i].size()*sizeof(glm::vec2));
      node->change_parameters(tx,ty,tz,rx,ry,rz);
      if(i == num_objects-1) curr_node = node;
      node_list.push_back(node);
    }
    //std::cout << tex_coords[0][0].x << std::endl;
}


//-----------------------------------------------------------------

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("ass3_vshader.glsl");
  std::string fragment_shader_file("ass3_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
  vTex = glGetAttribLocation( shaderProgram, "vTex" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
	
	//texture 
	GLuint tex=LoadTexture("images/horse.bmp",284,177 );
  glBindTexture(GL_TEXTURE_2D, tex);
	
  // Creating the hierarchy:
  // We are using the original colorcube function to generate the vertices of the cuboid
  load_scene();

  //note that the buffers are initialized in the respective constructors
 
  /*node1 = new csX75::HNode(NULL,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node2 = new csX75::HNode(node1,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node2->change_parameters(2.0,0.0,0.0,0.0,0.0,0.0);
  node3 = new csX75::HNode(node2,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node3->change_parameters(2.0,0.0,0.0,0.0,0.0,0.0);
  root_node = curr_node = node3;*/

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-14.0, 14.0, -14.0, 14.0, 2.0, 14.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-14.0, 14.0, -14.0, 14.0, -10.0, 10.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);

  node_list[0]->render_tree();

}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(1024, 1024, "CS475/CS675 Assignment 3: Starwars", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

