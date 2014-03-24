#ifndef PACKETTYPES_H_INCLUDED
#define PACKETTYPES_H_INCLUDED

enum
{
    Login,
    CharacterSpawned,
    CharacterDied,
    PlayerInputed,
    CharacterInputs,
    CreateEntities
};

typedef unsigned char PacketID;

#endif // PACKETTYPES_H_INCLUDED
