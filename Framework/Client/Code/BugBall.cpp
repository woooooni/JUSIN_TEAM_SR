#include "BugBall.h"
#include "Export_Function.h"

CBugBall::CBugBall(LPDIRECT3DDEVICE9 pGraphicDev) : CBullet(pGraphicDev, OBJ_ID::BUG_BALL)
{
}
CBugBall::CBugBall(const CBugBall& rhs)
	:CBullet(rhs)
{

}

CBugBall::~CBugBall()
{
}

HRESULT CBugBall::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"BugBall", L"Proto_Texture_BugBall", 0.1f);
	m_pAnimator->Play_Animation(L"BugBall", true);
	m_vDir = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({0.5f, 0.5f, 0.5f });
	m_tMaterial.Emissive = { 1.2f,1.2f,1.2f,1.2f };
	Set_Active(true);
	return S_OK;
}

_int CBugBall::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	m_pAnimator->Play_Animation(L"BugBall", true);

	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, m_fSpeed);
	if (m_fMoveTime < 0.f)
	{
		if (Is_Active())
		{
			Set_Active(false);
			CEventMgr::GetInstance()->DeleteObjEvt(this);


			int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
			Stop_Sound((CHANNELID)iSound);
			Play_Sound(L"SFX_97_BugBulletDisappear.wav", (CHANNELID)iSound, 0.5f);
		}
			
		m_fMoveTime = 0.f;
	}
	m_fMoveTime -= 10.f * fTimeDelta;
	return iExit;
}

void CBugBall::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y < 0.f)
	{
		if (Is_Active())
			Set_Active(false);
		m_fMoveTime = 0.f;

		int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_97_BugBulletDisappear.wav", (CHANNELID)iSound, 0.5f);
	}
	__super::LateUpdate_Object();


}

void CBugBall::Render_Object(void)
{
	if (!Is_Active())
		return ;


		__super::Render_Object();

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;

		D3DCOLORVALUE vColor = { 255.0f,	255.f,255.f, 255.f };

		pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
		pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
		pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
		pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
		pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
		pEffect->SetFloat("g_AlphaRef", 0.0f);


		IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
		pEffect->SetTexture("g_Texture", pTexture);


		CLightMgr::GetInstance()->Set_LightToEffect(pEffect);


		pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();




	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CBugBall::Add_Component(void)
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


CBugBall* CBugBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBugBall* pInstance = new CBugBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BugBall Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CBugBall::Free()
{
	__super::Free();
}

void CBugBall::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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
	Stop_Sound((CHANNELID)iSound);
	Play_Sound(L"SFX_94_BugBallHit.wav", (CHANNELID)iSound, 0.5f);
}