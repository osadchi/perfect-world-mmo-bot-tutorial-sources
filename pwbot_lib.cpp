#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include "pwbot.h"
#include "pwbot_config.h"


class PWBotLibrary
{
    private:
        PWBot* pwbot__ = nullptr;
        PWBotConfig* config__ = nullptr;
    private:
        void Send(const std::vector<unsigned char>& data);
        void Move(float x, float z, float y);
        void Dig(uint32_t id);
       float Distance(float x1, float x2, float z1, float z2, float y1, float y2);
        void HardExit();
    public:
        PWBotLibrary();
        void SetCameraDistance(float distance);
        void TurnFly(bool on = true);
        void ExecutePath();
};

void PWBotLibrary::HardExit()
{
    std::string s;
    s += "taskkill -f -pid " + std::to_string(GetCurrentProcessId());
    WinExec(s.c_str(), SW_HIDE);
}

void PWBotLibrary::Send(const std::vector<unsigned char>& data)
{
    typedef int __stdcall (*send_packet_t)(SOCKET s, char* data, int len, int flags);
    send_packet_t send_packet_f = ((send_packet_t)((uint32_t)GetModuleHandle(nullptr) + 0x4FAE07));
    
    SOCKET s = pwbot__->GetSocket();

    if (!s)
        return;
    
    if (!data.size())
        return;

    if (send_packet_f(s, (char*)&data[0], data.size(), 0) == SOCKET_ERROR)
        HardExit();
}

void PWBotLibrary::Move(float x, float z, float y)
{
    typedef int __thiscall (*init_move_t)(int, unsigned int*);
    init_move_t init_move_f = (init_move_t)((uint32_t)GetModuleHandle(nullptr) + 0x35A050);

    typedef int __thiscall (*get_move_obj_t)(DWORD*, int, int, char);
    get_move_obj_t get_move_obj_f = (get_move_obj_t)((uint32_t)GetModuleHandle(nullptr) + 0x368F50);

    typedef int __thiscall (*set_move_coords_t)(int, unsigned int, float*);
    set_move_coords_t set_move_coords_f = (set_move_coords_t)((uint32_t)GetModuleHandle(nullptr) + 0x36F0A0);

    std::srand(time(nullptr));
    unsigned int zeros[8] = {(unsigned int)(std::rand()%1000),0,0,(unsigned int)(std::rand()%1000),0,0,0,(unsigned int)(std::rand()%1000)};
    init_move_f(pwbot__->GetCharBase(), (unsigned int*)&zeros);

    int objmove = pwbot__->GetMove();
    if (!objmove)
        return;
    objmove = get_move_obj_f((DWORD*)objmove,1,1,0);
    if(!objmove)
        return;
    float coords[3] = {x,z,y};
    set_move_coords_f(objmove,0,(float*)coords);
}

void PWBotLibrary::Dig(uint32_t id)
{
    if (!id)
        return;
    
    unsigned char* d = reinterpret_cast<unsigned char*>(&id);
    std::vector<unsigned char> data = {0x22,0x13,0x12,0x36,0x00,d[0],d[1],d[2],d[3],0x00,0x00,0x08,0x00,0x01,0x0C,0x00,0x00,0x00,0x00,0x00,0x00};
    int tries = 0;
    while (tries < 10 && pwbot__->GetIsCharFree() && pwbot__->GetCharHealth())
    {
        ++tries;
        Send(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    while (!pwbot__->GetIsCharFree())
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

float PWBotLibrary::Distance(float x1, float x2, float z1, float z2, float y1, float y2)
{
    return std::sqrt( std::pow(x1-x2,2) + std::pow(z1-z2,2) + std::pow(y1-y2,2) );
}

void PWBotLibrary::SetCameraDistance(float distance)
{
    pwbot__->SetCameraDistance(distance);
}

void PWBotLibrary::TurnFly(bool on)
{
    short flyapparatus = pwbot__->GetFlyApparatus();
    unsigned char* flarr = reinterpret_cast<unsigned char*>(&flyapparatus);
    std::vector<unsigned char> data = {0x22,0x0B,0x0A,0x28,0x00,0x01,0x01,0x0C,0x00,flarr[0],flarr[1],0x00,0x00};
    while (pwbot__->GetFlyStatus() < 16 && on || pwbot__->GetFlyStatus() >= 16 && !on)
    {
        Send(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void PWBotLibrary::ExecutePath()
{
    uint32_t charid = pwbot__->GetCharId();
    std::wstring path_name;
    for (PWBotChar c : config__->GetChars())
    {
        if (c.charid == charid)
        {
            path_name = c.path_name;
            break;
        }
    }
    std::vector<std::vector<float>> path;
    std::vector<uint32_t> resource_types;
    for (PWBotPath p : config__->GetPaths())
    {
        if (p.name == path_name)
        {
            path = p.path;
            resource_types = p.resource_types;
            break;
        }
    }
    for (std::vector<float> dist : path)
    {
        float walkspeed = pwbot__->GetWalkSpeed();
        float flyspeed = pwbot__->GetFlySpeed();
        std::vector<float> pos = pwbot__->GetCharPos();
        auto mov_fn = [this,&walkspeed,&flyspeed](std::vector<float>& dist, std::vector<float>& pos)
        {
            while (Distance(dist[0], pos[0], dist[1], pos[1], dist[2], pos[2]) > 5.0f)
            {
                if (Distance(dist[0], pos[0], 0, 0, dist[2], pos[2]) <= 1.0f)
                {
                    if (dist[1] > pos[1])
                        TurnFly(true);
                    else
                        TurnFly(false);
                    Move(dist[0]-15.5f, dist[1], dist[2]+15.5f);
                    std::this_thread::sleep_for(std::chrono::milliseconds(600));
                }
                Move(dist[0], dist[1], dist[2]);
                pwbot__->SetMovePointZDiff( (std::abs(dist[1] - pos[1]) > flyspeed ? flyspeed * (dist[1] > pos[1] ? 1.0f : -1.0f) : dist[1] - pos[1]) );
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                pos = pwbot__->GetCharPos();
            }

        };
        mov_fn(dist, pos);
        for (uint32_t i = 0; i <= 0xC00/4; ++i)
        {
            uint32_t type = pwbot__->GetResourceType(i);
            if (!type || std::find(resource_types.begin(), resource_types.end(), type) == resource_types.end())
                continue;
            std::vector<float> r_pos = {pwbot__->GetResourcePosX(i), pwbot__->GetResourcePosZ(i), pwbot__->GetResourcePosY(i)};
            if (Distance(r_pos[0], pos[0], r_pos[1], pos[1], r_pos[2], pos[2]) > 25.0f)
                continue;
            mov_fn(r_pos, pos);
            Dig(pwbot__->GetResourceId(i));
        }
    }
}

PWBotLibrary::PWBotLibrary()
{
    config__ = new PWBotConfig();
    PWBotOffsets o = config__->GetOffsets();
    pwbot__ = new PWBot(GetCurrentProcess(), o.socket, o.charorigin, o.charbase, o.move, o.movepoint, o.movepointwalking, o.movepointX, o.movepointZ, o.movepointY, o.movepointXdiff, o.movepointZdiff, o.movepointYdiff, o.mousepointedat, o.targetedat, o.charid, o.ischarfree, o.charhealth, o.charmana, o.charposX, o.charposZ, o.charposY, o.charwalkspeed, o.charflyapparatus, o.charflyspeed, o.charflystatus, o.cameradistance, o.resourceorigin, o.resourceentity, o.resourceobject, o.resourceid, o.resourcetype, o.resourceposX, o.resourceposZ, o.resourceposY, o.playersorigin, o.playersfirstid);
}


DWORD WINAPI MainThread(LPVOID param)
{
    PWBotLibrary* pwblib = new PWBotLibrary();
    pwblib->SetCameraDistance(80.0f);
    pwblib->TurnFly();
    while (1)
    {
        pwblib->ExecutePath();
    }
    FreeLibraryAndExitThread((HMODULE)param, 0);
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    default:
        break;
    }
    return TRUE;
}

