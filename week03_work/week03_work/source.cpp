#include <glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <math.h>

const int width = 640;
const int height = 480;

float* pixels = new float[width*height * 3];
void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.
// j0 =< j1
// i0 =< i1
void drawLine(const int& i0,const int& j0,const int& i1,const int& j1, const float& red, const float& green, const float& blue)
{
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}

// x0 =< x1
// y0 =< y1
// filled: false->only border line; true->filled with colour
void drawSquare(const int& x0, const int& x1, const int& y0, const int& y1, const float& red, const float& green, const float& blue, bool filled)
{
	if (!filled)
	{
		for (int x = x0; x <= x1; x++)
		{
			drawPixel(x, y0, red, green, blue);
			drawPixel(x, y1, red, green, blue);
		}

		for (int y = y0; y <= y1; y++)
		{
			drawPixel(x0, y, red, green, blue);
			drawPixel(x1, y, red, green, blue);
		}
	}

	else
	{
		for (int x = x0; x <= x1; x++)
		{
			for (int y = y0; y <= y1; y++)
			{
				drawPixel(x, y, red, green, blue);
			}
		}
	}
}

// x0 =< x1
// y0 =< y1
void drawTriangle(const int& x0, const int& x1, const int& y0, const int& y1, const float& red, const float& green, const float& blue)
{
	const int half_x = (x1 + x0) / 2;
	for (int x = x0; x <= x1; x++)
	{
		drawPixel(x, y0, red, green, blue);
	}

	drawLine(x0, y0, half_x, y1, red, green, blue);
	drawLine(half_x, y1, x1, y0, red, green, blue);
}

void drawCircle(const int& x, const int& y, const int& r, const float& red, const float& green, const float& blue)
{
	float r_square = r * r;

	for (int i = -r; i <= 0; i++)
	{
		drawPixel(i + x, sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(i + x, -1 * sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(sqrt(r_square - i *i) + x, i + y, red, green, blue);
		drawPixel(-1 * sqrt(r_square - i *i) + x, i + y, red, green, blue);
	}

	for (int i = r; i >= 0; i--)
	{
		drawPixel(i + x, sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(i + x, -1 * sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(sqrt(r_square - i *i) + x, i + y, red, green, blue);
		drawPixel(-1 * sqrt(r_square - i *i) + x, i + y, red, green, blue);
	}
}

void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

	//const int i = rand() % width, j = rand() % height;
	//drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing
	int i0 = 80, i1 = 160;
	int j0 = 360, j1 = 440;
	for (int i = 0; i < 3; i++)
	{
		drawLine(i0 + i, j0, i1 + i, j1, 0.0f, 0.0f, 0.0f);
	}
	
	drawSquare(260, 340, 360, 440, 0.0f, 0.0f, 0.0f, false);

	drawSquare(460, 540, 360, 440, 0.0f, 1.0f, 0.0f, true);

	drawTriangle(80, 160, 160, 240, 0.0f, 0.0f, 0.0f);

	drawCircle(500, 100, 60, 0.0f, 0.0f, 0.0f);

	//TODO: try moving object

}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}

