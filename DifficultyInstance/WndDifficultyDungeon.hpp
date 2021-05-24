#pragma once

#if defined(__DIFFICULTY_INSTANCE)

class CWndDifficultyDungeon : public CWndNeuz
{
public:
    CWndDifficultyDungeon() = default;
    ~CWndDifficultyDungeon() = default;
    BOOL Initialize(CWndBase* pWndParent = NULL, DWORD nType = MB_OK) override;
    BOOL OnChildNotify(UINT message, UINT nID, LRESULT* pLResult) override;
    void OnInitialUpdate() override;
};

#endif //__DIFFICULTY_INSTANCE
