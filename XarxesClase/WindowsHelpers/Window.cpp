#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
{
	_window.create(sf::VideoMode(width, height), title);

	//TODO: Afegir al constructor tot el que calgui. 
}

void Window::AddButton(Button* bt)
{
	_mutex.lock();

	_buttons.push_back(bt);

	//Posicions a la lista com a Z-Buffer. Haurà d'anar canviant: si pintem el taulell i després movem un botó,
	//haurem d'ajustar el botó perquè estigui a dalt de tot (bringToFront). 
	//Caldrà gestionar aquest bringToFront. 
	//Recomanació: que la posició dels botons tingui una posició relativa: pujar el botó a la llista _buttons i a _objectsToDraw
	//Haurem de recórrer la llista de dreta (final) a esquerra (inici) per pintar correctament els objectes en ordre
	_objectsToDraw.push_back(bt);

	_mutex.unlock();
}

void Window::RunWindowsLoop()
{
	//TODO: Fer aquesta funció thread safe

	//Com si fos l'update de unity.
	//Hem de gestionar si volem que es pintin diverses coses en diferents moments:
	//Lista de lambdas per l'update, llista de lambdas pel fixed update, llista de lambdas per el start...
	//Tanmateix, per la pràctica no cal complicar-se
	while (_window.isOpen())
	{
		_window.clear(sf::Color::Black);

		for (sf::Drawable* drawable : _objectsToDraw)
		{
			//Idealment, aquest if no hauria d'estar: hauríem d'estar segurs que els objectes que entren aquí no es borren
			//Però com que estem treballant amb punters, és millor prevenir
			if (drawable != nullptr)
			{
				//Tècnicament no està pintant per pantalla.
				//Està pintant al Z-Buffer de debò, l'intern.
				_window.draw(*drawable);
			}
		}

		//Pintar, ara sí, la pantalla
		_window.display();

		sf::Event event;
		while (_window.pollEvent(event))
		{
			//TODO: Fer de manera més elegant
			//TODO: Crear funcions enlloc de ficar-ho dins dels cases del switch
			//TODO: Afegir la resta de event types que ens caldran
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					_window.close();
					break;
				}
				case sf::Event::MouseButtonPressed:
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//Com que estem fent un joc en 2D, probablement no mourem el món
						//Per si de cas, passem el pixel de la posició del ratolí a world coords
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						//Com hem dit anteriorment, cal iterar la llista a l'inrevés
						for (auto it = _buttons.rbegin(); it != _buttons.rend(); it++)
						{
							Button* bt = *it;
							if (bt->CheckBounds(worldPos.x, worldPos.y))
							{
								_lastClickedDownButton = bt;
								break;
							}
						}
					}
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//Com que estem fent un joc en 2D, probablement no mourem el món
						//Per si de cas, passem el pixel de la posició del ratolí a world coords
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						if (_lastClickedDownButton != nullptr && _lastClickedDownButton->CheckBounds(worldPos.x, worldPos.y))
						{
							_lastClickedDownButton->onClick();
						}
						else
						{
							//TODO: Cancel click or drag or others
						}
					}
					break;
				}
				case sf::Event::MouseWheelScrolled:
				{
					//TODO: Handle MouseWheelScrolled
					break;
				}
			}
		}
	}
}
