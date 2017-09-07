#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_thread.h>
#include <queue>
#include "Map.h"
#include "Sprite.h"
#include "SpriteData.h"
#include "Hiro.h"
#include "Commandos.h"
#include "Diablo.h"
#include "HyperLightDrifter.h"
#include "Explosive.h"
#include "Ent.h"

const MapData* const mapIDs[] = {
	&map1Data,	//ID: 0
	&map2Data,  //ID: 1
	&map3Data	//ID: 2
};
const MapData* curMap = &map1Data;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture *tilemapTexture;
Sprite *curSprite = NULL;
std::vector<Sprite*> vSprites;
std::vector<Sprite*>::iterator itCurSprite;
unsigned int resolutionX = 640;
unsigned int resolutionY = (resolutionX / 4) * 3;
unsigned const char bghoehe = 100, bgbreite = 100;
unsigned char bgTiles[bghoehe*bgbreite] = {};
unsigned char bgWalk[bghoehe*bgbreite] = {};

void loadMap(unsigned const char& mapID) {
	//clear bg map
	curMap = mapIDs[mapID];
	for (int i = bghoehe*bgbreite; i--;) {
		bgTiles[i] = curMap->borderTile;
		bgWalk[i] = 0;
	}
	//copy map to bg
	int topLeftIndex = 8 * bgbreite + 8; //int topLeftIndex = (bghoehe / 2 - curMap->height / 2) * bgbreite + bgbreite / 2 - curMap->width / 2;
	int curMapTileIndex = 0;
	for (int i = 0; i < curMap->height; i++) {
		for (int j = 0; j < curMap->width; j++) {
			bgTiles[topLeftIndex + j] = curMap->tileData[curMapTileIndex];
			bgWalk[topLeftIndex + j] = curMap->walkData[curMapTileIndex];
			++curMapTileIndex;
		}
		topLeftIndex += bgbreite;
	}
	//load connection strips
	const char NorthConnectedMapID = curMap->connectionData[MapData::North].mapID;
	if (NorthConnectedMapID != -1) {
		const MapData* NorthConnectedMap = mapIDs[curMap->connectionData[MapData::North].mapID];
		int ConnStripIndex = (NorthConnectedMap->height - 8) * NorthConnectedMap->width;
		for (int i = curMap->connectionData[MapData::North].yOffset; i < curMap->connectionData[MapData::North].yOffset + 8; i++) {
			for (int j = 0; j < NorthConnectedMap->width; j++) {
				if (i >= 0 && i < bghoehe && j + curMap->connectionData[MapData::North].xOffset >= 0 && j + curMap->connectionData[MapData::North].xOffset < bgbreite) {
					bgTiles[i*bgbreite + j + curMap->connectionData[MapData::North].xOffset] = NorthConnectedMap->tileData[ConnStripIndex];
					bgWalk[i*bgbreite + j + curMap->connectionData[MapData::North].xOffset] = NorthConnectedMap->walkData[ConnStripIndex];
				}
				++ConnStripIndex;
			}
		}
	}
	const MapData::ConnectionData southConnection = curMap->connectionData[MapData::South];
	if (southConnection.mapID != -1) {
		const MapData* SouthConnectedMap = mapIDs[southConnection.mapID];
		unsigned int ConnStripIndex = 0;
		int curBgRow = curMap->height + 8 + southConnection.yOffset;
		int curBgColumn = southConnection.xOffset;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < SouthConnectedMap->width; j++) {
				if (curBgRow + i >= 0 && curBgRow + i < bghoehe && curBgColumn + j >= 0 && curBgColumn + j < bgbreite) {
					bgTiles[(curBgRow + i) * bgbreite + curBgColumn + j] = SouthConnectedMap->tileData[ConnStripIndex];
					bgWalk[(curBgRow + i) * bgbreite + curBgColumn + j] = SouthConnectedMap->walkData[ConnStripIndex];
				}
				++ConnStripIndex;
			}
		}
	}
	const MapData::ConnectionData westConnection = curMap->connectionData[MapData::West];
	if (westConnection.mapID != -1) {
		const MapData* WestConnectedMap = mapIDs[westConnection.mapID];
		unsigned int ConnStripIndex = WestConnectedMap->width - 8;
		int curBgRow = westConnection.yOffset;
		int curBgColumn = westConnection.xOffset;
		for (int i = 0; i < WestConnectedMap->height; i++) {
			for (int j = 0; j < 8; j++) {
				if (curBgRow + i >= 0 && curBgRow + i < bghoehe && curBgColumn + j >= 0 && curBgColumn + j < bgbreite) {
					bgTiles[(curBgRow + i) * bgbreite + curBgColumn + j] = WestConnectedMap->tileData[ConnStripIndex + j];
					bgWalk[(curBgRow + i) * bgbreite + curBgColumn + j] = WestConnectedMap->walkData[ConnStripIndex + j];
				}
			}
			ConnStripIndex += WestConnectedMap->width;
		}
	}
	const MapData::ConnectionData eastConnection = curMap->connectionData[MapData::East];
	if (eastConnection.mapID != -1) {
		const MapData* EastConnectedMap = mapIDs[eastConnection.mapID];
		unsigned int ConnStripIndex = 0;
		int curBgRow = eastConnection.yOffset;
		int curBgColumn = curMap->width + 8 + eastConnection.xOffset;
		for (int i = 0; i < EastConnectedMap->height; i++) {
			for (int j = 0; j < 8; j++) {
				if (curBgRow + i >= 0 && curBgRow + i < bghoehe && curBgColumn + j >= 0 && curBgColumn + j < bgbreite) {
					bgTiles[(curBgRow + i) * bgbreite + curBgColumn + j] = EastConnectedMap->tileData[ConnStripIndex + j];
					bgWalk[(curBgRow + i) * bgbreite + curBgColumn + j] = EastConnectedMap->walkData[ConnStripIndex + j];
				}
			}
			ConnStripIndex += EastConnectedMap->width;
		}
	}
	//load new tilemap
	SDL_Surface *surface = SDL_LoadBMP(curMap->pathTileset);
	if (surface == NULL) {
		printf("load bmp error: %s\n", SDL_GetError()); return;
	}
	tilemapTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	//remove sprites except current player sprite
	if (vSprites.size() > 0)
		for (std::vector<Sprite*>::iterator it = vSprites.begin(); it != vSprites.end() - 1; ++it)
			if (*it != curSprite)
				delete *it;
	vSprites.clear();
	//load new map sprites
	Sprite* newSprite = NULL;
	for (unsigned char spriteNum = 0; spriteNum < curMap->numSprites; ++spriteNum) {
		newSprite = new Sprite(curMap->sprites[spriteNum].sprite, false);
		newSprite->setPos((curMap->sprites[spriteNum].mapPos.x + 8) * 16, (curMap->sprites[spriteNum].mapPos.y + 8) * 16);
		vSprites.push_back(newSprite);
	}
	if (curSprite == NULL) {
		itCurSprite = vSprites.begin();
		curSprite = *itCurSprite;
	}
	else {
		vSprites.push_back(curSprite);
		itCurSprite = vSprites.begin();
	}
}

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError()); return -1;
	}

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError()); return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); return -1;
	}
	IMG_Init(IMG_INIT_PNG);

	Uint32 time;
	SDL_RenderSetLogicalSize(renderer, resolutionX, resolutionY);
	SDL_SetRenderDrawColor(renderer, 100, 50, 150, 20);
	SDL_Rect playerPos = { 0,0,0,0 }, srcRect = { 0,0,16,16 }, destRect = { 0,0,16,16 };
	int tileBlockNum, tileNum;
	int mapTopLeftX, mapTopLeftY, playerXOffset = 0, playerYOffset = 0, walkData;

	loadMap(0);

	//sprite init - load map part
	//SDL_Texture* tHiro = Sprite::loadTexture(Hiro.path);
	//SDL_Texture* tCommando = Sprite::loadTexture(Commandos.path);
	//SDL_Texture* tDiablo = Sprite::loadTexture(Diablo.path);
	//SDL_Texture* tHyperLightDrifter = Sprite::loadTexture(HyperLightDrifter.path);
	//SDL_Texture* tExplosive = Sprite::loadTexture(Explosive.path);
	//SDL_Texture* tEnt = Sprite::loadTexture(Ent.path);
	//Sprite* newSprite = new Sprite(tHiro, &Hiro, false);
	//newSprite->setPos(14 * 16, 16 * 16);
	//vSprites.push_back(newSprite);
	//Sprite* pHiro1 = newSprite;
	//newSprite = new Sprite(tCommando, &Commandos, false);
	//newSprite->setPos(11 * 16, 13 * 16);
	//vSprites.push_back(newSprite);
	//newSprite = new Sprite(tDiablo, &Diablo, false);
	//newSprite->setPos(15 * 16, 10 * 16);
	//vSprites.push_back(newSprite);
	//Sprite* pDiablo1 = newSprite;
	//newSprite = new Sprite(tHyperLightDrifter, &HyperLightDrifter, false);
	//newSprite->setPos(12 * 16, 17 * 16);
	//vSprites.push_back(newSprite);
	//curSprite = newSprite;
	//newSprite = new Sprite(tEnt, &Ent, false);
	//newSprite->setPos(300, 20);
	//vSprites.push_back(newSprite);
	//itCurSprite = vSprites.begin();

	char i, j;
	bool quit = 0;
	SDL_Event e;
	const Uint8* keystates;
	static unsigned char LastPressed[100];
	static unsigned char KeyPressed[100];
	char tmpLastDir = 0, animSet = 0;
	SDL_Rect curGridPos;
	while (!quit)
	{
		time = SDL_GetTicks();

		curGridPos = curSprite->mapPos;
		curGridPos.x = (curGridPos.x / 16) - 8;
		if (curGridPos.x < 0 && curMap->connectionData[MapData::West].mapID != -1) {
			curSprite->mapPos.y += (8 - curMap->connectionData[MapData::West].yOffset) * 16;
			loadMap(curMap->connectionData[MapData::West].mapID);
			curSprite->mapPos.x = (curMap->width + 8) * 16;
		}
		else if (curGridPos.x >= curMap->width && curMap->connectionData[MapData::East].mapID != -1) {
			curSprite->mapPos.x = 16 * 8;
			curSprite->mapPos.y += (8 - curMap->connectionData[MapData::East].yOffset) * 16;
			loadMap(curMap->connectionData[MapData::East].mapID);
		}
		else {
			curGridPos.y = (curGridPos.y / 16) - 8;
			if (curGridPos.y < 0 && curMap->connectionData[MapData::North].mapID != -1) {
				curSprite->mapPos.x += (8 - curMap->connectionData[MapData::North].xOffset) * 16;
				loadMap(curMap->connectionData[MapData::North].mapID);
				curSprite->mapPos.y = (curMap->height + 8) * 16;
			}
			else if (curGridPos.y >= curMap->height && curMap->connectionData[MapData::South].mapID != -1) {
				curSprite->mapPos.x += (8 - curMap->connectionData[MapData::South].xOffset) * 16;
				curSprite->mapPos.y = 16 * 8;
				loadMap(curMap->connectionData[MapData::South].mapID);
			}
		}


		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		keystates = SDL_GetKeyboardState(NULL);
		if (keystates[SDL_SCANCODE_UP]) {
			tmpLastDir = 1;
			if (!animSet) {
				animSet = 1;
				curSprite->pushAnim(3);
			}
		}
		else if (keystates[SDL_SCANCODE_DOWN]) {
			tmpLastDir = 0;
			if (!animSet) {
				animSet = 1;
				curSprite->pushAnim(1);
			}
		}
		else if (keystates[SDL_SCANCODE_LEFT]) {
			tmpLastDir = 2;
			if (!animSet) {
				animSet = 1;
				curSprite->pushAnim(5);
			}
		}
		else if (keystates[SDL_SCANCODE_RIGHT]) {
			tmpLastDir = 3;
			if (!animSet) {
				animSet = 1;
				curSprite->pushAnim(7);
			}
		}
		else if (keystates[SDL_SCANCODE_KP_PLUS]) {
			resolutionX -= 12;
			resolutionY = (resolutionX / 4) * 3;
			SDL_RenderSetLogicalSize(renderer, resolutionX, resolutionY);
		}
		else if (keystates[SDL_SCANCODE_KP_MINUS]) {
			resolutionX += 12;
			resolutionY = (resolutionX / 4) * 3;
			SDL_RenderSetLogicalSize(renderer, resolutionX, resolutionY);
		}
		else if (keystates[SDL_SCANCODE_KP_8]) {
			loadMap(0);
		}
		else if (keystates[SDL_SCANCODE_KP_4]) {
			loadMap(1);
		}
		else if (keystates[SDL_SCANCODE_KP_6]) {
			Sprite* newExplodeBulletRight = new Sprite(&Explosive, true);
			newExplodeBulletRight->setPos(curSprite->mapPos.x, curSprite->mapPos.y);
			vSprites.push_back(newExplodeBulletRight);
			const char explosivemove[] = { 0,0,1 };
			newExplodeBulletRight->pushAnim(3, explosivemove);
			itCurSprite = vSprites.begin();
			//curSprite = newExplodeBulletRight;
			SDL_Delay(50);
		}
		else if (keystates[SDL_SCANCODE_KP_2]) {
			/*if (LastPressed[SDL_SCANCODE_KP_2] == 0)
				KeyPressed[SDL_SCANCODE_KP_2] = 1;
			else
				KeyPressed[SDL_SCANCODE_KP_2] = 0;
			LastPressed[SDL_SCANCODE_KP_2] = 1;

			if (KeyPressed[SDL_SCANCODE_KP_2]) {*/
			Sprite* newHyperLightDrifter = new Sprite(&HyperLightDrifter, true);
			newHyperLightDrifter->setPos(100, 100);
			vSprites.push_back(newHyperLightDrifter);
			const char hyperlightdriftermove[] = { 7,7,1,1,5,5,3,3,7,7,1,1,5,5,3,3 };
			newHyperLightDrifter->pushAnim(16, hyperlightdriftermove);
			itCurSprite = vSprites.begin();

			//}
		}
		else if (keystates[SDL_SCANCODE_KP_3]) {
			curSprite->pushAnim((curSprite->animList.front() + 1) % curSprite->sData->numAnimations);
			SDL_Delay(200);
		}
		else if (keystates[SDL_SCANCODE_KP_0]) {
			if (++itCurSprite == vSprites.end())
				itCurSprite = vSprites.begin();
			curSprite = *itCurSprite;
			SDL_Delay(200);
		}
		else if (keystates[SDL_SCANCODE_KP_7]) {
			const char diablomove[] = {
				1,1,0,7,7,3,3,7,10,8,11,9
			};
			/*pDiablo1->pushAnim(12, diablomove);
			const char hiromove[] = {
				11,11,12,9,9,10,10,10,8,8,12,2,12,0
			};
			pHiro1->pushAnim(14, hiromove);
			SDL_Delay(200);*/
		}
		else {
			if (animSet) {
				curSprite->pushAnim(tmpLastDir * 2);
				animSet = 0;
			}
			LastPressed[SDL_SCANCODE_KP_2] = 0;
		}

		destRect.x = (resolutionX / 2) - (512 / 2) - curSprite->mapPos.x % 16;
		destRect.y = (resolutionY / 2) - (512 / 2) - curSprite->mapPos.y % 16;
		SDL_RenderClear(renderer);
		for (i = 0; i < 32; i++) {
			mapTopLeftY = (curSprite->mapPos.y / 16) - 16 + i;
			mapTopLeftX = (curSprite->mapPos.x / 16) - 17;
			for (j = 0; j < 32; j++) {
				++mapTopLeftX;

				//unsigned int mapTopTile = mapTopLeftY*map1breite + mapTopLeftX;
				unsigned int mapTopTile = mapTopLeftY*bgbreite + mapTopLeftX;

				//if (mapTopLeftX >= map1breite || mapTopLeftX < 0 || mapTopLeftY >= map1hoehe || mapTopLeftY < 0) {
				if (mapTopLeftX >= bgbreite || mapTopLeftX < 0 || mapTopLeftY >= bghoehe || mapTopLeftY < 0) {
					tileNum = 13;// curMap->borderTile;//tileNum = map1border;
					tileBlockNum = 0;
				}
				else {
					tileNum = bgTiles[mapTopTile];//tileNum = map1tiledata[mapTopTile];
					walkData = bgWalk[mapTopTile];//walkData = map1walkdata[mapTopTile];
					tileBlockNum = (walkData >> 3) & 0x3;
				}

				srcRect.y = tileBlockNum * 512 + 16 * (tileNum / 8);
				srcRect.x = 16 * (tileNum % 8);

				if (tileBlockNum > 2) {//format: bit 7 6 5 (walk data), 4 3 (tile block num) 2 1 0 (pause count) )
					if (((walkData + 1) & 0x7) == 0) {
						if ((++tileNum % 8) == 0)
							bgTiles[mapTopTile] -= 7;//map1tiledata[mapTopTile] -= 7;
						else
							bgTiles[mapTopTile] = tileNum;//map1tiledata[mapTopTile] = tileNum;
						bgWalk[mapTopTile] &= 0xf8;//map1walkdata[mapTopTile] &= 0xf8;
					}
					else {
						bgWalk[mapTopTile] = (walkData & 0xf8) | ((walkData + 1) & 0x7);//map1walkdata[mapTopTile] = (walkData & 0xf8) | ((walkData + 1) & 0x7);
					}
				}

				SDL_RenderCopy(renderer, tilemapTexture, &srcRect, &destRect);
				destRect.x += 16;
			}
			destRect.x = (resolutionX / 2) - (512 / 2) - curSprite->mapPos.x % 16;
			destRect.y += 16;
		}
		//sprite drawing order
		for (std::vector<Sprite*>::iterator it = vSprites.begin(); it != vSprites.end() - 1; ++it)
			for (std::vector<Sprite*>::iterator it2 = vSprites.begin(); it2 != vSprites.end() - 1; ++it2)
				if ((*it2)->mapPos.y + (*it2)->mapPos.h > (*(it2 + 1))->mapPos.y + (*(it2 + 1))->mapPos.h)
					std::iter_swap(it2, it2 + 1);
		//add sprites to BG
		int bildmitteX = (resolutionX / 2);
		int bildmitteY = (resolutionY / 2);
		int ursprungX = bildmitteX - curSprite->mapPos.x;
		int ursprungY = bildmitteY - curSprite->mapPos.y;
		SDL_Rect tmp;
		for (std::vector<Sprite*>::iterator it = vSprites.begin(); it != vSprites.end();) {
			if ((*it)->objectInUse) {
				if ((abs((*it)->mapPos.x - curSprite->mapPos.x) < 250) && (abs((*it)->mapPos.y - curSprite->mapPos.y) < 250)) {
					const SDL_Rect* srcTmp = &(*it)->getFrameCoord();
					if ((*it)->objectInUse) {
						tmp.h = (*it)->mapPos.h;
						tmp.w = (*it)->mapPos.w;
						tmp.x = ursprungX + (*it)->mapPos.x;
						tmp.y = ursprungY + (*it)->mapPos.y;

						tmp.x -= (*it)->mapPos.w / 2;
						tmp.y -= (*it)->mapPos.h / 2;

						SDL_RenderCopy(renderer, (*it)->spriteTexture, srcTmp, &tmp);
					}
				}
				++it;
			}
			else {
				if (curSprite == (*it)) curSprite = NULL;
				delete *it;
				it = vSprites.erase(it);
				if (it == vSprites.end()) it = vSprites.begin();
				itCurSprite = it;
				if (curSprite == NULL) curSprite = *itCurSprite;
			}
		}
		SDL_RenderPresent(renderer);
		if (20 > (SDL_GetTicks() - time)) SDL_Delay(20 - (SDL_GetTicks() - time));
	}
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}