#include "PlantCannon.h"
#include "Export_Function.h"
#include "PlantBall.h"
CPlantCannon::CPlantCannon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::SPIT_CACTUS)
{
}

CPlantCannon::CPlantCannon(const CPlantCannon& rhs)
	: CMonster(rhs)
{
}

CPlantCannon::~CPlantCannon()
{
}




HRESULT CPlantCannon::Ready_Object(void)
{
	Set_State(MONSTER_STATE::ATTACK);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"PlantCannon_Attack_Left", L"Proto_Texture_PlantCannon_Attack_Left", 0.1f);
	m_pAnimator->Add_Animation(L"PlantCannon_Attack_Right", L"Proto_Texture_PlantCannon_Attack_Right", 0.1f);
	m_fMinHeight = 0.75f;
	m_pTransformCom->Set_Pos(&_vec3(1.0f, 0.0f, 9.0f));
	m_pTransformCom->Set_Scale({ 2.f,2.f ,2.f });
	m_pAnimator->Play_Animation(L"PlantCannon_Attack_Left", true);
	m_tStat = { 3,3,1 };

	m_bPushable = false;
	return S_OK;
}

_int CPlantCannon::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	if (Is_Active())
	{
		Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);
	}
	return iExit;
}
void CPlantCannon::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CPlantCannon::Render_Object(void)
{
	if (Is_Active())
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

void CPlantCannon::Update_Attack(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 7&&m_bShoot)
	{
		_vec3  vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		CPlantBall* pPlantBall = CPlantBall::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pPlantBall, );
		_vec3 BulletPos = vPos;
		BulletPos.y += 0.25f;
		if (m_bRight)
			vDir = { 1,0,0 };
		else
			vDir = { -1,0,0 };
		pPlantBall->Get_TransformCom()->Set_Pos(&BulletPos);
		pPlantBall->Set_Dir(vDir);
		pPlantBall->Set_Owner(this);
		pPlantBall->Set_Atk(m_tStat.iAttack);
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
		pLayer->Add_GameObject(L"PlantBall", pPlantBall);
			m_bShoot = false;
		
	}
	if (m_pAnimator->GetCurrAnimation()->Get_Idx()==9)
	{
		m_bShoot = true;
	}
}

void CPlantCannon::Update_Move(_float fTimeDelta)
{
}
void CPlantCannon::Update_Idle(_float fTimeDelta)
{
}
void CPlantCannon::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CPlantCannon::Update_Regen(_float fTimeDelta)
{
}


HRESULT CPlantCannon::Add_Component(void)
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
void CPlantCannon::Set_Right(_bool _bRight)
{
	m_bRight = _bRight; 
	if (_bRight)
		m_pAnimator->Play_Animation(L"PlantCannon_Attack_Right", true);
	else
		m_pAnimator->Play_Animation(L"PlantCannon_Attack_Left", true);
}
CPlantCannon* CPlantCannon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlantCannon* pInstance = new CPlantCannon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("PlantCannon Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlantCannon::Trace(_float fTimeDelta)
{
}

void CPlantCannon::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;
	

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vPos - vTargetPos;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1)
			Set_State(MONSTER_STATE::DIE);
	}
	if (dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner() == nullptr)
		return;
	if (dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		m_tStat.iHp -= 1;
	}
}