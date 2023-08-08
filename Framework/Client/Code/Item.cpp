#include "Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType, OBJ_ID _eID)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ITEM, _eID)
	, m_eItemType(_eItemType)
	, m_iInvenCount(0)
	, m_eCode(ITEM_CODE::ITEM_END)
	, m_tInfo({ 0.f, 0.f, 85.f, 85.f })
	, m_bIsInInven(false)

{

}

CItem::CItem(const CItem& rhs)
	: Engine::CGameObject(rhs)
	, m_eItemType(rhs.m_eItemType)
	,m_iInvenCount(rhs.m_iInvenCount)
	, m_eCode(rhs.m_eCode)
	, m_bIsInInven(rhs.m_bIsInInven)
	, m_tInfo(rhs.m_tInfo)

{

}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CItem::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsInInven)
		return 0;

	_int iExit = __super::Update_Object(fTimeDelta);

	
	return iExit;
}

void CItem::LateUpdate_Object(void)
{
	if (m_bIsInInven)
		return;
	__super::LateUpdate_Object();
}

void CItem::Render_Object(void)
{
	
	__super::Render_Object();
}

void CItem::Render_UI()
{
	Ready_TransWorld();
	__super::Render_Object();

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

_bool CItem::Check_Clicked()
{
	auto& pt = CKeyMgr::GetInstance()->GetMousePos();

	if (pt.x > m_tInfo.fX - m_tInfo.fCX * 0.5f && pt.x < m_tInfo.fX + m_tInfo.fCX * 0.5f
		&& pt.y > m_tInfo.fY - m_tInfo.fCY * 0.5f && pt.y < m_tInfo.fY + m_tInfo.fCY * 0.5f)
	{
		return true;
	}

	return false;
}

CTexture* CItem::Get_Texture()
{
	return m_pTextureCom;
}

HRESULT CItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_CRcTex"));
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
	return S_OK;

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

}

wstring CItem::Get_Explain(const ITEM_CODE& pCode)
{


	switch (pCode)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		return L"체력을 조금 회복하는 과일";
		break;
	case Engine::ITEM_CODE::HP_MIDDLE:
		return L"체력을 어느정도 회복하는 과일";

		break;
	case Engine::ITEM_CODE::HP_BIG:
		return L"체력을 많이 회복하는 비싼 과일";

		break;
	case Engine::ITEM_CODE::SPEED_SMALL:
		return L"속도가 조금 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::SPEED_MIDDLE:
		return L"속도가 어느정도 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::SPEED_BIG:
		return L"속도가 많이 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::LEAF:
		return L"흔한 나뭇잎";

		break;
	case Engine::ITEM_CODE::TWIG:
		return L"흔한 나뭇가지";

		break;

	case ITEM_CODE::BUTTERFLY:

		return L"아마도 나비";
		break;
	case Engine::ITEM_CODE::SASUM:
		break;
	case Engine::ITEM_CODE::HAT_DRILL:
		return L"땅을 파고들어 공격할수 있는 파워풀한 모자";
		break;
	case Engine::ITEM_CODE::HAT_LIGHT:
		return L"주변을 밝게 밝혀주는 모자";
		break;
	case Engine::ITEM_CODE::HAT_MASK:
		return L"번개를 불러일으키는 무시무시한 모자";
		break;
	case Engine::ITEM_CODE::HAT_MISSLE:
		return L"미래적인 공격을 퍼부을수 있는 강력한 모자";
		break;
	case Engine::ITEM_CODE::HAT_MONKEY:
		return L"소원을 빌면 하늘에서 운석이 내려오는 낭만적인 모자";
		break;
	case Engine::ITEM_CODE::HAT_TURTLE:
		return L"딱딱한 거북이 등껍질에 숨을수있는 모자";
		break;
	case Engine::ITEM_CODE::DRAWING_CROSSROAD:
		return L"몰티즈가 있는 교차로를 그린 그림";
		break;
	case Engine::ITEM_CODE::DRAWING_MONKEYSTATUE:
		return L"커다란 원숭이 동상을 그린 그림";

		break;
	case Engine::ITEM_CODE::DRAWING_MONKEYTOWN:
		return L"활기찬 원숭이 마을을 그린 그림";

		break;
	case Engine::ITEM_CODE::DRAWING_COLORS:
		return L"색조합을 알려주는 그림";

	case ITEM_CODE::DRAWING_BUGSTATUE:
		return L"무언가 비밀을 푸는데 열쇠가 될듯한 그림";

		break;
	default:
		break;
	}


	MSG_BOX("ItemCode Error");

	return L"";
}

wstring CItem::Get_Name(const ITEM_CODE& pCode)
{
	switch (pCode)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		return L"건강 열매";
		break;

	case Engine::ITEM_CODE::HP_MIDDLE:
		return  L"수입산 사과";
		break;

	case Engine::ITEM_CODE::HP_BIG:
		return  L"국내산 사과";
		break;

	case Engine::ITEM_CODE::SPEED_SMALL:
		return  L"신비한 약초";
		break;

	case Engine::ITEM_CODE::SPEED_MIDDLE:
		return  L"더 신비한 약초";
		break;

	case Engine::ITEM_CODE::SPEED_BIG:
		return  L"진짜 신비한 약초";
		break;

	case Engine::ITEM_CODE::LEAF:
		return  L"나뭇잎";
		break;

	case Engine::ITEM_CODE::TWIG:
		return  L"나뭇가지";
		break;

	case Engine::ITEM_CODE::BUTTERFLY:
		return  L"장수풍뎅이 탈을 쓴 나비";
		break;

	case Engine::ITEM_CODE::SASUM:
		break;
	case Engine::ITEM_CODE::HAT_DRILL:
		return L"드릴 모자";
		break;
	case Engine::ITEM_CODE::HAT_LIGHT:
		return L"전구 모자";
		break;
	case Engine::ITEM_CODE::HAT_MASK:
		return L"천둥신의 가면";
		break;
	case Engine::ITEM_CODE::HAT_MISSLE:
		return L"미래의 스카우터";
		break;
	case Engine::ITEM_CODE::HAT_MONKEY:
		return L"원숭이신의 모자";
		break;
	case Engine::ITEM_CODE::HAT_TURTLE:
		return L"거북 모자";
		break;
	case Engine::ITEM_CODE::DRAWING_CROSSROAD:
		return L"교차로 그림";
		break;
	case Engine::ITEM_CODE::DRAWING_MONKEYSTATUE:
		return L"원숭이 동상 그림";

		break;
	case Engine::ITEM_CODE::DRAWING_MONKEYTOWN:
		return L"원숭이 마을 그림";

		break;
	case Engine::ITEM_CODE::DRAWING_COLORS:
		return L"삼원색 그림";

		break;

	case ITEM_CODE::DRAWING_BUGSTATUE:
		return L"벌레동상 그림";
	default:
		break;
	}

	

	MSG_BOX("ItemCode Error");

	return L"";

}


void CItem::Free()
{
	__super::Free();
}

void CItem::Ready_TransWorld()
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);


	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / matPreProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / matPreProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_vec3 vScale = _vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

}
