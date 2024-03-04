#pragma once
#include <list>
#include <mutex>
#include "Button.h"

class Window
{
	//TODO: Crear sistema de dispatcher

public:
	/// <summary>
	/// El constructor crea la finestra. No es pot tocar la finestra des d'un altre thread que no sigui el que la ha creat.
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="title"></param>
	Window(unsigned int width = 800, unsigned int height = 600, std::string title = "Window");

	void AddButton(Button* bt);
	void RunWindowsLoop();

private:
	//Posar un mutex global per tot el sistema de finestres funciona, però no és el més eficient:
	//mentre aquest mutex estigui bloquejat, no podrem fer res més
	//TODO: Crear més mutex
	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownButton;
	std::list<Button*> _buttons;

	//Tècnicament, com que el botó és drawable, podríem posar-lo aquí.
	//Però no seria eficient, ja que quan volguéssim comprovar si s'ha clicat un botó hauríem d'iterar per tots els drawables
	std::list<sf::Drawable*> _objectsToDraw;
};

