#include "../Include/stdafx.h"
#include "FieldObject.h"
#include "Export_Function.h"

CFieldObject::CFieldObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eObjId) : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, _eObjId)
, m_eFState(FIELD_OBJ_STATE::NOT_INTERACT)
, m_eCutSceneType(CCutSceneMgr::CUTSCENE_TYPE::TYPE_END)

{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}

CFieldObject::CFieldObject(const CFieldObject& rhs) : CGameObject(rhs), m_tInfo(rhs.m_tInfo)
, m_eFState(rhs.m_eFState)
, m_eCutSceneType(rhs.m_eCutSceneType)
{
}

CFieldObject::~CFieldObject()
{
}

_int CFieldObject::Update_Object(const _float& pFtimeDelta)
{
	return __super::Update_Object(pFtimeDelta);
}

void CFieldObject::LateUpdate_Object()
{
	_vec3  src, myPos;

	src = { 0, 0, 0 };

	for (auto& iter : m_vMovingPos)
	{
		src += iter.second;
	}
	m_pTransformCom->Get_Info(INFO_POS, &myPos);
	m_pTransformCom->Set_Pos(&(myPos + src));

	m_vMovingPos.clear();

	m_pColliderCom->LateUpdate_Component();
	
	__super::LateUpdate_Object();
}

void CFieldObject::Render_Object()
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

void CFieldObject::Push_Me(CCollider* other)
{
	if (!other->Is_Active() || !m_pColliderCom->Is_Active())
		return;

	if (other->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		return;


	const _vec3& vLeftScale = ((CBoxCollider*)m_pColliderCom)->Get_Scale();
	const _vec3& vLeftPos = ((CBoxCollider*)m_pColliderCom)->Get_Pos();
	const _vec3& vRightScale = ((CBoxCollider*)other)->Get_Scale();
	const _vec3& vRightPos = ((CBoxCollider*)other)->Get_Pos();

	if (fabs(vRightPos.x - vLeftPos.x) < 0.5f * (vLeftScale.x + vRightScale.x)
		&& fabs(vRightPos.y - vLeftPos.y) < 0.5f * (vLeftScale.y + vRightScale.y)
		&& fabs(vRightPos.z - vLeftPos.z) < 0.5f * (vLeftScale.z + vRightScale.z))
	{
		float colX = 0.5f * (vLeftScale.x + vRightScale.x) - fabs(vRightPos.x - vLeftPos.x);
		float colY = 0.5f * (vLeftScale.y + vRightScale.y) - fabs(vRightPos.y - vLeftPos.y);
		float colZ = 0.5f * (vLeftScale.z + vRightScale.z) - fabs(vRightPos.z - vLeftPos.z);

		if (colX < colZ)
		{
			if (vLeftPos.x > vRightPos.x)
				m_vMovingPos.insert({ other,_vec3(colX * 0.5f, 0, 0) });
			else
				m_vMovingPos.insert({ other,_vec3(colX * -0.5f, 0, 0) });
		}

		else if (colZ < colX)
		{
			if (vLeftPos.z > vRightPos.z)
				m_vMovingPos.insert({ other, _vec3(0, 0, 0.5f * colZ) });			
			else
				m_vMovingPos.insert({ other, _vec3(0, 0, -0.5f * colZ) });

		}
	}

}

HRESULT CFieldObject::Ready_Component()
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);
	
	return S_OK;

}
