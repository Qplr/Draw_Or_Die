#pragma once
#include "ShapeQueue.h"

class Application
{
	const enum Shapes
	{
		HOR_LINE, VER_LINE,
		SLASH, BACKSLASH,
		U_UP, U_DOWN, U_LEFT, U_RIGHT,
		S_X_NEG_POS, S_X_POS_NEG, S_Y_NEG_POS, S_Y_POS_NEG,
		DELTA_U, DELTA_D, DELTA_L, DELTA_R,
		OMEGA_U, OMEGA_D, OMEGA_L, OMEGA_R,
		M_U, M_D, M_L, M_R,
		TOTAL_SHAPES
	};
	const enum Colors
	{
		RED, GREEN, BLUE, YELLOW, TOTAL_COLORS
	};
	const char* shapesNames[TOTAL_SHAPES] = {
	"Horizontal line", "Vertical line", "Slash", "Backslash",
	"U-Shape (Up)", "U-Shape (Down)", "U-Shape (Left)", "U-Shape (Right)",
	"Horizontal S", "Horizontal Anti-S", "S-Shape", "Anti-S-Shape",
	"Delta (Up)", "Delta (Down)", "Delta (Left)", "Delta (Right)",
	"Omega (Up)", "Omega (Down)", "Omega (Left)", "Omega (Right)",
	"W (Up)", "W (Down)", "W (Left)", "W (Right)"
	};
	Color shapesColors[TOTAL_COLORS] = {
		Color(255, 0, 0),
		Color(0, 255, 0),
		Color(0, 0, 255),
		Color(255, 255, 0)
	};
	vec numbersForShapes[TOTAL_SHAPES][2];
	RenderWindow& m_window;
	vector<vec> m_points;
	bool isDrawing = false;
	const float drawingThickness = 5;
	const clock_t frametime;
	CircleShape brush;
	void drawLine(vec start, vec end);
	float vectorlength(vec v) const { return sqrt(v.x * v.x + v.y * v.y); }
	int recognizeSymbol();
public:
	Application(RenderWindow& window, int fps);
	void handleEvent(Event& event);
	void print();
};