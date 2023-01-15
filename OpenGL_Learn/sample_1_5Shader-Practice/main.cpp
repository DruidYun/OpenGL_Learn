#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
void processInput(GLFWwindow* window);
float vertices[] =  // λ�� // ��ɫ
{
	-0.4330125f,-0.25f,0.0f,1.0f,0.0f,0.0f,//����
	0.4330125f,-0.25f,0.0f,0.0f,1.0f,0.0f,//����
	0.0f,0.5,0.0f,0.0f,0.0f,1.0f//��
};
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
		std::cout << "����GLFW����ʧ����" << std::endl;
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
	Shader ourShader("shader.vs", "shader.fs");
	
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
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
			glClearColor(0.2, 0.3f, 0.3f, 1.0f);//״̬����
			glClear(GL_COLOR_BUFFER_BIT);//״̬ʹ��
			//glfw:��������������ѯIO�¼�����������/�ͷš�����ƶ���)		
			//glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
			glBindVertexArray(VAO); 
			float timeValue = glfwGetTime();
			ourShader.setFloat("xOffset", timeValue);
			//��Ⱦ������
			glDrawArrays(GL_TRIANGLES, 0, 3);
			processInput(window);
			ourShader.use();
			glfwSwapBuffers(window);//���󴰿�ϵͳ������window�����ĺ󻺴滭����ָ��û�
			glfwPollEvents();//����GLFW������еȴ�������¼�����Ϣ
	}
	//glfw:����ǰ������GLFW�ȹ���Դ��
	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}