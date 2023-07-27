#include "CParticle_Stone.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"



CParticle_Stone::CParticle_Stone(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_EFFECT, OBJ_ID::EFFECT)
{
}

CParticle_Stone::CParticle_Stone(const CParticle_Stone& rhs)
	: CGameObject(rhs)
{
}

CParticle_Stone::~CParticle_Stone()
{
}

HRESULT CParticle_Stone::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.1f));

	m_pAnimator->Add_Animation(L"ParticleStone", L"Proto_Texture_Effect_ParticleStone", 0.1f);

	m_pAnimator->Play_Animation(L"ParticleStone", false);

	Set_Active(false);

	m_fMinHeight = 0.2f;
	m_pRigidBodyCom->SetGravity(-98.0f);

	m_fAccTime = 0.0f;
	m_fEndTime = 1.0f;
	m_iAlpha = m_iR = m_iG = m_iB = 255;

	return S_OK;
}

_int CParticle_Stone::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;



	if (m_fAccTime > m_fEndTime)
	{
		m_iAlpha -= 5;
		if (m_iAlpha == 0)
		{
			CPool<CParticle_Stone>::Return_Obj(this);
		}
	}
	else
		m_fAccTime += fTimeDelta;


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CParticle_Stone::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;




	__super::LateUpdate_Object();
}

void CParticle_Stone::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_iR, m_iG, m_iB));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CParticle_Stone::Ready_Component(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;
}

void CParticle_Stone::Random_Particle(_vec3& _vPos)
{
	if (_vPos.y < 0.3f)
		_vPos.y = 0.3f;

	m_iAlpha = m_iR = m_iG = m_iB = 255;

	m_fAccTime = 0.0f;
	m_pRigidBodyCom->SetGround(false);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	_vPos.z -= 0.1f;
	_float fScale = 0.05f + ((rand() % 10) * 0.005f);
	_float fAngle = _float(rand() % 360);
	_float fForceX = (rand() % 21) * 2.0f + 10.0f;
	_float fForceZ = (rand() % 21) * 2.0f + 10.f;
	_float fForceY = ((rand() % 10) * 5.0f) + 30.0f;

	_int iX = rand() % 2;
	if (iX == 1)
		fForceX *= -1.0f;

	m_pTransformCom->Set_Scale(_vec3(fScale, fScale, 0.0f));
	m_pTransformCom->RotationAxis(_vec3(0.0f, 0.0f, 1.0f), fAngle);
	m_pTransformCom->Set_Pos(&_vPos);

	m_pRigidBodyCom->AddForce(_vec3(fForceX, fForceY, fForceZ));

	Set_Active(true);

}

void CParticle_Stone::Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iCount)
{
	_uint iStoneCount = _iCount;
	int iTemp;

	_vScale = _vScale * 0.01f;

	for (_uint i = 0; iStoneCount > i; ++i)
	{
		_vec3 vPos;

		vPos.x = (rand() % 100) * _vScale.x;
		vPos.y = (rand() % 100) * _vScale.y;
		vPos.z = (rand() % 100) * _vScale.z;

		iTemp = rand() % 2;
		if (iTemp == 1)
			vPos.x *= -1.0f;

		

		iTemp = rand() % 2;
		if (iTemp == 1)
			vPos.z *= -1.0f;

		vPos += _vPos;

		CGameObject* pStone = CPool<CParticle_Stone>::Get_Obj();
		if (pStone)
		{
			dynamic_cast<CParticle_Stone*>(pStone)->Random_Particle(vPos);
			pStone->Set_Active(true);
			Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"StoneParticle", pStone);
		}
		else
		{
			pStone = dynamic_cast<CParticle_Stone*>(pStone)->Create(Engine::Get_Device());
			if (pStone)
			{
				pStone->Set_Active(true);
				dynamic_cast<CParticle_Stone*>(pStone)->Random_Particle(vPos);
				Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"StoneParticle", pStone);
			}
		}
	}
}

CParticle_Stone* CParticle_Stone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle_Stone* pInstance = new CParticle_Stone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Particle_Stone_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle_Stone::Free()
{
	__super::Free();
}
