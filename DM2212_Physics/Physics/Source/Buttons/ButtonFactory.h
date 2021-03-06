#pragma once
#include "Button.h"
#include "ProgressBar.h"

#include "../Mesh.h"
#include "Text.h"
#include <string>

class ButtonFactory
{
public:
	static Button* createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize);
	static Button* createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize, FONTTYPE type);
	static Button* createNoTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture);
	static Button* createButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture, float offsetX, float offsetY, Color textColour, std::string textString, float textSize, FONTTYPE type = CALIBRI);
	
	static ProgressBar* createProgressBar(std::string buttonName, float originX, float originY, float thickness, float length, PROGRESSBAR_TYPE type, Mesh* quadTexture);

};

