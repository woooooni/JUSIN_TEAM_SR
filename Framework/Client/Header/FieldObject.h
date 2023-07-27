#pragma once
#include "Engine_Define.h"
#include "GameObject.h"
class CFieldObject : public CGameObject
{
public:
	enum FIELD_OBJ_STATE
	{
		NOT_INTERACT,
		PUSHING,
		LIFTING,
		STATE_END
	};

public:
	CFieldObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eObjId);
	CFieldObject(const CFieldObject& rhs);
	virtual ~CFieldObject();


	const OBJ_INFO& Get_ObjInfo() { return m_tInfo; }

	virtual void		Set_SubscribeEvent(_uint pEvent) {};

	virtual _int		Update_Object(const _float& pFtimeDelta) override;

	virtual void		LateUpdate_Object()	override;

	virtual void		Render_Object() override;

	virtual CGameObject* Get_GrabObj() { return this; }

	
	virtual void			Set_FieldState(const FIELD_OBJ_STATE& pState)
	{
		m_eFState = pState;
	}

protected:
	OBJ_INFO m_tInfo;

	FIELD_OBJ_STATE m_eFState;

	void	Push_Me(CCollider* other);
	map<CCollider*, _vec3>	m_vMovingPos;

	virtual HRESULT		Ready_Component() ;

};

