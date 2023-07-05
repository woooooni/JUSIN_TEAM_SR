#include "..\..\Header\Component.h"

CComponent::CComponent()
	: m_pGraphicDev(nullptr), m_bClone(false), m_pOwner(nullptr), m_eType(COMPONENT_TYPE::COM_END), m_bActive(true)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev, COMPONENT_TYPE _eType)
	: m_pGraphicDev(pGraphicDev), m_bClone(false), m_pOwner(nullptr), m_eType(_eType), m_bActive(true)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true), m_pOwner(nullptr), m_eType(rhs.m_eType), m_bActive(true)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
