#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "SunGollem.h"
#include "GolemParts.h"

CSunGollem::CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_fSpeed(5.f)
{
}
CSunGollem::CSunGollem(const CSunGollem& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_eState(rhs.m_eState)
{

}

CSunGollem::~CSunGollem()
{
}

HRESULT CSunGollem::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Body", L"Proto_Texture_SunGolem_Idle_Head", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Body", L"Proto_Texture_SunGolem_Dirty_Body", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);
	
		
	
	m_pTransformCom->Set_Pos(&_vec3(4.0f, 2.0f, 4.0f));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vRandomPos[0] = { vPos.x - 1, vPos.y, vPos.z };
	m_vRandomPos[1] = { vPos.x , vPos.y, vPos.z };
	m_vRandomPos[2] = { vPos.x + 1, vPos.y, vPos.z };
	m_pTransformCom->Set_Scale({ 2,2,2 });
	Set_Speed(5.f);
	Set_State(SUNGOLEM_STATE::REGEN);
	FAILED_CHECK_RETURN(Ready_Parts(), E_FAIL);

	return S_OK;
}

_int CSunGollem::Update_Object(const _float& fTimeDelta)
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

	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
	{
		(*iter)->Update_Object(fTimeDelta);
	}
	return iExit;
}

void CSunGollem::LateUpdate_Object(void)
{
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->LateUpdate_Object();
}

void CSunGollem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->Render_Object();
}

HRESULT CSunGollem::Add_Component(void)
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

void CSunGollem::Update_Idle(_float fTimeDelta)
{
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
		if(rand()%10>7)
			Set_State(SUNGOLEM_STATE::MOVE);
		else if(rand() % 10 < 2)
			Set_State(SUNGOLEM_STATE::ATTACK);
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Dirty(_float fTimeDelta)
{
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
		Set_State(SUNGOLEM_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Move(_float fTimeDelta)
{

	

	m_fSpeed -= m_fMoveTime * m_fMoveTime*0.01f;


	_vec3 vPos;
	_vec3 vDir;

	m_pTransformCom->Move_Pos(&m_vVerticalDir, fTimeDelta, m_fSpeed);
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->Get_TransformCom()->Move_Pos(&m_vVerticalDir, fTimeDelta, m_fSpeed);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	
	vDir = m_vRandomPos[m_iRand] - vPos;
	vDir.y = 0.f;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 1.f);
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->Get_TransformCom()->Move_Pos(&vDir, fTimeDelta, 1.f);

	if (vPos.y < 2.f&&m_fSpeed<0)
	{
			m_vVerticalDir= {0.f, 1.f ,0.f };
			m_fSpeed = 10.f;
			Set_State(SUNGOLEM_STATE::DIRTY);
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Body", true);
			m_iRand = rand() % 3 ;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;

}

void CSunGollem::Update_Attack(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		for (_int i = 0; i < m_iActiveArm; i++)
		{
			if (i >= 2 && 3 <= i)
			{
				Create_Fist(true, i);
			}
			else
				Create_Fist(false, i);
		}
		m_iActiveArm += 2;
		if (m_iActiveArm > 6)
			m_iActiveArm = 2;
		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;
}

void CSunGollem::Update_Die(_float fTimeDelta)
{
}

void CSunGollem::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0.f, 1.f * fTimeDelta, 0.f };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	
	if (vPos.y > 2.f)
	{
		vPos.y = 2.f;
		m_pTransformCom->Set_Info(INFO_POS, &vPos);
		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CSunGollem* CSunGollem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSunGollem* pInstance = new CSunGollem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SunGollem Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSunGollem::Free()
{
	__super::Free();
}
void CSunGollem::Create_Fist(bool _BummerFist, _int _iSrc)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos += {float(rand() % 33) * 0.1f, (float)_iSrc, float(rand() % 34 - 17) * 0.1f};
	CGolemFist* pGolemFist = CGolemFist::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemFist, );
	pGolemFist->Get_TransformCom()->Set_Pos(&vPos);
	pGolemFist->Set_State(m_eState);
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
	pLayer->Add_GameObject(L"GolemFist", pGolemFist);

}
HRESULT CSunGollem::Ready_Parts(void) 
{
	_vec3 vPos, vAxisZ;
	vAxisZ = {0.f,0.f,1.f};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vPartPos[HEAD] = { vPos.x,vPos.y + 0.5f,vPos.z - 0.01f };
	m_vPartPos[UPPERJAW] = { vPos.x,vPos.y + 0.1f,vPos.z - 0.011f };
	m_vPartPos[LOWERJAW] = { vPos.x,vPos.y - 0.2f,vPos.z - 0.0105f };
	m_vPartPos[LEFTLEG] = { vPos.x-0.6f,vPos.y - 1.f,vPos.z - 0.01f };
	m_vPartPos[RIGHTLEG] = { vPos.x + 0.6f,vPos.y - 1.f,vPos.z - 0.01f };
	m_vPartPos[LEFTARM0] = { vPos.x - 1.5f,vPos.y - 0.5f,vPos.z - 0.0103f };
	m_vPartPos[LEFTARM1] = { vPos.x - 1.75f,vPos.y + 0.5f,vPos.z - 0.0102f };
	m_vPartPos[LEFTARM2] = { vPos.x - 1.5f,vPos.y + 1.5f,vPos.z - 0.0101f };
	m_vPartPos[RIGHTARM0] = { vPos.x + 1.5f,vPos.y - 0.5f,vPos.z - 0.0103f };
	m_vPartPos[RIGHTARM1] = { vPos.x + 1.75f,vPos.y + 0.5f,vPos.z - 0.0102f };
	m_vPartPos[RIGHTARM2] = { vPos.x + 1.5f,vPos.y + 1.5f,vPos.z - 0.0101f };
	m_vPartPos[LEFTHAND0] = { vPos.x - 1.9f,vPos.y - 0.5f,vPos.z - 0.0104f };
	m_vPartPos[LEFTHAND1] = { vPos.x - 2.15f,vPos.y + 0.5f,vPos.z - 0.0103f };
	m_vPartPos[LEFTHAND2] = { vPos.x - 1.9f,vPos.y + 1.5f,vPos.z - 0.0102f };
	m_vPartPos[RIGHTHAND0] = { vPos.x + 1.9f,vPos.y - 0.5f,vPos.z - 0.0104f };
	m_vPartPos[RIGHTHAND1] = { vPos.x + 2.15f,vPos.y + 0.5f,vPos.z - 0.0103f };
	m_vPartPos[RIGHTHAND2] = { vPos.x + 1.9f,vPos.y + 1.5f,vPos.z - 0.0102f };
	//	HEAD, LOWERJAW, UPPERJAW, LEFTLEG, RIGHTLEG, LEFTARM0, LEFTARM1,
	//	LEFTARM2, RIGHTARM0, RIGHTARM1, RIGHTARM2, LEFTHAND0,
	//	LEFTHAND1, LEFTHAND2, RIGHTHAND0, RIGHTHAND1, RIGHTHAND2
	m_vecParts.reserve(BODYPARTS::PARTSEND);
	CGolemHead* pGolemHead = CGolemHead::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemHead, E_FAIL);
	pGolemHead->Get_TransformCom()->Set_Pos(&m_vPartPos[HEAD]);
	m_vecParts.push_back(pGolemHead);
	CGolemUpperJaw* pGolemUpperJaw = CGolemUpperJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemUpperJaw, E_FAIL);
	pGolemUpperJaw->Get_TransformCom()->Set_Pos(&m_vPartPos[UPPERJAW]);
	m_vecParts.push_back(pGolemUpperJaw);
	CGolemLowerJaw* pGolemLowerJaw = CGolemLowerJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemLowerJaw, E_FAIL);
	pGolemLowerJaw->Get_TransformCom()->Set_Pos(&m_vPartPos[LOWERJAW]);
	m_vecParts.push_back(pGolemLowerJaw);
	CGolemLeftLeg* pGolemLeftLeg = CGolemLeftLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemLeftLeg, E_FAIL);
	pGolemLeftLeg->Get_TransformCom()->Set_Pos(&m_vPartPos[LEFTLEG]);
	m_vecParts.push_back(pGolemLeftLeg);
	CGolemRightLeg* pGolemRightLeg = CGolemRightLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemRightLeg, E_FAIL);
	pGolemRightLeg->Get_TransformCom()->Set_Pos(&m_vPartPos[RIGHTLEG]);
	m_vecParts.push_back(pGolemRightLeg);
	for (_int i = LEFTARM0; i < LEFTARM2+1; i++)
	{
		CGolemLeftArm* pGolemLeftArm = CGolemLeftArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemLeftArm, E_FAIL);
		pGolemLeftArm->Set_ArmNum(i - LEFTARM0);
		pGolemLeftArm->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(-45.f*(i - LEFTARM1)));
		pGolemLeftArm->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		m_vecParts.push_back(pGolemLeftArm);
	}
	for (_int i = RIGHTARM0; i < RIGHTARM2 + 1; i++)
	{
		CGolemRightArm* pGolemRightArm = CGolemRightArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemRightArm, E_FAIL);
		pGolemRightArm->Set_ArmNum(i - RIGHTARM0);
		pGolemRightArm->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(45.f * (i - RIGHTARM1)));
		pGolemRightArm->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		m_vecParts.push_back(pGolemRightArm);
	}
	for (_int i = LEFTHAND0; i < LEFTHAND2 + 1; i++)
	{
		CGolemLeftHand* pGolemLeftHand = CGolemLeftHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemLeftHand, E_FAIL);
		pGolemLeftHand->Set_ArmNum(i - LEFTHAND0);
		pGolemLeftHand->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		pGolemLeftHand->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(-90.f));
		m_vecParts.push_back(pGolemLeftHand);
	}
	for (_int i = RIGHTHAND0; i < RIGHTHAND2 + 1; i++)
	{
		CGolemRightHand* pGolemRightHand = CGolemRightHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemRightHand, E_FAIL);
		pGolemRightHand->Set_ArmNum(i - RIGHTHAND0);
		pGolemRightHand->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		pGolemRightHand->Get_TransformCom()->RotationAxis(vAxisZ,D3DXToRadian( 90.f));
		m_vecParts.push_back(pGolemRightHand);
	}
	return S_OK;
}
