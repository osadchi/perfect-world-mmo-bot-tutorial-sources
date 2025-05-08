#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <wx/xml/xml.h>
#include <wx/mstream.h>

#include "pwbot.h"


struct PWBotChar
{
    public:
        std::wstring login;
        std::wstring password;
        std::wstring role;
        uint32_t charid;
        std::wstring path_name;
};

struct PWBotPath
{
    public:
        std::wstring name;
        std::vector<std::vector<float>> path;
        std::vector<uint32_t> resource_types;
};

struct PWBotOffsets
{
    public:
        uint32_t socket;
        uint32_t charorigin;
        uint32_t charbase;
        uint32_t move;
        std::vector<uint32_t> movepoint;
        uint32_t movepointwalking;
        uint32_t movepointX;
        uint32_t movepointZ;
        uint32_t movepointY;
        uint32_t movepointXdiff;
        uint32_t movepointZdiff;
        uint32_t movepointYdiff;
        uint32_t mousepointedat;
        uint32_t targetedat;
        uint32_t charid;
        uint32_t ischarfree;
        uint32_t charhealth;
        uint32_t charmana;
        uint32_t charposX;
        uint32_t charposZ;
        uint32_t charposY;
        uint32_t charwalkspeed;
        uint32_t charflyspeed;
        uint32_t charflyapparatus;
        uint32_t charflystatus;
        uint32_t cameradistance;
        uint32_t resourceorigin;
        std::vector<uint32_t> resourceentity;
        uint32_t resourceobject;
        uint32_t resourceid;
        uint32_t resourcetype;
        uint32_t resourceposX;
        uint32_t resourceposZ;
        uint32_t resourceposY;
        uint32_t playersorigin;
        std::vector<uint32_t> playersfirstid;
};

class PWBotConfig : public wxXmlDocument
{
    private:
        std::wstring elcli__ = L"";
        std::vector<PWBotChar> chars__;
        std::vector<PWBotPath> paths__;
        PWBotOffsets offsets__ = {};
    private:
        std::wstring UInt32ToHexWstring(uint32_t value);
        void AddOffsetToDocument(wxXmlNode* node, std::wstring name, uint32_t value);
        void AddOffsetToDocument(wxXmlNode* node, std::wstring name, std::vector<uint32_t> values);
        std::vector<uint32_t> SpaceSeparatedStdWstringToUInt32List(std::wstring value);
    public:
        PWBotConfig();
        bool CreateEmpty();
        bool AppendChar(std::wstring login, std::wstring password, std::wstring charname, std::wstring path_name);
        void SetCharId(int i, uint32_t charid);
        void RemoveChar(std::wstring charname);
        bool AppendPath(std::wstring name, std::vector<std::vector<float>> path, std::vector<uint32_t> resource_types);
        const std::vector<PWBotChar>& GetChars();
        const std::vector<PWBotPath>& GetPaths();
        const PWBotOffsets& GetOffsets();
        const std::wstring& GetElCli();
        bool LoadConfig();
        bool UnloadConfig();
        ~PWBotConfig();
};

std::wstring PWBotConfig::UInt32ToHexWstring(uint32_t value)
{
    std::wstringstream ss;
    ss << std::hex << value;
    return L"0x" + ss.str();
}

void PWBotConfig::AddOffsetToDocument(wxXmlNode* node, std::wstring name, uint32_t value)
{
    wxXmlNode* offset = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Offset");
    offset->AddAttribute(name, UInt32ToHexWstring(value));
}

void PWBotConfig::AddOffsetToDocument(wxXmlNode* node, std::wstring name, std::vector<uint32_t> values)
{
    if (!values.size())
        return;
    std::vector<uint32_t> backward = values;
    std::reverse(backward.begin(), backward.end());
    for (uint32_t value : backward)
    {
        AddOffsetToDocument(node, name, value);
    }
}

std::vector<uint32_t> PWBotConfig::SpaceSeparatedStdWstringToUInt32List(std::wstring value)
{
    std::vector<uint32_t> res;
    uint32_t dec;
    std::wstring peace;
    std::wstringstream ss(value);
    while (std::getline(ss, peace, L' '))
    {
        std::wstringstream(peace) >> std::dec >> dec;
        res.push_back(dec);
    }
    return res;
}

PWBotConfig::PWBotConfig()
{
    if(!LoadConfig() && CreateEmpty())
    {
        MessageBoxW(nullptr, L"Пустой конфиг файл создан, пожалуйста, заполните его и перезапустите программу.", L"Первый запуск!", MB_OK);
    }
}

bool PWBotConfig::CreateEmpty()
{

    wxXmlNode* root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "Root");
    SetRoot(root);
    wxXmlNode* offsets = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Offsets");
    wxXmlNode* offset = new wxXmlNode(offsets, wxXML_ELEMENT_NODE, "Offset");
    offset->AddAttribute("name", "charbase");
    offset->AddAttribute("value", "0x08FBECCC");
    offset = new wxXmlNode(offsets, wxXML_ELEMENT_NODE, "Offset");
    offset->AddAttribute("name", "charbase");
    offset->AddAttribute("value", "0x04");
    wxXmlNode* chars = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Chars");
    wxXmlNode* character = new wxXmlNode(chars, wxXML_ELEMENT_NODE, "Char");
    character->AddAttribute("login", "your_login");
    character->AddAttribute("password", "1234567890qQ");
    character->AddAttribute("role", "char_name");
    character->AddAttribute("charid", "0x0");
    wxXmlNode* paths = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Paths");
    wxXmlNode* path = new wxXmlNode(paths, wxXML_ELEMENT_NODE, "Path");
    wxXmlNode* point = new wxXmlNode(path, wxXML_ELEMENT_NODE, "Point");
    point->AddAttribute("x", "-1800.98987");
    point->AddAttribute("z", "256.4594");
    point->AddAttribute("y", "567.4554");
    Save("config.xml");

    return true;
}

bool PWBotConfig::AppendChar(std::wstring login, std::wstring password, std::wstring charname, std::wstring path_name)
{

    if (login.empty())
        return false;
    
    if (password.empty())
        return false;
    
    if (charname.empty())
        return false;

    for (PWBotChar c : chars__)
    {
        if (c.login == login)
            return false;
    }

    PWBotChar c = {login, password, charname, 0, path_name};
    chars__.push_back(c);

    return true;

}

void PWBotConfig::SetCharId(int i, uint32_t charid)
{
    chars__[i].charid = charid;
}

void PWBotConfig::RemoveChar(std::wstring charname)
{

    for (std::vector<PWBotChar>::iterator it = chars__.begin(); it != chars__.end(); ++it)
    {
        if (it->role == charname)
        {
            chars__.erase(it);
            return;
        }
    }

}

bool PWBotConfig::AppendPath(std::wstring name, std::vector<std::vector<float>> path, std::vector<uint32_t> resource_types)
{
    if (name.empty())
        return false;
    
    if (!path.size())
        return false;

    for (std::vector<float> p_ : path)
    {
        if (p_.size() != 3)
            return false;
    }

    if (!resource_types.size())
        return false;

    PWBotPath p = {name, path, resource_types};
    paths__.push_back(p);

    return true;
}

const std::vector<PWBotChar>& PWBotConfig::GetChars()
{
    return chars__;
}

const std::vector<PWBotPath>& PWBotConfig::GetPaths()
{
    return paths__;
}

const PWBotOffsets& PWBotConfig::GetOffsets()
{
    return offsets__;
}

const std::wstring& PWBotConfig::GetElCli()
{
    return elcli__;
}

bool PWBotConfig::LoadConfig()
{
    if (!Load("config.xml", "UTF-8", wxXMLDOC_KEEP_WHITESPACE_NODES))
        return false;
    
    wxXmlNode* child = GetRoot()->GetChildren();
    while (child)
    {
        std::wstring name = child->GetName().ToStdWstring();
        if (name == L"ElementClient")
        {
            elcli__ = child->GetAttribute("path").ToStdWstring();
        }
        else if (name == L"Paths")
        {
            wxXmlNode* path = child->GetChildren();
            while (path)
            {
                if (path->GetName().ToStdWstring() != L"Path")
                {
                    path = path->GetNext();
                    continue;
                }

                PWBotPath p = {};
                wxXmlAttribute* attr = path->GetAttributes();
                while (attr)
                {
                    if (attr->GetName().ToStdWstring() == L"name")
                        p.name = attr->GetValue().ToStdWstring();
                    else if (attr->GetName().ToStdWstring() == L"resource_types")
                    {
                        for (uint32_t type : SpaceSeparatedStdWstringToUInt32List(attr->GetValue().ToStdWstring()))
                        {
                            if (type)
                                p.resource_types.push_back(type);
                        }
                    }

                    attr = attr->GetNext();
                }

                wxXmlNode* point = path->GetChildren();
                while (point)
                {
                    if (point->GetName().ToStdWstring() != L"Point")
                    {
                        point = point->GetNext();
                        continue;
                    }

                    float x=0,z=0,y=0;
                    attr = point->GetAttributes();
                    while (attr)
                    {
                        std::wstring axis = attr->GetName().ToStdWstring();
                        if (axis == L"x")
                        {
                            std::wstringstream(attr->GetValue().ToStdWstring()) >> x;
                        }
                        else if (axis == L"z")
                        {
                            std::wstringstream(attr->GetValue().ToStdWstring()) >> z;
                        }
                        else if (axis == L"y")
                        {
                            std::wstringstream(attr->GetValue().ToStdWstring()) >> y;
                        }
                        attr = attr->GetNext();
                    }
                    p.path.push_back({x,z,y});
                    point = point->GetNext();
                }
                paths__.push_back(p);
                path = path->GetNext();
            }
        }
        else if (name == L"Chars")
        {
            wxXmlNode* character = child->GetChildren();
            while (character)
            {
                if (character->GetName().ToStdWstring() != L"Char")
                {
                    character = character->GetNext();
                    continue;
                }

                PWBotChar c = {};
                wxXmlAttribute* attr = character->GetAttributes();
                while (attr)
                {
                    std::wstring name = attr->GetName().ToStdWstring();
                    if (name == L"login")
                    {
                        c.login = attr->GetValue().ToStdWstring();
                    }
                    else if (name == L"password")
                    {
                        c.password = attr->GetValue().ToStdWstring();
                    }
                    else if (name == L"role")
                    {
                        c.role = attr->GetValue().ToStdWstring();
                    }
                    else if (name == L"charid")
                    {
                        uint32_t charid;
                        std::wstringstream(attr->GetValue().ToStdWstring()) >> std::hex >> charid;
                        c.charid = charid;
                    }
                    else if (name == "path_name")
                    {
                        c.path_name = attr->GetValue().ToStdWstring();
                    }
                    attr = attr->GetNext();
                }
                chars__.push_back(c);
                character = character->GetNext();
            }
        }
        else if (name == L"Offsets")
        {
            wxXmlNode* offset = child->GetChildren();
            while (offset)
            {
                if (offset->GetName().ToStdWstring() != L"Offset")
                {
                    offset = offset->GetNext();
                    continue;
                }

                wxXmlAttribute* attr = offset->GetAttributes();
                while (attr)
                {
                    uint32_t val;
                    std::wstringstream(attr->GetValue().ToStdWstring()) >> std::hex >> val;
                    std::wstring name = attr->GetName().ToStdWstring();
                    if (name == L"socket")
                    {
                        offsets__.socket = val;
                    }
                    else if (name == L"charorigin")
                    {
                        offsets__.charorigin = val;
                    }
                    else if (name == L"charbase")
                    {
                        offsets__.charbase = val;
                    }
                    else if (name == L"move")
                    {
                        offsets__.move = val;
                    }
                    else if (name == L"movepoint")
                    {
                        offsets__.movepoint.push_back(val);
                    }
                    else if (name == L"movepointwalking")
                    {
                        offsets__.movepointwalking = val;
                    }
                    else if (name == L"movepointX")
                    {
                        offsets__.movepointX = val;
                    }
                    else if (name == L"movepointZ")
                    {
                        offsets__.movepointZ = val;
                    }
                    else if (name == L"movepointY")
                    {
                        offsets__.movepointY = val;
                    }
                    else if (name == L"movepointXdiff")
                    {
                        offsets__.movepointXdiff = val;
                    }
                    else if (name == L"movepointZdiff")
                    {
                        offsets__.movepointZdiff = val;
                    }
                    else if (name == L"movepointYdiff")
                    {
                        offsets__.movepointYdiff = val;
                    }
                    else if (name == L"mousepointedat")
                    {
                        offsets__.mousepointedat = val;
                    }
                    else if (name == L"targetedat")
                    {
                        offsets__.targetedat = val;
                    }
                    else if (name == L"charid")
                    {
                        offsets__.charid = val;
                    }
                    else if (name == L"ischarfree")
                    {
                        offsets__.ischarfree = val;
                    }
                    else if (name == L"charhealth")
                    {
                        offsets__.charhealth = val;
                    }
                    else if (name == L"charmana")
                    {
                        offsets__.charmana = val;
                    }
                    else if (name == L"charposX")
                    {
                        offsets__.charposX = val;
                    }
                    else if (name == L"charposZ")
                    {
                        offsets__.charposZ = val;
                    }
                    else if (name == L"charposY")
                    {
                        offsets__.charposY = val;
                    }
                    else if (name == L"charwalkspeed")
                    {
                        offsets__.charwalkspeed = val;
                    }
                    else if (name == L"charflyspeed")
                    {
                        offsets__.charflyspeed = val;
                    }
                    else if (name == L"charflyapparatus")
                    {
                        offsets__.charflyapparatus = val;
                    }
                    else if (name == L"charflystatus")
                    {
                        offsets__.charflystatus = val;
                    }
                    else if (name == L"cameradistance")
                    {
                        offsets__.cameradistance = val;
                    }
                    else if (name == L"resourceorigin")
                    {
                        offsets__.resourceorigin = val;
                    }
                    else if (name == L"resourceentity")
                    {
                        offsets__.resourceentity.push_back(val);
                    }
                    else if (name == L"resourceobject")
                    {
                        offsets__.resourceobject = val;
                    }
                    else if (name == L"resourceid")
                    {
                        offsets__.resourceid = val;
                    }
                    else if (name == L"resourcetype")
                    {
                        offsets__.resourcetype = val;
                    }
                    else if (name == L"resourceposX")
                    {
                        offsets__.resourceposX = val;
                    }
                    else if (name == L"resourceposZ")
                    {
                        offsets__.resourceposZ = val;
                    }
                    else if (name == L"resourceposY")
                    {
                        offsets__.resourceposY = val;
                    }
                    else if (name == L"playersorigin")
                    {
                        offsets__.playersorigin = val;
                    }
                    else if (name == L"playersfirstid")
                    {
                        offsets__.playersfirstid.push_back(val);
                    }
                    attr = attr->GetNext();
                }
                offset = offset->GetNext();
            }
        }

        child = child->GetNext();
    }

    return true;
}

bool PWBotConfig::UnloadConfig()
{
    delete DetachRoot();
    wxXmlNode* root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "Root");
    SetRoot(root);
    wxXmlNode* elcli;
    wxXmlNode* offsets;
    wxXmlNode* offset;
    wxXmlNode* chars;
    wxXmlNode* character;
    wxXmlNode* paths;
    wxXmlNode* path;
    wxXmlNode* point;

    elcli = new wxXmlNode(root, wxXML_ELEMENT_NODE, "ElementClient");
    elcli->AddAttribute("path", elcli__);

    offsets = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Offsets");
    AddOffsetToDocument(offsets, L"playersfirstid", offsets__.playersfirstid);
    AddOffsetToDocument(offsets, L"playersorigin", offsets__.playersorigin);
    AddOffsetToDocument(offsets, L"resourceposY", offsets__.resourceposY);
    AddOffsetToDocument(offsets, L"resourceposZ", offsets__.resourceposZ);
    AddOffsetToDocument(offsets, L"resourceposX", offsets__.resourceposX);
    AddOffsetToDocument(offsets, L"resourcetype", offsets__.resourcetype);
    AddOffsetToDocument(offsets, L"resourceid", offsets__.resourceid);
    AddOffsetToDocument(offsets, L"resourceobject", offsets__.resourceobject);
    AddOffsetToDocument(offsets, L"resourceentity", offsets__.resourceentity);
    AddOffsetToDocument(offsets, L"resourceorigin", offsets__.resourceorigin);
    AddOffsetToDocument(offsets, L"cameradistance", offsets__.cameradistance);
    AddOffsetToDocument(offsets, L"charflyspeed", offsets__.charflyspeed);
    AddOffsetToDocument(offsets, L"charflyapparatus", offsets__.charflyapparatus);
    AddOffsetToDocument(offsets, L"charflystatus", offsets__.charflystatus);
    AddOffsetToDocument(offsets, L"charwalkspeed", offsets__.charwalkspeed);
    AddOffsetToDocument(offsets, L"charposY", offsets__.charposY);
    AddOffsetToDocument(offsets, L"charposZ", offsets__.charposZ);
    AddOffsetToDocument(offsets, L"charposX", offsets__.charposX);
    AddOffsetToDocument(offsets, L"charmana", offsets__.charmana);
    AddOffsetToDocument(offsets, L"charhealth", offsets__.charhealth);
    AddOffsetToDocument(offsets, L"ischarfree", offsets__.ischarfree);
    AddOffsetToDocument(offsets, L"charid", offsets__.charid);
    AddOffsetToDocument(offsets, L"targetedat", offsets__.targetedat);
    AddOffsetToDocument(offsets, L"mousepointedat", offsets__.mousepointedat);
    AddOffsetToDocument(offsets, L"movepointYdiff", offsets__.movepointYdiff);
    AddOffsetToDocument(offsets, L"movepointXdiff", offsets__.movepointXdiff);
    AddOffsetToDocument(offsets, L"movepointZdiff", offsets__.movepointZdiff);
    AddOffsetToDocument(offsets, L"movepointY", offsets__.movepointY);
    AddOffsetToDocument(offsets, L"movepointZ", offsets__.movepointZ);
    AddOffsetToDocument(offsets, L"movepointX", offsets__.movepointX);
    AddOffsetToDocument(offsets, L"movepointwalking", offsets__.movepointwalking);
    AddOffsetToDocument(offsets, L"movepoint", offsets__.movepoint);
    AddOffsetToDocument(offsets, L"move", offsets__.move);
    AddOffsetToDocument(offsets, L"charbase", offsets__.charbase);
    AddOffsetToDocument(offsets, L"charorigin", offsets__.charorigin);
    AddOffsetToDocument(offsets, L"socket", offsets__.socket);

    chars = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Chars");
    for (PWBotChar c : chars__)
    {
        character = new wxXmlNode(chars, wxXML_ELEMENT_NODE, "Char");
        character->AddAttribute("login", c.login);
        character->AddAttribute("password", c.password);
        character->AddAttribute("role", c.role);
        character->AddAttribute("charid", UInt32ToHexWstring(c.charid));
        character->AddAttribute("path_name", c.path_name);
    }

    paths = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Paths");
    for (PWBotPath p : paths__)
    {
        path = new wxXmlNode(paths, wxXML_ELEMENT_NODE, "Path");
        path->AddAttribute("name", p.name);
        std::wstringstream ss;
        for (std::vector<uint32_t>::iterator it = p.resource_types.begin(); it != p.resource_types.end(); ++it)
        {
            ss << *it;
            if (it != p.resource_types.end()-1)
                ss << L" ";
        }
        path->AddAttribute("resource_types", ss.str());

        std::reverse(p.path.begin(),p.path.end());
        for (std::vector<float> pnt : p.path)
        {
            point = new wxXmlNode(path, wxXML_ELEMENT_NODE, "Point");
            point->AddAttribute("x", std::to_wstring(pnt[0]).c_str());
            point->AddAttribute("z", std::to_wstring(pnt[1]).c_str());
            point->AddAttribute("y", std::to_wstring(pnt[2]).c_str());
        }
    }

    if (!Save("config.xml"))
        return false;
    
    return true;
}

PWBotConfig::~PWBotConfig()
{
    UnloadConfig();
}