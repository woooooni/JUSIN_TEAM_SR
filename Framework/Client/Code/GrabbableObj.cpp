#include "../Include/stdafx.h"
#include "GrabbableObj.h"
#include "Export_Function.h"


CGrabbableObj::CGrabbableObj(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT)
{
}

CGrabbableObj::CGrabbableObj(const CGrabbableObj& rhs) : CGameObject(rhs)
{
}

CGrabbableObj::~CGrabbableObj()
{
}

void CGrabbableObj::Grap()
{
	CGameObject* player = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");

	_vec3 src, up;

	player->Get_TransformCom()->Get_Info(INFO_POS, &src);

	player->Get_TransformCom()->Get_Info(INFO_UP, &up);

	m_pTransformCom->Set_Pos(&(src + up * 1.0f /*이 수치를 조종해 잡았을때 위치를 조절가능*/));


}

void CGrabbableObj::UnGrap()
{



}
