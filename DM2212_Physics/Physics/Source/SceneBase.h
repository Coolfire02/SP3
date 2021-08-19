#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Button.h"
#include "Shape.h"

typedef struct TileSetting TileSetting;

class SceneBase : public Scene
{
protected:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:



	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_TEXT = 1,

		GEO_BALL = 2,
		GEO_CUBE = 3,

		//Tiles Enum Start
		//Make sure to use = to ensure maintenance of IDs (to never conflict)
		GEO_TILES_START = 4,
		GEO_TILEGRID = 5,
		GEO_PLAYER_GIRL1 = 6,
		GEO_BLOCK_UP_RED = 7,

		// GRAVEYARD SCENE TILES
		GEO_GY_CENTER = 8,
		GEO_GY_TOP_MID = 9,
		GEO_GY_TOP_LEFT = 10,
		GEO_GY_TOP_RIGHT = 11,
		GEO_GY_LEFT = 12,
		GEO_GY_RIGHT = 13,
		GEO_GY_BOT_MID = 14,
		GEO_GY_BOT_LEFT = 15,
		GEO_GY_BOT_RIGHT = 16,
		GEO_GY_PLATFORM_LEFT = 17,
		GEO_GY_PLATFORM_RIGHT = 18,
		GEO_GY_PLATFORM_MID = 19,
		GEO_GY_CORNER_LEFT = 20,
		GEO_GY_CORNER_RIGHT = 21,
		GEO_GY_EDGE_LEFT = 22,
		GEO_GY_EDGE_RIGHT = 23,
		GEO_LOBBY_MACHINE1 = 24,
		GEO_LOBBY_MACHINE2 = 25,
		GEO_LOBBY_MACHINE3 = 26,
		GEO_LOBBY_MACHINE4 = 27,
		GEO_LOBBY_MACHINE5 = 28,
		GEO_LOBBY_FLOOR = 29,
		GEO_LOBBY_WALL = 30,
		GEO_OCEAN_1_TOPLEFT = 31,
		GEO_OCEAN_1_TOPMID = 32,
		GEO_OCEAN_1_TOPRIGHT = 33,
		GEO_OCEAN_1_MIDLEFT = 34,
		GEO_OCEAN_1_MIDMID = 35,
		GEO_OCEAN_1_MIDRIGHT = 36,
		GEO_OCEAN_1_BOTLEFT = 37,
		GEO_OCEAN_1_BOTMID = 38,
		GEO_OCEAN_1_BOTRIGHT = 39,
		GEO_OCEAN_2_LEFT = 40,
		GEO_OCEAN_2_MID = 41,
		GEO_OCEAN_2_RIGHT = 42,
		GEO_OCEAN_3_BLOCK = 43,
		
		GEO_ROBOT_BIGCUBE_1_TOPLEFT = 44,
		GEO_ROBOT_BIGCUBE_1_TOPMID = 45,
		GEO_ROBOT_BIGCUBE_1_TOPRIGHT = 46,
		GEO_ROBOT_BIGCUBE_1_MIDLEFT = 47,
		GEO_ROBOT_BIGCUBE_1_MIDMID = 48,
		GEO_ROBOT_BIGCUBE_1_MIDRIGHT = 49,
		GEO_ROBOT_BIGCUBE_1_BOTLEFT = 50,
		GEO_ROBOT_BIGCUBE_1_BOTMID = 51,
		GEO_ROBOT_BIGCUBE_1_BOTRIGHT = 52,
		GEO_ROBOT_MEDIUMCUBE_2_TOPLEFT = 53,
		GEO_ROBOT_MEDIUMCUBE_2_TOPRIGHT = 54,
		GEO_ROBOT_MEDIUMCUBE_2_BOTLEFT = 55,
		GEO_ROBOT_MEDIUMCUBE_2_BOTRIGHT = 56,
		GEO_ROBOT_BIGCUBE_3_TOPLEFT = 57,
		GEO_ROBOT_BIGCUBE_3_TOPMID = 58,
		GEO_ROBOT_BIGCUBE_3_TOPRIGHT = 59,
		GEO_ROBOT_BIGCUBE_3_MIDLEFT = 60,
		GEO_ROBOT_BIGCUBE_3_MIDMID = 61,
		GEO_ROBOT_BIGCUBE_3_MIDRIGHT = 62,
		GEO_ROBOT_BIGCUBE_3_BOTLEFT = 63,
		GEO_ROBOT_BIGCUBE_3_BOTMID = 64,
		GEO_ROBOT_BIGCUBE_3_BOTRIGHT = 65,
		GEO_ROBOT_BIGCUBE_4_TOPLEFT = 66,
		GEO_ROBOT_BIGCUBE_4_TOPMID = 67,
		GEO_ROBOT_BIGCUBE_4_TOPRIGHT = 68,
		GEO_ROBOT_BIGCUBE_4_MIDLEFT = 69,
		GEO_ROBOT_BIGCUBE_4_MIDMID = 70,
		GEO_ROBOT_BIGCUBE_4_MIDRIGHT = 71,
		GEO_ROBOT_BIGCUBE_4_BOTLEFT = 72,
		GEO_ROBOT_BIGCUBE_4_BOTMID = 73,
		GEO_ROBOT_BIGCUBE_4_BOTRIGHT = 74,
		GEO_ROBOT_SMALLCUBE_5_TOP = 75, //Acid
		GEO_ROBOT_SMALLCUBE_5_BOTTOM = 76,
		GEO_ROBOT_SMALLCUBE_6_TOP = 77, //Ladder
		GEO_ROBOT_SMALLCUBE_6_MID = 78,
		GEO_ROBOT_SMALLCUBE_6_BOTTOM = 79,
		GEO_ROBOT_SMALLCUBE_7_MISCDECOR = 80,
		GEO_ROBOT_SMALLCUBE_8_MISCDECOR = 81,
		GEO_ROBOT_SMALLCUBE_9_MISCDECOR = 82,
		GEO_ROBOT_SMALLCUBE_10_MISCDECOR = 83,
		GEO_ROBOT_SMALLCUBE_11_MISCDECOR = 84,
		GEO_ROBOT_SMALLCUBE_12_MISCDECOR = 85,
		GEO_ROBOT_SMALLCUBE_13_MISCDECOR = 86,
		GEO_ROBOT_SMALLCUBE_14_MISCDECOR = 87,
		GEO_ROBOT_SMALLCUBE_15_MISCDECOR = 88,

		GEO_ROBOT_SMALLCUBE_16_POTION1 = 89,
		GEO_ROBOT_SMALLCUBE_16_POTION2 = 90,
		GEO_ROBOT_SMALLCUBE_16_POTION3 = 91,
		GEO_ROBOT_SMALLCUBE_17_DIAMOND1 = 92,
		GEO_ROBOT_SMALLCUBE_17_DIAMOND2 = 93,
		GEO_ROBOT_SMALLCUBE_17_DIAMOND3 = 94,
		GEO_ROBOT_SMALLCUBE_17_DIAMOND4 = 95,
		GEO_ROBOT_SMALLCUBE_18_SIGN1 = 96,

		//Tiles End
		GEO_TILES_END,


		GEO_BG,

		
		GEO_BUTTON,
		GEO_WALL,

		GEO_100,
		GEO_10,
		GEO_50,
		GEO_HIGHLIGHT,

		GEO_LIGHTBALL,
	
		NUM_GEOMETRY,
	};
public:

	MS modelStack;

	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void SetLights();

	void LoadTile(GEOMETRY_TYPE type, std::string fileName, double length, double height, SHAPE_TYPE shapeType);
	TileSetting* GetTileSetting(GEOMETRY_TYPE type);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int limit);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	TileSetting* tileSize[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;
};

struct TileSetting
{
	double gridLength, gridHeight;
	SHAPE_TYPE shapeType;
	TileSetting(double length = 1, double height = 1, SHAPE_TYPE shape = SHAPE_TYPE::RECTANGLE) : gridLength(length), gridHeight(height), shapeType(shape) {}
};

#endif