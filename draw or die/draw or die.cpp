#include <iostream>
#include "Application.h"
#include <thread>

RenderWindow window(VideoMode::getFullscreenModes()[0], L"Здарова, сталкер");
Application app(window, 144);

void screen()
{
    app.print();
}

int main()
{
    window.setActive(false);
    Event ev;
    thread windowprint(screen);
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
                window.close();
            else
                app.handleEvent(ev);
        }
    }
    windowprint.join();
}