#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
void processInput(GLFWwindow* window);
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
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
			glClearColor(0, 0.5f, 1.0f, 1.0f);//状态设置
			glClear(GL_COLOR_BUFFER_BIT);//状态使用
			//glfw:交换缓冲器和轮询IO事件（按键按下/释放、鼠标移动等)
			processInput(window);
			glfwSwapBuffers(window);
			glfwPollEvents();
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