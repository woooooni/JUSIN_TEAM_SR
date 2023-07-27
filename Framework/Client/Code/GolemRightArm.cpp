#include "Export_Function.h"
#include "GolemRightArm.h"
#include "SunGollem.h"

CGolemRightArm::CGolemRightArm(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemRightArm::CGolemRightArm(const CGolemRightArm& rhs)
	: CGolemPart(rhs)
{

}

CGolemRightArm::~CGolemRightArm()
{
}

HRESULT CGolemRightArm::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_RightArm", L"Proto_Texture_SunGolem_Idle_RightArm", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_RightArm", L"Proto_Texture_SunGolem_Dirty_RightArm", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_RightArm", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.7f * 2.f, 0.7f * 2.f,0.7f * 2.f });

	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemRightArm::Update_Object(const _float& fTimeDelta)
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

void CGolemRightArm::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemRightArm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CGolemRightArm::Add_Component(void)
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

void CGolemRightArm::Update_Idle(_float fTimeDelta)
{
	if (m_bBreath)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.069f, -0.8f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 275.456f)
				m_fRotationAngle -= 43.1f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 318.8f)
				m_fRotationAngle	-= 43.1f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(1.79, 1.551, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 329.6f)
				m_fRotationAngle -= 43.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
	}

	


}

void CGolemRightArm::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_RightArm", true);
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);
	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CGolemRightArm::Update_Move(_float fTimeDelta)
{
}

void CGolemRightArm::Update_Attack(_float fTimeDelta)
{
}

void CGolemRightArm::Update_Die(_float fTimeDelta)
{
}

void CGolemRightArm::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { 1.3f, -0.5f,	0.01f };
		m_fRotationAngle = 248.f;
		break;
	case 1:
		Move_Offset(_vec3(1.3f, - 0.3f , m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.77f, -0.8f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 263.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 318.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.f, 2.256f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 361.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;
	case 3:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.77f, -0.8f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 263.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.2f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 318.f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.f, 2.256f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 361.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;
	case 4:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.57727f, -0.665475f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 248.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(1.83094f, 0.101436f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 291.172f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(1.71295f, 1.13736f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 331.177f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
		break;
	case 5:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.069f, -1.5f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 281.512f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(3.f, 0.5f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 335.602f)
				m_fRotationAngle += 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.49f, 2.651f, -0.01f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 380.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;


	default:
		break;
	}
}
CGolemRightArm* CGolemRightArm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemRightArm* pInstance = new CGolemRightArm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemRightArm Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemRightArm::Free()
{
	__super::Free();
}
