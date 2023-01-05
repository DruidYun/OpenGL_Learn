#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
void processInput(GLFWwindow* window);
float vertices[] =
{
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	0.0f,0.5,0.0f
};
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//前两句设置版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	GLFWwindow* window = glfwCreateWindow(800, 600, "YEYE ZAI XUE OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "爷创建GLFW窗口失败了" << std::endl;
		glfwTerminate();
		return -1;
	}
	//GLFW将窗口的上下文设置为当前线程的上下文
	glfwMakeContextCurrent(window);
	//GLAD
	// glad:加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "错误加载GLAD" << std::endl;
		return -1;
	}
	//创建VBO和VAO对象，并赋予ID
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//绑定VBO和VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//为当前绑定到target的缓冲区对象创建一个新的数据存储。
	// 如果data不是NULL，则使用来自此指针的数据初始化数据存储
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//告知Shader如何解析缓冲里的属性值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//开启VAI管理的第一个属性值
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
			glClearColor(0, 0.5f, 1.0f, 1.0f);//状态设置
			glClear(GL_COLOR_BUFFER_BIT);//状态使用
			//glfw:交换缓冲器和轮询IO事件（按键按下/释放、鼠标移动等)
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			processInput(window);
			glfwSwapBuffers(window);//请求窗口系统将参数window关联的后缓存画面呈现给用户
			glfwPollEvents();//告诉GLFW检查所有等待处理的事件和消息
	}
	//glfw:回收前面分配的GLFW先关资源。
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}