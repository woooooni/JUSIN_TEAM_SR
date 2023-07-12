#pragma once

#include "Base.h"
#include "Engine_Define.h"

class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

public:
	enum class UI_TYPE { };

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	HRESULT Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev);
	void Update_UIMgr(const _float& fTimeDelta);

public:
	virtual void Free() override;
};

