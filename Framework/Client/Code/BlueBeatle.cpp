
#include "BlueBeatle.h"
#include "Export_Function.h"

CBlueBeatle::CBlueBeatle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev), m_fMoveTime(0.f)
{
}

CBlueBeatle::CBlueBeatle(const CBlueBeatle& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CBlueBeatle::~CBlueBeatle()
{
}


void CBlueBeatle::Update_Idle(_float fTimeDelta)
{

	if (m_fMoveTime > 10.f)
	{
		Set_State(MONSTER_STATE::MOVE);
		m_pAnimator->Play_Animation(L"BlueBeatle_Move_Down");
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;
}
void CBlueBeatle::Update_Move(_float fTimeDelta)
{

	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"BlueBeatle_Idle_Down");
		vDst = { float(rand() % 10),1.f,float(rand() % 10) };
		if (vDst != m_vDst)
			m_vDst = vDst;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
}
void CBlueBeatle::Update_Die(_float fTimeDelta)
{
}

HRESULT CBlueBeatle::Ready_Object(void)
{
	Set_State(MONSTER_STATE::IDLE);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"BlueBeatle_Idle_Down", L"Proto_Texture_BlueBeatle_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"BlueBeatle_Move_Down", L"Proto_Texture_BlueBeatle_Move_Down", 0.1f);

	m_pTransformCom->Set_Pos(&_vec3(10.0f, 1.0f, 10.0f));
	Set_Speed(5.f);
	m_pAnimator->Play_Animation(L"BlueBeatle_Idle_Down");

	return S_OK;
}


void CBlueBeatle::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBlueBeatle::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBlueBeatle::Add_Component(void)
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

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}
CBlueBeatle* CBlueBeatle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlueBeatle* pInstance = new CBlueBeatle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BlueBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlueBeatle::Trace(_float fTimeDelta)
{
}

void CBlueBeatle::Update_Regen(_float fTimeDelta)
{
}


void CBlueBeatle::Update_Attack(_float fTimeDelta)
{
}
