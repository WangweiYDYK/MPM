#include <windows.h>
#include "vector.h"
#include <vector>
#include "constants.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Border{
public:
    Vector2f normal;
    int type;                                   // [1] sticky - [2] Separating - [3] Sliding
    std::vector<Vector2f> X_corner;
    Border();
    Border(const int inType, const Vector2f& inNormal, const std::vector<Vector2f>& inX_corner);
    ~Border() {};

    //function
    void Collision(const Vector2f& node_coordinates,		// Apply collision with border object
		Vector2f& node_velocity,
		std::vector<int>& collision,
		const int b);

	void Friction(Vector2f& Vi_fri,							// Apply friction with border object
		const Vector2f& Vi_col,
		const Vector2f& Vi);

	void DrawBorder();                                       // Draw border edges

    										

    static std::vector<Border> InitializeBorders()			// Initialize array of borders
	{
		std::vector<Border> outBorders;
		std::vector<Vector2f> Corners;

		/* Left border */
		Corners.push_back(Vector2f(CUB, CUB));
		Corners.push_back(Vector2f(CUB, Y_GRID -  CUB));
		outBorders.push_back(Border(2, Vector2f(1, 0), Corners));
		Corners.clear();
		
		/* Right border */
		Corners.push_back(Vector2f(X_GRID - CUB, CUB));
		Corners.push_back(Vector2f(X_GRID - CUB, Y_GRID - CUB));
		outBorders.push_back(Border(2, Vector2f(-1, 0), Corners));
		Corners.clear();

		/* Bottom border */
		Corners.push_back(Vector2f(CUB, CUB));
		Corners.push_back(Vector2f(X_GRID - CUB, CUB));
		outBorders.push_back(Border(2, Vector2f(0, 1), Corners));
		Corners.clear();

		/* Top border */
		Corners.push_back(Vector2f(CUB, Y_GRID - CUB));
		Corners.push_back(Vector2f(X_GRID - CUB, Y_GRID - CUB));
		outBorders.push_back(Border(2, Vector2f(0, -1), Corners));
		Corners.clear();

		return outBorders;
	}

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in Vector2f aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = Vector2f(apos[0], apos[1]]);\n"
    "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";
};
