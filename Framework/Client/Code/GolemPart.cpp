#include "..\Header\GolemPart.h"
#include "Export_Function.h"

CGolemPart::CGolemPart(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SUN_GOLLEM)
	, m_eState(SUNGOLEM_STATE::REGEN)
{

}
CGolemPart::CGolemPart(const CGolemPart& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_pTarget(nullptr)
{

}

CGolemPart::~CGolemPart()
{
}

HRESULT CGolemPart::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CGolemPart::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	if (Is_Active())
	{
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		switch (m_eState)
		{
		case SUNGOLEM_STATE::IDLE:
			Update_Idle(fTimeDelta);
			break;
		case SUNGOLEM_STATE::MOVE:
			Update_Move(fTimeDelta);
			break;
		case SUNGOLEM_STATE::REGEN:
			Update_Regen(fTimeDelta);
			break;
		case SUNGOLEM_STATE::ATTACK:
			Update_Attack(fTimeDelta);
			break;
		case SUNGOLEM_STATE::DIE:
			Update_Die(fTimeDelta);
			break;
		}
	}
	return iExit;
}

void CGolemPart::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemPart::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CGolemPart::Add_Component(void)
{
	return S_OK;
}

void CGolemPart::Offset(_vec3 _vPos)
{
	_vec3 vPos = _vPos + m_vOffset;
	m_pTransformCom->Set_Pos(&vPos);
}

void CGolemPart::Move_Offset(_vec3 _vPos,_float fTimeDelta,_float _fSpeed)
{
	_vec3 vDir = _vPos - m_vOffset;
	D3DXVec3Normalize(&vDir, &vDir);
	m_vOffset += vDir * fTimeDelta * _fSpeed;
}

void CGolemPart::Move_Offset_ByDir(_vec3 _vDir, _float fTimeDelta, _float _fSpeed)
{
	D3DXVec3Normalize(&_vDir, &_vDir);
	m_vOffset += _vDir * fTimeDelta * _fSpeed;
}
void CGolemPart::Free()
{
	__super::Free();
}

