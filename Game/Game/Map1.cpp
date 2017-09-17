#include "Map.h"
#include "Hiro.h"
#include "HyperLightDrifter.h"
#include "Commandos.h"
#include "Diablo.h"
#include "Explosive.h"

const char map1breite = 50, map1hoehe = 20, map1border = 1;
unsigned char map1tiledata[map1hoehe*map1breite] = {
	 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,176,177,177,177,177,178,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	 7,1,1,1,184,0,0,0,0,186,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	 7,1,1,1,184,0,8,8,0,186,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,184,0,8,8,0,186,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,72,73,74,75,76,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,184,0,0,0,0,186,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,80,81,82,83,84,1,1,1,13,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,88,89,90,91,92,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,96,97,98,99,100,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,62,63,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,13,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,69,70,71,77,1,1,1,1,1,1,1,1,1,1,7,
	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,13,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,78,79,1,1,1,1,1,1,1,1,1,1,1,7,
	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,13,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
	 7,7,7,7,7,7,1,1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};
unsigned char map1walkdata[map1hoehe*map1breite] = {
	 224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 224,0,0,0,0,24,24,24,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 224,0,0,0,0,24,24,24,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,24,24,24,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,224,224,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,24,24,24,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,224,224,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,224,224,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,0,224,224,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,0,0,0,0,0,0,0,0,0,0,0,224,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,224,0,0,0,0,0,0,0,0,0,0,0,224,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	 224,224,224,224,224,224,0,0,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224
};
const MapData map1Data = {
	"tilesAnim.bmp", map1hoehe, map1breite, map1tiledata, map1walkdata,
	{ { -1,0,0 }, //nord //mapID, xOffset, yOffset
	  { 1,-2,0 }, //s�d
	  { 2,0,18 }, //west
	  { 2,0,3 } }, //ost
	map1border, 2,                                    //x,y,w,h
	{ { &Hiro,{ 2, 2, 0, 0 }, 0 },{ &HyperLightDrifter,{ 3,4,0,0 } ,0 } }
};

const char map2breite = 20, map2hoehe = 20, map2border = 188;
unsigned char map2tiledata[map2hoehe*map2breite] = {
	159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,1,1,159,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,159,
	159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159
};
unsigned char map2walkdata[map2hoehe*map2breite] = {
	224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,1,1,224,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,
	224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224
};
const MapData map2Data = {
	"fireredtiles.bmp", map2hoehe, map2breite, map2tiledata, map2walkdata,
	{ { 0,18,0 },{ -1,0,0 },{ -1,0,0 },{ -1,0,0 } }, map2border, 4,
	{ { &Commandos,{ 9, 7, 0, 0 }, 0 },{ &Commandos,{ 9,5,0,0 } ,2 },{ &Commandos,{ 7,6,0,0 } ,4 },{ &Commandos,{ 11,6,0,0 } ,6 } }
};

const char map3breite = 10, map3hoehe = 10, map3border = 188;
unsigned char map3tiledata[map3hoehe*map3breite] = {
	159,159,159,159,159,159,159,159,159,159,
	159,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,1,
	159,1,1,1,1,1,1,1,1,1,
	159,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,159,
	159,1,1,1,1,1,1,1,1,159,
	1,1,1,1,1,1,1,1,1,159,
	1,1,1,1,1,1,1,1,1,159,
	159,159,159,159,159,159,159,159,159,159
};
unsigned char map3walkdata[map3hoehe*map3breite] = {
	224,224,224,224,224,224,224,224,224,224,
	224,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,0,
	224,0,0,0,0,0,0,0,0,0,
	224,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,224,
	224,0,0,0,0,0,0,0,0,224,
	0,0,0,0,0,0,0,0,0,224,
	0,0,0,0,0,0,0,0,0,224,
	224,224,224,224,224,224,224,224,224,224
};
const MapData map3Data = {
	"fireredtiles.bmp", map3hoehe, map3breite, map3tiledata, map3walkdata,
	{ { -1,0,0 },{ -1,0,0 },{ 0,0,13 },{ 0,0,-2 } }, map3border, 1,
	{ { &Diablo,{ 5, 3, 0, 0 }, 0 } }
};