#include "Export_Function.h"
#include "GolemLeftArm.h"
#include "SunGollem.h"

CGolemLeftArm::CGolemLeftArm(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)

{
}
CGolemLeftArm::CGolemLeftArm(const CGolemLeftArm& rhs)
	:CGolemPart(rhs)

{

}

CGolemLeftArm::~CGolemLeftArm()
{
}

HRESULT CGolemLeftArm::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_LeftArm", L"Proto_Texture_SunGolem_Idle_LeftArm", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_LeftArm", L"Proto_Texture_SunGolem_Dirty_LeftArm", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LeftArm", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.7f * 2.f, 0.7f * 2.f,0.7f * 2.f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemLeftArm::Update_Object(const _float& fTimeDelta)
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

void CGolemLeftArm::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemLeftArm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)m_fAlpha, 255, 255, 255));
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CGolemLeftArm::Add_Component(void)
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

void CGolemLeftArm::Update_Idle(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -0.8f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 83.126f)
			m_fRotationAngle += 83.126 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.498318f, -0.011f), fTimeDelta,3.f);
			if (m_fRotationAngle < 43.1)
				m_fRotationAngle += 43.1 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-1.79f, 1.551f, -0.01f), fTimeDelta,3.f);
			if (m_fRotationAngle < 27.f)
				m_fRotationAngle += 27.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}


}

void CGolemLeftArm::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_LeftArm", true);
	switch (m_iIndex)
	{
	case 0:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.77f, -0.8f, -0.012f), fTimeDelta, 2.f);
			m_fRotationAngle = 110.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 40.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.f, 2.256f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 1.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 1:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.57727f, -0.665475f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 110.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-1.83094f, 0.101436f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 66.828)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-1.71295f, 1.13736f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 27.7399)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.069f, -1.5f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 79.7f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-3.f, 0.498318f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 29.498f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.5f, 2.651f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -15.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;


	default:
		break;
	}
}

void CGolemLeftArm::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -0.8f, -0.012f), fTimeDelta,3.f);
			if (m_fRotationAngle < 83.126f)
				m_fRotationAngle += 83.126 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.498318f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 43.1)
				m_fRotationAngle += 43.1 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-1.79f, 1.551f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 27.f)
				m_fRotationAngle += 27.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemLeftArm::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -0.8f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 83.126f)
				m_fRotationAngle += 83.126 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.498318f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 43.1)
				m_fRotationAngle += 43.1 * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-1.79f, 1.551f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 27.f)
				m_fRotationAngle += 27.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemLeftArm::Update_Die(_float fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS,&vPos);
	if (vPos.y > 0.5f)
		m_vOffset.y -= 2.f * fTimeDelta;
	else
		m_fAlpha -= 100.f*fTimeDelta;
	if (m_fAlpha <= 0.f)
		Set_Active(false);
}

void CGolemLeftArm::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { -1.3f, - 0.5f,	0.01f };
		m_fRotationAngle = 110.f;
		break;
	case 1:
		Move_Offset(_vec3(-1.3f, -0.3f, 0.01f), fTimeDelta, 2.f);
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.77f, -0.8f, -0.012f), fTimeDelta, 2.f);
			m_fRotationAngle = 110.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if(m_fRotationAngle>40.f)
				m_fRotationAngle -= 35.f*fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.f, 2.256f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 1.f)
			m_fRotationAngle -= 55.f * fTimeDelta; 
		}
			break; 
	case 3:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.77f, -0.8f, -0.012f), fTimeDelta, 2.f);
			m_fRotationAngle = 110.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 40.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.f, 2.256f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 1.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 4:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.57727f, -0.665475f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 110.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-1.83094f, 0.101436f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 66.828)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-1.71295f, 1.13736f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 27.7399)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		break;
	case 5:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.069f, -1.5f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 79.7f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-3.f, 0.498318f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 29.498f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.5f, 2.651f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -15.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;


	default:
		break;
	}
}
CGolemLeftArm* CGolemLeftArm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemLeftArm* pInstance = new CGolemLeftArm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemLeftArm Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemLeftArm::Free()
{
	__super::Free();
}
