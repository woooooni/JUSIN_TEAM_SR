#include "SludgeBall.h"
#include "Export_Function.h"

CSludgeBall::CSludgeBall(LPDIRECT3DDEVICE9 pGraphicDev) :CBullet(pGraphicDev, OBJ_ID::MONSTER_SKILL)
{
}
CSludgeBall::CSludgeBall(const CSludgeBall& rhs)
	: CBullet(rhs)
{

}

CSludgeBall::~CSludgeBall()
{
}

HRESULT CSludgeBall::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SludgeBall", L"Proto_Texture_SludgeBall", 0.1f);
	m_pAnimator->Play_Animation(L"SludgeBall", true);
	m_vDst = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.0f, 1.0f, 1.0f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.0f, 1.0f, 1.0f });
	m_fMinHeight = 0.26f;

	m_pMonsterAim = CMonsterAim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMonsterAim, E_FAIL);
	m_pMonsterAim->Set_Active(true);
	Set_Active(true);
	return S_OK;
}

_int CSludgeBall::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	if (Is_Active())
	{
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		m_pAnimator->Play_Animation(L"SludgeBall", true);
		_vec3 vDir, vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = m_vDst - vPos;
		vDir.y = 0.f;

		if (m_pAnimator->GetCurrAnimation()->Is_Finished())
		{
			if (Is_Active())
				Set_Active(false);
		}
		else if (vPos.y <= 0.26f)
		{
			if (m_pAnimator->GetCurrAnimation()->Get_Idx() < 6)
			{
				m_pAnimator->GetCurrAnimation()->Set_Idx(6);
				m_pMonsterAim->Set_Active(false);
			}
			return iExit;
		}
		else if (m_pAnimator->GetCurrAnimation()->Get_Idx() > 6)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(1);
		}
		
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), fTimeDelta, 7.5f);

		m_pMonsterAim->Get_TransformCom()->Set_Pos(&_vec3{ m_vDst.x,0.01f,m_vDst.z });
		m_pMonsterAim->Set_Red(0);
		m_pMonsterAim->Update_Object(fTimeDelta);
	}
	return iExit;
}

void CSludgeBall::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	m_pMonsterAim->LateUpdate_Object();
}

void CSludgeBall::Render_Object(void)
{
	if (Is_Active())
	{
		__super::Render_Object();
		m_pMonsterAim->Render_Object();

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


		IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
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
}

HRESULT CSludgeBall::Add_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}


CSludgeBall* CSludgeBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSludgeBall* pInstance = new CSludgeBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SludgeBall Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSludgeBall::Free()
{
	__super::Free();
}
void CSludgeBall::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER && m_pOwner->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() < 6)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(6);
			m_pMonsterAim->Set_Active(false);
		}
	}
}