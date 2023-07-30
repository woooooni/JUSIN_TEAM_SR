#include "FloorDoor.h"
#include "Export_Function.h"
#include "KeyMgr.h"

#include "Pool.h"
#include "Effect_Smoke.h"
#include "Particle_LargeStone.h"
#include "CParticle_Stone.h"
#include "Effect_StoneParticle.h"
#include "Portal.h"
#include "GameMgr.h"


CFloorDoor::CFloorDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFixedObj(pGraphicDev, OBJ_ID::PROP)
{
}

CFloorDoor::CFloorDoor(const CFloorDoor& rhs)
	: CFixedObj(rhs)
{
}

CFloorDoor::~CFloorDoor()
{
}

HRESULT CFloorDoor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	Engine::Add_Subscribe(21079635, this);

	m_fOpenDelay = 1.0f;


	m_fAccStoneTime = 0.0f;
	m_fStoneTime = 0.2;

	m_fAccLargeTime = 0.0f;
	m_fLargeTime = 1.3;


	Set_Active(true);


	return S_OK;
}

_int CFloorDoor::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);

	if (KEY_TAP(KEY::N))
		Open_Door();

	if (m_bOpenDelay)
	{
		m_fOpenDelay -= fTimeDelta;
		if (m_fOpenDelay < 0.0f)
		{
			m_bOpen = true;
			m_bOpenDelay = false;
		}
			
	}

	if (m_bOpen && !m_bFinish)
	{
		m_pTransformCom->Move_Pos(&m_vDir, 1.0f, fTimeDelta);
		


		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);


		if (m_vDir.x < 0.0f)
		{
			CCamera* pCamera = dynamic_cast<CCamera*>(Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
			NULL_CHECK_RETURN(pCamera, E_FAIL);
			pCamera->CamShake(fTimeDelta, 1.0f);

			Play_Sound(L"SFX_355_BigRockRolling.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, 0.5f);
		}


		if (D3DXVec3Length(&(m_vStartPos - vPos)) > 5.0f)
		{
			m_bFinish = true;

			if (m_vDir.x < 0.0f)
			{
				CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST2);
				_vec3 vPortalPos = _vec3(63.6f, 0.5f, 54.4f);
				pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
				dynamic_cast<CBoxCollider*>(pPortal->Get_ColliderCom())->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
				Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"NextPortal", pPortal);
				CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);
				Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
			}
		}

		if (m_fAccStoneTime > m_fStoneTime)
		{
			CParticle_Stone::Get_Effect(vPos, _vec3(2.5f, 0.5f, 5.0f), 30);
			m_fAccStoneTime = 0.0f;
		}
		else
			m_fAccStoneTime += fTimeDelta;

		if (m_fAccLargeTime > m_fLargeTime)
		{
			CParticle_LargeStone::Get_Effect(vPos, _vec3(2.5f, 0.5f, 5.0f), 10);
			m_fAccLargeTime = 0.0f;
		}
		else
			m_fAccLargeTime += fTimeDelta;



	}

	return S_OK;
}

void CFloorDoor::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CFloorDoor::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

void CFloorDoor::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CFloorDoor::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CFloorDoor::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}



void CFloorDoor::Open_Door()
{
	m_bOpenDelay = true;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vStartPos = vPos;




	if (m_vDir.x < 0.0f)
	{
		_vec3 vPos;
		_vec3 vEffectPos;


		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vPos.x += 2.5f;



		for (_int i = 0; 20 > i; ++i)
		{
			vEffectPos = vPos;
			vEffectPos.z -= 4.8f;
			vEffectPos.z += i * (9.6f / 20.0f);

			CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
			if (!pEffect)
			{
				pEffect = CEffect_Smoke::Create(m_pGraphicDev);
				pEffect->Ready_Object();
			}
			dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(1.0f, 1.0f, 1.0f), 186, 132, 72);
		}

		_vec3 vDir = { 0.0f, 0.0f, 5.0f };
		_float fAngle;

		for (_int i = 0; 360 > i; ++i)
		{
			vEffectPos = vDir;
			fAngle = 0.0f;
			fAngle += 1.0f * i;

			fAngle = D3DXToRadian(fAngle);

			_matrix matRot;
			D3DXMatrixRotationAxis(&matRot, &_vec3(0.0f, 1.0f, 0.0f), fAngle);

			D3DXVec3TransformNormal(&vEffectPos, &vEffectPos, &matRot);
			vEffectPos += vPos;


			CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
			if (!pEffect)
			{
				pEffect = CEffect_Smoke::Create(m_pGraphicDev);
				pEffect->Ready_Object();
			}
			dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(1.0f, 1.0f, 1.0f), 186, 132, 72);
		}

		Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
		Play_Sound(L"SFX_356_BigRockCrash.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, 0.5f);

	}

}

void CFloorDoor::Set_Door(_vec3& _vPos, _vec3& _vScale, _vec3& _vDir)
{
	_vPos.y = 0.04f;

	_float m_fAngle;

	m_fAngle = D3DXToRadian(90.0f);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}
	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fAngle);
	m_pTransformCom->Set_Pos(&_vPos);

	m_vDir = _vDir;

	CComponent* pComponent = nullptr;
	if (m_vDir.x < 0.0f)
	{
		
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_FloorDoorLeft"));
		NULL_CHECK(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	}
	else
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_FloorDoorRight"));
		NULL_CHECK(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	}

	

}

HRESULT CFloorDoor::Ready_Component()
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



	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);
	


	return S_OK;
}

CFloorDoor* CFloorDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFloorDoor* pInstance = new CFloorDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FloorDoor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFloorDoor::Free()
{
	__super::Free();
}
