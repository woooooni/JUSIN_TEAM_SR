#include "Door.h"
#include "Export_Function.h"
#include "KeyMgr.h"
#include "Effect_Smoke.h"
#include "Pool.h"
#include "GameMgr.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFixedObj(pGraphicDev, OBJ_ID::PROP)
{
}

CDoor::CDoor(const CDoor& rhs)
	: CFixedObj(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	Engine::Add_Subscribe(21079634, this);
	m_pRigidBodyCom->SetGravity(-9.8f);
	return S_OK;
}

_int CDoor::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);

	if (KEY_TAP(KEY::N))
		Event_End(21079634);


	if (m_bOpen && !m_bFinish)
	{
		if (m_pRigidBodyCom->IsGround())
		{
			m_bFinish = true;

			CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
			NULL_CHECK_RETURN(pPlayer, E_FAIL);
			pPlayer->SetObj_Dir(OBJ_DIR::DIR_U);
			dynamic_cast<CPlayer*>(pPlayer)->Set_Stop(false);
		}
	}

	return S_OK;
}

void CDoor::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDoor::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(m_pTextureCom->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

void CDoor::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CDoor::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
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



		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z) && fX - fabs(vDir.x) < fY - fabs(vDir.y))
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
		else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
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
		}
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x) && fZ - fabs(vDir.z) < fY - fabs(vDir.y))
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

void CDoor::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CDoor::Event_Start(_uint iEventNum)
{
	m_fMinHeight = -(m_fMinHeight * 2.0f);
	m_pRigidBodyCom->SetGround(false);
	m_bOpen = true;

	Effect();
}

void CDoor::Event_End(_uint iEventNum)
{
	m_fMinHeight = -(m_fMinHeight * 2.0f);
	m_pRigidBodyCom->SetGround(false);
	m_bOpen = true;

	Effect();
}

void CDoor::Set_Door(_vec3& _vPos, _vec3& _vScale)
{
	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_vScale = _vScale;
	m_fMinHeight = _vPos.y;
	_vScale.x *= 2.0f;
	_vScale.z = 0.2f;
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vScale);
	Set_Active(true);
}

void CDoor::Open_Door()
{
	m_fMinHeight = -(m_fMinHeight * 2.0f);
	m_pRigidBodyCom->SetGround(false);
	m_bOpen = true;

	Effect();
}

HRESULT CDoor::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Door"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CDoor::Effect(void)
{
	_vec3 vPos, vEffectPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	
	vPos.x -= m_vScale.x * 0.5f;
	vPos.y = 0.3f;

	
	for (_uint i = 0; 21 > i; ++i)
	{
		vEffectPos = vPos;
		vEffectPos.x += (m_vScale.x / 20.f) * i;


		CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Smoke::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(1.5f, 1.5f, 1.5f), 255, 255, 255);
	}

}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor* pInstance = new CDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();
}
