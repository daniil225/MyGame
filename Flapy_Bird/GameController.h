#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_
// Данный файд будет отвечать за создание персонажа и за его анимирование
#include<SFML/Graphics.hpp>
#include<string>
#include<time.h>
#include<iostream>

using sf::Texture;
using sf::Sprite;
using sf::Image;

struct Animation
{
	Texture textures[100]; // Текстуры
	Sprite sprites[100]; // Спрайты
	std::string nameAnim; // Имя анимации
	int countCadres; // Количество кадров
	int stateCadre; // Текущий кадр

	bool colliderVisible;
};

class Object2D
{
private:
	Animation animations[15];
	int countAnimations;

	clock_t lastTime;

	int nowNumberAnim;

	void GetFormat(const std::string& str, std::string& format);
	void DeleteFormatInStr(const std::string& str, std::string& newStr);
	int GetNumberAnim(const std::string word);

public:

	Object2D();

	sf::Vector2f position;
	sf::Vector2f scale;

	float width;
	float height;

	void SetDefaultSprite(const std::string path);
	void AddAnim(const std::string path, const std::string nameState, int countCadres, sf::IntRect collider = { 0,0,0,0 });

	int Animate(const std::string name, float msBetweenCadres, sf::RenderWindow &window);
	void Move(float x, float y);
	void SetScale(float x, float y);
	void SetPosition(float x, float y);
};



Object2D::Object2D() : countAnimations(0)
{
	scale = { 1.0, 1.0 };
	for (int i = 0; i < 10; i++)
	{
		animations[i].nameAnim = "";
	}
	lastTime = 0;
	nowNumberAnim = 0;
}

void Object2D::GetFormat(const std::string &str, std::string &format)
{
	bool isFormat = false;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '.') isFormat = true;
		if (isFormat)	format += str[i];
	}
}


void Object2D::DeleteFormatInStr(const std::string& str, std::string& newStr)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '.') break;
		newStr += str[i];
	}
}


int Object2D::GetNumberAnim(const std::string word)
{
	int numberAnimation = -1;
	for (int i = 0; i < 10; i++)
	{
		if (word == animations[i].nameAnim)
		{
			numberAnimation = i;
			break;
		}
	}

	return numberAnimation;
}


void Object2D::AddAnim(const std::string path, const std::string nameState, int countCadres, sf::IntRect collider)
{
	for (int i = 0; i < countCadres; i++)
	{
		std::string fullPath;
		std::string format;
		std::string pathNoFormat;

		GetFormat(path, format);
		DeleteFormatInStr(path, pathNoFormat);

		fullPath = fullPath + pathNoFormat;

		// В конец прибавляем номер анимации начиная с нуля
		std::string num = "";
		
		num = std::to_string(i);
		fullPath = fullPath + num;
		fullPath = fullPath + format;

		// Загрузка спрайта
		animations[countAnimations].textures[i].loadFromFile(fullPath);

		if (collider.width != 0 && collider.height != 0)
		{
			animations[countAnimations].sprites[i].setTextureRect(collider);
			animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
		}
		else
		{
			animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
		}

		animations[countAnimations].nameAnim += nameState;

		animations[countAnimations].countCadres = countCadres;
		animations[countAnimations].stateCadre = 0;
		countAnimations++;
	}

}

int Object2D::Animate(const std::string name, float msBetweenCadres, sf::RenderWindow& window)
{
	int nowNumberAnim = GetNumberAnim(name);

	if (nowNumberAnim == -1) return -1;

	clock_t secondInGame = clock();

	int stateCadre = animations[nowNumberAnim].stateCadre;
	if (stateCadre == animations[nowNumberAnim].countCadres)
	{
		animations[nowNumberAnim].stateCadre = 0;
		stateCadre = 0;
	}

	animations[nowNumberAnim].sprites[stateCadre].setPosition(position);
	animations[nowNumberAnim].sprites[stateCadre].setScale(scale);

	if (secondInGame - lastTime >= msBetweenCadres)
	{
		lastTime = clock();
		animations[nowNumberAnim].stateCadre++;
	}

	window.draw(animations[nowNumberAnim].sprites[stateCadre]);
	return stateCadre;
}



void Object2D::Move(float x, float y)
{
	position = { x, y };

	animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}

void Object2D::SetScale(float x, float y)
{
	scale = { x, y };
	animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setScale(scale);
}

void Object2D::SetPosition(float x, float y)
{
	position = { x, y };
	animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}

void Object2D::SetDefaultSprite(const std::string path)
{
	animations[0].textures[0].loadFromFile(path);
	animations[0].sprites[0].setTexture(animations[0].textures[0]);

	animations[0].nameAnim += "default";
	animations[0].countCadres = 1;
	animations[0].stateCadre = 0;
}


#endif // !GAMECONTROLLER_H_
