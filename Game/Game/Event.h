#pragma once
#include <list>
#include <SDL.h>
#include "Sprite.h"
extern unsigned char curMapID;
//unsigned char bgTiles[];
class Event {
public:
	Event(char x, char y, unsigned char w = 1, unsigned char h = 1, /*char*/ int wBefore = 0, /*char*/ int wAfter = 0) {
		uniquePos = { x,y,w,h };
		gridPos = &uniquePos;
		assignedSprite = NULL;
		waitBefore = wBefore;
		waitAfter = wAfter;
		//bgTiles[gridPos->x + 8 + (gridPos->y + 8) * 100] = 0;
	}
	Event(Sprite* s, int wBefore = 0, int wAfter = 0) {
		gridPos = &s->gridPos;
		assignedSprite = s;
		waitBefore = wBefore;
		waitAfter = wAfter;
		//bgTiles[gridPos->x + 8 + (gridPos->y + 8) * 100] = 0;
	}
	virtual void handleCollision(Sprite* s) = 0;
	virtual Event* clone() const = 0;
	//private:
	SDL_Rect* gridPos;
	SDL_Rect uniquePos;
	Sprite* assignedSprite;
	int waitBefore, waitAfter;
};

class EventManagement {
public:
	class ActivatedEvent {
	public:
		ActivatedEvent(Event* event, Sprite* param) {
			this->event = event->clone();
			this->param = param;
		}
		~ActivatedEvent() {
			delete event;
		}
		Event* event;
		Sprite* param;
		bool executed = false;
	};
	static Event* addEvent(Event* newEvent, bool runNow = true, Sprite* param = NULL, unsigned char queueID = 0, bool checkCollisionNow = false) {
		lEvents.push_back(newEvent);
		if (runNow) {
			activateEvent(newEvent, param, queueID);
		}
		if (checkCollisionNow) {
			checkCollision(newEvent);

		}
		return newEvent;
	}
	static void checkCollision(Sprite* s, Event* e) {
		SDL_Rect* sPos = &s->gridPos, *ePos = 0;
		if (e->assignedSprite != NULL) {
			if (e->assignedSprite == s) return;
			ePos = &e->assignedSprite->gridPos;
		}
		else if (e->assignedSprite == NULL)
			ePos = &e->uniquePos;
		if ((sPos->x == ePos->x || sPos->x == ePos->x - 1) &&
			(sPos->y == ePos->y || sPos->y == ePos->y - 1))
			activateEvent(e, s);
		return;
	}
	static void checkCollision(Sprite* s) {
		for (std::list<Event*>::iterator it = lEvents.begin(); it != lEvents.end(); ++it) {
			checkCollision(s, *it);
		}
	}static void checkCollision(Event* e);
	/*static void checkCollision(Sprite* s) {
		Sprite* sTmp;
		for (std::list<Event*>::iterator it = lEvents.begin(); it != lEvents.end(); ++it) {
			sTmp = (*it)->assignedSprite;
			if (
				(sTmp != NULL && sTmp != s &&
					//((s->mapPos.x >= sTmp->mapPos.x - sTmp->mapPos.w / 2) && (s->mapPos.x <= sTmp->mapPos.x + sTmp->mapPos.w / 2)) &&
					//((s->mapPos.y + s->mapPos.h / 2 > sTmp->mapPos.y - sTmp->mapPos.h / 2) && (s->mapPos.y <= sTmp->mapPos.y + sTmp->mapPos.h)))
					//||
					//(sTmp == NULL &&
					//((s->gridPos.x >= (*it)->uniquePos.x - (*it)->uniquePos.w / 2) && (s->gridPos.x <= (*it)->uniquePos.x + (*it)->uniquePos.w / 2)) &&
					//((s->gridPos.y >= (*it)->uniquePos.y - (*it)->uniquePos.h / 2) && (s->gridPos.y <= (*it)->uniquePos.y + (*it)->uniquePos.h / 2)))
					//)
				(s->gridPos.x == sTmp->gridPos.x || s->gridPos.x == sTmp->gridPos.x - 1) &&
					(s->gridPos.y == sTmp->gridPos.y || s->gridPos.y == sTmp->gridPos.y - 1)) ||
					(sTmp == NULL &&
				(s->gridPos.x == (*it)->uniquePos.x || s->gridPos.x == (*it)->uniquePos.x - 1) &&
						(s->gridPos.y == (*it)->uniquePos.y || s->gridPos.y == (*it)->uniquePos.y - 1)))
				activateEvent(*it, s);
		}
	}*/
	static void activateEvent(Event* e, Sprite* s, unsigned char queueID = 0) {
		lEventActivationQueues[queueID].push(new ActivatedEvent(e, s));
	}
	static void delEvent(Event* e) {
		lEvents.erase(std::find(lEvents.begin(), lEvents.end(), e));
		delete e;
	}
	static void delAllEvents() {
		for (std::list<Event*>::iterator it = lEvents.begin(); it != lEvents.end(); ++it)
			delete *it;
		lEvents.clear();
		for (unsigned char i = 0; i < 4; ++i)
			while (!lEventActivationQueues[i].empty()) {
				delete lEventActivationQueues[i].front();
				lEventActivationQueues[i].pop();
			}
	}
	static std::list<Event*> lEvents;
	static std::queue<ActivatedEvent*> lEventActivationQueues[4];
	static void RunNextEvent() {
		for (unsigned char i = 0; i < 4; ++i) {
			if (lEventActivationQueues[i].size() > 0) {
				ActivatedEvent* curEv = lEventActivationQueues[i].front();
				if (curEv->event->waitBefore) --curEv->event->waitBefore;
				else if (!curEv->executed++)
					curEv->event->handleCollision(curEv->param);
				else if (curEv->event->waitAfter) --curEv->event->waitAfter;
				else {
					lEventActivationQueues[i].pop();
					delete curEv;
				}
			}
		}
	}
private:
	EventManagement() {};
};

extern void WarpSprite(Sprite* s, unsigned char destMapID, char destX, char destY);
class WarpEvent :public Event {
public:
	WarpEvent(char x, char y, unsigned char w, unsigned char h,
		unsigned char destMapID, char destX, char destY, int wBefore = 0, int wAfter = 0) :Event(x, y, w, h, wBefore, wAfter) {
		this->destMapID = destMapID;
		destPos = { destX,destY,0,0 };
	}
	WarpEvent(Sprite* s, unsigned char destMapID, char destX, char destY, int wBefore = 0, int wAfter = 0) :Event(s, wBefore, wAfter) {
		this->destMapID = destMapID;
		destPos = { destX,destY,0,0 };
	}
	void handleCollision(Sprite* s) {
		if (s == NULL) {
			printf("Warpevent ohne Sprite gestartet\n"); return;
		}
		printf("WarpEvent: destination = Map%d, x%d, y%d\n", destMapID, destPos.x, destPos.y);
		WarpSprite(s, destMapID, destPos.x, destPos.y);
	}
	WarpEvent* clone() const {
		if (assignedSprite != NULL)
			return new WarpEvent(assignedSprite, destMapID, destPos.x, destPos.y, waitBefore, waitAfter);
		return new WarpEvent(uniquePos.x, uniquePos.y, uniquePos.w, uniquePos.h, destMapID, destPos.x, destPos.y, waitBefore, waitAfter);
	}
private:
	unsigned char destMapID;
	SDL_Rect destPos;
};
class ChangeAnimEvent :public Event {
public:
	ChangeAnimEvent(char x, char y, unsigned char numAnims, std::vector<unsigned char>* anims, Sprite* movingSprite = NULL, bool autoDel = false, int wBefore = 0, int wAfter = 0) :Event(x, y, 1, 1, wBefore, wAfter) {
		this->movingSprite = movingSprite;
		setAutoDel = autoDel;
		this->numAnims = numAnims;
		this->anims = anims;
	}
	ChangeAnimEvent(Sprite* s, unsigned char numAnims, std::vector<unsigned char>* anims, Sprite* movingSprite = NULL, bool autoDel = false, int wBefore = 0, int wAfter = 0) :Event(s, wBefore, wAfter) {
		this->movingSprite = movingSprite;
		setAutoDel = autoDel;
		this->numAnims = numAnims;
		this->anims = anims;
	}
	void handleCollision(Sprite* s) {
		if (movingSprite != NULL) s = movingSprite;
		if (numAnims == 1)
			s->pushAnim((*anims)[0]);
		else {
			s->pushAnim(/*numAnims, */anims, true);
		}
		s->autoDelete = setAutoDel;
	}
	ChangeAnimEvent* clone() const {
		if (assignedSprite != NULL)
			return new ChangeAnimEvent(assignedSprite, numAnims, anims, movingSprite, setAutoDel, waitBefore, waitAfter);
		return new ChangeAnimEvent(uniquePos.x, uniquePos.y, numAnims, anims, movingSprite, setAutoDel, waitBefore, waitAfter);
	}
	~ChangeAnimEvent() {
		delete anims;
	}
private:
	Sprite* movingSprite;
	bool setAutoDel;
	unsigned char numAnims;
	std::vector<unsigned char>* anims;
	//unsigned char** anims;
};

class StateMachineTriggerEvent :public Event {
public:
	class MapScriptState {
	public:
		virtual void init() = 0;
		virtual void exit() = 0;
		virtual void handleEvents() = 0;
		static void changeState(unsigned char mapID, MapScriptState* newState) {
			if (mapScriptStates[mapID] != NULL) {
				mapScriptStates[mapID]->exit();
				delete mapScriptStates[mapID];
			}
			mapScriptStates[mapID] = newState;
			newState->init();
		}
		static MapScriptState* mapScriptStates[];
	};
	StateMachineTriggerEvent(char x, char y, unsigned char mapID, unsigned char eventFlagBitIndex, int wBefore = 0, int wAfter = 0) :Event(x, y, 0, 0, wBefore, wAfter) {
		this->eventFlagBitIndex = eventFlagBitIndex;
		this->mapID = mapID;
	}
	StateMachineTriggerEvent(Sprite* s, unsigned char mapID, unsigned char eventFlagBitIndex, int wBefore = 0, int wAfter = 0) :Event(s, wBefore, wAfter) {
		this->eventFlagBitIndex = eventFlagBitIndex;
		this->mapID = mapID;
	}
	void handleCollision(Sprite* s) {
		printf("Event\n");
		//if (assignedSprite != NULL) assignedSprite->autoDelete = true;
		mapEventFlagBitmap[mapID] |= 1 << eventFlagBitIndex;
		if (mapID == curMapID && MapScriptState::mapScriptStates[mapID] != NULL)
			MapScriptState::mapScriptStates[mapID]->handleEvents();
	}
	StateMachineTriggerEvent* clone() const {
		if (assignedSprite != NULL)
			return new StateMachineTriggerEvent(assignedSprite, mapID, eventFlagBitIndex, waitBefore, waitAfter);
		return new StateMachineTriggerEvent(uniquePos.x, uniquePos.y, mapID, eventFlagBitIndex, waitBefore, waitAfter);
	}
	static unsigned int mapEventFlagBitmap[];
	unsigned char mapID;
private:
	unsigned char eventFlagBitIndex;
};

class ChangeBGTileEvent :public Event {
public:
	ChangeBGTileEvent(unsigned char bgGridX, unsigned char bgGridY, unsigned char bgTileGridX, unsigned char bgTileGridY, unsigned char tileData, unsigned char walkData) :Event(bgGridX, bgGridY) {
		init(bgTileGridX, bgTileGridY, tileData, walkData);
	}
	ChangeBGTileEvent(Sprite* s, unsigned char bgTileGridX, unsigned char bgTileGridY, unsigned char tileData, unsigned char walkData) :Event(s, 0, 0) {
		init(bgTileGridX, bgTileGridY, tileData, walkData);
	}
	void handleCollision(Sprite* s);
	ChangeBGTileEvent* clone() const {
		if (assignedSprite != NULL)
			return new ChangeBGTileEvent(assignedSprite, tileGridX, tileGridY, tileData, walkData);
		return new ChangeBGTileEvent(uniquePos.x, uniquePos.y, tileGridX, tileGridY, tileData, walkData);
	}
private:
	void init(unsigned char bgTileGridX, unsigned char bgTileGridY, unsigned char tileData, unsigned char walkData) {
		tileGridX = bgTileGridX;
		tileGridY = bgTileGridY;
		this->tileData = tileData;
		this->walkData = walkData;
	}
	unsigned char tileGridX;
	unsigned char tileGridY;
	unsigned char tileData;
	unsigned char walkData;
};

class ChangeTimeEvent :public Event {
public:
	ChangeTimeEvent(unsigned char x, unsigned char y, unsigned char colRed, unsigned char colGreen, unsigned char colBlue, int wBefore = 0, int wAfter = 0) :Event(x, y, 0, 0, wBefore, wAfter) {
		this->colRed = colRed;
		this->colGreen = colGreen;
		this->colBlue = colBlue;
	}
private:
	unsigned char colRed;
	unsigned char colGreen;
	unsigned char colBlue;
	void handleCollision(Sprite* s);
	ChangeTimeEvent* clone() const {
		return new ChangeTimeEvent(gridPos->x, gridPos->y, colRed, colGreen, colBlue, waitBefore, waitAfter);
	}
};