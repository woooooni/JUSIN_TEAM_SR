#pragma once
#include	"Engine_Define.h"
#include	"Scene.h"

class CStage1 : public	CScene
{
protected:
	explicit CStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage1();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

public:
	static CStage1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;


	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Layer_Player() override;

	virtual HRESULT Ready_Layer_Camera() override;

	virtual HRESULT Ready_Layer_Terrrain() override;

	virtual HRESULT Ready_Layer_Environment() override;

	virtual HRESULT Ready_Layer_Monster() override;

	virtual HRESULT Ready_Layer_InterationObj() override;

	virtual HRESULT Ready_Layer_Effect() override;

	virtual HRESULT Ready_Layer_UI() override;

};

