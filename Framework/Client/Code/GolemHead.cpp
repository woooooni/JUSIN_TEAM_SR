#include "Export_Function.h"
#include "GolemHead.h"
#include "SunGollem.h"
CGolemHead::CGolemHead(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemHead::CGolemHead(const CGolemHead& rhs)
	: CGolemPart(rhs)
{

}

CGolemHead::~CGolemHead()
{
}

HRESULT CGolemHead::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Head", L"Proto_Texture_SunGolem_Idle_Head", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Head", L"Proto_Texture_SunGolem_Dirty_Head", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Head", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 2.f * 1.5f,2.f * 2.f,2.f * 1.5f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
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
	__super::LateUpdate_Object();
}

void CGolemHead::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
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
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f,-0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}


}

void CGolemHead::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_Head", true);
	switch (m_iIndex)
	{

		case 0:
			Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);

			break;
		case 1:
			Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);
			break;
		case 2:
			Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 2.f);
			break;

		default:
			break;
	}
}

void CGolemHead::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemHead::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemHead::Update_Die(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);
		break;
	case 1:
		Move_Offset(_vec3(0.f, 1.f, -0.05f), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.f, 0.8f, -0.05f), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(0.f, 0.6f, -0.05f), fTimeDelta, 2.f);

		break;
	case 4:
		Move_Offset(_vec3(0.f, 0.3f, -0.05f), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, 0.f, -0.05f), fTimeDelta, 2.f);
		break;

	default:
		break;
	}
}

void CGolemHead::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = {0.f, 1.f,	-0.05f };
		break;
	case 1:
		Move_Offset(_vec3(0.f,	1.f, -0.05f), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.f, 1.f, -0.05f), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);

		break;
	case 4:
		Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 2.f);
		break;

	default:
		break;
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
