#include "BossDoor.h"
#include "Export_Function.h"
#include "KeyMgr.h"

#include "Pool.h"
#include "Effect_Smoke.h"
#include "Particle_LargeStone.h"
#include "CParticle_Stone.h"
#include "Effect_StoneParticle.h"
#include "Portal.h"
#include "GameMgr.h"
#include "Player.h"


CBossDoor::CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFixedObj(pGraphicDev, OBJ_ID::PROP)
{
}

CBossDoor::CBossDoor(const CBossDoor& rhs)
	: CFixedObj(rhs)
{
}

CBossDoor::~CBossDoor()
{
}

HRESULT CBossDoor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	Engine::Add_Subscribe(21079635, this);

	m_fOpenDelay = 1.0f;


	m_fAccStoneTime = 0.0f;
	m_fStoneTime = 0.3;

	m_fAccLargeTime = 0.0f;
	m_fLargeTime = 1.5;
	m_fAngle = 0.0f;

	m_fAccSmokeTime = 0.0f;
	m_fSmokeTime = 0.3f;

	Set_Active(true);


	return S_OK;
}

_int CBossDoor::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
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
		if (m_vDir.x < 0.0f)
			m_fAngle -= 0.2f;
		else
			m_fAngle += 0.2f;

		_float fRadian = D3DXToRadian(m_fAngle);

		_vec3 vDir;
		vDir = m_vOriginPos - m_vStartPos;
		
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &_vec3(0.0f, 1.0f, 0.0f), fRadian);
		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

		_matrix matWorld;
		D3DXMatrixIdentity(&matWorld);

		for (_uint i = 0; INFO_END > i; ++i)
		{
			_vec3 vInfo;
			memcpy(&vInfo, matWorld.m[i], sizeof(_vec3));
			m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
		}
		m_pTransformCom->Set_Scale(m_vScale);
		m_pTransformCom->RotationAxis(_vec3(0.0f, 1.0f, 0.0f), fRadian);
		m_pTransformCom->Set_Pos(&(m_vStartPos + vDir));

		
		if (m_vDir.x < 0.0f)
		{
			CCamera* pCamera = dynamic_cast<CCamera*>(Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
			NULL_CHECK_RETURN(pCamera, E_FAIL);
			pCamera->CamShake(fTimeDelta, 1.0f);

			Play_Sound(L"SFX_355_BigRockRolling.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, 0.5f);
		}


		if (fabs(m_fAngle) > 80.0f)
		{
			m_bFinish = true;
			m_pColliderCom->Set_Active(false);
			


			if (m_vDir.x < 0.0f)
			{
				CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_VILLAGE);
				_vec3 vPortalPos = _vec3(9.f, 0.5f, 23.f);
				pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
				Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"NextPortal", pPortal);

				CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
				NULL_CHECK_RETURN(pPlayer, E_FAIL);
				dynamic_cast<CPlayer*>(pPlayer)->Set_Stop(false);

				Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
			}
		}



		_vec3 vEffectPos;
		_float fLength;
	
		if (m_fAccStoneTime > m_fStoneTime)
		{
			

			for (_uint i = 0; 10 > i; ++i)
			{
				D3DXVec3Normalize(&vDir, &vDir);
				fLength = (m_vScale.x / 10.0f) * i;
				vEffectPos = m_vStartPos + (vDir * fLength);
				vEffectPos.y = 0.0f;
				CParticle_Stone::Get_Effect(vEffectPos, _vec3(0.3f, 0.3f, 0.3f), 2);
				m_fAccStoneTime = 0.0f;
			}
		}
		else
			m_fAccStoneTime += fTimeDelta;

		if (m_fAccLargeTime > m_fLargeTime)
		{
			for (_uint i = 0; 10 > i; ++i)
			{
				D3DXVec3Normalize(&vDir, &vDir);
				fLength = (m_vScale.x / 10.0f) * i;
				vEffectPos = m_vStartPos + (vDir * fLength);
				vEffectPos.y = 0.0f;
				CParticle_LargeStone::Get_Effect(vEffectPos, _vec3(0.3f, 0.3f, 0.3f), 1);
				m_fAccLargeTime = 0.0f;
			}
		}
		else
			m_fAccLargeTime += fTimeDelta;

		if (m_fAccSmokeTime > m_fSmokeTime)
		{
			for (_uint i = 0; 10 > i; ++i)
			{
				D3DXVec3Normalize(&vDir, &vDir);
				fLength = (m_vScale.x / 10.0f) * i;
				vEffectPos = m_vStartPos + (vDir * fLength);
				vEffectPos.y = 0.3f;

				CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
				if (!pEffect)
				{
					pEffect = CEffect_Smoke::Create(m_pGraphicDev);
					pEffect->Ready_Object();
				}
				dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(0.5f, 0.5f, 0.5f), 139, 69, 19);
				
				m_fAccSmokeTime = 0.0f;
			}
		}
		else
			m_fAccSmokeTime += fTimeDelta;

		
	}

	return S_OK;
}

void CBossDoor::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBossDoor::Render_Object(void)
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


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(m_pTextureCom->Get_Idx());
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

void CBossDoor::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBossDoor::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER)
	{
		_float fDeltaX = 0.f, fDeltaY = 0.f, fDeltaZ = 0.f;

		_vec3 vPos, vOtherPos;
		_vec3 vMin, vMax;
		_vec3 vOtherMin, vOtherMax;

		CTransform* pTransform = m_pTransformCom;
		CTransform* pOtherTransform = pCollider->GetOwner()->Get_TransformCom();

		CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
		CBoxCollider* pOtherBoxCollider = dynamic_cast<CBoxCollider*>(pCollider);


		NULL_CHECK_RETURN(pTransform, );
		NULL_CHECK_RETURN(pOtherTransform, );
		NULL_CHECK_RETURN(pBoxCollider, );
		NULL_CHECK_RETURN(pOtherBoxCollider, );

		pTransform->Get_Info(INFO_POS, &vPos);
		pOtherTransform->Get_Info(INFO_POS, &vOtherPos);

		_vec3 vDir = vOtherPos - vPos;
		_float fX = (pBoxCollider->Get_Scale().x * 0.5f) + (pOtherBoxCollider->Get_Scale().x * 0.5f);
		_float fY = (pBoxCollider->Get_Scale().y * 0.5f) + (pOtherBoxCollider->Get_Scale().y * 0.5f);
		_float fZ = (pBoxCollider->Get_Scale().z * 0.5f) + (pOtherBoxCollider->Get_Scale().z * 0.5f);


		if (fabs(vDir.x) >= fX)
			return;

		if (fabs(vDir.y) >= fY)
			return;

		if (fabs(vDir.z) >= fZ)
			return;



		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z))
		{
			if (vDir.x < 0.f)
			{
				vDir.x -= (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.x += (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		/*else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
		{
			if (vDir.y < 0.f)
			{
				vDir.y -= (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.y += (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}*/
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x))
		{
			if (vDir.z < 0.f)
			{
				vDir.z -= (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.z += (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}


	}
}

void CBossDoor::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}



void CBossDoor::Open_Door()
{
	m_bOpenDelay = true;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);


	if (m_vDir.x < 0.0f)
	{
		_vec3 vPos;
		_vec3 vEffectPos;

		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.x += m_vScale.x * 0.5f;
		vPos.y -= m_vScale.y * 0.5f;

		_float fLength = m_vScale.y;

		for (_int i = 0; 20 > i; ++i)
		{
			vEffectPos = vPos;
			vEffectPos.y += (fLength / 20.0f) * i;

			CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
			if (!pEffect)
			{
				pEffect = CEffect_Smoke::Create(m_pGraphicDev);
				pEffect->Ready_Object();
			}
			dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(1.5f, 1.5f, 1.5f), 139, 69, 19);
		}

		Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
		Play_Sound(L"SFX_356_BigRockCrash.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, 0.5f);
	}


}

void CBossDoor::Set_Door(_vec3& _vPos, _vec3& _vScale, _vec3& _vDir)
{
	m_vScale = _vScale;
	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->Set_Pos(&_vPos);
	_vScale.z = 2.0f;
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vScale);

	m_vDir = _vDir;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vOriginPos = m_vStartPos = vPos;

	if (m_vDir.x < 0.0f)
	{
		m_vStartPos.x -= (m_vScale.x * 0.5f);
	}
	else
	{
		m_vStartPos.x += (m_vScale.x * 0.5f);
	}

	CComponent* pComponent = nullptr;
	if (m_vDir.x < 0.0f)
	{
		
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Prop"));
		NULL_CHECK(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_pTextureCom->Set_Idx(123);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	}
	else
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Prop"));
		NULL_CHECK(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_pTextureCom->Set_Idx(124);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	}

	

}

HRESULT CBossDoor::Ready_Component()
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


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	return S_OK;
}

CBossDoor* CBossDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossDoor* pInstance = new CBossDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FloorDoor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossDoor::Free()
{
	__super::Free();
}
