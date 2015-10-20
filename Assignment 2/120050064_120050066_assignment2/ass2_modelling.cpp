#include "ass2_modelling.hpp"

GLuint shaderProgram;
GLuint vbo[4], vao[4];

glm::mat4 DCS_matrix = glm::mat4(
	(WINDOW_RIGHT-WINDOW_LEFT)/2,0,0,0, 
	0,(WINDOW_TOP-WINDOW_BOTTOM)/2,0,0, 
	0,0,0.5,0, 
	(WINDOW_RIGHT+WINDOW_LEFT)/2,(WINDOW_TOP+WINDOW_BOTTOM)/2,0.5,1
);
glm::mat4 ortho_matrix = glm::ortho(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP, NEAR_PLANE, FAR_PLANE);
glm::mat4 modelview_matrix;
GLuint view_mode_ind, uModelViewMatrix, orthomatrixInd, DCSind;

//-----------------------------------------------------------------

void update_centroid(){
	glm::vec4 sum = glm::vec4 (0.0,0.0,0.0,0.0);
	for(int k=0; k<4; k++){
		for(int i=0; i<v_positions[k].size(); i++){
			sum = sum + scaling_matrix[k] * v_positions[k][i];
		}
	}
	centroid.x = sum.x/sum.w;
	centroid.y = sum.y/sum.w;
	centroid.z = sum.z/sum.w;
	centroid.w = 1.0;
	centroid = inspection_matrix * centroid;
	if (view_mode == 4){
		centroid = glm::vec4(centroid.x/centroid.w, centroid.y/centroid.w, centroid.z/centroid.w, 1);
	}
	if (view_mode == 5){
		centroid = DCS_matrix * glm::vec4(centroid.x/centroid.w, centroid.y/centroid.w, centroid.z/centroid.w, 1);
	}
}

glm::mat4 wcsTOvcs(){
	float tx = glm::dot(uaxis, eye);
	float ty = glm::dot(vaxis, eye);
	float tz = glm::dot(naxis, eye);
	glm::mat4 V = glm::mat4(uaxis.x,vaxis.x,naxis.x,0, uaxis.y,vaxis.y,naxis.y,0, uaxis.z,vaxis.z,naxis.z,0, -tx,-ty,-tz,1);
	return V;
}

glm::mat4 vcsTOccs(){
	glm::mat4 V = glm::mat4(2*N/(R-L),0,0,0, 0,2*N/(T-B),0,0, (R+L)/(R-L),(T+B)/(T-B),-(F+N)/(F-N),-1, 0,0,-2*F*N/(F-N),0);
	return V;
}

glm::mat4 construct_scaling_matrix(double sx,double sy,double sz,double rx,double ry,double rz,double tx,double ty,double tz){
	glm::mat4 s_matrix = glm::mat4(sx,0,0,0, 0,sy,0,0, 0,0,sz,0, 0,0,0,1);
	glm::mat4 rx_matrix = glm::mat4(1,0,0,0, 0,cos(rx),sin(rx),0, 0,-sin(rx),cos(rx),0, 0,0,0,1);
	glm::mat4 ry_matrix = glm::mat4(cos(ry),0,-sin(ry),0, 0,1,0,0, sin(ry),0,cos(ry),0, 0,0,0,1);
	glm::mat4 rz_matrix = glm::mat4(cos(rz),sin(rz),0,0, -sin(rz),cos(rz),0,0, 0,0,1,0, 0,0,0,1);
	glm::mat4 t_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, tx,ty,tz,1);
	
	return t_matrix * rz_matrix * ry_matrix * rx_matrix * s_matrix;
}

glm::mat4 translate(double tx, double ty, double tz){
	return construct_scaling_matrix(1,1,1,0,0,0,tx,ty,tz);
}
glm::mat4 rotate(double rx, double ry, double rz){
	return construct_scaling_matrix(1,1,1,rx,ry,rz,0,0,0);
}

void load_data(std::string file, int i){
	std::string line;
	std::ifstream myfile (file.c_str());
	if (myfile.is_open())
		{
		v_positions[i].clear();
		v_colors[i].clear();
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
			v_positions[i].push_back(position);
			v_colors[i].push_back(color);
		}
		myfile.close();
	}
	else std::cout << "Unable to open file"<< std::endl;
}

void update_frustum(){
	frustum_vertex.push_back(glm::vec4(eye, 1.0));
	frustum_vertex.push_back(glm::vec4(eye + R*uaxis + T*vaxis - N*naxis, 1.0));
	frustum_vertex.push_back(glm::vec4(eye + L*uaxis + T*vaxis - N*naxis, 1.0));
	frustum_vertex.push_back(glm::vec4(eye + L*uaxis + B*vaxis - N*naxis, 1.0));
	frustum_vertex.push_back(glm::vec4(eye + R*uaxis + B*vaxis - N*naxis, 1.0));
	frustum_vertex.push_back(glm::vec4(eye + F/N*(R*uaxis + T*vaxis - N*naxis), 1.0));
	frustum_vertex.push_back(glm::vec4(eye + F/N*(L*uaxis + T*vaxis - N*naxis), 1.0));
	frustum_vertex.push_back(glm::vec4(eye + F/N*(L*uaxis + B*vaxis - N*naxis), 1.0));
	frustum_vertex.push_back(glm::vec4(eye + F/N*(R*uaxis + B*vaxis - N*naxis), 1.0));
	
	glm::vec4 cyan = glm::vec4(0,1,1,1);
	glm::vec4 magenta = glm::vec4(1,0,1,1);
	glm::vec4 red = glm::vec4(1,0,0,1);
	
	v_positions[3].push_back(frustum_vertex[0]); v_colors[3].push_back(red);
	v_positions[3].push_back(frustum_vertex[0]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[1]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[0]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[2]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[0]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[3]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[0]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[4]); v_colors[3].push_back(magenta);
	v_positions[3].push_back(frustum_vertex[1]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[5]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[2]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[6]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[3]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[7]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[4]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[8]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[1]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[2]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[2]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[3]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[3]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[4]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[4]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[1]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[5]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[6]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[6]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[7]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[7]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[8]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[8]); v_colors[3].push_back(cyan);
	v_positions[3].push_back(frustum_vertex[5]); v_colors[3].push_back(cyan);
	
}

void initBuffersGL(void)
{
	//Ask GL for a Vertex Attribute Object (vao)
	glGenVertexArrays (4, &vao[0]);
	//Set it as the current array to be used by binding it
	glBindVertexArray (vao[0]);
	//Ask GL for a Vertex Buffer Object (vbo)
	glGenBuffers (1, &vbo[0]);
	glBindVertexArray (vao[1]);
	glGenBuffers (1, &vbo[1]);
	glBindVertexArray (vao[2]);
	glGenBuffers (1, &vbo[2]);
	glBindVertexArray (vao[3]);
	glGenBuffers (1, &vbo[3]);
	
	
	std::string file;
	scaling_matrix.push_back(glm::mat4(1.0f));
	scaling_matrix.push_back(glm::mat4(1.0f));
	scaling_matrix.push_back(glm::mat4(1.0f));
	scaling_matrix.push_back(glm::mat4(1.0f));

	
	std::cin>>file;
	load_data(file, 0);
	
	std::cin >> sx[0] >> sy[0] >> sz[0];
	std::cin >> rx[0] >> ry[0] >> rz[0];
	std::cin >> tx[0] >> ty[0] >> tz[0];
	scaling_matrix[0] = construct_scaling_matrix(sx[0], sy[0], sz[0], rx[0], ry[0], rz[0], tx[0], ty[0], tz[0]);
	
	std::cin>>file;
	load_data(file, 1);
	
	std::cin >> sx[1] >> sy[1] >> sz[1];
	std::cin >> rx[1] >> ry[1] >> rz[1];
	std::cin >> tx[1] >> ty[1] >> tz[1];
	scaling_matrix[1] = construct_scaling_matrix(sx[1], sy[1], sz[1], rx[1], ry[1], rz[1], tx[1], ty[1], tz[1]);
	
	std::cin>>file;
	load_data(file, 2);
	
	
	std::cin >> sx[2] >> sy[2] >> sz[2];
	std::cin >> rx[2] >> ry[2] >> rz[2];
	std::cin >> tx[2] >> ty[2] >> tz[2];
	scaling_matrix[2] = construct_scaling_matrix(sx[2], sy[2], sz[2], rx[2], ry[2], rz[2], tx[2], ty[2], tz[2]);
	
	std::cin >> ex >> ey >> ez;
	std::cin >> lx >> ly >> lz;
	std::cin >> ux >> uy >> uz;
	std::cin >> L >> R >> T >> B >> N >> F;
	
	eye = glm::vec3(ex, ey, ez);
	lookAt = glm::normalize(glm::vec3(lx, ly, lz));
	upV = glm::normalize(glm::vec3(ux, uy, uz)); 
	
	//std::cout << "ex " << eye.x << std::endl;
	
	naxis = -lookAt;
	uaxis = glm::normalize(glm::cross(upV, naxis));
	vaxis = glm::normalize(glm::cross(naxis, uaxis));
	
	update_frustum();
	
	// Load shaders and use the resulting shader program
	std::string vertex_shader_file("ass2_vshader.glsl");
	std::string fragment_shader_file("ass2_fshader.glsl");

	std::vector<GLuint> shaderList;
	shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

	shaderProgram = csX75::CreateProgramGL(shaderList);
	glUseProgram( shaderProgram );

	uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
	orthomatrixInd = glGetUniformLocation( shaderProgram, "orthoMatrix");
	view_mode_ind = glGetUniformLocation( shaderProgram, "mode");
	DCSind = glGetUniformLocation( shaderProgram, "DCSMatrix");

	// set up vertex arrays

	GLuint vPosition, vColor;
	
	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBufferData (GL_ARRAY_BUFFER, v_positions[0].size()*sizeof(glm::vec4) + v_colors[0].size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions[0].size()*sizeof(glm::vec4), &v_positions[0][0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions[0].size()*sizeof(glm::vec4), v_colors[0].size()*sizeof(glm::vec4), &v_colors[0][0] );
	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions[0].size()*sizeof(glm::vec4)) );
	
	glBindVertexArray (vao[1]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
	glBufferData (GL_ARRAY_BUFFER, v_positions[1].size()*sizeof(glm::vec4) + v_colors[1].size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions[1].size()*sizeof(glm::vec4), &v_positions[1][0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions[1].size()*sizeof(glm::vec4), v_colors[1].size()*sizeof(glm::vec4), &v_colors[1][0] );
	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions[1].size()*sizeof(glm::vec4)) );
	
	glBindVertexArray (vao[2]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[2]);
	glBufferData (GL_ARRAY_BUFFER, v_positions[2].size()*sizeof(glm::vec4) + v_colors[2].size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions[2].size()*sizeof(glm::vec4), &v_positions[2][0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions[2].size()*sizeof(glm::vec4), v_colors[2].size()*sizeof(glm::vec4), &v_colors[2][0] );
	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions[2].size()*sizeof(glm::vec4)) );
	
	glBindVertexArray (vao[3]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[3]);
	glBufferData (GL_ARRAY_BUFFER, v_positions[3].size()*sizeof(glm::vec4) + v_colors[3].size()*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions[3].size()*sizeof(glm::vec4), &v_positions[3][0] );
	glBufferSubData( GL_ARRAY_BUFFER, v_positions[3].size()*sizeof(glm::vec4), v_colors[3].size()*sizeof(glm::vec4), &v_colors[3][0] );
	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions[3].size()*sizeof(glm::vec4)) );
}


void renderGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniform1f(view_mode_ind, view_mode);
	glUniformMatrix4fv(orthomatrixInd, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
	glUniformMatrix4fv(DCSind, 1, GL_FALSE, glm::value_ptr(DCS_matrix));
	
	modelview_matrix = inspection_matrix * scaling_matrix[0];
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
  	glBindVertexArray (vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, v_positions[0].size());
	
	modelview_matrix = inspection_matrix * scaling_matrix[1];
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
	glBindVertexArray (vao[1]);
  	glDrawArrays(GL_TRIANGLES, 0, v_positions[1].size());
  
  	modelview_matrix = inspection_matrix * scaling_matrix[2];
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
	glBindVertexArray (vao[2]);
  	glDrawArrays(GL_TRIANGLES, 0, v_positions[2].size());
  
  	modelview_matrix = inspection_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
	glBindVertexArray (vao[3]);
  	glDrawArrays(GL_POINTS, 0, 1);
  	glDrawArrays(GL_LINES, 1, 32);
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

