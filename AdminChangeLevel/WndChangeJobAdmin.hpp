#pragma once

#if defined(__APP_CHANGE_JOB_ADMIN)
class WndChangeJobAdmin : public CWndNeuz
{
    CWndButton* btnOk = nullptr;
    CWndButton* btnCancel = nullptr;
    CWndEdit* editLevel = nullptr;
    CWndComboBox* listJob = nullptr;
    CWndButton* checkHero = nullptr;
    CWndButton* checkMaster = nullptr;


public:
    WndChangeJobAdmin() = default;
    ~WndChangeJobAdmin() = default;
    BOOL Initialize(CWndBase* pWndParent = NULL, DWORD nType = MB_OK) override;
    void OnInitialUpdate() override;
    BOOL OnChildNotify(UINT message, UINT nID, LRESULT* pLResult) override;
};
#endif //__APP_CHANGE_JOB_ADMIN
