#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum MATRIX_INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class COMPONENT_TYPE	
	{ 
		COM_BUFFER,
		COM_TRANSFORM, 
		COM_CIRCLE_COLLIDER,
		COM_BOX_COLLIDER,
		COM_TEXTURE,
		COM_ANIMATOR,
		COM_RIGIDBODY,
		COM_END
	};

	enum class LAYER_TYPE { CAMERA, ENVIRONMENT, PLAYER, SPAWNER, MONSTER, EFFECT, UI, BACK_GROUND, LAYER_END };
	enum class SCENE_TYPE { LOADING, STAGE1, STAGE2, STAGE3, TOOL, SCENE_END };

	//Object
	enum class OBJ_TYPE { OBJ_SKYBOX, OBJ_CAMERA, OBJ_ENVIRONMENT, OBJ_PLAYER, OBJ_SPAWNER, OBJ_MONSTER, OBJ_EFFECT, OBJ_UI, OBJ_BACKGROUND, OBJ_END };
	enum class OBJ_DIR { DIR_U, DIR_D, DIR_L, DIR_R, DIR_LU, DIR_RU, DIR_LD, DIR_RD, DIR_END };
	enum class OBJ_HITTYPE { HIT_ONCE, HIT_REPEAT, HIT_BREAK, HIT_END };
	enum class JELLY_COLLOR_NORMAL { CYAN, MAGENTA, YELLOW, JELLY_NORMALEND };
	enum class JELLY_COLLOR_COMBINE{ BLUE = 1, GREEN, RED, JELLY_COMBINEEND};

	// Collision
	enum class COLLIDER_TYPE { COLLIDER_BOX, COLLIDER_CIRCLE, COLLIDER_END };
	enum class COLLISION_DIR{ NOT_COLLIDE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_FRONT, DIR_BACK, DIR_END };
	
	// Transform
	enum class DIR_TRANSFORM { DIR_X, DIR_Y, DIR_Z, DIR_END };

	// Monster & Boss
	enum class MONSTER_STATE { IDLE, MOVE, REGEN, ATTACK, DIE, END };
	enum class SUNGOLEM_STATE { REGEN, IDLE, DIRTY, MOVE, ATTACK, DIE, END };

	// Player
	enum class PLAYER_STATE { IDLE, MOVE, RUN, ROLL, JUMP, SWING, HIT ,LIFT, DOWN, GAMEOVER, PUSH, GETTIEM, BALLOONFLY, DRAWING , SKILL, DIE, END };
	enum class BALLOONFLY_STATE { FLYREADY, FLYING, FLYINGEND, END };
	enum class LIFT_STATE {LIFTREADY, LIFTUP, LIFTDOWN, LIFTIDLE, LIFTWALK, END};

	typedef enum class COLLIDE_STATE
	{
		COLLIDE_WALL,
		COLLIDE_PLAYER,
		COLLIDE_MONSTER,
		COLLIDE_GRAB,
		COLLIDE_PUSH,
		COLLIDE_BREAK,
		COLLIDE_BULLET,
		COLLIDE_BOMB,
		COLLIDE_BALPAN,
		COLLIDE_SWING,
		COLLIDE_END
	} COLLISION_GROUP;

	enum class COLLIDER_PLAYER
	{
		COLLIDER_ATTACK,
		COLLIDER_GRAB,
		COLLIDER_END
	};
	
	// KeyBoard
	enum class KEY_STATE
	{
		NONE,
		TAP,
		HOLD,
		AWAY
	};

	enum class KEY
	{
		LEFT_ARROW,
		RIGHT_ARROW,
		UP_ARROW,
		DOWN_ARROW,
		TAB,
		F1, F2, F3, F4, F5, F6, F7, F8, F9,
		INSERT, DEL, HOME, END, PAGE_UP, PAGE_DOWN,
		NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT, CTRL, SHIFT,
		SPACE, ENTER, ESC,
		LBTN, RBTN,
		TYPE_END
	};
}
#endif // Engine_Enum_h__
