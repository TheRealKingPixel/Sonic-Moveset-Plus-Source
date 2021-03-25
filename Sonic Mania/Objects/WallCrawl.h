#ifndef OBJ_WALLCRAWL_H
#define OBJ_WALLCRAWL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWallCrawl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWallCrawl;

// Object Entity
extern ObjectWallCrawl *WallCrawl;

// Standard Entity Events
void WallCrawl_Update();
void WallCrawl_LateUpdate();
void WallCrawl_StaticUpdate();
void WallCrawl_Draw();
void WallCrawl_Create(void* data);
void WallCrawl_StageLoad();
void WallCrawl_EditorDraw();
void WallCrawl_EditorLoad();
void WallCrawl_Serialize();

// Extra Entity Functions


#endif //!OBJ_WALLCRAWL_H