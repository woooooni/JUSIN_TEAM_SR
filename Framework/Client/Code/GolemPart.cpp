#include "..\Header\GolemPart.h"
#include "Export_Function.h"
#include "Pool.h"
#include "Effect_MotionTrail.h"

CGolemPart::CGolemPart(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SUN_GOLLEM)
	, m_eState(SUNGOLEM_STATE::REGEN), m_fRotationAngle(0)
	, m_fAccMotionTrail(0.f)
	, m_fMotionTrailTime(0.1f)
{

}
CGolemPart::CGolemPart(const CGolemPart& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_pTarget(nullptr)
{

}

CGolemPart::~CGolemPart()
{
}

HRESULT CGolemPart::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CGolemPart::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CGolemPart::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemPart::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CGolemPart::Add_Component(void)
{
	return S_OK;
}

void CGolemPart::Move_to_Offset(_vec3 _vPos)
{
	_vec3 vPos = _vPos + m_vOffset;
	m_pTransformCom->Set_Pos(&vPos);
}

void CGolemPart::Move_Offset(_vec3 _vPos,_float fTimeDelta,_float _fSpeed)
{
	_vec3 vDir = {};
	D3DXVec3Lerp(&vDir, &m_vOffset, &_vPos, _fSpeed * fTimeDelta);

	m_vOffset = vDir;
}

void CGolemPart::Rotate()
{
	_vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_Info(INFO_RIGHT, &_vec3(vScale.x, 0.f, 0.f));
	m_pTransformCom->Set_Info(INFO_UP, &_vec3(0.f, vScale.y, 0.f));
	m_pTransformCom->Set_Info(INFO_LOOK, &_vec3(0.f, 0.f, vScale.z));
	
	_vec3 vAxisZ = { 0.f,0.f,1.f };
	m_pTransformCom->RotationAxis(vAxisZ, D3DXToRadian(m_fRotationAngle));
}

void CGolemPart::Generate_MotionTrail(_float fTimeDelta)
{
	m_fAccMotionTrail += fTimeDelta;
	if (m_fAccMotionTrail >= m_fMotionTrailTime)
	{
		m_fAccMotionTrail = 0.f;
		CEffect_MotionTrail* pMotionTrail = CPool<CEffect_MotionTrail>::Get_Obj();

		if (pMotionTrail == nullptr)
			pMotionTrail = CEffect_MotionTrail::Create(m_pGraphicDev);

		pMotionTrail->Set_Texture(
			m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx()));

		_vec3 vTrailPos, vTrailRight, vTrailUp, vTrailLook;
		m_pTransformCom->Get_Info(INFO_RIGHT, &vTrailRight);
		m_pTransformCom->Get_Info(INFO_UP, &vTrailUp);
		m_pTransformCom->Get_Info(INFO_LOOK, &vTrailLook);
		m_pTransformCom->Get_Info(INFO_POS, &vTrailPos);

		vTrailPos.z += 0.1f;
		pMotionTrail->Get_TransformCom()->Set_Info(INFO_RIGHT, &vTrailRight);
		pMotionTrail->Get_TransformCom()->Set_Info(INFO_UP, &vTrailUp);
		pMotionTrail->Get_TransformCom()->Set_Info(INFO_LOOK, &vTrailLook);
		pMotionTrail->Get_TransformCom()->Set_Info(INFO_POS, &vTrailPos);

		Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"MotionTrail", pMotionTrail);
	}
}

void CGolemPart::Move_Offset_ByDir(_vec3 _vDir, _float fTimeDelta, _float _fSpeed)
{
	D3DXVec3Normalize(&_vDir, &_vDir);
	m_vOffset += _vDir * fTimeDelta * _fSpeed;
}
void CGolemPart::Free()
{
	__super::Free();
}

