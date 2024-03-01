#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <random>
#include<thread>
#include<mutex>
#include <condition_variable>
#include <chrono>
#include <windows.h>

#include"Geometry/RectanglePlane.h"
#include"Geometry/SpherePlane.h"
#include"Geometry/LinePlane.h"

#include "RRT/Config.h"
#include "RRT/RRTBasic.h"
#include "RRT/RRTInformed.h"

#include"Tool/shader.h"

double lastTime = 0.0;
int frameCount = 0;

// 几何结构体
struct Geometry_List
{
	std::vector<Sphere> sphere_list;
	std::vector<RectanglePlane::Rectangle> rectangle_list;
	std::vector<Line> line_list;
};
// 绘制结构体
struct Geo_plane_list
{
	std::vector<SpherePlane> sphere_plane_list;
	std::vector<RectanglePlane::RectanglePlane> rectangle_plane_list;
	std::vector<LinePlane> line_plane_list;
};
// 互斥锁
std::mutex mtx;
std::condition_variable cv;  // 条件变量
int count = 0;
bool inserted = false; bool transed = false; bool renwu = false;
std::string Shader::dirName;
// 错误回调函数
void errorCallback(int error, const char* description);
// 退出回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// 窗口变化监听函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 图元绘制函数
void draw_element(BufferGeometry* element, Shader);
// 窗口尺寸
const GLuint width = 1920, height = 1080;

// 计算线程
void thread1(GLFWwindow* window, glm::vec2 ratio, Geometry_List& geo_list, std::vector<glm::vec2>& geo_line_list);
// 几何体转换OPENGL绘制体
void trans(GLFWwindow* window, Geo_plane_list& plane_list,
	const Geometry_List& geo_list, std::unique_lock<std::mutex>& lock);
// 最终绘制
void lastdraw(GLFWwindow* window, Shader& ourShader,const Geo_plane_list& plane_list, std::unique_lock<std::mutex>& lock);
// 绘制资源销毁
void dispose_draw(Geo_plane_list& plane_list);
// 帧率计算
void calculateFPS(GLFWwindow* window);
// RRT对象指针
//RRTBasic* rrt_pointer;
RRTInformed* rrt_pointer;
Config* rrtconfig;
// 绘制体数量计数
int sphere_num, line_num, rectangle_num;
bool once = false;
int main()
{
	// RRT树
	Tree* testTree=new Tree(glm::vec2(-400, -400));
	glm::vec2 target(400, 400);

	// 获取当前工程路径
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);
	// 着色器文件路径
	//Shader::dirName = "src/05_LineCreate/";
	Shader::dirName = buffer;
	glfwInit();
	if (!glfwInit())
	{
		return -1;
	}
	// 设置错误回调函数
	glfwSetErrorCallback(errorCallback);
	// 配置GLFW以使用OpenGL 3.3核心剖面
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// MSAA抗锯齿
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(width, height, "RRT", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 注册窗口变化监听
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 设置退出回调函数
	glfwSetKeyCallback(window, key_callback);
	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// 视窗比例记录
	glm::vec2 ratio = glm::vec2(2.0f / width, 2.0f / height);
	// 创建着色器
	Shader ourShader("\\shader\\vertex.glsl", "\\shader\\fragment.glsl");
	 //// 线条抗锯齿
	 //glEnable(GL_BLEND);
	 //glEnable(GL_LINE_SMOOTH);
	 //glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST); // Antialias the lines
	 //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 双列表
	Geometry_List geo_list;
	Geo_plane_list plane_list;

	// 线条列表
	std::vector<glm::vec2> geo_line_list;
	// 静态矩形障碍物创建
	RectanglePlane::Rectangle geo_rectangle_one;
	geo_rectangle_one.m_center = glm::vec2(0.0f, 100.0f); geo_rectangle_one.m_width = 700.0f;
	geo_rectangle_one.m_height = 150.0f; geo_rectangle_one.m_ratio = ratio;
	geo_rectangle_one.m_angle = 0.0f;
	RectanglePlane::RectanglePlane rectangle_one(geo_rectangle_one);
	plane_list.rectangle_plane_list.push_back(rectangle_one);
	// RRT参数设置
	std::vector<RectanglePlane::Rectangle>* obstacle_list = new std::vector<RectanglePlane::Rectangle>; //障碍物列表
	obstacle_list->push_back(geo_rectangle_one);
	rrtconfig = new Config(testTree, target, 20.0f, obstacle_list, 2000, 40.0f); //RRT配置参数
	//RRTBasic rrt(rrtconfig);
	RRTInformed rrt(rrtconfig);
	rrt_pointer = &rrt;
	// 路径起终点单独绘制
	Sphere* geo_sphere_root = new Sphere();

	glm::vec2* root_tmp = new glm::vec2(rrt_pointer->_tree->RootNode->m_position.x, rrt_pointer->_tree->RootNode->m_position.y);
	geo_sphere_root->m_center = *root_tmp; geo_sphere_root->m_radius = 5.0f; geo_sphere_root->m_radio = ratio;
	geo_sphere_root->m_color = glm::vec3(1,0,0);
	SpherePlane* sphere_root=new SpherePlane(*geo_sphere_root);
	plane_list.sphere_plane_list.push_back(*sphere_root);

	root_tmp = &(rrtconfig->gotarget);
	geo_sphere_root->m_center = *root_tmp; geo_sphere_root->m_radius = 5.0f; geo_sphere_root->m_radio = ratio;
	geo_sphere_root->m_color = glm::vec3(0, 0, 1);
	sphere_root = new SpherePlane(*geo_sphere_root);
	plane_list.sphere_plane_list.push_back(*sphere_root);
	delete sphere_root, root_tmp, geo_sphere_root;

	// 计算线程绑定
	std::thread thread_first(thread1, window,ratio, std::ref(geo_list), std::ref(geo_line_list));
	// 循环转换与绘制
	while (!glfwWindowShouldClose(window)) {
		{
			std::unique_lock<std::mutex> lock(mtx);
			// 数据转换
			trans(window, std::ref(plane_list), std::ref(geo_list), lock);
		}
		{
			std::unique_lock<std::mutex> lock(mtx);
			// 最终绘制				
			lastdraw(window, ourShader, std::ref(plane_list), lock);
		}
		//calculateFPS(window);
	}
	// 解除链接资源（不再绘制）
	thread_first.join();

	// 解除绘制资源
	dispose_draw(std::ref(plane_list));

	delete testTree;
	delete rrtconfig;

	glfwTerminate();
	return 0;
}

void errorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void draw_element(BufferGeometry* element, Shader shader) {
	// 创建变换矩阵
	glm::mat4 trans = glm::mat4(1.0f);
	shader.setMat4("transform", element->trans);
	element->draw();
}
void thread1(GLFWwindow* window,glm::vec2 ratio, Geometry_List& geo_list, std::vector<glm::vec2>& geo_line_list) {
	while (!glfwWindowShouldClose(window))
	{
		if (!rrt_pointer->_tree->foundPath && count < rrtconfig->m_maxcount) {
			Node* tmp_node = rrt_pointer->AddOneNodeToTree();
			if (tmp_node == nullptr) continue;
			Sphere geo_sphere_tmp1, geo_sphere_tmp2;
			Line geo_line_tmp;

			// 新节点存入线形列表和球体列表
			glm::vec2 tmp = glm::vec2(tmp_node->m_position.x, tmp_node->m_position.y);
			geo_line_list.push_back(tmp);
			geo_sphere_tmp1.m_center = tmp; geo_sphere_tmp1.m_radius = 2.5f; geo_sphere_tmp1.m_radio = ratio;
			// 父节点存入线形列表和球体列表
			tmp = glm::vec2(tmp_node->parent->m_position.x, tmp_node->parent->m_position.y);
			geo_sphere_tmp2.m_center = tmp; geo_sphere_tmp2.m_radius = 2.5f; geo_sphere_tmp2.m_radio = ratio;
			geo_line_list.push_back(tmp);
			geo_line_tmp.m_width = 5; geo_line_tmp.m_color = glm::vec3(0, 1, 1); geo_line_tmp.m_ratio = ratio;

			{
				std::unique_lock<std::mutex> lock(mtx);
				geo_list.sphere_list.push_back(geo_sphere_tmp1);
				geo_list.sphere_list.push_back(geo_sphere_tmp2);
				if (geo_line_list.size() > 1) {
					auto it = geo_line_list.end();
					geo_line_tmp.m_end = *(it - 1);
					geo_line_tmp.m_start = *(it - 2);
					geo_list.line_list.push_back(geo_line_tmp);
				}
			}
			count++;
			//if (count > 1000) renwu = true;
			//inserted = true;
			//cv.notify_one();
			// 等待1秒
			//std::chrono::seconds duration(1);
			//std::chrono::milliseconds duration(1000);
			//std::this_thread::sleep_for(duration);
		}
		else if (rrt_pointer->_tree->foundPath&&!once)
		{
			Line geo_line_smooth;
			Node* self = rrt_pointer->_tree->targetNode;
			geo_line_smooth.m_width = 5; geo_line_smooth.m_color = glm::vec3(1, 0, 0); geo_line_smooth.m_ratio = ratio;
			{
				std::unique_lock<std::mutex> lock(mtx);
				while (self->parent != nullptr)
				{
					geo_line_smooth.m_end = self->m_position;
					geo_line_smooth.m_start = self->parent->m_position;
					geo_list.line_list.push_back(geo_line_smooth);
					self = self->parent;
				}
			}
			//inserted = true;
			once = true;
			//cv.notify_one();
		}
		else {
			//std::cout << "搜索失败" << std::endl;
			//break;
		}
		inserted = true;
		cv.notify_one();

	}
}
void trans(GLFWwindow* window, Geo_plane_list& plane_list,
	const Geometry_List& geo_list, std::unique_lock<std::mutex>& lock) {
	//if (rrt_pointer->_tree->foundPath||!(count< rrtconfig->m_maxcount)) return;
	// 等待子线程通知
	cv.wait(lock, [] { return inserted; });
	if (geo_list.sphere_list.size() != sphere_num) {
		int n = geo_list.sphere_list.size() - sphere_num;
		for (auto i = geo_list.sphere_list.rbegin();i != geo_list.sphere_list.rbegin() + n;i++) {
			SpherePlane* sphere_tmp = new SpherePlane(*i);
			plane_list.sphere_plane_list.push_back(*sphere_tmp);
			delete sphere_tmp;
		}
		sphere_num = geo_list.sphere_list.size();
	}
	if (geo_list.rectangle_list.size() != rectangle_num) {		
		int j = geo_list.rectangle_list.size() - rectangle_num;
		for (auto i = geo_list.rectangle_list.rbegin(); i != geo_list.rectangle_list.rbegin()+j; i++)
		{
			RectanglePlane::RectanglePlane* rectangle_tmp = new RectanglePlane::RectanglePlane(*i);
			plane_list.rectangle_plane_list.push_back(*rectangle_tmp);
			delete rectangle_tmp;
		}
		rectangle_num = geo_list.rectangle_list.size();
	}
	if (geo_list.line_list.size() != line_num) {		
		int k = geo_list.line_list.size() - line_num;
		for (auto i = geo_list.line_list.rbegin(); i != geo_list.line_list.rbegin()+k; i++)
		{
			LinePlane* line_tmp = new LinePlane(*i);
			plane_list.line_plane_list.push_back(*line_tmp);
			delete line_tmp;
		}
		line_num = geo_list.line_list.size();
	}
	transed = true;
}
void lastdraw(GLFWwindow* window, Shader& ourShader,const Geo_plane_list& plane_list, std::unique_lock<std::mutex>& lock) {
	//cv.wait(lock, [] { return transed || rrt_pointer->_tree->foundPath|| !(count < rrtconfig->m_maxcount); });
	cv.wait(lock, [] { return transed; });
	// 事件检查
	glfwPollEvents();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// 设置视口参数
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	// 调用着色器
	ourShader.use();
	RectanglePlane::RectanglePlane tmp=plane_list.rectangle_plane_list[0];
	// 线宽设置
	// glLineWidth(5.0f);
	for (SpherePlane i : plane_list.sphere_plane_list) {
		draw_element(&i, ourShader);
	};
	for (RectanglePlane::RectanglePlane i : plane_list.rectangle_plane_list) {
		draw_element(&i, ourShader);
	};
	for (LinePlane i : plane_list.line_plane_list) {
		draw_element(&i, ourShader);
	};
	glfwSwapBuffers(window);
	inserted = false; transed = false;
}
void dispose_draw(Geo_plane_list& plane_list) {
	for (SpherePlane i : plane_list.sphere_plane_list) i.dispose();
	for (RectanglePlane::RectanglePlane i : plane_list.rectangle_plane_list) i.dispose();
	for (LinePlane i : plane_list.line_plane_list) i.dispose();
}
void calculateFPS(GLFWwindow* window) {
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	frameCount++;

	// 计算帧率
	if (deltaTime >= 1.0) {
		double fps = frameCount / deltaTime;
		std::cout << "FPS: " << fps << std::endl;

		frameCount = 0;
		lastTime = currentTime;
	}

	// 绘制帧率信息，你可以使用OpenGL的绘图函数来实现
	// 例如，使用ImGui库来方便地在窗口中显示文本信息
	// ImGui::Text("FPS: %.2f", fps);
}

