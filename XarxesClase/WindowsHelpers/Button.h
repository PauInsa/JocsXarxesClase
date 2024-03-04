#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Sprite
{
public:
	//Void perquè quan cliquem al botó no retorna res
	//Opcionalment, podria rebre si és el botó esquerre o el dret del ratolí, el sender (qui ha desencadenat la lambda,
	//per exemple, quin botó: ens serviria si tinguéssim una sola lambda i la volguéssim passar a 3 botons diferents)
	typedef std::function<void()> OnClick;

	//La funció lambda no s'executa en el context d'una classe. La podem crear des de qualsevol lloc (és genèrica).
	//Si volem fer servir objectes de la classe on està declarada, hem de passar-li referències, per exemple [this]
	
	//La reserva de memòria pot ser:
	//Strongly: l'objecte referència no es pot destruir mentre es faci servir la lambda
	//Weakly: l'objecte referència sí es pot destruir mentre es faci servir la lambda
	//A C++ això no existeix com a tal, ho hem de fer a mà. Qui s'encarrega de fer això és el garbage collector.
	
	//[] = reserva d'espai de memòria
	//() = els paràmetres de la funció
	//{} = el codi que fa la funció
	OnClick onClick = [](){};

	//No li passem width ni height perquè per defecte agafem els de la textura
	//Considerar la possibilitat de passar-li la textura ja carregada.
	//Evitar carregar 12 vegades la textura dels peons.
	Button(float x, float y, std::string texturePath);

	/// <summary>
	/// Comprovar si el ratolí està fora de la caixa del botó
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	bool CheckBounds(unsigned int x, unsigned int y);

private:
	sf::Texture texture;
};

