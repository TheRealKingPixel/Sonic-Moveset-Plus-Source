#include "RetroEngine.hpp"

SpriteAnimation spriteAnimationList[SPRFILE_COUNT];

short LoadAnimation(const char *filename, Scopes scope)
{
    char buffer[0x100];
    FileInfo info;
    StrCopy(buffer, "Data/Sprites/");
    StrAdd(buffer, filename);

    uint hash[4];
    GEN_HASH(filename, hash);

    for (int i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH(spriteAnimationList[i].hash, hash)) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SPRFILE_COUNT; ++id) {
        if (spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SPRFILE_COUNT)
        return -1;

    char nameBuffer[0x8][0x20];
    byte sheetIDs[0x18];
    sheetIDs[0] = 0;
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x525053) {
            CloseFile(&info);
            return -1;
        }

        SpriteAnimation *spr = &spriteAnimationList[id];
        spr->scope           = scope;
        memcpy(spr->hash, hash, 4 * sizeof(uint));

        uint frameCount = ReadInt32(&info);
        AllocateStorage(frameCount * sizeof(SpriteFrame), (void**)&spr->frames, DATASET_STG, false);

        byte sheetCount = ReadInt8(&info);
        for (int s = 0; s < sheetCount; ++s) {
            ReadString(&info, buffer);
            sheetIDs[s] = LoadSpriteSheet(buffer, scope);
        }

        byte hitboxCount = ReadInt8(&info);
        for (int h = 0; h < hitboxCount; ++h) {
            ReadString(&info, nameBuffer[h]);
        }

        spr->animCount = ReadInt16(&info);
        AllocateStorage(spr->animCount * sizeof(SpriteAnimationEntry), (void **)&spr->animations, DATASET_STG, false);
        int frameID = 0;
        for (int a = 0; a < spr->animCount; ++a) {
            SpriteAnimationEntry *animation = &spr->animations[a];
            ReadString(&info, hashBuffer);
            GenerateHash(animation->hash, StrLength(hashBuffer));

            animation->frameCount      = ReadInt16(&info);
            animation->frameListOffset = frameID;
            animation->animationSpeed  = ReadInt16(&info);
            animation->loopIndex       = ReadInt8(&info);
            animation->rotationFlag    = ReadInt8(&info);

            for (int f = 0; f < animation->frameCount; ++f) {
                SpriteFrame *frame = &spr->frames[frameID++];
                frame->sheetID     = sheetIDs[ReadInt8(&info)];
                frame->delay       = ReadInt16(&info);
                frame->id          = ReadInt16(&info);
                frame->sprX        = ReadInt16(&info);
                frame->sprY        = ReadInt16(&info);
                frame->width       = ReadInt16(&info);
                frame->height      = ReadInt16(&info);
                frame->pivotX      = ReadInt16(&info);
                frame->pivotY      = ReadInt16(&info);

                frame->hitboxCnt = hitboxCount;
                for (int h = 0; h < hitboxCount; ++h) {
                    frame->hitboxes[h].left   = ReadInt16(&info);
                    frame->hitboxes[h].top    = ReadInt16(&info);
                    frame->hitboxes[h].right  = ReadInt16(&info);
                    frame->hitboxes[h].bottom = ReadInt16(&info);
                }
            }
        }

        CloseFile(&info);

        return id;
    }
    return -1;
}

short CreateAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope)
{
    char buffer[0x100];
    StrCopy(buffer, "Data/Sprites/");
    StrAdd(buffer, filename);

    uint hash[4];
    GEN_HASH(filename, hash);

    for (int i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH(spriteAnimationList[i].hash, hash)) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SPRFILE_COUNT; ++id) {
        if (spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SPRFILE_COUNT)
        return -1;

    SpriteAnimation *spr = &spriteAnimationList[id];
    spr->scope           = scope;
    memcpy(spr->hash, hash, 4 * sizeof(uint));

    AllocateStorage(sizeof(SpriteFrame) * (frameCount > 0x400 ? 0x400 : frameCount), (void **)&spr->frames, DATASET_STG, true);
    AllocateStorage(sizeof(SpriteAnimationEntry) * (animCount > 0x40 ? 0x40 : animCount), (void **)&spr->animations, DATASET_STG, true);

    return id;
}

void ProcessAnimation(EntityAnimationData *data)
{
    if (data) {
        if (data->framePtrs) {
            data->animationTimer += data->animationSpeed;
            if (data->framePtrs == (SpriteFrame *)1) {
                int delay = data->frameDelay;
                while (data->animationTimer < delay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - delay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                }
            }
            else {
                while (data->animationTimer < data->frameDelay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - data->frameDelay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                    data->frameDelay = data->framePtrs[data->frameID].delay;
                }
            }
        }
    }
}

int GetStringWidth(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length, int spacing)
{
    if (sprIndex >= SPRFILE_COUNT)
        return 0;
    if (!info)
        return 0;

    SpriteAnimation *spr       = &spriteAnimationList[sprIndex];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        if (startIndex >= 0) {
            if (startIndex >= info->textLength)
                startIndex = info->textLength - 1;
        }
        else {
            startIndex = 0;
        }

        if (length > 0) {
            if (length > info->textLength)
                length = info->textLength;
        }
        else {
            length = info->textLength;
        }

        int w = 0;
        for (int c = startIndex; c < length; ++c) {
            int charFrame = info->text[c];
            if (charFrame < anim->frameCount) {
                w += spr->frames[charFrame + anim->frameListOffset].width;
                if (c + 1 >= length)
                    return w;
                w += spacing;
            }
        }
        return w;
    }
    return 0;
}

void SetSpriteString(ushort spriteIndex, ushort animID, TextInfo* info) {
    if (spriteIndex >= SPRFILE_COUNT)
        return;
    if (!info)
        return;

    
    SpriteAnimation *spr = &spriteAnimationList[spriteIndex];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        for (int c = 0; c < info->textLength; ++c) {
            int charVal = info->text[c];
            info->text[c] = -1;
            for (int f = 0; f < anim->frameCount; ++f) {
                if (spr->frames[f + anim->frameListOffset].id == charVal) {
                    info->text[c] = f;
                    break;
                }
            }
        }
    }
}