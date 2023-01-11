#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
void processInput(GLFWwindow* window);
float vertices[] =  // λ�� // ��ɫ
{
	-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,//����
	0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,//����
	0.0f,0.5,0.0f,0.0f,0.0f,1.0f//��
};
const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos,1.0);\n"
	"ourColor = aColor;\n"
	"}\0";
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = ourColor;\n"
	"}\n\0";
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ǰ�������ð汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//���ú���ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	GLFWwindow* window = glfwCreateWindow(800, 600, "YEYE ZAI XUE OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ү����GLFW����ʧ����" << std::endl;
		glfwTerminate();
		return -1;
	}
	//GLFW�����ڵ�����������Ϊ��ǰ�̵߳�������
	glfwMakeContextCurrent(window);
	// GLAD
	// glad:��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "�������GLAD" << std::endl;
		return -1;
	}

	//�����ͱ�����ɫ������
	//������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//��������� 
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::Shader::VERTEX::COMPILATION_FAIED\n" << infoLog << std::endl;
	}
	//Ƭ����ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//���������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::Shader::FRAGMENT::COMPILATION_FAIED\n" << infoLog << std::endl;
	}
	//��ɫ������
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//���Ӵ�����
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//����VBO��VAO���󣬲�����ID
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��VBO��VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Ϊ��ǰ�󶨵�target�Ļ��������󴴽�һ���µ����ݴ洢��
	// ���data����NULL����ʹ�����Դ�ָ������ݳ�ʼ�����ݴ洢
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��֪Shader��ν��������������ֵ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//����VAI����ĵ�һ������ֵ
	glEnableVertexAttribArray(0);
	//��֪Shader��ν��������������ֵ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*4));
	//����VAI����ĵ�һ������ֵ
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
			glClearColor(0.2, 0.3f, 0.3f, 1.0f);//״̬����
			glClear(GL_COLOR_BUFFER_BIT);//״̬ʹ��
			//glfw:��������������ѯIO�¼�����������/�ͷš�����ƶ���)

			glUseProgram(shaderProgram);
			//����uniformֵ
			//float timeValue = glfwGetTime();
			//float greenValue = sin(timeValue) / 2.0f + 0.5F;
			//float redValue = cos(timeValue) / 2.0f + 0.5F;
			//float blueValue = cos(timeValue+45) / 2.0f + 0.5F;
			int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			//glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
			glBindVertexArray(VAO);
			//��Ⱦ������
			glDrawArrays(GL_TRIANGLES, 0, 3);
			processInput(window);
			glfwSwapBuffers(window);//���󴰿�ϵͳ������window�����ĺ󻺴滭����ָ��û�
			glfwPollEvents();//����GLFW������еȴ�������¼�����Ϣ
	}
	//glfw:����ǰ������GLFW�ȹ���Դ��
	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}