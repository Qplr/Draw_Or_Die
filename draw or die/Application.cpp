#include "Application.h"

Application::Application(RenderWindow& window, int fps): m_window(window), frametime(CLOCKS_PER_SEC / fps)
{
	brush.setRadius(drawingThickness);
	brush.setOrigin(drawingThickness / 2, drawingThickness / 2);
	ifstream file(CONFIG_FILE_PATH);
	int beg, end;
	vec tempVec;
	string temp;
	if (file.is_open())
	{
		for (int i = 0; i < TOTAL_SHAPES; i++)
		{
			getline(file, temp);
			file >> ws;
			beg = 0;
			end = 0;
			for (int j = 0; j < 2; j++)
			{
				beg = temp.find_first_of(DIGITS, end);
				end = temp.find_first_not_of(DIGITS, beg);
				tempVec.x = stoi(temp.substr(beg, end));
				beg = temp.find_first_of(DIGITS, end);
				end = temp.find_first_not_of(DIGITS, beg);
				tempVec.y = stoi(temp.substr(beg, end));
				numbersForShapes[i][j] = tempVec;
			}
		}
		file.close();
	}
}

void  Application::drawLine(vec start, vec end)
{
	bool steep = false;
	int startX = start.x, startY = start.y, endX = end.x, endY = end.y;
	int deltaX = abs(startX - endX), deltaY = abs(startY - endY), delta;
	int stepX = -1, stepY = -1;

	if (endX - startX > 0)
		stepX *= -1;
	if (endY - startY > 0)
		stepY *= -1;
	if (deltaY > deltaX)
	{
		steep = true;
		swap(startX, startY);
		swap(deltaX, deltaY);
		swap(stepX, stepY);
	}
	delta = deltaY * 2 - deltaX;
	for (int i = 0; i <= deltaX; i++)
	{
		if (steep)
			brush.setPosition(Vector2f(startY, startX));
		else
			brush.setPosition(Vector2f(startX, startY));
		m_window.draw(brush);
		while (delta >= 0)
		{
			startY += stepY;
			delta -= deltaX * 2;
		}
		startX += stepX;
		delta += deltaY * 2;
	}
	brush.setPosition(Vector2f(endX, endY));
	m_window.draw(brush);
}

void addBit(int& i, bool bit)
{
	i <<= 1;
	i += bit;
}

int Application::recognizeSymbol()
{
	if (m_points.size() < 3)
		return -1;
	vector<vec> points = m_points;
	vec shape;
	bool intervalX = false, intervalY = false;
	float avgX = 0, avgY = 0;
	for (int i = 0; i < points.size(); i++)
	{
		avgX += points.at(i).x;
		avgY += points.at(i).y;
	}
	avgX /= points.size();
	avgY /= points.size();
	bool relativeToX = points.at(0).x > avgX, relativeToY = points.at(0).y > avgY;
	vec min(points.at(0)), max(points.at(0));
	//
	for (int i = 1; i < points.size(); i++)
	{
		if (points.at(i).x < min.x)
			min.x = points.at(i).x;
		else if (points.at(i).x > max.x)
			max.x = points.at(i).x;
		if (points.at(i).y < min.y)
			min.y = points.at(i).y;
		else if (points.at(i).y > max.y)
			max.y = points.at(i).y;
		if (points.at(i).x > avgX != relativeToX)
		{
			relativeToX = points.at(i).x > avgX;
			intervalX = !intervalX;
			addBit(shape.x, intervalX);
		}
		if (points.at(i).y > avgY != relativeToY)
		{
			relativeToY = points.at(i).y > avgY;
			intervalY = !intervalY;
			addBit(shape.y, intervalY);
		}
	}
	addBit(shape.x, relativeToX);
	addBit(shape.y, relativeToY);
	max -= min;
	cout << shape.x << ' ' << shape.y << endl;
	if (max.x > max.y * 5)
		shape.y = 0;
	if (max.y > max.x * 5)
		shape.x = 0;
	for(int i = 0; i < TOTAL_SHAPES; i++)
		for (int j = 0; j < 2; j++)
			if (numbersForShapes[i][j] == shape)
			{
				brush.setFillColor(shapesColors[i % TOTAL_COLORS]);
				return i;
			}
	return -1;
}

void Application::handleEvent(Event& event)
{
	switch (event.type)
	{
	case Event::MouseButtonPressed:
		if (event.key.code == Mouse::Left)
		{
			isDrawing = true;
			m_points.clear();
			brush.setFillColor(Color::White);
		}
		break;
	case Event::MouseButtonReleased:
		if (event.key.code == Mouse::Left)
		{
			isDrawing = false;
			int t = recognizeSymbol();
			if (t != -1)
				cout << shapesNames[t] << endl;
		}
		break;
	case Event::MouseMoved:
		if (isDrawing)
			if (vectorlength(Mouse::getPosition(m_window) - (m_points.empty() ? vec(0, 0) : m_points.back())) > drawingThickness)
			{
				m_points.push_back(Mouse::getPosition(m_window));
			}
		break;
	}
}

void Application::print()
{
	clock_t timer = clock();
	m_window.setActive(true);
	while (m_window.isOpen())
	{
		if (clock() > timer + frametime)
		{
			timer = clock();
			m_window.clear();
			for (int i = 1; i < m_points.size(); i++)
				drawLine(m_points[i - (m_points.size() > 1 ? 1: 0)] , m_points[i]);
			m_window.display();
		}
	}
}
