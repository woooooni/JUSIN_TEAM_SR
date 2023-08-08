#include "CactusNeedle.h"
#include "Export_Function.h"

CCactusNeedle::CCactusNeedle(LPDIRECT3DDEVICE9 pGraphicDev) : CBullet(pGraphicDev,  OBJ_ID::MONSTER_SKILL)
{
}
CCactusNeedle::CCactusNeedle(const CCactusNeedle& rhs)
	: CBullet(rhs)
{

}

CCactusNeedle::~CCactusNeedle()
{
}

HRESULT CCactusNeedle::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"CactusNeedlePoison", L"Proto_Texture_CactusNeedlePoison", 0.1f);
	m_pAnimator->Add_Animation(L"CactusNeedle", L"Proto_Texture_CactusNeedle", 0.1f);
	m_pAnimator->Play_Animation(L"CactusNeedle", true);
	m_vDir = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f }); 
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({0.2f, 0.2f, 0.2f });
	_vec3 AxisX = { 1,0,0 };
	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
	return S_OK;
}

_int CCactusNeedle::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	if (m_bPoison == true)
		m_pAnimator->Play_Animation(L"CactusNeedlePoison", true);
	else
		m_pAnimator->Play_Animation(L"CactusNeedle", true);

	_vec3 AxisY = { 0,1,0 };
	if(m_bSpin)
	m_pTransformCom->RotationAxis(AxisY, D3DXToRadian(1.f));
	m_pTransformCom->Get_Info(INFO_UP, &m_vDir);
	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 5.f);
	if (m_fMoveTime < 0.f)
	{
		if (Is_Active())
		{
			Set_Active(false);
			CEventMgr::GetInstance()->DeleteObjEvt(this);
		}
			
		m_fMoveTime = 0.f;
	}
	m_fMoveTime -= 10.f * fTimeDelta;
	return iExit;
}

void CCactusNeedle::LateUpdate_Object(void)
{
	if (!Is_Active())
		return  ;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::LateUpdate_Object();


}

void CCactusNeedle::Render_Object(void)
{
	if (!Is_Active())
		return  ;
	
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



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();


	
}

HRESULT CCactusNeedle::Add_Component(void)
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}


CCactusNeedle* CCactusNeedle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCactusNeedle* pInstance = new CCactusNeedle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CactusNeedle Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CCactusNeedle::Free()
{
	__super::Free();
}

void CCactusNeedle::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{


	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER && m_pOwner->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = m_vDir;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= -1;
		Set_Owner(pCollider->GetOwner());
		m_vDir = vDir;
		m_fMoveTime = 20.f;
	}
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER && Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		if (Is_Active())
			Set_Active(false);
	}
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER && Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		if (Is_Active())
			Set_Active(false);
	}
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BOSS && Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		if (Is_Active())
			Set_Active(false);
	}

	int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
}