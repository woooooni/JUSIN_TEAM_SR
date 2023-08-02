#include "../Include/stdafx.h"
#include "BlockObj.h"
#include "Export_Function.h"
#include "Effect_StoneParticle.h"

CBlockObj::CBlockObj(LPDIRECT3DDEVICE9 p_Dev) 
	: CFieldObject(p_Dev, OBJ_ID::BLOCK_OBJ)
	, m_iEventNum(0)
	, m_bIsBlocking(false)
	, m_iFollowingEvent(0)
	, m_vBlockPos(0, 0.5f, -0.4f)
	,m_pBlurTex(nullptr)
{
}

CBlockObj::CBlockObj(const CBlockObj& rhs) : CFieldObject(rhs), m_iEventNum(rhs.m_iEventNum), m_bIsBlocking(rhs.m_bIsBlocking), m_iFollowingEvent(rhs.m_iFollowingEvent), m_pBlurTex(rhs.m_pBlurTex)
{
}

CBlockObj::~CBlockObj()
{
}

HRESULT CBlockObj::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_Block_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"Block_Idle", L"Proto_Tex_Block_Block_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"UnBlocking", L"Proto_Tex_Block_UnBlocking", 0.1f);
	m_pAnimator->Add_Animation(L"Blocking", L"Proto_Tex_Block_Blocking", 0.1f);
	m_pAnimator->Add_Animation(L"Unblock_Idle", L"Proto_Tex_Block_Unblock_Idle", 0.1f);


	return S_OK;
}

_int CBlockObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	if (!m_bIsBlocking)
	{
		if (m_vBlockPos.y > 0.f)
			m_vBlockPos.y -= fTimeDelta;
		else
		{
			m_pAnimator->Play_Animation(L"Idle", false);
			if (m_vBlockPos.y != 0.f)
				m_vBlockPos.y = 0.f;
		}

	}
	else
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

		m_pAnimator->Play_Animation(L"Unblock_Idle", false);

		if (m_vBlockPos.y < 0.5f)
		{
			m_vBlockPos.y += fTimeDelta;
		}
		else if (m_vBlockPos.y != 0.5f)
			m_vBlockPos.y = 0.5f;
	}
	
	return __super::Update_Object(fTimeDelta);
}

void CBlockObj::LateUpdate_Object(void)
{
	
}

void CBlockObj::Render_Object(void)
{
	__super::Render_Object();

	if (m_vBlockPos.y > 0.f)
	{
		_matrix mat;
		_vec3 pos;
		m_pTransformCom->Get_Info(INFO_POS, &pos);
		pos += m_vBlockPos;
		D3DXMatrixScaling(&mat, 0.8f, 0.8f, 0.8f);
		mat._41 += pos.x;
		mat._42 += pos.y - 0.1f;
		mat._43 += pos.z;


		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;


		pEffect->SetMatrix("g_WorldMatrix", &mat);
		pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
		pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
		pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
		pEffect->SetFloat("g_AlphaRef", 0.0f);


		IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
		pEffect->SetTexture("g_Texture", pTexture);


		CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

		

		pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		if (m_iBlurEvent != 0)
		{
			mat._43 -= 0.01f;

			LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

			CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
			if (pCamera == nullptr)
				return;

			_vec3 vPos;
			pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
			D3DVECTOR vCamPos = vPos;

			D3DCOLORVALUE vColor = { 0.8f, 1.0f, 0.8f, 1.0f };

			pEffect->SetMatrix("g_WorldMatrix", &mat);
			pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
			pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
			pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
			pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
			pEffect->SetFloat("g_AlphaRef", 0.f);


			IDirect3DBaseTexture9* pTexture = m_pBlurTex->Get_Texture(0);
			pEffect->SetTexture("g_Texture", pTexture);


			CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	

			pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			m_pBufferCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
	}
}

void CBlockObj::Free()
{
	if(m_pBlurTex)
		Safe_Release(m_pBlurTex);
	__super::Free();
}

CBlockObj* CBlockObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos, _bool p_isFirstBlock)
{
	CBlockObj* ret = new CBlockObj(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("BlockObjCreate Failed");
		return nullptr;
	}

	ret->Set_SubscribeEvent(p_EventNum);
	ret->m_iFollowingEvent = p_EventNum;
	ret->m_bOriginState = p_isFirstBlock;

	if (p_isFirstBlock)
		ret->m_pAnimator->Play_Animation(L"Unblock_Idle", false);
	else
		ret->m_pAnimator->Play_Animation(L"Idle", false);

	ret->m_bIsBlocking = p_isFirstBlock;
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x, 0.01f, p_Pos.z));

	return ret;
}

void CBlockObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBlockObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (m_bIsBlocking)
	{
		if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
			|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
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
}

void CBlockObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CBlockObj::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_Block_Moving"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CBlockObj::Event_Start(_uint iEventNum)
{
	if(iEventNum == m_iFollowingEvent)
		Change_State();

	if (iEventNum == m_iBlurEvent)
		m_iBlurEvent = 0;
}

void CBlockObj::Event_End(_uint iEventNum)
{
	if (iEventNum == m_iFollowingEvent)
		Change_State();

	if (iEventNum == m_iBlurEvent)
		m_iBlurEvent = 0;

}

void CBlockObj::Reset_Event()
{
	if (m_bIsBlocking != m_bOriginState)
	{
		Change_State();
	}
}

void CBlockObj::Set_SubscribeEvent(_uint pEvent)
{
	m_iFollowingEvent = pEvent;
	Add_Subscribe(pEvent, this);
}

void CBlockObj::Set_BlurEvent(_uint pEventNum, const _tchar* pName)
{
	if(m_iFollowingEvent != pEventNum)
		Add_Subscribe(pEventNum, this);
	m_iBlurEvent = pEventNum;

	wstring tmp = L"Proto_Tex_Block_Blur_";
	tmp += pName;

	m_pBlurTex = dynamic_cast<CTexture*>(Clone_Proto(tmp.c_str()));
	NULL_CHECK(m_pBlurTex);
}

void CBlockObj::Change_State()
{
	m_bIsBlocking = !m_bIsBlocking;
	if (true == m_bIsBlocking)
	{
		CLayer* pLayerEff = Engine::Get_Layer(LAYER_TYPE::EFFECT);
		NULL_CHECK_RETURN(pLayerEff, );

		CEffect_StoneParticle* pParticle = CEffect_StoneParticle::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pParticle, );
		NULL_CHECK_RETURN(m_pTransformCom, );
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.z -= .5f;
		pParticle->Get_Effect(vPos, _vec3(1.f, 1.f, 1.f), 50);

		pLayerEff->Add_GameObject(L"Stone_Particle", pParticle);

		Stop_Sound(CHANNELID::SOUND_EFFECT_INTERACTION);
		Play_Sound(L"SFX_22_StoneGateMove.wav", CHANNELID::SOUND_EFFECT_INTERACTION, 0.5f);
		
	}
}
