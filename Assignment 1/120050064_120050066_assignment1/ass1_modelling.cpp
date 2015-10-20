/*
  CSX75 Tutorial 3

  A program which opens a window and draws the "color cube."

  Use the arrow keys and PgUp,PgDn,
  keys to make the cube move.

  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Parag Chaudhuri, 2015
*/


#include "ass1_modelling.hpp"

GLuint shaderProgram;
GLuint vbo[2], vao[2];

glm::mat4 translation_matrix;
glm::mat4 ortho_matrix = glm::ortho(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP, NEAR_PLANE, FAR_PLANE);
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;

//-----------------------------------------------------------------

// To load the given file of data (XYZRGB) 
void reload_buffer(){ 
	std::cout<<"LOAD: Please enter file name...."<<std::endl;
	std::string file_name,line;
	std::cin>>file_name;
	std::ifstream myfile (file_name.c_str());
	if (myfile.is_open())
		{
		v_positions.clear();
		v_colors.clear();
		v_positions_clicked.clear();
		v_colors_clicked.clear();
		clicked_positions.clear();
		clicked_colors.clear();
		while ( getline (myfile,line) )
			{
			std::stringstream ss(line);
			std::string item;
			double help[6];
			int count = 0;
			while(getline(ss,item,' ')){
				help[count] = stod(item);
				count++;
			}
			glm::vec4 position, color;
			position.x = help[0];
			position.y = help[1];
			position.z = help[2];
			position.w = 1.0;
			color.x = help[3];
			color.y = help[4];
			color.z = help[5];
			color.w = 1.0;
			v_positions.push_back(position);
			v_colors.push_back(color);
		}
		myfile.close();
		num_vertices = v_positions.size();

		glBindVertexArray (vao[0]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
		glBufferData (GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4) + v_colors.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions.size()*sizeof(glm::vec4), &v_positions[0] );
		glBufferSubData( GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4), v_colors.size()*sizeof(glm::vec4), &v_colors[0] );
		
		GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size()*sizeof(glm::vec4)) );

		std::cout<<"File loaded "<<std::endl;
		inspection_matrix = glm::mat4(1.0f);
	}
	else std::cout << "Unable to open file"<< std::endl;
}

// To save the model into a file 
void save_buffer(){
	std::cout<<"SAVE: Please enter file name...."<<std::endl;
	std::string s;
	std::cin >> s;
	std::ofstream myfile;
	myfile.open (s,std::ios::trunc);
	for(int i=0; i<v_positions.size(); i++){
		myfile << std::fixed << std::setprecision(2) <<  v_positions[i].x << ' ' << v_positions[i].y << ' ' << v_positions[i].z << ' ';
		myfile << std::fixed << std::setprecision(2) <<  v_colors[i].x << ' ' << v_colors[i].y << ' ' << v_colors[i].z << std::endl;
	}
	for(int i=0; i<v_positions_clicked.size(); i++){
		myfile << std::fixed << std::setprecision(2) <<  v_positions_clicked[i].x << ' ' << v_positions_clicked[i].y << ' ' << v_positions_clicked[i].z << ' ';
		myfile << std::fixed << std::setprecision(2) <<  v_colors_clicked[i].x << ' ' << v_colors_clicked[i].y << ' ' << v_colors_clicked[i].z << std::endl;
	}
	myfile.close(); 
	std::cout<<"File saved as: " << s <<std::endl;
}

//To add vertex by clicking on the window
void add_vertex(){
	double x = (xcord-WINDOW_WIDTH/2)*((WINDOW_RIGHT-WINDOW_LEFT)/WINDOW_WIDTH);
	double y = -1*(ycord-WINDOW_HEIGHT/2)*((WINDOW_TOP-WINDOW_BOTTOM)/WINDOW_HEIGHT);
	glm::vec4 p = glm::vec4(x, y, zcord, 1.0);
	p = glm::inverse(inspection_matrix)*p;
	std::cout << "Vertex added: " << x << ' ' << y << ' ' << zcord << std::endl;
	srand(time(NULL));
	glm::vec4 c = glm::vec4(((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), 1.0);
	clicked_positions.push_back(p);
	clicked_colors.push_back(c);
	v_positions_clicked.clear();
	v_colors_clicked.clear();
	if(clicked_positions.size()>=3){
		num_vertices += 3*clicked_positions.size();
		for(int i=0; i<clicked_positions.size(); i++){
			v_positions_clicked.push_back(clicked_positions[i]);
			v_positions_clicked.push_back(clicked_positions[(i+1)%clicked_positions.size()]);
			v_positions_clicked.push_back(clicked_positions[(i+2)%clicked_positions.size()]);
			v_colors_clicked.push_back(clicked_colors[i]);
			v_colors_clicked.push_back(clicked_colors[(i+1)%clicked_positions.size()]);
			v_colors_clicked.push_back(clicked_colors[(i+2)%clicked_positions.size()]);
		}
		glBindVertexArray (vao[1]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
		glBufferData (GL_ARRAY_BUFFER, v_positions_clicked.size()*sizeof(glm::vec4) + v_colors_clicked.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, v_positions_clicked.size()*sizeof(glm::vec4), &v_positions_clicked[0] );
		glBufferSubData(GL_ARRAY_BUFFER,v_positions_clicked.size()*sizeof(glm::vec4), v_colors_clicked.size()*sizeof(glm::vec4), &v_colors_clicked[0]);
		GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions_clicked.size()*sizeof(glm::vec4)) );
	}
}

// To remove the last added vertex from the model
void remove_vertex(){
	if(clicked_positions.size()==0) {
		std::cout<<"No vertices present" << std::endl;
		return;
	}
	clicked_positions.erase(clicked_positions.end()-1);
	clicked_colors.erase(clicked_colors.end()-1);
	std::cout<<"Vertex removed"<<std::endl;
	v_positions_clicked.clear();
	v_colors_clicked.clear();
	if(clicked_positions.size()>=3){
		num_vertices += 3*clicked_positions.size();
		for(int i=0; i<clicked_positions.size(); i++){
			v_positions_clicked.push_back(clicked_positions[i]);
			v_positions_clicked.push_back(clicked_positions[(i+1)%clicked_positions.size()]);
			v_positions_clicked.push_back(clicked_positions[(i+2)%clicked_positions.size()]);
			v_colors_clicked.push_back(clicked_colors[i]);
			v_colors_clicked.push_back(clicked_colors[(i+1)%clicked_positions.size()]);
			v_colors_clicked.push_back(clicked_colors[(i+2)%clicked_positions.size()]);
		}
		glBindVertexArray (vao[1]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
		glBufferData (GL_ARRAY_BUFFER, v_positions_clicked.size()*sizeof(glm::vec4) + v_colors_clicked.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, v_positions_clicked.size()*sizeof(glm::vec4), &v_positions_clicked[0]);
		glBufferSubData(GL_ARRAY_BUFFER,v_positions_clicked.size()*sizeof(glm::vec4), v_colors_clicked.size()*sizeof(glm::vec4), &v_colors_clicked[0]);
		GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions_clicked.size()*sizeof(glm::vec4)) );
	}
}

// To find the centroid of the model, final output is with respect to the inspecting coordinates
void update_centroid(){
	glm::vec4 sum = glm::vec4 (0.0,0.0,0.0,0.0);
	for(int i=0; i<v_positions.size(); i++){
		sum = sum + v_positions[i];
	}
	for(int i=0; i<v_positions_clicked.size(); i++){
		sum = sum + v_positions_clicked[i];
	}
	if(v_positions_clicked.size() + v_positions.size()){
		centroid.x = sum.x/(v_positions_clicked.size() + v_positions.size());
		centroid.y = sum.y/(v_positions_clicked.size() + v_positions.size());
		centroid.z = sum.z/(v_positions_clicked.size() + v_positions.size());
		centroid.w = 1.0;
	}
	else centroid = glm::vec4(0.0,0.0,0.0,1.0);
	centroid = inspection_matrix * centroid;
}

// To show new window for the new models to be made
void show_new_window(){
	v_positions.clear();
	v_colors.clear();
	v_positions_clicked.clear();
	v_colors_clicked.clear();
	clicked_positions.clear();
	clicked_colors.clear();
	num_vertices = 0;

	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBufferData (GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4) + v_colors.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions.size()*sizeof(glm::vec4), &v_positions[0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4), v_colors.size()*sizeof(glm::vec4), &v_colors[0] );

	GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size()*sizeof(glm::vec4)) );
	
	glBindVertexArray (vao[1]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
	glBufferData (GL_ARRAY_BUFFER, v_positions_clicked.size()*sizeof(glm::vec4) + v_colors_clicked.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions_clicked.size()*sizeof(glm::vec4), &v_positions_clicked[0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions_clicked.size()*sizeof(glm::vec4), v_colors_clicked.size()*sizeof(glm::vec4), &v_colors_clicked[0]);

	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions_clicked.size()*sizeof(glm::vec4)) );
	
	std::cout<<"Current mode : Modelling"<<std::endl;
}

void initBuffersGL(void)
{
	//Ask GL for a Vertex Attribute Object (vao)
	glGenVertexArrays (2, &vao[0]);
	//Set it as the current array to be used by binding it
	glBindVertexArray (vao[0]);

	//Ask GL for a Vertex Buffer Object (vbo)
	glGenBuffers (1, &vbo[0]);
	glBindVertexArray (vao[1]);
	glGenBuffers (1, &vbo[1]);


	//Set it as the current buffer to be used by binding it
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	//glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
	//Copy the points into the current buffer

	glBufferData (GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4) + v_colors.size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions.size()*sizeof(glm::vec4), &v_positions[0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions.size()*sizeof(glm::vec4), v_colors.size()*sizeof(glm::vec4), &v_colors[0] );

	// Load shaders and use the resulting shader program
	std::string vertex_shader_file("ass1_vshader.glsl");
	std::string fragment_shader_file("ass1_fshader.glsl");

	std::vector<GLuint> shaderList;
	shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

	shaderProgram = csX75::CreateProgramGL(shaderList);
	glUseProgram( shaderProgram );

	// set up vertex arrays

	GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size()*sizeof(glm::vec4)) );

	uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
}


void renderGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelview_matrix = ortho_matrix * inspection_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	// Draw 2 objects from 2 different array objects  
  glBindVertexArray (vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, v_positions_clicked.size());
	glBindVertexArray (vao[0]);
  glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment-1: Modelling-Viewing Pipeline", NULL, NULL);
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

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  glfwSetCursorPosCallback(window, csX75::cursor_pos_callback);
  glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  //Initial mode 
  std::cout<<"Current mode : Modelling"<<std::endl;
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

