#ifndef OBJ_LAUNCHER_H
#define OBJ_LAUNCHER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLauncher : Object{

};

// Entity Class
struct EntityLauncher : Entity {

};

// Object Struct
extern ObjectLauncher *Launcher;

// Standard Entity Events
void Launcher_Update();
void Launcher_LateUpdate();
void Launcher_StaticUpdate();
void Launcher_Draw();
void Launcher_Create(void* data);
void Launcher_StageLoad();
void Launcher_EditorDraw();
void Launcher_EditorLoad();
void Launcher_Serialize();

// Extra Entity Functions


#endif //!OBJ_LAUNCHER_H