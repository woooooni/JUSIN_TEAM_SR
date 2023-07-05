#include "Export_Function.h"
#include "GolemHead.h"

CGolemHead::CGolemHead(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
, m_eState(SUNGOLEM_STATE::REGEN)

{
}
CGolemHead::CGolemHead(const CGolemHead& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(rhs.m_eState)
{

}

CGolemHead::~CGolemHead()
{
}

HRESULT CGolemHead::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Body", L"Proto_Texture_SunGolem_Idle_Body", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Body", L"Proto_Texture_SunGolem_Dirty_Body", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 2,2,2 });

	Set_State(SUNGOLEM_STATE::REGEN);
}

_int CGolemHead::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
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
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemHead::LateUpdate_Object(void)
{
}

void CGolemHead::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGolemHead::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}

void CGolemHead::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_fMoveTime > 5.f)
	{
		vDir = { 0,1 ,0 };
	}
	else
	{
		vDir = { 0,-1 ,0 };
	}
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fTimeDelta);
	if (m_fMoveTime > 10.f)
	{
	
//			Set_State(SUNGOLEM_STATE::DIRTY);
//			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Body", true);
			m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;


	


}

void CGolemHead::Update_Dirty(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_fMoveTime > 5.f)
	{
		vDir = { 0,1 ,0 };
	}
	else
	{
		vDir = { 0,-1 ,0 };
	}
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fTimeDelta);
	if (m_fMoveTime > 10.f)
	{

		Set_State(SUNGOLEM_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;
}

void CGolemHead::Update_Move(_float fTimeDelta)
{
}

void CGolemHead::Update_Attack(_float fTimeDelta)
{
}

void CGolemHead::Update_Die(_float fTimeDelta)
{
}

void CGolemHead::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0,1 ,0 };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fTimeDelta);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y > 2.f)
	{

		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CGolemHead* CGolemHead::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemHead* pInstance = new CGolemHead(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemHead Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemHead::Free()
{
	__super::Free();
}
