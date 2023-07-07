#include "Export_Function.h"
#include "GolemFist.h"
#include "SunGollem.h"

CGolemFist::CGolemFist(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET)
, m_eState(SUNGOLEM_STATE::REGEN)

{
}
CGolemFist::CGolemFist(const CGolemFist& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(rhs.m_eState)
{

}

CGolemFist::~CGolemFist()
{
}

HRESULT CGolemFist::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Fist", L"Proto_Texture_SunGolem_Idle_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Fist", L"Proto_Texture_SunGolem_Dirty_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerFist", L"Proto_Texture_SunGolem_Dirty_BummerFist", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Fist", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f, 1.f,1.f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemFist::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	_vec3 vDir = {0, -1, 0};
	if (m_eState != SUNGOLEM_STATE::DIRTY)
		Update_Idle(fTimeDelta);
	else
		Update_Dirty(fTimeDelta);

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta,20.f);
	return iExit;
}

void CGolemFist::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS,&vPos);
	__super::LateUpdate_Object();
	if (vPos.y < -1.f)
	{
		if(Is_Active())
		Engine::CCameraMgr::GetInstance()->GetMainCamera()->CamShake(0.1f);
		Set_Active(false);
	}
}

void CGolemFist::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGolemFist::Add_Component(void)
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

void CGolemFist::Update_Idle(_float fTimeDelta)
{
	if (m_bDirty == true)
	{
		if (m_bBummer == true)
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerFist", true);
		else
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Fist", true);
	}
}

void CGolemFist::Update_Dirty(_float fTimeDelta)
{

	
}

CGolemFist* CGolemFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemFist* pInstance = new CGolemFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemFist Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemFist::Free()
{
	__super::Free();
}
