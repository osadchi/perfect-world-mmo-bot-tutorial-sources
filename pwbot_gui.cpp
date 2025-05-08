#define __WIN32_WINNT 0x0600

#include "pwbot_config.h"

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <thread>
#include <chrono>
#include <pthread.h>

struct ElementClient
{
    int i;
    HANDLE handle;
};

class PWBotGUI : public wxApp
{
    private:
        wxFrame* frame__ = nullptr;
        wxScrolled<wxWindow>* scrAccountList__ = nullptr;
        wxTextCtrl* fldLogin__ = nullptr;
        wxTextCtrl* fldPassword__ = nullptr;
        wxTextCtrl* fldName__ = nullptr;
        wxChoice* cmbxPaths__ = nullptr;
    private:
        PWBotConfig* config__ = nullptr;
        std::vector<ElementClient> elclies__ = {};
        HANDLE bindedto__ = nullptr;
        bool isrecording__ = false;
    public:
        virtual bool OnInit();
        virtual ~PWBotGUI();
    public:
        void UpdateAccountList();
        void AppendAccount(wxCommandEvent& e);
        void StartStop(wxCommandEvent& e);
        void Remove(wxCommandEvent& e);
        void CreatePath(wxCommandEvent& e);
        void CreatePathCliSel(wxCommandEvent& e);
        void RecordPath(wxCommandEvent& e);
        void CreatePathClose(wxCloseEvent& e);
};

bool PWBotGUI::OnInit()
{
    config__ = new PWBotConfig();
    // Создаем окно нашего бота
    frame__ = new wxFrame(nullptr, wxID_ANY, "PWBot", wxPoint(5, 5), wxSize(250, 420), wxCAPTION|wxSTAY_ON_TOP|wxCLOSE_BOX);
    frame__->Show(true);
    // Создает панель с Табами
    wxNotebook* tabs = new wxNotebook(frame__, wxID_ANY, wxDefaultPosition, frame__->GetClientSize());
    // СОздаем панельки
    wxPanel* pnlControl = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, tabs->GetClientSize());
    wxPanel* pnlSettings = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, tabs->GetClientSize());
    // Устанавливаем панели в табы
    tabs->InsertPage(0, pnlControl, wxString::FromUTF8("Управление"));
    tabs->InsertPage(1, pnlSettings, wxString::FromUTF8("Настройки"));
    // Создаем окно с вертикальной прокруткой.
    scrAccountList__ = new wxScrolled<wxWindow>(pnlControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL);
    UpdateAccountList();
    wxSizer* szV = new wxBoxSizer(wxVERTICAL);
    wxSizer* szH;

    wxButton* btnStart = new wxButton(pnlControl, wxID_ANY, wxString::FromUTF8("Запустить все"));
    wxButton* btnStop = new wxButton(pnlControl, wxID_ANY, wxString::FromUTF8("Остановить все"));
    szH = new wxBoxSizer(wxHORIZONTAL);
    szH->Add(btnStop, 0, wxEXPAND|wxALL, 8);
    szH->Add(btnStart, 0, wxEXPAND|wxALL, 8);

    szV = new wxBoxSizer(wxVERTICAL);
    szV->Add(scrAccountList__, 1, wxEXPAND|wxALL, 16);
    szV->Add(szH);
    pnlControl->SetSizer(szV);
    pnlControl->Layout();


    szV = new wxBoxSizer(wxVERTICAL);
    fldLogin__ = new wxTextCtrl(pnlSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    fldPassword__ = new wxTextCtrl(pnlSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    fldName__ = new wxTextCtrl(pnlSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    wxButton* btnAdd = new wxButton(pnlSettings, wxID_ANY, wxString::FromUTF8("Добавить"));
    btnAdd->Bind(wxEVT_BUTTON, &PWBotGUI::AppendAccount, this);
    cmbxPaths__ = new wxChoice(pnlSettings, wxID_ANY);
    wxButton* btnCreatePath = new wxButton(pnlSettings, wxID_ANY, wxString::FromUTF8("Создать новый маршрут"));
    btnCreatePath->Bind(wxEVT_BUTTON, &PWBotGUI::CreatePath, this);


    fldLogin__->SetHint(wxString::FromUTF8("Логин"));
    fldPassword__->SetHint(wxString::FromUTF8("Пароль"));
    fldName__->SetHint(wxString::FromUTF8("Имя персонажа"));
    for (PWBotPath path : config__->GetPaths())
    {
        cmbxPaths__->Append(path.name);
    }

    szV->Add(fldLogin__, 0, wxEXPAND| wxTOP, 8);
    szV->Add(fldPassword__, 0, wxEXPAND|wxTOP, 8);
    szV->Add(fldName__, 0, wxEXPAND|wxTOP, 8);
    szV->AddSpacer(5);
    szH = new wxBoxSizer(wxHORIZONTAL);
    szH->Add(btnAdd, 0);
    szH->AddSpacer(6);
    szH->Add(cmbxPaths__, 1);
    szV->Add(szH, 0, wxEXPAND);
    szV->AddStretchSpacer();
    szV->Add(btnCreatePath, 0, wxALIGN_CENTER);
    szV->AddStretchSpacer();
    pnlSettings->SetSizer(szV);
    pnlSettings->Layout();

    return true;
}

void PWBotGUI::UpdateAccountList()
{
    for (auto el : scrAccountList__->GetChildren())
    {
        scrAccountList__->RemoveChild(el);
    }
    if (wxSizer* sz = scrAccountList__->GetSizer())
    {
        sz->Clear(true);
        scrAccountList__->SetSizer(nullptr, true);
    }
    wxSizer* szV = new wxBoxSizer(wxVERTICAL);
    wxSizer* szH;
    const std::vector<PWBotChar>& chars = config__->GetChars();
    for (int i = 0; i < chars.size(); ++i) // получить вектор с аккаунтами из пвбот_конфиг и сделать фориач
    {
        szH = new wxBoxSizer(wxHORIZONTAL);
        szH->SetMinSize(wxSize(0, 52));
    
        wxStaticText* label = new wxStaticText(scrAccountList__, wxID_ANY, chars[i].role);
        wxButton* btnStartStop = new wxButton(scrAccountList__, 32767 - i - 1, wxString::FromUTF8("СТАРТ"), wxDefaultPosition, wxSize(42,18));
        btnStartStop->Bind(wxEVT_BUTTON, &PWBotGUI::StartStop, this);
        wxButton* btnRemove = new wxButton(scrAccountList__, 32767 - i - 1, wxString::FromUTF8("Удал."), wxDefaultPosition, wxSize(32,18));
        btnRemove->Bind(wxEVT_BUTTON, &PWBotGUI::Remove, this);
        szH->AddSpacer(5);
        szH->Add(label, 0, wxEXPAND|wxTOP, (52-label->GetSize().GetHeight())/2);
        szH->AddSpacer(10);
        szH->Add(btnStartStop, 0, wxTOP, (52-btnStartStop->GetSize().GetHeight())/2);
        szH->AddSpacer(4);
        szH->Add(btnRemove, 0, wxTOP, (52-btnRemove->GetSize().GetHeight())/2);
        szV->Add(szH, 0);
    }
    scrAccountList__->SetSizer(szV);
    scrAccountList__->FitInside();
    scrAccountList__->Layout();
    scrAccountList__->SetScrollRate(1,1);
}

void PWBotGUI::AppendAccount(wxCommandEvent& e)
{
    std::wstring login = fldLogin__->GetValue().ToStdWstring();
    std::wstring password = fldPassword__->GetValue().ToStdWstring();
    std::wstring charname = fldName__->GetValue().ToStdWstring();
    std::wstring path_name = cmbxPaths__->GetString(cmbxPaths__->GetSelection()).ToStdWstring();
    if (login.empty())
        return;
    if (password.empty())
        return;
    if (charname.empty())
        return;
    
    config__->AppendChar(login, password, charname, path_name);

    fldLogin__->SetValue("");
    fldPassword__->SetValue("");
    fldName__->SetValue("");

    UpdateAccountList();

}

void PWBotGUI::StartStop(wxCommandEvent& e)
{
    wxButton* btn = (wxButton*)(e.GetEventObject());
    int i = 0x7FFF - btn->GetId() - 1;
    const PWBotChar& character = config__->GetChars()[i];
    
    for (std::vector<ElementClient>::iterator it = elclies__.begin(); it != elclies__.end(); ++it)
    {
        if (i != it->i)
            continue;
        btn->SetLabel(wxString::FromUTF8("СТАРТ"));
        elclies__.erase(it);
        TerminateProcess(it->handle, 0);
        if (bindedto__ == it->handle)
            bindedto__ = nullptr;
        return;
    }

    PROCESS_INFORMATION pi = {};
    STARTUPINFOW si = {};

    CreateProcessW(config__->GetElCli().c_str(), (L"startbypatcher game:cpw console:1 user:" + character.login + L" pwd:" + character.password + L" role:" + character.role).data(), nullptr, nullptr, true, 0, nullptr, (config__->GetElCli() + L"\\..\\").c_str(), &si, &pi);

    ElementClient elcli = {i, pi.hProcess};
    elclies__.push_back(elcli);
    btn->SetLabel(wxString::FromUTF8("СТОП"));
    std::thread t_([this]
    {
        ElementClient elcli = elclies__.back();
        PWBotOffsets o = config__->GetOffsets();
        
        PWBot* pwb = new PWBot(elcli.handle, o.socket, o.charorigin, o.charbase, o.move, o.movepoint, o.movepointwalking, o.movepointX, o.movepointZ, o.movepointY, o.movepointXdiff, o.movepointZdiff, o.movepointYdiff, o.mousepointedat, o.targetedat, o.charid, o.ischarfree, o.charhealth, o.charmana, o.charposX, o.charposZ, o.charposY, o.charwalkspeed, o.charflyapparatus, o.charflyspeed, o.charflystatus, o.cameradistance, o.resourceorigin, o.resourceentity, o.resourceobject, o.resourceid, o.resourcetype, o.resourceposX, o.resourceposZ, o.resourceposY, o.playersorigin, o.playersfirstid);
        
        DWORD lpExitCode = 0;
        while (GetExitCodeProcess(elcli.handle, &lpExitCode) && lpExitCode == STILL_ACTIVE)
        {
            uint32_t charid = pwb->GetCharId();
            if (charid)
            {
                config__->SetCharId(elcli.i, charid);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        config__->UnloadConfig();
        std::wstring cmdWstr = L"cp config.xml " + config__->GetElCli() + L"\\..\\config.xml";
        std::string cmdstr(cmdWstr.begin(), cmdWstr.end());
        WinExec(cmdstr.c_str(), SW_HIDE);
        char szFileName[MAX_PATH];
        GetModuleFileName(nullptr, szFileName, MAX_PATH);
        
        std::string fileName;
        fileName += szFileName;
        fileName += "\\..\\pwbot_lib.dll";
        void * loc = VirtualAllocEx(elcli.handle, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (loc)
            WriteProcessMemory(elcli.handle, loc, fileName.c_str(), fileName.size(), 0);
        else
            std::cout<<"no loc"<<std::endl;
        HANDLE hThread = CreateRemoteThread(elcli.handle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread)
            CloseHandle(hThread);
        else
            std::cout<<"no thread"<<std::endl;
    });

    t_.detach();
    return;

}

void PWBotGUI::Remove(wxCommandEvent& e)
{
    wxButton* btn = (wxButton*)(e.GetEventObject());
    int i = 0x7FFF - btn->GetId() - 1;

    for (std::vector<ElementClient>::iterator it = elclies__.begin(); it != elclies__.end(); ++it)
    {
        if (it->i != i)
            continue;
        TerminateProcess(it->handle, 0);
        if (bindedto__ == it->handle)
            bindedto__ = nullptr;
    }

    PWBotChar character = config__->GetChars()[i];
    config__->RemoveChar(character.role);
    UpdateAccountList();
}

void PWBotGUI::CreatePath(wxCommandEvent& e)
{
    wxFrame* frm = new wxFrame(frame__, wxID_ANY, wxString::FromUTF8("Создание маршрута"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSTAY_ON_TOP|wxCLOSE_BOX);
    frm->Bind(wxEVT_CLOSE_WINDOW, &PWBotGUI::CreatePathClose, this);
    wxSizer* szV = new wxBoxSizer(wxVERTICAL);
    wxArrayString strs = {};
    for (const PWBotChar& chr : config__->GetChars())
    {
        strs.Add(chr.role);
    }
    wxChoice* chc = new wxChoice(frm, wxID_ANY, wxDefaultPosition, wxSize(275, 35), strs);
    chc->Bind(wxEVT_CHOICE, &PWBotGUI::CreatePathCliSel, this);
    szV->Add(chc, 0, wxEXPAND);

    wxButton* recStart = new wxButton(frm, wxID_ANY, wxString::FromUTF8("Начать запись"), wxDefaultPosition, wxDefaultSize);
    recStart->Bind(wxEVT_BUTTON, &PWBotGUI::RecordPath, this);
    wxTextCtrl* recName = new wxTextCtrl(frm, 0xAAA);
    recName->SetHint(wxString::FromUTF8("Введите имя маршрута"));
    wxSizer* szH = new wxBoxSizer(wxHORIZONTAL);
    szH->Add(recName, 1);
    szH->AddSpacer(6);
    szH->Add(recStart);
    szV->Add(szH, 0, wxEXPAND);

    wxTextCtrl* outline = new wxTextCtrl(frm, 0xBBB, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    szV->Add(outline, 1, wxEXPAND);

    frm->SetSizer(szV);
    frm->Layout();
    frm->Show(true);
}

void PWBotGUI::CreatePathCliSel(wxCommandEvent& e)
{
    wxChoice* chc = (wxChoice*)e.GetEventObject();
    bindedto__ = nullptr;
    for (ElementClient elcli : elclies__)
    {
        if (elcli.i == chc->GetSelection())
        {
            bindedto__ = elcli.handle;
            break;
        }
    }
}

void PWBotGUI::RecordPath(wxCommandEvent& e)
{
    if (isrecording__)
    {
        isrecording__ = false;
        ((wxButton*)e.GetEventObject())->SetLabel(wxString::FromUTF8("Начать запись"));
        return;
    }

    if (!bindedto__)
        return;

    std::wstring nameofthepath;
    wxTextCtrl* output = nullptr;
    for (wxWindow* win : ((wxWindow*)e.GetEventObject())->GetParent()->GetChildren())
    {
        if (win->GetId() == 0xAAA)
        {
            nameofthepath = ((wxTextCtrl*)win)->GetValue();
        }
        else if (win->GetId() == 0xBBB)
        {
            output = (wxTextCtrl*)win;
        }
    }

    if (!output)
        return;
    
    *output << "We got succeded";

    PWBotOffsets o = config__->GetOffsets();
    PWBot* pwb = new PWBot(bindedto__, o.socket, o.charorigin, o.charbase, o.move, o.movepoint, o.movepointwalking, o.movepointX, o.movepointZ, o.movepointY, o.movepointXdiff, o.movepointZdiff, o.movepointYdiff, o.mousepointedat, o.targetedat, o.charid, o.ischarfree, o.charhealth, o.charmana, o.charposX, o.charposZ, o.charposY, o.charwalkspeed, o.charflyapparatus, o.charflyspeed, o.charflystatus, o.cameradistance, o.resourceorigin, o.resourceentity, o.resourceobject, o.resourceid, o.resourcetype, o.resourceposX, o.resourceposZ, o.resourceposY, o.playersorigin, o.playersfirstid);

    std::thread t_([=]()
    {
        std::vector<std::vector<float>> path;
        std::vector<uint32_t> types;
        std::vector<float> pos = {0,0,0};
        std::vector<float> curr;
        float dis;
        while (isrecording__)
        {
            curr = pwb->GetCharPos();
            if ((dis = std::sqrt(std::pow(pos[0] - curr[0], 2) + std::pow(pos[1] - curr[1], 2) + std::pow(pos[2] - curr[2], 2))) > 10.0f)
            {
                pos = curr;
                wxTheApp->CallAfter([&]()
                {
                    if (isrecording__)
                    {
                        *output << "\r\n" << wxString::FromUTF8("Новая точка: ") << pos[0] << " " << pos[1] << " " << pos[2];
                        path.push_back({pos[0],pos[1],pos[2]});
                        for (uint32_t type : pwb->GetNearResourceTypes())
                        {
                            if (std::find(types.begin(),types.end(),type) == types.end())
                            {
                                *output << "\r\n" << wxString::FromUTF8("новый тип: ") << (int)type;
                                types.push_back(type);
                            }
                        }
                    }
                });
            }
            else
                wxTheApp->CallAfter([=]()
                {
                    if (isrecording__)
                        *output << "\r\n" << wxString::FromUTF8("Дистанция ") << dis;
                });
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        config__->AppendPath(nameofthepath, path, types);
    });
    isrecording__ = true;
    ((wxButton*)e.GetEventObject())->SetLabel(wxString::FromUTF8("Остановить запись"));
    t_.detach();
    
    
}

void PWBotGUI::CreatePathClose(wxCloseEvent& e)
{
    isrecording__ = false;
    e.Skip();
}

PWBotGUI::~PWBotGUI()
{
    delete config__;
}

wxIMPLEMENT_APP(PWBotGUI);

