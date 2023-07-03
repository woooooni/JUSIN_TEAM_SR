#pragma once
#include "GameObject.h"
#include "Animator.h"

class CPlayer_State
{
public:
	CPlayer_State(CGameObject* _pOwner);
	~CPlayer_State();

public:
	virtual HRESULT		Ready_State(void) PURE;
	virtual _int		Update_State(const _float& fTimeDelta) PURE;
	virtual void		LateUpdate_State(void) PURE;
	virtual void		Render_State(void) PURE;

protected:
	CGameObject* m_pOwner;

	void				Change_Dir(OBJ_DIR _eDir,const wstring& _Animation)
	{
		if (m_pOwner->GetObj_Dir() != _eDir)
		{
			m_pOwner->SetObj_Dir(_eDir);
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(_Animation);
		}
	}
};

