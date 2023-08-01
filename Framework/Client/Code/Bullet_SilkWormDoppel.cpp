#include "Bullet_SilkWormDoppel.h"
#include "Export_Function.h"
#include "Effect_MothFlyLine.h"
#include "GameMgr.h"
CBullet_SilkWormDoppel::CBullet_SilkWormDoppel(LPDIRECT3DDEVICE9 pGraphicDev) : CBullet(pGraphicDev,  OBJ_ID::MONSTER_SKILL)
{
}
CBullet_SilkWormDoppel::CBullet_SilkWormDoppel(const CBullet_SilkWormDoppel& rhs)
	: CBullet(rhs)
{

}

CBullet_SilkWormDoppel::~CBullet_SilkWormDoppel()
{
}

HRESULT CBullet_SilkWormDoppel::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Regen", L"Proto_Texture_BugBoss_Phase2_Regen", 0.1f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Attack", L"Proto_Texture_BugBoss_Phase2_Attack", 0.2f);
	m_pAnimator->Play_Animation(L"BugBoss_Phase2_Regen", true);
	m_vDir = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f }); 
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 0.2f, 0.2f, 0.2f });
	return S_OK;
}

_int CBullet_SilkWormDoppel::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	m_fActivateTime -= fTimeDelta;
	if (m_fActivateTime < 0.f)
	{
		if (!m_bShoot)
		{
			m_bShoot = true;
			_vec3 AxisX = { 1,0,0 };
			m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
			m_pTransformCom->Set_Pos(&m_vRandPos);
			_vec3 vUp, vLook, vRight;
			m_pTransformCom->Get_Info(INFO_UP, &vUp);
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
			m_vDir.y = 0.f;
			float fAngle = acosf(D3DXVec3Dot(D3DXVec3Normalize(&m_vDir, &m_vDir), D3DXVec3Normalize(&vUp, &-vUp)));

			if (D3DXVec3Dot(&vRight, &m_vDir) > 0)
			{
				fAngle *= -1.f;
			}
			m_pTransformCom->RotationAxis(-vLook, fAngle);
			m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
			m_pAnimator->GetCurrAnimation()->Set_Idx(3);
		}
		float fAccel = 1.5f * (10.f-m_fMoveTime) * (10.f - m_fMoveTime);
		m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 15.f * fAccel);
		m_fMoveTime -= fTimeDelta;
	}
	else
	{
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 0)
			m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}
	if (m_fActivateTime < 2.f||!m_bReady)
	{

		_vec3 vTargetPos, vPos, vDir;
		CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
		if (nullptr == pTarget)
			return S_OK;
		pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;
		Create_Line();
		m_bReady = true;
	}


	if (m_fMoveTime < 0.f)
	{
		if (Is_Active())
		{
			Set_Active(false);
			m_pLine->Set_Active(false);
			CEventMgr::GetInstance()->DeleteObjEvt(this);
		}
			
		m_fMoveTime = 0.f;
	}
	m_fMoveTime -= 10.f * fTimeDelta;
	return iExit;
}

void CBullet_SilkWormDoppel::LateUpdate_Object(void)
{
	if (!Is_Active())
		return  ;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::LateUpdate_Object();


}

void CBullet_SilkWormDoppel::Render_Object(void)
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



		pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	
}

HRESULT CBullet_SilkWormDoppel::Add_Component(void)
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
void CBullet_SilkWormDoppel::Create_Line()
{
	if (m_pLine)
		dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Render(true);
	else
	{
		m_pLine = CEffect_MothFlyLine::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pLine, );
		dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Owner(this);
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::EFFECT);
		pLayer->Add_GameObject(L"Effect_MothFlyLine", m_pLine);
	}
	_vec3 vPos, vUp, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y = 0.09f;
	m_pLine->Get_TransformCom()->Set_Pos(&vPos);
	_vec3 vScale = m_pLine->Get_TransformCom()->Get_Scale();
	vScale.y = sqrtf(((45.f * 2.f) * (45.f * 2.f)) * 2.f);
	m_pLine->Get_TransformCom()->Set_Scale(vScale);
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	vPos -= m_vDir * vScale.y * 0.5f;
	m_pLine->Get_TransformCom()->Set_Pos(&vPos);
	vPos += m_vDir * vScale.y;
	dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Dst(vPos);
	m_pLine->Get_TransformCom()->Get_Info(INFO_UP, &vUp);
	m_pLine->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
	m_pLine->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);

	float fAngle = acosf(D3DXVec3Dot(D3DXVec3Normalize(&m_vDir, &m_vDir), D3DXVec3Normalize(&vUp, &-vUp)));
	if (D3DXVec3Dot(&vRight, &m_vDir) > 0)
	{
		fAngle *= -1.f;
	}
	m_pLine->Get_TransformCom()->RotationAxis(-vLook, fAngle);


}
CBullet_SilkWormDoppel* CBullet_SilkWormDoppel::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRandPos, _float _fTime, COLOR_STATE _eColor, _float _iAtk)
{
	CBullet_SilkWormDoppel* pInstance = new CBullet_SilkWormDoppel(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Bullet_SilkWormDoppel Create Failed");
		return nullptr;
	}

	pInstance->Get_TransformCom()->Set_Pos(&vPos);
	pInstance->m_fActivateTime = _fTime;
	pInstance->m_eColor = _eColor;
	pInstance->m_iAtk = _iAtk;
	pInstance->m_vRandPos = vRandPos;
	return pInstance;
}

CBullet_SilkWormDoppel* CBullet_SilkWormDoppel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet_SilkWormDoppel* pInstance = new CBullet_SilkWormDoppel(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Bullet_SilkWormDoppel Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBullet_SilkWormDoppel::Free()
{
	__super::Free();
}
