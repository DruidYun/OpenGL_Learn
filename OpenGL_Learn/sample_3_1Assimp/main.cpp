#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
#include "model.h"
#include "stb_image.h"
#include "camera.h"
//多光源
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
// setting
const int screenWidth = 800;
const int screenHeight = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
//colors
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * objectColor; // = (1.0f, 0.5f, 0.31f);
//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//前两句设置版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//设置核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "XUE OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗口失败了" << std::endl;
		glfwTerminate();
		return -1;
	}
	//GLFW将窗口的上下文设置为当前线程的上下文
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// GLAD
	// glad:加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "错误加载GLAD" << std::endl;
		return -1;
	}
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};
	glm::vec3 pointLightColors[] = {
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.4f, 0.1f, 0.1f)
	};
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);
	Shader ourShader("model_loading.vs", "model_loading.fs");
	Model ourModel("../../resources/objects/nanosuit/nanosuit.obj");
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//帧时间
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// 输入 
		processInput(window);
		// 渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//启动着色器和设置uniform
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 64.0f);
		//光照参数
		// Directional light
		glUniform3f(glGetUniformLocation(ourShader.ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "dirLight.ambient"), 0.1f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "dirLight.diffuse"), 0.2f, 0.2f, 0.2);
		glUniform3f(glGetUniformLocation(ourShader.ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[0].linear"), 0.14);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[0].quadratic"), 0.07);
		// Point light 2
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[1].linear"), 0.14);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[1].quadratic"), 0.07);
		// Point light 3
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[2].linear"), 0.22);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[2].quadratic"), 0.20);
		// Point light 4
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[3].linear"), 0.14);
		glUniform1f(glGetUniformLocation(ourShader.ID, "pointLights[3].quadratic"), 0.07);
		// SpotLight
		glUniform3f(glGetUniformLocation(ourShader.ID, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.ID, "spotLight.linear"), 0.09);
		glUniform1f(glGetUniformLocation(ourShader.ID, "spotLight.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(ourShader.ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(ourShader.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
		/*
		//直射光
		ourShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		ourShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//点光源
		/*
		for (unsigned int i = 0; i < 4; i++)
		{
			std::string str = "pointLights[";
			std::string pointLightIdx = std::to_string(i);
			ourShader.setVec3(str+pointLightIdx+"].position", pointLightPositions[i]);
			ourShader.setVec3(str + pointLightIdx + "].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3(str + pointLightIdx + "].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3(str + pointLightIdx + "].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat(str + pointLightIdx + "].constant", 1.0f);
			ourShader.setFloat(str + pointLightIdx + "].linear", 0.09f);
			ourShader.setFloat(str + pointLightIdx + "].quadratic", 0.032f);
		} 
		
		//聚光灯 手电筒
		ourShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
		ourShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.position", camera.Position);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		*/

		// view / projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.6f)); 
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1f));	
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		glfwSwapBuffers(window);//请求窗口系统将参数window关联的后缓存画面呈现给用户
		glfwPollEvents();//告诉GLFW检查所有等待处理的事件和消息
	}
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
