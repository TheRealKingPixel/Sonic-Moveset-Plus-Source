#ifndef OBJ_MATHHELPERS_H
#define OBJ_MATHHELPERS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMathHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMathHelpers;

// Object Entity
extern ObjectMathHelpers *MathHelpers;

// Standard Entity Events
void MathHelpers_Update(void);
void MathHelpers_LateUpdate(void);
void MathHelpers_StaticUpdate(void);
void MathHelpers_Draw(void);
void MathHelpers_Create(void* data);
void MathHelpers_StageLoad(void);
void MathHelpers_EditorDraw(void);
void MathHelpers_EditorLoad(void);
void MathHelpers_Serialize(void);

// Extra Entity Functions
void MathHelpers_Unknown1(Vector2 *pos, int a1, int a3, int a4, int a5, int a6);
void MathHelpers_Unknown2(Vector2 *pos, int a1, int a3, int a4, int a5, int a6);
void MathHelpers_Unknown3(Vector2 *pos, int a1, int a3, int a4, int a5, int a6);
void MathHelpers_Unknown4(Vector2 *pos, int a1, int a3, int a4, int a5, int a6);
Vector2 MathHelpers_Unknown5(int percent, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
uint MathHelpers_Unknown6(uint a1);
int MathHelpers_Unknown7(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
bool32 MathHelpers_PointInHitbox(int direction, int x1, int y1, Hitbox *hitbox, int x2, int y2);
bool32 MathHelpers_Unknown9(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2);
int MathHelpers_Unknown10(int px1, int py1, int px2, int py2, int tx1, int tx2);
bool32 MathHelpers_Unknown11(int tx1, int tx2, int ty1, int ty2, int px2, int py2);
int MathHelpers_Unknown12(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2);
int MathHelpers_Unknown13(int a1, int a2);

#endif //!OBJ_MATHHELPERS_H