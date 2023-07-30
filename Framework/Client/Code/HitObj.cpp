#include "../Include/stdafx.h"
#include "HitObj.h"
#include "Export_Function.h"
#include <time.h>

CHitObj::CHitObj(LPDIRECT3DDEVICE9 p_Dev) 
	: CFieldObject(p_Dev, OBJ_ID::BUG_STATUE)
	, m_eHitType(OBJ_HITTYPE::HIT_ONCE)
	, m_fEffectTime(0.f), m_bHitted(false)
	, m_iEventNum(0)
	, m_vOrigin(0, 0, 0)
	, m_vToward(0, 0, 0)
	, m_fResetTime(0.f)
{
}

CHitObj::CHitObj(const CHitObj& rhs) : CFieldObject(rhs), m_eHitType(rhs.m_eHitType), m_fEffectTime(rhs.m_fEffectTime), m_bHitted(rhs.m_bHitted), m_iEventNum(rhs.m_iEventNum), m_vOrigin(rhs.m_vOrigin), m_vToward(rhs.m_vToward), m_fResetTime(rhs.m_fResetTime)
{
}

CHitObj::~CHitObj()
{
}

HRESULT CHitObj::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

	FAILED_CHECK(m_pAnimator->Add_Animation(L"Bug", L"Proto_Tex_BugStatue", 0.f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Monkey", L"Proto_Tex_MonkeyStatue", 0.f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Rat", L"Proto_Tex_RatStatue", 0.f));

	m_pBlurAnimator = dynamic_cast<CAnimator*>(Clone_Proto(L"Proto_Animator"));	
	FAILED_CHECK(m_pBlurAnimator->Add_Animation(L"Bug", L"Proto_Tex_BugStatue_Blur", 0.f));
	FAILED_CHECK(m_pBlurAnimator->Add_Animation(L"Monkey", L"Proto_Tex_MonkeyStatue_Blur", 0.f));
	FAILED_CHECK(m_pBlurAnimator->Add_Animation(L"Rat", L"Proto_Tex_RatStatue_Blur", 0.f));


    return S_OK;
}

_int CHitObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	if (m_fEffectTime > 0.f)
	{
		


		m_pTransformCom->Move_Pos(&m_vToward, 1.f, fTimeDelta);

		m_fEffectTime -= fTimeDelta;
		m_fResetTime += fTimeDelta;

		if (m_fResetTime > 0.1f)
		{
			_vec3		vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			D3DXVec3Normalize(&m_vToward, &(m_vOrigin - vPos));

			if (m_vToward == _vec3(0, 0, 0))
				Make_Toward();
			m_fResetTime = 0.f;
		}
	}
	else if (m_vToward != _vec3(0, 0, 0))
	{
		m_vToward = { 0, 0, 0 };
		m_fEffectTime = 0.f;
		m_pTransformCom->Set_Pos(&m_vOrigin);
	}

    _int ret = __super::Update_Object(fTimeDelta);
    return ret;
}

void CHitObj::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CHitObj::Render_Object(void)
{

	__super::Render_Object();

	if (!m_bHitted && m_eHitType == OBJ_HITTYPE::HIT_ONCE)
	{
		m_pBlurAnimator->Render_Component();

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

void CHitObj::Free()
{
	Safe_Release(m_pBlurAnimator);
    __super::Free();
}

CHitObj* CHitObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos, const _tchar* statuename)
{

    CHitObj* ret = new CHitObj(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("HitObjCreate Failed");
        return nullptr;
    }

	ret->m_iEventNum = p_EventNum;
	ret->m_vOrigin = { p_Pos.x, 0.5f, p_Pos.z };
	ret->m_pTransformCom->Set_Pos(&p_Pos);
	ret->m_pAnimator->Play_Animation(statuename, false);
	ret->m_pBlurAnimator->Play_Animation(statuename, false);

    return ret;
}

void CHitObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING)
	{
		switch (m_eHitType)
		{
		case Engine::OBJ_HITTYPE::HIT_ONCE:

			if (m_bHitted)
				return;
			else
				m_bHitted = true;


		case Engine::OBJ_HITTYPE::HIT_REPEAT:

			m_fEffectTime = 0.5f;
			Make_Toward();
			if (m_eCutSceneType != CCutSceneMgr::CUTSCENE_TYPE::TYPE_END)
			{
				CCutSceneMgr::GetInstance()->Set_EventNum(m_iEventNum);
				CCutSceneMgr::GetInstance()->Start_CutScene(m_eCutSceneType);
			}
			else
				Check_Event_Start(m_iEventNum);


			break;
		case Engine::OBJ_HITTYPE::HIT_BREAK:
			break;
		case Engine::OBJ_HITTYPE::HIT_END:
			break;
		default:
			break;
		}

	}
}

void CHitObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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

HRESULT CHitObj::Ready_Component()
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

	Set_MinHeight(0.5f);

	return S_OK;

}

void CHitObj::Make_Toward()
{
	srand(unsigned(time(NULL)));
	do
	{
		m_vToward = { (float)((rand() % 90) - 45), (float)((rand() % 90) - 45) , (float)((rand() % 90) - 45) };

	} while (m_vToward == _vec3(0, 0, 0));

	D3DXVec3Normalize(&m_vToward, &m_vToward);

}

