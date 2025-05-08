#pragma once

#include <psapi.h>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

struct MemPiece
{
    HANDLE handle = 0;
    uint32_t address = 0;

    void read(uint32_t& ref)
    {
        if (!ReadProcessMemory(handle, (BYTE*)address, &ref, sizeof(uint32_t), 0))
            ref = 0;
    }

    void read(float& ref)
    {
        if (!ReadProcessMemory(handle, (BYTE*)address, &ref, sizeof(float), 0))
            ref = 0;
    }

    void read(short& ref)
    {
        if (!ReadProcessMemory(handle, (BYTE*)address, &ref, sizeof(short), 0))
            ref = 0;
    }

    void read(char& ref)
    {
        if (!ReadProcessMemory(handle, (BYTE*)address, &ref, sizeof(char), 0))
            ref = 0;
    }

    void write(const uint32_t& val)
    {
        WriteProcessMemory(handle, (BYTE*)address, (void*)&val, sizeof(uint32_t), 0);
    }

    void write(const float& val)
    {
        WriteProcessMemory(handle, (BYTE*)address, (void*)&val, sizeof(float), 0);
    }

    void write(const short& val)
    {
        WriteProcessMemory(handle, (BYTE*)address, (void*)&val, sizeof(short), 0);
    }

    void write(const char& val)
    {
        WriteProcessMemory(handle, (BYTE*)address, (void*)&val, sizeof(char), 0);
    }

    MemPiece& operator+=(const uint32_t& offset)
    {
        if (!address)
        {
            HMODULE* module_arr;
            LPBYTE module_arr_bytes;
            DWORD bytes_required;

            if (!handle)
                handle = GetCurrentProcess();
            if (EnumProcessModules(handle, NULL, 0, &bytes_required))
            {
                if (bytes_required)
                {
                    module_arr_bytes = (LPBYTE)LocalAlloc(LPTR, bytes_required);
                    if (module_arr_bytes)
                    {
                        unsigned int module_count = bytes_required / sizeof(HMODULE);
                        module_arr = (HMODULE*)module_arr_bytes;

                        if (EnumProcessModules(handle, module_arr, bytes_required, &bytes_required))
                        {
                            address = (uint32_t) module_arr[0];
                        }
                        LocalFree(module_arr_bytes);
                    }
                }
            }
            address += offset;
            return *this;
        }

        if (!ReadProcessMemory(handle, (BYTE*)address, &address, sizeof(uint32_t), 0))
            address = 0;
        else
            address += offset;
        return *this;
    }

    MemPiece& operator+=(const std::vector<uint32_t>& offsets)
    {
        for (uint32_t offset : offsets)
        {
            *this += offset;
        }

        return *this;
    }
};



class PWBot
{
    private:
        HANDLE handle__ = 0;
    private:
        uint32_t socket__;
        uint32_t charorigin__;
        uint32_t charbase__;
        uint32_t move__;
        std::vector<uint32_t> movepoint__;
        uint32_t movepointwalking__;
        uint32_t movepointX__;
        uint32_t movepointZ__;
        uint32_t movepointY__;
        uint32_t movepointXdiff__;
        uint32_t movepointZdiff__;
        uint32_t movepointYdiff__;
        uint32_t mousepointedat__;
        uint32_t targetedat__;
        uint32_t charid__;
        uint32_t ischarfree__;
        uint32_t charhealth__;
        uint32_t charmana__;
        uint32_t charposX__;
        uint32_t charposZ__;
        uint32_t charposY__;
        uint32_t charwalkspeed__;
        uint32_t charflyapparatus__;
        uint32_t charflyspeed__;
        uint32_t charflystatus__;
        uint32_t cameradistance__;
        uint32_t resourceorigin__;
        std::vector<uint32_t> resourceentity__;
        uint32_t resourceobject__;
        uint32_t resourceid__;
        uint32_t resourcetype__;
        uint32_t resourceposX__;
        uint32_t resourceposZ__;
        uint32_t resourceposY__;
        uint32_t playersorigin__;
        std::vector<uint32_t> playersfirstid__;
    public:
        PWBot(HANDLE handle, uint32_t socket, uint32_t charorigin, uint32_t charbase, uint32_t move, std::vector<uint32_t> movepoint, uint32_t movepointwaking, uint32_t movepointX, uint32_t movepointZ, uint32_t movepointY, uint32_t movepointXdiff, uint32_t movepointZdiff, uint32_t movepointYdiff, uint32_t mousepointedat, uint32_t targetedat, uint32_t charid, uint32_t ischarfree, uint32_t charhealth, uint32_t charmana, uint32_t charposX, uint32_t charposZ, uint32_t charposY, uint32_t charwalkspeed, uint32_t charflyapparatus, uint32_t charflyspeed, uint32_t charflystatus, uint32_t cameradistance, uint32_t resourceorigin, std::vector<uint32_t> resourceentity, uint32_t resourceobject, uint32_t resourceid, uint32_t resourcetype, uint32_t resourceposX, uint32_t resourceposZ, uint32_t resourceposY, uint32_t playersorigin, std::vector<uint32_t> playersfirstid);
    public:
          SOCKET GetSocket();
        uint32_t GetCharOrigin();
        uint32_t GetCharBase();
        uint32_t GetMove();
        uint32_t GetMovePoint();
        uint32_t GetMovePointWalking();
           float GetMovePointX();
           float GetMovePointZ();
           float GetMovePointY();
        uint32_t GetCharId();
            char GetIsCharFree();
        uint32_t GetCharHealth();
            char GetFlyStatus();
           short GetFlyApparatus();
           float GetWalkSpeed();
           float GetFlySpeed();
           float GetCharPosX();
           float GetCharPosZ();
           float GetCharPosY();
        uint32_t GetResourceId(uint32_t i);
        uint32_t GetResourceType(uint32_t i);
           float GetResourcePosX(uint32_t i);
           float GetResourcePosZ(uint32_t i);
           float GetResourcePosY(uint32_t i);
           void SetMovePointZDiff(float diff);
           void SetWalkSpeed(float speed);
           void SetFlySpeed(float speed);
           void SetCameraDistance(float distance);
        std::vector<float> GetCharPos();
        std::vector<uint32_t> GetNearResourceTypes();

};

PWBot::PWBot(HANDLE handle, uint32_t socket, uint32_t charorigin, uint32_t charbase, uint32_t move, std::vector<uint32_t> movepoint, uint32_t movepointwalking, uint32_t movepointX, uint32_t movepointZ, uint32_t movepointY, uint32_t movepointXdiff, uint32_t movepointZdiff, uint32_t movepointYdiff, uint32_t mousepointedat, uint32_t targetedat, uint32_t charid, uint32_t ischarfree, uint32_t charhealth, uint32_t charmana, uint32_t charposX, uint32_t charposZ, uint32_t charposY, uint32_t charwalkspeed, uint32_t charflyapparatus, uint32_t charflyspeed, uint32_t charflystatus, uint32_t cameradistance, uint32_t resourceorigin, std::vector<uint32_t> resourceentity, uint32_t resourceobject, uint32_t resourceid, uint32_t resourcetype, uint32_t resourceposX, uint32_t resourceposZ, uint32_t resourceposY, uint32_t playersorigin, std::vector<uint32_t> playersfirstid)
{
    handle__ = handle;
    socket__ = socket;
    charorigin__ = charorigin;
    charbase__ = charbase;
    move__ = move;
    movepoint__ = movepoint;
    movepointwalking__ = movepointwalking;
    movepointX__ = movepointX;
    movepointZ__ = movepointZ;
    movepointY__ = movepointY;
    movepointXdiff__ = movepointXdiff;
    movepointZdiff__ = movepointZdiff;
    movepointYdiff__ = movepointYdiff;
    mousepointedat__ = mousepointedat;
    targetedat__ = targetedat;
    charid__ = charid;
    ischarfree__ = ischarfree;
    charhealth__ = charhealth;
    charmana__ = charmana;
    charposX__ = charposX;
    charposZ__ = charposZ;
    charposY__ = charposY;
    charwalkspeed__ = charwalkspeed;
    charflyapparatus__ = charflyapparatus;
    charflyspeed__ = charflyspeed;
    charflystatus__ = charflystatus;
    cameradistance__ = cameradistance;
    resourceorigin__ = resourceorigin;
    resourceentity__ = resourceentity;
    resourceobject__ = resourceobject;
    resourceid__ = resourceid;
    resourcetype__ = resourcetype;
    resourceposX__ = resourceposX;
    resourceposZ__ = resourceposZ;
    resourceposY__ = resourceposY;
    playersorigin__ = playersorigin;
    playersfirstid__ = playersfirstid;
}

SOCKET PWBot::GetSocket()
{
    MemPiece m = {};
    m.handle = handle__;
    m += socket__;
    uint32_t socket = 0;
    m.read(socket);
    return (SOCKET)socket;
}

uint32_t PWBot::GetCharOrigin()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    uint32_t charorigin = 0;
    m.read(charorigin);
    return charorigin;
}

uint32_t PWBot::GetCharBase()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    uint32_t charbase = 0;
    m.read(charbase);
    return charbase;
}

uint32_t PWBot::GetMove()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    uint32_t move = 0;
    m.read(move);
    return move;
}

uint32_t PWBot::GetMovePoint()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    uint32_t movepoint = 0;
    m.read(movepoint);
    return movepoint;
}

uint32_t PWBot::GetMovePointWalking()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    m += movepointwalking__;
    uint32_t movepointwalking = 0;
    m.read(movepointwalking);
    return movepointwalking;
}

float PWBot::GetMovePointX()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    m += movepointwalking__;
    m += movepointX__;
    float movepointX = 0;
    m.read(movepointX);
    return movepointX;
}

float PWBot::GetMovePointZ()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    m += movepointwalking__;
    m += movepointZ__;
    float movepointZ = 0;
    m.read(movepointZ);
    return movepointZ;
}

float PWBot::GetMovePointY()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    m += movepointwalking__;
    m += movepointY__;
    float movepointY = 0;
    m.read(movepointY);
    return movepointY;
}

uint32_t PWBot::GetCharId()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charid__;
    uint32_t charid = 0;
    m.read(charid);
    return charid;
}

char PWBot::GetIsCharFree()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += ischarfree__;
    char ischarfree = 0;
    m.read(ischarfree);
    return ischarfree;
}

uint32_t PWBot::GetCharHealth()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charhealth__;
    uint32_t charhealth = 0;
    m.read(charhealth);
    return charhealth;
}

char PWBot::GetFlyStatus()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charflystatus__;
    char status = 0;
    m.read(status);
    return status;
}

short PWBot::GetFlyApparatus()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charflyapparatus__;
    short apparatus = 0;
    m.read(apparatus);
    return apparatus;
}

float PWBot::GetWalkSpeed()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charwalkspeed__;
    float speed = 0;
    m.read(speed);
    return speed;
}

float PWBot::GetFlySpeed()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charflyspeed__;
    float speed = 0;
    m.read(speed);
    return speed;
}

float PWBot::GetCharPosX()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charposX__;
    float charposX = 0;
    m.read(charposX);
    return charposX;
}

float PWBot::GetCharPosZ()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charposZ__;
    float charposZ = 0;
    m.read(charposZ);
    return charposZ;
}

float PWBot::GetCharPosY()
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charposY__;
    float charposY = 0;
    m.read(charposY);
    return charposY;
}

uint32_t PWBot::GetResourceId(uint32_t i)
{
    MemPiece m = {};
    m.handle = handle__;
    m += resourceorigin__;
    m += resourceentity__;
    m += i*0x04;
    m += 0;
    if (!m.address)
        return 0;
    m.address += resourceobject__;
    m += resourceid__;
    uint32_t resourceid = 0;
    m.read(resourceid);
    return resourceid;
}

uint32_t PWBot::GetResourceType(uint32_t i)
{
    MemPiece m = {};
    m.handle = handle__;
    m += resourceorigin__;
    m += resourceentity__;
    m += i*0x04;
    m += 0;
    if (!m.address)
        return 0;
    m.address += resourceobject__;
    m += resourcetype__;
    uint32_t resourcetype = 0;
    m.read(resourcetype);
    return resourcetype;
}

float PWBot::GetResourcePosX(uint32_t i)
{
    MemPiece m = {};
    m.handle = handle__;
    m += resourceorigin__;
    m += resourceentity__;
    m += i*0x04;
    m += 0;
    if (!m.address)
        return 0;
    m.address += resourceobject__;
    m += resourceposX__;
    float resourceposX = 0;
    m.read(resourceposX);
    return resourceposX;
}

float PWBot::GetResourcePosZ(uint32_t i)
{
    MemPiece m = {};
    m.handle = handle__;
    m += resourceorigin__;
    m += resourceentity__;
    m += i*0x04;
    m += 0;
    if (!m.address)
        return 0;
    m.address += resourceobject__;
    m += resourceposZ__;
    float resourceposZ = 0;
    m.read(resourceposZ);
    return resourceposZ;
}

float PWBot::GetResourcePosY(uint32_t i)
{
    MemPiece m = {};
    m.handle = handle__;
    m += resourceorigin__;
    m += resourceentity__;
    m += i*0x04;
    m += 0;
    if (!m.address)
        return 0;
    m.address += resourceobject__;
    m += resourceposY__;
    float resourceposY = 0;
    m.read(resourceposY);
    return resourceposY;
}

void PWBot::SetMovePointZDiff(float diff)
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += move__;
    m += movepoint__;
    m += movepointwalking__;
    m += movepointZdiff__;
    m.write(diff);
}

void PWBot::SetWalkSpeed(float speed)
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charwalkspeed__;
    m.write(speed);
}

void PWBot::SetFlySpeed(float speed)
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += charflyspeed__;
    m.write(speed);
}

void PWBot::SetCameraDistance(float distance)
{
    MemPiece m = {};
    m.handle = handle__;
    m += charorigin__;
    m += charbase__;
    m += cameradistance__;
    m.write(distance);
}

std::vector<float> PWBot::GetCharPos()
{
    std::vector<float> pos;
    pos.push_back(GetCharPosX());
    pos.push_back(GetCharPosZ());
    pos.push_back(GetCharPosY());
    return pos;
}

std::vector<uint32_t> PWBot::GetNearResourceTypes()
{
    std::vector<uint32_t> types;
    for (uint32_t i = 0; i <= 0xC00/4; ++i)
    {
        uint32_t type = GetResourceType(i);
        if (type && std::find(types.begin(), types.end(), type) == types.end())
            types.push_back(type);
    }
    return types;
}
