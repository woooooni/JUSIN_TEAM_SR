#include "Export_Function.h"
#include "Texture.h"


CAnimator::CAnimator()
{
}

CAnimator::CAnimator(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice, COMPONENT_TYPE::COM_ANIMATOR)
	, m_pCurAnimation(nullptr)
{
}

CAnimator::CAnimator(const CAnimator & rhs)
	: CComponent(rhs)
	, m_pCurAnimation(nullptr)
{
	for (auto iter = rhs.m_mapTexture.begin(); iter != rhs.m_mapTexture.end(); ++iter)
		m_mapTexture.insert({ iter->first, (CTexture*)(iter->second->Clone()) });
}


CAnimator::~CAnimator()
{
}

HRESULT CAnimator::Ready_Animator()
{
	return S_OK;
}

_int CAnimator::Update_Component(const _float & fTimeDelta)
{
	if (nullptr != m_pCurAnimation)
		m_pCurAnimation->Update_Component(fTimeDelta);
	return S_OK;
}

void CAnimator::LateUpdate_Component()
{

}

void CAnimator::Render_Component()
{
	if (m_pCurAnimation == nullptr)
		return;

	m_pCurAnimation->Render_Texture();
}

HRESULT CAnimator::Add_Animation(const wstring & _strKey, const wstring& _strProtoTexture, _float _fFrame)
{
	CTexture* pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(_strProtoTexture.c_str()));
	NULL_CHECK_RETURN_MSG(pTexture, E_FAIL, L"Add_Texture_Animation Failed.");

	pTexture->Set_Frame(_fFrame);
	
	m_mapTexture.insert({ _strKey, pTexture });

	return S_OK;
}

HRESULT CAnimator::Play_Animation(const wstring & _strKey)
{
	auto iter = m_mapTexture.find(_strKey);

	if (iter == m_mapTexture.end())
		NULL_CHECK_RETURN_MSG(nullptr, E_FAIL, L"Play_Animation Failed.");

	if (m_pCurAnimation == iter->second)
		return E_FAIL;

	if(nullptr != m_pCurAnimation)
		m_pCurAnimation->Set_Idx(0);

	m_pCurAnimation = iter->second;
	m_pCurAnimation->Set_Finished(false);

	return S_OK;
}


CAnimator * CAnimator::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CAnimator*			pInstance = new CAnimator(_pDevice);

	if (FAILED(pInstance->Ready_Animator()))
	{
		MSG_BOX("Animator Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnimator::Free()
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), CDeleteMap());
	m_mapTexture.clear();
}

CComponent * CAnimator::Clone(void)
{
	return new CAnimator(*this);
}
