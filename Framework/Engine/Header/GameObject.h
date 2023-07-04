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
	const wstring& Get_Name() { return m_strName; }
	void Set_Name(const wstring& _strName) { m_strName = _strName; }

public:
	virtual HRESULT	Ready_Object(void);
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void LateUpdate_Object(void);
	virtual void Render_Object(void);



public:
	CComponent* Get_Component(COMPONENT_TYPE eType, COMPONENTID eID);

	OBJ_TYPE GetObj_Type() { return m_eType; }
	void SetObj_Type(OBJ_TYPE _eType) { m_eType = _eType; }

	OBJ_DIR GetObj_Dir() { return m_eDir; }
	void SetObj_Dir(OBJ_DIR _eDir) { m_eDir = _eDir; }

	OBJ_STATE GetObj_State() { return m_eState; }
	void SetObj_State(OBJ_STATE _eState) { m_eState = _eState; }

	void Set_Active(_float _bActive) { m_bActive = _bActive; }
	_bool Is_Active() { return m_bActive; }

	CVIBuffer* Get_BufferCom() { return m_pBufferCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CCollider* Get_ColliderCom() { return m_pColliderCom; }
	CAnimator* Get_AnimatorCom() { return m_pAnimator; }
	CTexture* Get_TextureCom() { return m_pTextureCom; }

public:
	void Set_Billboard();

public:
	// 충돌 호출
	virtual void Collision_Enter(CGameObject* pCollisionObj) {};
	virtual void Collision_Stay(CGameObject* pCollisionObj) {};
	virtual void Collision_Exit(CGameObject* pCollisionObj) {};

	
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
	wstring		m_strName;
	OBJ_TYPE	m_eType;
	OBJ_DIR		m_eDir;
	OBJ_STATE	m_eState;

private:
	_bool		m_bActive;

public:
	virtual void		Free();
};


END