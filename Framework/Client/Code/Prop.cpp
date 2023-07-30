#include "Prop.h"
#include "Export_Function.h"

CProp::CProp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFixedObj(pGraphicDev, OBJ_ID::PROP)
{
	
}

CProp::CProp(const CProp& rhs)
	: CFixedObj(rhs)
{

}

CProp::~CProp()
{

}

HRESULT CProp::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	return S_OK;
}

_int CProp::Update_Object(const _float& fTimeDelta)
{
	if(!Is_Door(m_pTextureCom->Get_Idx()))
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CProp::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CProp::Render_Object(void)
{
	//Set_Billboard();
	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(m_pTextureCom->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	__super::Render_Object();
}

void CProp::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	
	
}

void CProp::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_ITEM
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_BREAK)
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



		if (fX - fabs(vDir.x) <= fZ - fabs(vDir.z))
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

void CProp::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

}

_bool CProp::Is_Door(_uint iIdx)
{
	return !(iIdx != 125
		&& iIdx != 128
		&& iIdx != 149
		&& iIdx != 150
		&& iIdx != 151
		&& iIdx != 157
		&& iIdx != 187
		&& iIdx != 188
		&& iIdx != 189
		&& iIdx != 190
		&& iIdx != 191
		&& iIdx != 202
		&& iIdx != 245
		&& iIdx != 246);
}

HRESULT CProp::Ready_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Prop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	return S_OK;
}

CProp* CProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProp* pInstance = new CProp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Prop Object Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProp::Free()
{

}
