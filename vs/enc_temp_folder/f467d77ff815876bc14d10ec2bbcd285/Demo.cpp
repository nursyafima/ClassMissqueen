#include "Demo.h"

Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildTexturedFloor();
	BuildTexturedWall();
	BuildTexturedCeiling();
	BuildTexturedTable();
	BuildTexturedChair();
	BuildTexturedBoard();
	BuildTexturedBoardIn();
	BuildTexturedLamp();
	InitCamera();
}

void Demo::DeInit() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.001f;
		}
	}

	// update camera movement 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		VerticalCamera(-CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		VerticalCamera(CAMERA_SPEED);
	}

	// update camera rotation
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1200;
	angleZ = (float)((midY - mouseY)) / 400;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Set Lighting Attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 10, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 13, 0);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);


	DrawTexturedFloor();
	DrawTexturedWall();
	DrawTexturedCeiling();
	DrawTexturedTable();
	DrawTexturedChair();
	DrawTexturedBoard();
	DrawTexturedBoardIn();
	DrawTexturedLamp();
}

void Demo::BuildTexturedLamp() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("white.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		// front
		0.0, 12.5, 1.0, 0, 0, -1.0f,  -1.0f,  -1.0f, // 0
		1.0, 12.5, 1.0, 1, 0, -1.0f,  -1.0f,  -1.0f, // 1
		1.0, 13.0, 1.0, 1, 1, -1.0f,  -1.0f,  -1.0f,   // 2
		0.0, 13.0, 1.0, 0, 1, -1.0f,  -1.0f,  -1.0f,  // 3
								
		// right				
		1.0, 13.0, 1.0, 0, 0, -1.0f,  -1.0f,  -1.0f, // 4
		1.0, 13.0, 0.0, 1, 0, -1.0f,  -1.0f,  -1.0f, // 5
		1.0, 12.5, 0.0, 1, 1, -1.0f,  -1.0f,  -1.0f,// 6
		1.0, 12.5, 1.0, 0, 1, -1.0f,  -1.0f,  -1.0f,// 7
								
		// back					
		0.0, 12.5, 0.0, 0, 0, -1.0f,  -1.0f,  -1.0f,// 8 
		1.0, 12.5, 0.0, 1, 0, -1.0f,  -1.0f,  -1.0f,// 9
		1.0, 13.0, 0.0, 1, 1, -1.0f,  -1.0f,  -1.0f, // 10
		0.0, 13.0, 0.0, 0, 1, -1.0f,  -1.0f,  -1.0f, // 11
								
		// left					
		0.0, 12.5, 0.0, 0, 0, -1.0f,  -1.0f,  -1.0f,// 12
		0.0, 12.5, 1.0, 1, 0, -1.0f,  -1.0f,  -1.0f,// 13
		0.0, 13.0, 1.0, 1, 1, -1.0f,  -1.0f,  -1.0f, // 14
		0.0, 13.0, 0.0, 0, 1, -1.0f,  -1.0f,  -1.0f, // 15
								
		// upper				
		1.0, 13.0, 1.0, 0, 0, -1.0f,  -1.0f,  -1.0f,  // 16
		0.0, 13.0, 1.0, 1, 0, -1.0f,  -1.0f,  -1.0f, // 17
		0.0, 13.0, 0.0, 1, 1, -1.0f,  -1.0f,  -1.0f, // 18
		1.0, 13.0, 0.0, 0, 1, -1.0f,  -1.0f,  -1.0f,  // 19
								
		// bottom				
		0.0, 12.5, 0.0, 0, 0, -1.0f,  -1.0f,  -1.0f,// 20
		1.0, 12.5, 0.0, 1, 0, -1.0f,  -1.0f,  -1.0f, // 21
		1.0, 12.5, 1.0, 1, 1, -1.0f,  -1.0f,  -1.0f, // 22
		0.0, 12.5, 1.0, 0, 1, -1.0f,  -1.0f,  -1.0f,// 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedLamp()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedFloor()
{
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("tiles.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLfloat vertices[] = {

		-12.0, -0.5, -12.0,  0,  0,	0.0f,  -1.0f,  0.0f,
		 12.0, -0.5, -12.0, 50,  0,	0.0f,  -1.0f,  0.0f,
		 12.0, -0.5,  12.0, 50, 50,	0.0f,  -1.0f,  0.0f,
		-12.0, -0.5,  12.0,  0, 50,	0.0f,  -1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Demo::DrawTexturedFloor()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); 
	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.8f, 0.8f, 0.8f);

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedWall()
{
	 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("3.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	GLfloat vertices[] = {
		
		-12.0, -0.5, -12.0, 0, 0, 0.0f,  -0.4f,  -1.0f,//0
		 12.0, -0.5, -12.0, 5, 0, 0.0f,  -0.4f,  -1.0f,//1
		 12.0, 13.0, -12.0, 5, 3, 0.0f,  -0.4f,  -1.0f,//2
		-12.0, 13.0, -12.0, 0, 3, 0.0f,  -0.4f,  -1.0f,//3
								  
		-12.0, -0.5,  12.0,	0, 0, 0.0f,  -0.4f,  1.0f,//4
		 12.0, -0.5,  12.0,	5, 0, 0.0f,  -0.4f,  1.0f,//5
		 12.0, 13.0,  12.0,	5, 3, 0.0f,  -0.4f,  1.0f,//6
		-12.0, 13.0,  12.0,	0, 3, 0.0f,  -0.4f,  1.0f,//7
								  
		-12.0, -0.5, -12.0, 0, 0, -1.0f,  -0.4f,  0.0f,//8
		-12.0, -0.5,  12.0,	5, 0, -1.0f,  -0.4f,  0.0f,//9
		-12.0, 13.0,  12.0,	5, 3, -1.0f,  -0.4f,  0.0f,//10
		-12.0, 13.0, -12.0, 0, 3, -1.0f,  -0.4f,  0.0f,//11
												
		12.0, -0.5, -12.0,	0, 0,  1.0f,  -0.4f,  0.0f,//12
		12.0, -0.5,  12.0,	5, 0,  1.0f,  -0.4f,  0.0f,//13
		12.0, 13.0,  12.0,	5, 3,  1.0f,  -0.4f,  0.0f,//14
		12.0, 13.0, -12.0,	0, 3,  1.0f,  -0.4f,  0.0f//15
	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3,
		4,  5,  6,  4,  6,  7,
		8,  9,  10, 8,  10, 11,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Demo::DrawTexturedWall()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO3);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedCeiling()
{
 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("ceiling.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	GLfloat vertices[] = {
		
		// bottom
		-12.0, 13.0, -12.0,  0,  0,0.0f,  -1.0f,  0.0f,
		 12.0, 13.0, -12.0, 50,  0,0.0f,  -1.0f,  0.0f,
		 12.0, 13.0,  12.0, 50, 50,0.0f,  -1.0f,  0.0f,
		-12.0, 13.0,  12.0,  0, 50,0.0f,  -1.0f,  0.0f,
	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3

	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);

	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedCeiling()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO4);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedTable()
{
	
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLfloat vertices[] = {
	
		//table top
		// top
		-4.5f,   2.4f,  1.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  // front top left 0
		 4.5f,   2.4f,  1.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  // front top right 1
		 4.5f,   2.4f, -2.5f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  // back top right 2
		-4.5f,   2.4f, -2.5f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  // back top left 3 
		//bottom
		-4.5f,   2.2f,  1.5f, 0.0f, 0.0f, 0.0f, -0.1f,  5.8f,  // front bottom left 4
		 4.5f,   2.2f,  1.5f, 0.0f, 1.0f, 0.0f, -0.1f,  5.8f,  // front bottom right 5
		 4.5f,   2.2f, -2.5f, 1.0f, 1.0f, 0.0f, -0.1f,  5.8f,  // back bottom right 6
		-4.5f,   2.2f, -2.5f, 0.0f, 1.0f, 0.0f, -0.1f,  5.8f,  // back bottom left 7 
		//front
		-4.5f,   2.2f, 1.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f, // front bottom left 8
		 4.5f,   2.2f, 1.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f, // front bottom right 9
		 4.5f,   2.4f, 1.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,// back bottom right 10
		-4.5f,   2.4f, 1.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f, // back bottom left 11 
		//back
		-4.5f,   2.2f, -2.5f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f, // front bottom left 12
		 4.5f,   2.2f, -2.5f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f, // front bottom right 13
		 4.5f,   2.4f, -2.5f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f, // back bottom right 14
		-4.5f,   2.4f, -2.5f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f, // back bottom left 15 
		//right
		 4.5f,   2.2f,  1.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,	// front bottom left 16
		 4.5f,   2.2f, -2.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // front bottom right 17
		 4.5f,   2.4f, -2.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // back bottom right 18
		 4.5f,   2.4f,  1.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// back bottom left 19 
		//left
		-4.5f,   2.2f,  1.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,// front bottom left 20
		-4.5f,   2.2f, -2.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,// front bottom right 21
		-4.5f,   2.4f, -2.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,// back bottom right 22
		-4.5f,   2.4f,  1.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,// back bottom left 23

		//left leg
		//front
		-4.3f,  2.2f, 1.4f,	0.0f, 0.0f, 0.0f,  0.0f,  1.0f,	  // front top left 32
		-4.1f,  2.2f, 1.4f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,   // front top right 33
		-4.1f, -0.5f, 1.4f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,   // back top right 34
		-4.3f, -0.5f, 1.4f, 0.0f, 1.0f,	0.0f,  0.0f,  1.0f,	  // back top left 35
		//back
		-4.3f,  2.2f, -2.4f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  // front top left 36
		-4.1f,  2.2f, -2.4f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  // front top right 37
		-4.1f, -0.5f, -2.4f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  // back top right 38
		-4.3f, -0.5f, -2.4f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  // back top left 39
		//left
		-4.3f,  2.2f,  1.4f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, // front top left 40
		-4.3f,  2.2f, -2.4f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f, // front top right 41
		-4.3f, -0.5f, -2.4f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f, // back top right 42
		-4.3f, -0.5f,  1.4f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, // back top left 43
		//right
		-4.1f,  2.2f,  1.4f, 0.0f, 0.0f, 1.0f,  0.0f,  9.0f,  // front top left 44
		-4.1f,  2.2f, -2.4f, 1.0f, 0.0f, 1.0f,  0.0f,  9.0f,  // front top right 45
		-4.1f, -0.5f, -2.4f, 1.0f, 1.0f, 1.0f,  0.0f,  9.0f,  // back top right 46
		-4.1f, -0.5f,  1.4f, 0.0f, 1.0f, 1.0f,  0.0f,  9.0f,  // back top left 47
		//right leg
		//front
		 4.1f,  2.2f,  1.4f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,   // front bottom left 56
		 4.3f,  2.2f,  1.4f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,   // front bottom right 57
		 4.3f, -0.5f,  1.4f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,   // back bottom right 58
		 4.1f, -0.5f,  1.4f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,   // back bottom left 59
		//back 
		 4.1f,  2.2f, -2.4f,  0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, // front bottom left 60
		 4.3f,  2.2f, -2.4f,  1.0f, 0.0f, 0.0f,  0.0f,  -1.0f, // front bottom right 61
		 4.3f, -0.5f, -2.4f,  1.0f, 1.0f, 0.0f,  0.0f,  -1.0f, // back bottom right 62
		 4.1f, -0.5f, -2.4f,  0.0f, 1.0f, 0.0f,  0.0f,  -1.0f, // back bottom left 63
		//left
		 4.1f,  2.2f,  1.4f,  0.0f, 0.0f,-1.0f,  0.0f,  9.0f,  // front bottom left 64
		 4.1f,  2.2f, -2.4f,  1.0f, 0.0f,-1.0f,  0.0f,  9.0f,  // front bottom right 65
		 4.1f, -0.5f, -2.4f,  1.0f, 1.0f,-1.0f,  0.0f,  9.0f,  // back bottom right 66
		 4.1f, -0.5f,  1.4f,  0.0f, 1.0f,-1.0f,  0.0f,  9.0f,  // back bottom left 67
		//right
		 4.3f,  2.2f,  1.4f,  0.0f, 0.0f,-1.0f,  0.0f,  0.0f,  // front bottom left 68
		 4.3f,  2.2f, -2.4f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,  // front bottom right 69
		 4.3f, -0.5f, -2.4f,  1.0f, 1.0f,-1.0f,  0.0f,  0.0f,  // back bottom right 70
		 4.3f, -0.5f,  1.4f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,  // back bottom left 71
		//front shield
		//top
		-4.1f,  2.2f,  1.4f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,   // front top left 72
		 4.1f,  2.2f,  1.4f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,   // front top right 73
		 4.1f,  2.2f,  1.2f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,   // back top right 74
		-4.1f,  2.2f,  1.2f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,   // back top left 75
		//bottom
		-4.1f,  0.2f,  1.4f,  0.0f, 0.0f, 0.0f, -1.0f,  1.0f,  // front bottom left 76
		 4.1f,  0.2f,  1.4f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,  // front bottom right 77
		 4.1f,  0.2f,  1.2f,  1.0f, 1.0f, 0.0f, -1.0f,  1.0f,  // back bottom right 78
		-4.1f,  0.2f,  1.2f,  0.0f, 1.0f, 0.0f, -1.0f,  1.0f,  // back bottom left 79
		//front
		-4.1f,  2.2f,  1.4f,  0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,   // front bottom left 80
		 4.1f,  2.2f,  1.4f,  1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,   // front bottom right 81
		 4.1f,  0.2f,  1.4f,  1.0f, 1.0f, 0.0f,  0.0f,  -1.0f,   // back bottom right 82
		-4.1f,  0.2f,  1.4f,  0.0f, 1.0f, 0.0f,  0.0f,  -1.0f,   // back bottom left 83
		//back
		-4.1f,  2.2f,  1.2f,  0.0f, 0.0f, 0.0f,  0.0f,   1.0f,     // front bottom left 84
		 4.1f,  2.2f,  1.2f,  1.0f, 0.0f, 0.0f,  0.0f,   1.0f,     // front bottom right 85
		 4.1f,  0.2f,  1.2f,  1.0f, 1.0f, 0.0f,  0.0f,   1.0f,     // back bottom right 86
		-4.1f,  0.2f,  1.2f,  0.0f, 1.0f, 0.0f,  0.0f,   1.0f,     // back bottom left 87
	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3,   // top
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom

		24,	25,	26,	24,	26,	27,  // top
		28,	29,	30,	28,	30,	31,	 // right
		32,	33,	34,	32,	34,	35,	 // back
		36,	38,	37,	36,	39,	38,	 // left
		40,	42,	41,	40,	43,	42,	 // upper
		44,	46,	45,	44,	47,	46,	 // bottom


		48,	49,	50,	48,	50,	51,  // top
		52,	53,	54,	52,	54,	55,	 // right
		56,	57,	58,	56,	58,	59,	 // back
		60,	62,	61,	60,	63,	62,	 // left
		64,	66,	65,	64,	67,	66,	 // upper
		68,	70,	69,	68,	71,	70,	 // bottom


		72,	73,	74,	72,	74,	75,  // top
		76,	77,	78,	76,	78,	79,	 // right
		80,	81,	82, 80,	82,	83,	 // back
		84,	86,	85,	84,	87,	86,	 // left
		88,	90,	89,	88,	91,	90,	 // upper
		92,	94,	93,	92,	95,	94	 // bottom


	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);

	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedTable()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO5);

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedChair()
{
	
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	GLfloat vertices[] = {
		
		//Bagian belakang
		//top
		-1.0f,  4.0f, -4.7f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f, // 0
		 1.0f,  4.0f, -4.7f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f, // 1
		 1.0f,  4.0f, -4.9f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f, // 2
		-1.0f,  4.0f, -4.9f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f, // 3 
		//bottom
		-1.0f,  1.4f, -4.7f, 0.0f, 0.0f,  0.0f,-1.0f,  0.0f, // 4
		 1.0f,  1.4f, -4.7f, 1.0f, 0.0f,  0.0f,-1.0f,  0.0f, // 5
		 1.0f,  1.4f, -4.9f, 1.0f, 1.0f,  0.0f,-1.0f,  0.0f, // 6
		-1.0f,  1.4f, -4.9f, 0.0f, 1.0f,  0.0f,-1.0f,  0.0f, // 7
		//front
		-1.0f,  4.0f, -4.7f, 0.0f, 0.0f,  0.0f, 0.0f,  1.0f, // front top left 8
		 1.0f,  4.0f, -4.7f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f, // front top right 9
		 1.0f,  1.4f, -4.7f, 1.0f, 1.0f,  0.0f, 0.0f,  1.0f, // back top right 10
		-1.0f,  1.4f, -4.7f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f, // back top left 11
		//back
		-1.0f,  4.0f, -4.9f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // front bottom left 12
		 1.0f,  4.0f, -4.9f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // front bottom right 13
		 1.0f,  1.4f, -4.9f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // back bottom right 14
		-1.0f,  1.4f, -4.9f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // back bottom left 15
		//left
		-1.0f,  4.0f, -4.9f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f, // front top left 16
		-1.0f,  4.0f, -4.7f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f, // front top right 17
		-1.0f,  1.4f, -4.7f, 1.0f, 1.0f, -1.0f, 0.0f,  0.0f, // back top right 18
		-1.0f,  1.4f, -4.9f, 0.0f, 1.0f, -1.0f, 0.0f,  0.0f, // back top left 19
		//right
		 1.0f,  4.0f, -4.9f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, // front bottom left 20
		 1.0f,  4.0f, -4.7f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, // front bottom right 21
		 1.0f,  1.4f, -4.7f, 1.0f, 1.0f,  1.0f, 0.0f,  0.0f, // back bottom right 22
		 1.0f,  1.4f, -4.9f, 0.0f, 1.0f,  1.0f, 0.0f,  0.0f, // back bottom left 23

		//Tempat duduk
		//top
	   -1.0f,  1.6f, -2.5f,  0.0f, 0.0f,  0.0f, 1.0f,  0.0f,  // front top left 24
		1.0f,  1.6f, -2.5f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f,  // front top right 25
		1.0f,  1.6f, -4.7f,  1.0f, 1.0f,  0.0f, 1.0f,  0.0f,  // back top right 26
	   -1.0f,  1.6f, -4.7f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f,  // back top left 27
		//bottom
	   -1.0f,  1.4f, -2.5f,  0.0f, 0.0f,  0.0f,-1.0f,  7.0f,  // front bottom left 28
		1.0f,  1.4f, -2.5f,  1.0f, 0.0f,  0.0f,-1.0f,  7.0f,  // front bottom right 29
		1.0f,  1.4f, -4.7f,  1.0f, 1.0f,  0.0f,-1.0f,  7.0f,  // back bottom right 30
	   -1.0f,  1.4f, -4.7f,  0.0f, 1.0f,  0.0f,-1.0f,  7.0f,  // back bottom left 31
		//front
	   -1.0f,  1.6f, -2.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,  // front top left 32
		1.0f,  1.6f, -2.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  // front top right 33
		1.0f,  1.4f, -2.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,  // back top right 34
	   -1.0f,  1.4f, -2.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  // back top left 35
		//back							  
       -1.0f,  1.6f, -4.7f, 0.0f, 0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom left 36
		1.0f,  1.6f, -4.7f,  1.0f,0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom right 37
		1.0f,  1.4f, -4.7f, 1.0f, 1.0f,	0.0f,  0.0f,  -1.0f,	// back bottom right 38
	   -1.0f,  1.4f, -4.7f, 0.0f, 1.0f,	0.0f,  0.0f,  -1.0f,	// back bottom left 39
		//left
		-1.0f,  1.6f, -2.5f,  0.0f, 0.0f,-1.0f,  0.0f,  0.0f,	  // front top left 40
		-1.0f, 1.6f, -4.7f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,	  // front top right 41
		-1.0f, 1.4f, -4.7f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	 // back top right 42
		-1.0f,  1.4f, -2.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,	// back top left 43
		//right
		1.0f, 1.6f, -2.5f, 0.0f, 0.0f,	1.0f,  0.0f,	0.0f,	//	 front bottom left 44
		1.0f, 1.6f, -4.7f,  1.0f,0.0f,	1.0f,  0.0f,	0.0f,	//	 front bottom right 45
		1.0f, 1.4f, -4.7f, 1.0f, 1.0f,	1.0f,  0.0f,	0.0f,	//	 back bottom right 46
		1.0f, 1.4f, -2.5f, 0.0f, 1.0f,	1.0f,  0.0f,	0.0f,	//	 back bottom left 47

		//Kaki kiri BELAKANG
		//top
		-1.0f,  1.4f, -4.7f, 0.0f, 0.0f,	0.0f,  1.0f,  0.0f, 	// front top left 48
		-0.8f, 1.4f, -4.7f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f, 	// front top right 49
		-0.8f, 1.4f, -4.9f, 1.0f, 1.0f,		0.0f,  1.0f,  0.0f, 	// back top right 50
		-1.0f,  1.4f, -4.9f, 0.0f, 1.0f,	0.0f,  1.0f,  0.0f, 	 // back top left 51
		//bottom
		-1.0f, -0.5f, -4.7f, 0.0f, 0.0f,	0.0f,  -1.0f,  0.0f,	// front bottom left 52
		-0.8f, -0.5f, -4.7f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f,	// front bottom right 53
		-0.8f, -0.5f, -4.9f, 1.0f, 1.0f,	0.0f,  -1.0f,  0.0f,	// back bottom right 54
		-1.0f, -0.5f, -4.9f, 1.0f, 0.0f,	0.0f,  -1.0f,  0.0f,	// back bottom left 55
		//front
		-1.0f,  1.4f, -4.7f, 0.0f, 0.0f,	0.0f,  0.0f,	1.0f,  // front top left 56
		-0.8f, 1.4f, -4.7f,  1.0f, 0.0f,	0.0f,  0.0f,	1.0f,   // front top right 57
		-0.8f, -0.5f, -4.7f, 1.0f, 1.0f,	0.0f,  0.0f,	1.0f,   // back top right 58
		-1.0f,  -0.5f, -4.7f, 0.0f, 1.0f,	0.0f,  0.0f,	1.0f,  // back top left 59
		//back
		-1.0f, 1.4f,  -4.9f,  0.0f,  0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom left 60
		-0.8f, 1.4f,  -4.9f,  1.0f, 0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom right 61
		-0.8f, -0.5f, -4.9f, 1.0f, 1.0f,	0.0f,  0.0f,  -1.0f,	// back bottom right 62
		-1.0f, -0.5f, -4.9f, 1.0f, 0.0f,	0.0f,  0.0f,  -1.0f,	// back bottom left 63
		//left
		-1.0f, 1.4f,  -4.9f, 0.0f, 0.0f,	1.0f,  0.0f,  0.0f,	// front top left 64
		-1.0f, 1.4f,  -4.7f, 1.0f, 0.0f,	1.0f,  0.0f,  0.0f,	// front top right 65
		-1.0f, -0.5f, -4.7f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,	// back top right 66
		-1.0f, -0.5f, -4.9f, 0.0f, 1.0f,	1.0f,  0.0f,  0.0f,	// back top left 67
		//right
		-0.8f, 1.4f, -4.7f, 0.0f, 0.0f,		1.0f,  0.0f,	0.0f,	// front bottom left 68
		-0.8f, 1.4f, -4.9f,  1.0f, 0.0f,	1.0f,  0.0f,	0.0f,	// front bottom right 69
		-0.8f, -0.5f, -4.9f, 1.0f, 1.0f,	1.0f,  0.0f,	0.0f,	// back bottom right 70
		-0.8f, -0.5f, -4.7f, 1.0f, 0.0f,	1.0f,  0.0f,	0.0f,	// back bottom left 71

		//Kaki kanan BELAKANG
		//top
		0.8f,  1.4f, -4.7f, 0.0f, 0.0f,0.0f,  1.0f,  0.0f, 		// front top left 72
		1.0f, 1.4f, -4.7f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f, 		 // front top right 73
		1.0f, 1.4f, -4.9f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f, 	// back top right 74
		0.8f,  1.4f, -4.9f, 0.0f, 1.0f,0.0f,  1.0f,  0.0f, 		// back top left 75
		//bottom
		0.8f, -0.5f, -4.7f, 0.0f, 0.0f,0.0f,  -1.0f,  0.0f,		// front bottom left 76
		1.0f, -0.5f, -4.7f,  1.0f,0.0f,0.0f,  -1.0f,  0.0f,		// front bottom right 77
		1.0f, -0.5f, -4.9f, 1.0f, 1.0f,0.0f,  -1.0f,  0.0f,		// back bottom right 78
		0.8f, -0.5f, -4.9f, 0.0f, 1.0f,0.0f,  -1.0f,  0.0f,		// back bottom left 79
		//front
		0.8f,  1.4f, -4.7f, 0.0f, 0.0f,0.0f,  0.0f,	1.0f,		// front top left 80
		1.0f, 1.4f, -4.7f,  1.0f, 0.0f,0.0f,  0.0f,	1.0f,		 // front top right 81
		1.0f, -0.5f, -4.7f, 1.0f, 1.0f,0.0f,  0.0f,	1.0f,		 // back top right 82
		0.8f,  -0.5f, -4.7f, 0.0f, 1.0f,0.0f,  0.0f,1.0f,	// back top left 83
		//back
		0.8f, 1.4f, -4.9f, 0.0f, 0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom left 84
		1.0f, 1.4f, -4.9f,  1.0f,0.0f,	0.0f,  0.0f,  -1.0f,	// front bottom right 85
		1.0f, -0.5f, -4.9f, 1.0f, 1.0f,0.0f,  0.0f,  -1.0f,		 // back bottom right 86
		0.8f, -0.5f, -4.9f, 0.0f, 1.0f,0.0f,  0.0f,  -1.0f,		 // back bottom left 87
		//left
		0.8f,  1.4f, -4.9f, 0.0f, 0.0f,1.0f,  0.0f,  0.0f,		// front top left 88
		0.8f, 1.4f, -4.7f,  1.0f, 0.0f,1.0f,  0.0f,  0.0f,		 // front top right 89
		0.8f, -0.5f, -4.7f, 1.0f, 1.0f,1.0f,  0.0f,  0.0f,		 // back top right 90
		0.8f,  -0.5f, -4.9f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,	// back top left 91
		//right
		1.0f, 1.4f, -4.7f, 0.0f, 0.0f,	1.0f,  0.0f,	0.0f,	// front bottom left 92
		1.0f, 1.4f, -4.9f,  1.0f,0.0f,	1.0f,  0.0f,	0.0f,	// front bottom right 93
		1.0f, -0.5f, -4.9f, 1.0f, 1.0f,1.0f,  0.0f,	0.0f,		 // back bottom right 94
		1.0f, -0.5f, -4.7f, 0.0f, 1.0f,1.0f,  0.0f,	0.0f,		 // back bottom left 95

		//Kaki kiri DEPAN
		-1.0f,  1.4f, -2.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 	// front top left 96
		-0.8f, 1.4f, -2.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 	 // front top right 97
		-0.8f, 1.4f, -2.7f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 	 // back top right 98
		-1.0f, 1.4f, -2.7f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 	// back top left 99

		-1.0f, -0.5f, -2.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,	// front bottom left 100
		-0.8f, -0.5f, -2.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,	 // front bottom right 101
		-0.8f, -0.5f, -2.7f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,	// back bottom right 102
		-1.0f, -0.5f, -2.7f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,	// back bottom left 103

		-1.0f, 1.4f, -2.5f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,	// front top left 104
		-0.8f, 1.4f, -2.5f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f,	 // front top right 105
		-0.8f, -0.5f, -2.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	  // back top right 106
		-1.0f, -0.5f, -2.5f, 0.0f, 1.0f,0.0f, 0.0f,  1.0f, 	 // back top left 107

		-1.0f, 1.4f, -2.7f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,		// front bottom left 108
		-0.8f, 1.4f, -2.7f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,		 // front bottom right 109
		-0.8f, -0.5f, -2.7f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,		 // back bottom right 110
		-1.0f, -0.5f, -2.7f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,		 // back bottom left 111

		-1.0f, 1.4f, -2.7f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		// front top left 112
		-1.0f, 1.4f, -2.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,		 // front top right 113
		-1.0f, -0.5f, -2.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,		  // back top right 114
		-1.0f, -0.5f, -2.7f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 		 // back top left 115

		-0.8f, 1.4f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		// front bottom left 116
		-0.8f, 1.4f, -2.7f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,		 // front bottom right 117
		-0.8f, -0.5f, -2.7f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,		 // back bottom right 118
		-0.8f, -0.5f, -2.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,		 // back bottom left 119

		//Kaki kanan DEPAN
		0.8f,  1.4f, -2.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 		// front top left 120
		1.0f, 1.4f, -2.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 		 // front top right 121
		1.0f, 1.4f, -2.7f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 	// back top right 122
		0.8f, 1.4f, -2.7f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,		// back top left 123

		0.8f, -0.5f, -2.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,		// front bottom left 124
		1.0f, -0.5f, -2.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,		  // front bottom right 125
		1.0f, -0.5f, -2.7f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,		// back bottom right 126
		0.8f, -0.5f, -2.7f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,		// back bottom left 127

		0.8f, 1.4f, -2.5f, 0.0f, 0.0f, 0.0f, 0.0f,   1.0f,		// front top left 128
		1.0f, 1.4f, -2.5f, 1.0f, 0.0f, 0.0f, 0.0f,   1.0f,		 // front top right 129
		1.0f, -0.5f, -2.5f, 1.0f, 1.0f, 0.0f, 0.0f,  1.0f,		 // back top right 130
		0.8f, -0.5f, -2.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f,		// back top left 131

		0.8f, 1.4f, -2.7f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// front bottom left 132
		1.0f, 1.4f, -2.7f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,	  // front bottom right 133
		1.0f, -0.5f, -2.7f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,	 // back bottom right 134
		0.8f, -0.5f, -2.7f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,	 // back bottom left 135

		0.8f, 1.4f, -2.7f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		// front top left 136
		0.8f, 1.4f, -2.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,		 // front top right 137
		0.8f, -0.5f, -2.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,		 // back top right 138
		0.8f, -0.5f, -2.7f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,		// back top left 139

		1.0f, 1.4f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// front bottom left 140
		1.0f, 1.4f, -2.7f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	  // front bottom right 141
		1.0f, -0.5f, -2.7f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	 // back bottom right 142
		1.0f, -0.5f, -2.5f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f	// back bottom left 143

	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		24,    25,    26,    24,    26,    27,
		28,    29,    30,    28,    30,    31,
		32,    33,    34,    32,    34,    35,
		36,    38,    37,    36,    39,    38,
		40,    42,    41,    40,    43,    42,
		44,    46,    45,    44,    47,    46,

		48,    49,    50,    48,    50,    51,
		52,    53,    54,    52,    54,    55,
		56,    57,    58,    56,    58,    59,
		60,    62,    61,    60,    63,    62,
		64,    66,    65,    64,    67,    66,
		68,    70,    69,    68,    71,    70,

		72,    73,74,72,74,75,
		76,    77,78,76,78,79,
		80,    81,82,80,82,83,
		84,   86,85,84,87,86,
		88,90,89,88,91,90,
		92,94,93,92,95,94,

		96,	97,	98,	96,	98,	99,
		100,	101,	102,	100,	102,	103,
		104,	105,	106,	104,	106,	107,
		108,	110,	109,	108,	111,	110,
		112,	114,	113,	112,	115,	114,
		116,	118,	117,	116,	119,	118,


		120,	121,	122,	120,	122,	123,
		124,	125,	126,	124,	126,	127,
		128,	129,	130,	128,	130,	131,
		132,	134,	133,	132,	135,	134,
		136,	138,	137,	136,	139,	138,
		140,	142,	141,	140,	143,	142

	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);

	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedChair()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "material.diffuse"), 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 1.0f);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedBoard()
{
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//Bagian belakang
		//Papan Tulis


		//Pinggir Kiri
		//front
		-7.8f, 3.7f, 11.7f, 0.0f, 0.0f, 0.0f,  0.0f,	1.0f,   // back top right 10
		-7.5f, 4.0f, 11.7f, 1.0f, 0.0f, 0.0f,  0.0f,	1.0f, // back top left 11
		-7.5f, 9.0f, 11.7f, 1.0f, 1.0f, 0.0f,  0.0f,	1.0f,   // front top right 9
		-7.8f, 9.3f, 11.7f, 0.0f, 1.0f, 0.0f,  0.0f,	1.0f,  // front top left 8

		//right
		-7.5f, 9.0f, 11.7f, 0.0f, 0.0f, 1.0f,  0.0f,	0.0f, // front bottom left 12
		-7.5f, 9.0f, 12.0f, 1.0f, 0.0f, 1.0f,  0.0f,	0.0f, // front bottom right 13
		-7.5f, 4.0f, 12.0f, 1.0f, 1.0f, 1.0f,  0.0f,	0.0f, // back bottom right 14
		-7.5f, 4.0f, 11.7f, 0.0f, 1.0f, 1.0f,  0.0f,	0.0f, // back bottom left 15 

		//left
		-7.8f, 9.3f, 12.0f, 0.0f, 0.0f, -1.0f,  0.0f,	0.0f,   // front top right 9
		-7.8f, 9.3f, 11.7f, 1.0f, 0.0f, -1.0f,  0.0f,	0.0f,  // front top left 8
		-7.8f, 3.7f, 11.7f, 1.0f, 1.0f, -1.0f,  0.0f,	0.0f,   // back top right 10
		-7.8f, 3.7f, 12.0f, 0.0f, 1.0f, -1.0f,  0.0f,	0.0f, // back top left 11

		////top
		//5.4f, 9.0f, -7.7f, 0.0f, 0.0f,   // front top right 9
		//5.1f, 9.3f, -7.7f, 1.0f, 0.0f,  // front top left 8
		//5.1f, 9.3f, -8.0f, 1.0f, 1.0f, // back top left 11 
		//5.4f, 9.0f, -8.0f, 0.0f, 1.0f,   // back top right 10

		////bottom
		//5.1f, 3.7f, -7.7f, 0.0f, 0.0f, // front bottom left 12
		//5.4f, 4.0f, -7.7f, 1.0f, 0.0f, // front bottom right 13
		//5.4f, 4.0f, -8.0f, 1.0f, 1.0f, // back bottom right 14
		//5.1f, 3.7f, -8.0f, 0.0f, 1.0f, // back bottom left 15 

		//Pinggir Kanan
		//front
		7.5f, 4.0f, 11.7f, 0.0f, 0.0f,0.0f,  0.0f,	1.0f,   // back top right 10
		7.8f, 3.7f, 11.7f, 1.0f, 0.0f,0.0f,  0.0f,	1.0f, // back top left 11
		7.8f, 9.3f, 11.7f, 1.0f, 1.0f,0.0f,  0.0f,	1.0f,   // front top right 9
		7.5f, 9.0f, 11.7f, 0.0f, 1.0f,0.0f,  0.0f,	1.0f,  // front top left 8

		//right
		7.8f, 9.3f, 11.7f, 0.0f, 0.0f,1.0f,  0.0f,	0.0f,  // front top left 16
		7.8f, 9.3f, 12.0f, 1.0f, 0.0f,1.0f,  0.0f,	0.0f,   // front top right 17
		7.8f, 3.7f, 12.0f, 1.0f, 1.0f,1.0f,  0.0f,	0.0f, // back top left 19
		7.8f, 3.7f, 11.7f, 0.0f, 1.0f,1.0f,  0.0f,	0.0f,   // back top right 18 

		//left
		7.5f, 4.0f, 12.0f, 0.0f, 0.0f,-1.0f,  0.0f,0.0f, // back top left 19
		7.5f, 4.0f, 11.7f, 1.0f, 0.0f,-1.0f,  0.0f,0.0f,   // back top right 18
		7.5f, 9.0f, 11.7f, 1.0f, 1.0f,-1.0f,  0.0f,0.0f,  // front top left 16
		7.5f, 9.0f, 12.0f, 0.0f, 1.0f,-1.0f,  0.0f,0.0f,   // front top right 17

		////top
		//20.4f, 9.0f, -7.7f, 0.0f, 0.0f,  // front top left 16
		//20.7f, 9.3f, -7.7f, 1.0f, 0.0f,   // front top right 17
		//20.7f, 9.3f, -8.0f, 1.0f, 1.0f,   // back top right 18
		//20.4f, 9.0f, -8.0f, 0.0f, 1.0f, // back top left 19 

		////bottom
		//20.4f, 4.0f, -8.0f, 0.0f, 0.0f, // back bottom left 23
		//20.7f, 3.7f, -8.0f, 1.0f, 0.0f, // back bottom right 22
		//20.7f, 3.7f, -7.7f, 1.0f, 1.0f, // front bottom right 21
		//20.4f, 4.0f, -7.7f, 0.0f, 1.0f, // front bottom left 20

		//Pinggir Atas
		//front
		-7.5f, 9.0f, 11.7f, 0.0f, 0.0f,0.0f,  0.0f,	  1.0f,	  // front top left 24
		7.5f, 9.0f, 11.7f, 1.0f, 0.0f, 0.0f,  0.0f,	  1.0f,	// back top left 27 
		7.8f, 9.3f, 11.7f, 1.0f, 1.0f, 0.0f,  0.0f,	  1.0f,	  // back top right 26
		-7.8f, 9.3f, 11.7f, 0.0f, 1.0f,0.0f,  0.0f,	  1.0f,	   // front top right 25

		//top
		7.8f, 9.3f, 11.7f, 0.0f, 0.0f,  0.0f,  0.5f,  0.0f,  // front top right 25
		-7.8f,  9.3f, 11.7f, 1.0f, 0.0f, 0.0f, 0.5f,  0.0f, // front top left 24
		-7.8f,  9.3f, 12.0f, 1.0f, 1.0f, 0.0f, 0.5f,  0.0f, // back top left 27 
		7.8f, 9.3f, 12.0f, 0.0f, 1.0f,  0.0f,  0.5f,  0.0f,  // back top right 26

		//bottom
		-7.5f, 9.0f, 12.0f, 0.0f, 0.0f,0.0f,  -1.0f,  0.0f, // back bottom left 31
		7.5f, 9.0f, 12.0f, 1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,// back bottom right 30
		7.5f, 9.0f, 11.7f, 1.0f, 1.0f, 0.0f,  -1.0f,  0.0f,// front bottom right 29
		-7.5f, 9.0f, 11.7f, 0.0f, 1.0f,0.0f,  -1.0f,  0.0f, // front bottom left 28

		//Pinggir Bawah
		//front
		-7.8f, 3.7f, 11.7f, 0.0f, 0.0f,0.0f,  0.0f,1.0f,	   // front top right 25
		7.8f, 3.7f, 11.7f, 1.0f, 0.0f, 0.0f,  0.0f,1.0f,	// back top left 27
		7.5f, 4.0f, 11.7f, 1.0f, 1.0f, 0.0f,  0.0f,1.0f,	  // back top right 26
		-7.5f, 4.0f, 11.7f, 0.0f, 1.0f,0.0f,  0.0f,1.0f,	  // front top left 24

		//top
		7.5f, 4.0f, 11.7f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  // front top right 33
		-7.5f,  4.0f, 11.7f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  // front top left 32
		-7.5f,  4.0f, 12.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f, // back top left 35 
		7.5f, 4.0f, 12.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  // back top right 34

		//bottom
		-7.8f, 3.7f, 12.0f, 0.0f, 0.0f,0.0f,  -1.0f,  0.0f, // back bottom left 39
		7.8f, 3.7f, 12.0f, 1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,// back bottom right 38
		7.8f, 3.7f, 11.7f, 1.0f, 1.0f, 0.0f,  -1.0f,  0.0f,// front bottom right 37
		-7.8f, 3.7f, 11.7f, 0.0f, 1.0f ,0.0f,  -1.0f,  0.0f

	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		24,    25,    26,    24,    26,    27,
		28,    29,    30,    28,    30,    31,
		32,    33,    34,    32,    34,    35,
		36,    38,    37,    36,    39,    38,
		40,    42,    41,    40,    43,    42,
		44,    46,    45,    44,    47,    46,

		48,    49,    50,    48,    50,    51,
		52,    53,    54,    52,    54,    55,
		56,    57,    58,    56,    58,    59,
		60,    62,    61,    60,    63,    62,
		64,    66,    65,    64,    67,    66,
		68,    70,    69,    68,    71,    70,

		72,73,74,72,74,75,
		76,77,78,76,78,79,
		80,81,82,80,82,83,
		84,86,85,84,87,86,
		88,90,89,88,91,90,
		92,94,93,92,95,94,

		96,	97,	98,	96,	98,	99,
		100,	101,	102,	100,	102,	103,
		104,	105,	106,	104,	106,	107,
		108,	110,	109,	108,	111,	110,
		112,	114,	113,	112,	115,	114,
		116,	118,	117,	116,	119,	118,


		120,	121,	122,	120,	122,	123,
		124,	125,	126,	124,	126,	127,
		128,	129,	130,	128,	130,	131,
		132,	134,	133,	132,	135,	134,
		136,	138,	137,	136,	139,	138,
		140,	142,	141,	140,	143,	142

	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);

	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedBoardIn()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedBoardIn()
{
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("board.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//Bagian belakang
		//Papan Tulis
		-7.5f, 4.0f, 11.8f, 0.0f, 0.0f,0.0f,  0.0f,	1.0f, // back bottom right 6
		7.5f, 4.0f, 11.8f, 1.0f, 0.0f, 0.0f,  0.0f,	1.0f,// back bottom left 7
		7.5f, 9.0f, 11.8f, 1.0f, 1.0f, 0.0f,  0.0f,	1.0f,// front bottom right 5
		-7.5f, 9.0f, 11.8f, 0.0f, 1.0f,0.0f,  0.0f,	1.0f, // front bottom left 4
	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3
	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);

	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedBoard()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.5f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;

	// forward positive cameraspeed and backward negative -cameraspeed.

	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;

}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::VerticalCamera(float speed) {
	float y = viewCamY - posCamY;


	posCamY = posCamY + y * speed;
	viewCamY = viewCamY + y * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	
	Demo().Start("Project Grafkom: Class Missqueen", 1920, 1080, false, true);
}