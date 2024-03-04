#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Sprite
{
public:
	//Void perqu� quan cliquem al bot� no retorna res
	//Opcionalment, podria rebre si �s el bot� esquerre o el dret del ratol�, el sender (qui ha desencadenat la lambda,
	//per exemple, quin bot�: ens serviria si tingu�ssim una sola lambda i la volgu�ssim passar a 3 botons diferents)
	typedef std::function<void()> OnClick;

	//La funci� lambda no s'executa en el context d'una classe. La podem crear des de qualsevol lloc (�s gen�rica).
	//Si volem fer servir objectes de la classe on est� declarada, hem de passar-li refer�ncies, per exemple [this]
	
	//La reserva de mem�ria pot ser:
	//Strongly: l'objecte refer�ncia no es pot destruir mentre es faci servir la lambda
	//Weakly: l'objecte refer�ncia s� es pot destruir mentre es faci servir la lambda
	//A C++ aix� no existeix com a tal, ho hem de fer a m�. Qui s'encarrega de fer aix� �s el garbage collector.
	
	//[] = reserva d'espai de mem�ria
	//() = els par�metres de la funci�
	//{} = el codi que fa la funci�
	OnClick onClick = [](){};

	//No li passem width ni height perqu� per defecte agafem els de la textura
	//Considerar la possibilitat de passar-li la textura ja carregada.
	//Evitar carregar 12 vegades la textura dels peons.
	Button(float x, float y, std::string texturePath);

	/// <summary>
	/// Comprovar si el ratol� est� fora de la caixa del bot�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	bool CheckBounds(unsigned int x, unsigned int y);

private:
	sf::Texture texture;
};

