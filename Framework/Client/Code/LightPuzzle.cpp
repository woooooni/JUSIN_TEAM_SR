#include "LightPuzzle.h"
#include "Export_Function.h"

CLightPuzzle::CLightPuzzle(LPDIRECT3DDEVICE9 pGraphicDev) : CFieldObject(pGraphicDev)
{
}

CLightPuzzle::CLightPuzzle(const CLightPuzzle& rhs) : CFieldObject(rhs)
{
}

CLightPuzzle::~CLightPuzzle()
{
}

void CLightPuzzle::Free()
{
	__super::Free();
}

void CLightPuzzle::Reverse_Puzzle(_bool isX)
{
	_vec3 curScale = m_pTransformCom->Get_Scale();
	if (isX)
	{
		m_pTransformCom->Set_Scale({ curScale.x * -1.f, curScale.y, curScale.x });
		for (auto& iter : m_lightDir)
		{
			iter.x *= -1.f;
		}
	}
	else
	{
		m_pTransformCom->Set_Scale({ curScale.x , curScale.y * -1.f, curScale.x });
		for (auto& iter : m_lightDir)
		{
			iter.y *= -1.f;
		}
	}


}
