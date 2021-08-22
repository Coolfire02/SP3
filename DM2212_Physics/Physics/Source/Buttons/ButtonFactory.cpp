#include "ButtonFactory.h"


Button* ButtonFactory::createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize)
{
	return createTextButton(buttonName, originX, originY, quadSizeX, quadSizeY, offsetX, offsetY, textColour, textString, textSize, CALIBRI);
}

Button* ButtonFactory::createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize, FONTTYPE type)
{
	Button* namedButton;
	namedButton = new Button(buttonName, originX, originY, quadSizeX, quadSizeY, nullptr);
	namedButton->spawnTextObject("Text", textColour, type, textSize);
	namedButton->getTextObject()->setTextString(textString);
	namedButton->getTextObject()->setTextOffsetFromTopLeft(offsetX, offsetY);
	return namedButton;
}

Button* ButtonFactory::createNoTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture)
{
	Button* noTextButton;
	noTextButton = new Button(buttonName, originX, originY, quadSizeX, quadSizeY, quadTexture);
	return noTextButton;
}

Button* ButtonFactory::createButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture, float offsetX, float offsetY, Color textColour, std::string textString, float textSize)
{
	Button* namedButton;
	namedButton = new Button(buttonName, originX, originY, quadSizeX, quadSizeY, quadTexture);
	namedButton->spawnTextObject("Text", textColour, CALIBRI, textSize);
	namedButton->getTextObject()->setTextString(textString);
	namedButton->getTextObject()->setTextOffsetFromTopLeft(offsetX, offsetY);
	return namedButton;
}