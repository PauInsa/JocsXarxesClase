#include "Button.h"

Button::Button(float x, float y, std::string texturePath)
{
	//Carregar textura
	if (!texture.loadFromFile(texturePath))
	{
		//Handle error
	}

	//Si volem, podem comprovar que la textura sigui prou gran.

	//Posicionar botó
	//setPostion ve heredada de Transformable. SFML està preparada per fer jocs i té el sistema de Transform
	this->setPosition(x, y);
	this->setTexture(texture);
}

bool Button::CheckBounds(unsigned int x, unsigned int y)
{
	return this->getGlobalBounds().contains(x, y);
}
