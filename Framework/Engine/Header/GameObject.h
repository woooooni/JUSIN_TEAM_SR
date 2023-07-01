#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;
class CVIBuffer;
class CTransform;
class CCollider;
class CTexture;
class CAnimator;


class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_TYPE _eType);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	const wstring&	Get_Name()	{ return m_strName; }
	void Set_Name(const wstring& _strName)	{ m_strName = _strName; }

public:
	virtual HRESULT	Ready_Object(void);
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void LateUpdate_Object(void);
	virtual void Render_Object(void);



public:
	CComponent* Get_Component(COMPONENT_TYPE eType, COMPONENTID eID);
	OBJ_TYPE Get_ObjType() { return m_eType; }
	void Set_ObjType(OBJ_TYPE _eType) { m_eType = _eType; }

public:
	void Set_Billboard();
	



private:
	CComponent* Find_Component(COMPONENT_TYPE eType, COMPONENTID eID);


protected:
	map<COMPONENT_TYPE, CComponent*>		m_mapComponent[ID_END];		
	LPDIRECT3DDEVICE9						m_pGraphicDev;

protected:
	CVIBuffer*		m_pBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CAnimator*		m_pAnimator = nullptr;
	CTexture*		m_pTextureCom = nullptr;

protected:
	_vec3		m_vLookTemp;
	wstring		m_strName;
	OBJ_TYPE	m_eType;
	


public:
	virtual void		Free();
};


END