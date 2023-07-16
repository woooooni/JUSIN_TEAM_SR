#pragma once

#include "Base.h"
#include "Engine_Define.h"

class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	HRESULT Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev);
	void Update_UIMgr(const _float& fTimeDelta);
	void Late_Update_UIMgr();
	void Render_UIMgr();


private:
		

public:
	virtual void Free() override;
};

