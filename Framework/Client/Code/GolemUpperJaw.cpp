#include "Export_Function.h"
#include "GolemUpperJaw.h"
#include "SunGollem.h"
CGolemUpperJaw::CGolemUpperJaw(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemUpperJaw::CGolemUpperJaw(const CGolemUpperJaw& rhs)
	: CGolemPart(rhs)

{

}

CGolemUpperJaw::~CGolemUpperJaw()
{
}

HRESULT CGolemUpperJaw::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_UpperJaw", L"Proto_Texture_SunGolem_Idle_UpperJaw", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_UpperJaw", L"Proto_Texture_SunGolem_Dirty_UpperJaw", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_UpperJaw", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f*1.5f,0.6f*2.f,0.6f*1.5f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemUpperJaw::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (Get_State())
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

void CGolemUpperJaw::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemUpperJaw::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CGolemUpperJaw::Add_Component(void)
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

void CGolemUpperJaw::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 0.4f, -0.055f), fTimeDelta,3.f);
	

	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemUpperJaw::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_UpperJaw", true);
	switch (m_iIndex)
	{
	case 0:
		Move_Offset(_vec3(-0.00f, 0.5f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 1:
		Move_Offset(_vec3(0.f, 0.4902f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.f, 0.4f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	default:
		break;
	}
}

void CGolemUpperJaw::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 0.4f, -0.055f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemUpperJaw::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 0.4f, -0.055f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemUpperJaw::Update_Die(_float fTimeDelta)
{
	_vec3 vOffset = m_vOffset;
	switch (m_iIndex)
	{

	case 0:
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 1:
		vOffset.y = 0.3f;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 2:
		vOffset.y = 0.1f;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 3:
		vOffset.y = -0.1f;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 4:
		vOffset.y = -0.4;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 5:
		vOffset.y = -0.7;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;


	default:
		break;
	}
}

void CGolemUpperJaw::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { -0.f,	0.1f, -0.055f };

		break;
	case 1:
		Move_Offset(_vec3(0.f,	0.1f, m_vOffset.z), fTimeDelta, 2.f);

		break;	
	case 2:
			Move_Offset(_vec3(0.f, 0.15f, m_vOffset.z), fTimeDelta, 2.f);
			m_fRotationAngle = 0.f;
			break;
	case 3:
		Move_Offset(_vec3(-0.00f, 0.5f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 4:
		Move_Offset(_vec3(0.f, 0.4902f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, 0.4f, m_vOffset.z), fTimeDelta, 2.f);
		break;


	default:
		break;
	}
}
CGolemUpperJaw* CGolemUpperJaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemUpperJaw* pInstance = new CGolemUpperJaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemUpperJaw Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemUpperJaw::Free()
{
	__super::Free();
}
