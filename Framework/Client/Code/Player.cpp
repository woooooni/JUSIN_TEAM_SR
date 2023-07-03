#include "..\Header\Player.h"

#include "Export_Function.h"
#include "Bullet.h"

#include "Scene.h"
#include "Terrain.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER)
	, m_fSpeed(5.f)
	, m_vDest(0.f, 0.f, 0.f)
{

}
CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDest(0.f, 0.f, 0.f)
{

}
CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Player", L"Proto_Texture_Player");

	m_pAnimator->Play_Animation(L"Player");

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	

	Key_Input(fTimeDelta);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Terrain"));
	_vec3 vDest;
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && Engine::IsPicking(pTerrain, &vDest))
		m_vDest = vDest;

	Player_Move(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	Set_Billboard();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Ready_Component(void)
{
	CComponent*			pComponent = nullptr;

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


void CPlayer::Key_Input(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::W))
	{
		_int i = 0;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_LU, 10.f, fTimeDelta);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_RU, 10.f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_U, 10.f, fTimeDelta);
		}
		return;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_LD, 10.f, fTimeDelta);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_RD, 10.f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_D, 10.f, fTimeDelta);
		}
		return;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_LU, 10.f, fTimeDelta);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_LD, 10.f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_L, 10.f, fTimeDelta);
		}
		return;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_RU, 10.f, fTimeDelta);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_RD, 10.f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_Pos(OBJ_DIR::DIR_R, 10.f, fTimeDelta);
		}
		return;
	}
}

void CPlayer::Player_Move(_float fTimeDelta)
{
	_float fDist = D3DXVec3Length(&m_vDest);
	if (fDist > 5.f)
	{
		_vec3 vDir, vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vDir = m_vDest - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 10.f);
	}
}
void CPlayer::Free()
{
	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}
