#include "Nexus.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "Bullet.h"
#include "GameMgr.h"
#include "DefenceEnter.h"
#include "KeyMgr.h"
#include "DesertRhino.h"
#include "FloorDoor.h"
#include "TrashBig.h"
#include "MothMage.h"
#include "MonsterSpawner.h"

CNexus::CNexus(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, OBJ_ID::NEXUS)
{
}

CNexus::CNexus(const CNexus& rhs)
	: CGameObject(rhs)
{
}

CNexus::~CNexus()
{
}

HRESULT CNexus::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(5.0f, 5.0f, 5.0f));

	m_fMinHeight = 0.2f;

	m_fDefenceAccTime = 0.0f;
	m_fDefenceTime = 60.0f;

	m_iMaxHp = m_iHp = 100;
	m_iAlpha = 255;

	CGameObject* pHpBar = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_BACK);
	NULL_CHECK_RETURN(pHpBar, E_FAIL);
	m_vecHpBar.push_back(pHpBar);

	pHpBar = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_GAUGE);
	NULL_CHECK_RETURN(pHpBar, E_FAIL);
	m_vecHpBar.push_back(pHpBar);

	pHpBar = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_FRAME);
	NULL_CHECK_RETURN(pHpBar, E_FAIL);
	m_vecHpBar.push_back(pHpBar);

	for (auto& iter : m_vecHpBar)
	{
		dynamic_cast<CUI_BossHP*>(iter)->Set_Name(BOSSNAME::NEXUS);
	}

	return S_OK;
}

_int CNexus::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_NEXUS);

	_int iExit = __super::Update_Object(fTimeDelta);


	if (m_bStart && !m_bClear)
	{
		m_fDefenceAccTime += fTimeDelta;

		for (auto& iter : m_vecHpBar)
		{
			iter->Set_Active(true);
			dynamic_cast<CUI_BossHP*>(iter)->Update_Object(fTimeDelta);
		}

		

		if (!m_bFail && m_fDefenceAccTime >= m_fDefenceTime)
		{
			m_fDefenceAccTime = m_fDefenceTime;
			m_bClear = true;

			CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
			NULL_CHECK_RETURN(pPlayer,E_FAIL);
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vPos.y = 0.0f;
			vPos.z += 10.0f;
			pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
			pPlayer->SetObj_Dir(OBJ_DIR::DIR_D);

			CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");
			NULL_CHECK_RETURN(pCamera, E_FAIL);

			dynamic_cast<CCamera*>(pCamera)->Set_TargetObj(this);
			dynamic_cast<CCamera*>(pCamera)->Add_Offset();

			vector<CGameObject*>& vecMonsters = Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec();

			for (auto& iter : vecMonsters)
			{
				if (iter->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
					dynamic_cast<CMonster*>(iter)->Set_State(MONSTER_STATE::DIE);
				else
					iter->Set_Active(false);
			}

			for (auto& iter : m_vecSpawner)
			{
				dynamic_cast<CMonsterSpawner*>(iter)->Set_Spawn(false);
			}
		}
	}

	if (m_bClear && !m_bOpen)
	{
		m_iAlpha -= 1;
		if (m_iAlpha <= 0)
		{
			m_iAlpha = 0;

			m_bOpen = true;

			CGameObject* pLeftDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"LeftDoor");
			NULL_CHECK_RETURN(pLeftDoor, E_FAIL);
			CGameObject* pRightDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"RightDoor");
			NULL_CHECK_RETURN(pRightDoor, E_FAIL);

			dynamic_cast<CFloorDoor*>(pLeftDoor)->Open_Door();
			dynamic_cast<CFloorDoor*>(pRightDoor)->Open_Door();
		}
	}

	if (m_bOpen)
	{
		CGameObject* pLeftDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"LeftDoor");
		NULL_CHECK_RETURN(pLeftDoor, E_FAIL);
		CGameObject* pRightDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"RightDoor");
		NULL_CHECK_RETURN(pRightDoor, E_FAIL);

		if (dynamic_cast<CFloorDoor*>(pLeftDoor)->Get_Finish() && dynamic_cast<CFloorDoor*>(pRightDoor)->Get_Finish())
		{
			CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
			NULL_CHECK_RETURN(pPlayer, E_FAIL);

			CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");
			NULL_CHECK_RETURN(pCamera, E_FAIL);

			dynamic_cast<CCamera*>(pCamera)->Set_TargetObj(pPlayer);
			//dynamic_cast<CCamera*>(pCamera)->Minus_Offset();
		}
	}


	if (m_bFail)
	{
		Reset_Defence();
	}

	if (m_iHp <= 0)
	{
		m_iHp = 0;
		m_bFail = true;
	}


	if (KEY_TAP(KEY::R))
	{
		m_iHp = 0;
	}

	if (KEY_TAP(KEY::T))
	{
		CGameObject* pMonster = CMothMage::Create(m_pGraphicDev);
		_vec3 vPos;
		CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

		pMonster->Get_TransformCom()->Set_Pos(&(vPos + _vec3(0.0f, 0.0f, -2.0f)));
		dynamic_cast<CMonster*>(pMonster)->Set_DefenceMode(this);
		Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"TrashBig", pMonster);

	}


	return iExit;
}

void CNexus::LateUpdate_Object(void)
{
	if (m_bStart && !m_bClear)
	{
		for (auto& iter : m_vecHpBar)
		{
			dynamic_cast<CUI_BossHP*>(iter)->LateUpdate_Object();
		}
	}

	__super::LateUpdate_Object();
}

void CNexus::Render_Object(void)
{
	if (!Is_Active())
		return;

	if (m_bStart && !m_bClear)
	{
		RECT rc = { WINCX / 2 - 50 + 1 , 60 , WINCX / 2 + 50 + 1 , 160 };
		wstring szBuf;
		szBuf = to_wstring(m_fDefenceTime - m_fDefenceAccTime);
		szBuf = szBuf.substr(0, 5);

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szBuf.c_str(), INT(szBuf.size()), &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	}


	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	__super::Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CNexus::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (m_bClear)
		return;

	if (pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		m_iHp -= 1;
		pCollider->GetOwner()->Set_Active(false);
	}

	if (pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_BULLET && 
		dynamic_cast<CBullet*>(pCollider->GetOwner())->Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		pCollider->GetOwner()->Set_Active(false);
		m_iHp -= 1;
	}

	if (m_iHp <= 0)
	{
		m_iHp = 0;
		m_bFail = true;
	}

}

void CNexus::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CNexus::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CNexus::Ready_Component(void)
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


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Nexus"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(5.0f, 5.0f, 2.0f));

	return S_OK;
}

CNexus* CNexus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNexus* pInstance = new CNexus(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Nexus_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNexus::Start_Defence()
{
	m_bStart = true;
	for (auto& iter : m_vecSpawner)
	{
		dynamic_cast<CMonsterSpawner*>(iter)->Set_Spawn(true);
	}

	
}

void CNexus::Reset_Defence()
{
	Reset();

	CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();

	NULL_CHECK_RETURN(pPlayer, );

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 63.6f;
	vStartPos.z = 10.4f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);
	pPlayer->SetObj_Dir(OBJ_DIR::DIR_U);

	CGameObject* pDefenceEnter = CDefenceEnter::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pDefenceEnter, );
	pDefenceEnter->Get_TransformCom()->Set_Pos(&_vec3(64.0f, 2.6f, 16.0f));
	Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"DefenceEnter", pDefenceEnter);


	vector<CGameObject*>& vecMonsters = Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec();

	for (auto& iter : vecMonsters)
	{
		if (iter->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
			dynamic_cast<CMonster*>(iter)->Set_State(MONSTER_STATE::DIE);
		else
			iter->Set_Active(false);
	}

	for (auto& iter : m_vecSpawner)
	{
		dynamic_cast<CMonsterSpawner*>(iter)->Set_Spawn(false);
	}
}

void CNexus::Free()
{
	__super::Free();
}
