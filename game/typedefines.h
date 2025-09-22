#ifndef _TYPEDEFINES_H
#define _TYPEDEFINES_H

typedef LONGLONG GHANDLE;
typedef short	 BUILD_ID;

typedef struct _GPOINT
{
	int x;
	int y;
}GPOINT;

typedef enum _G_TYPE_ID
{
	g_type_none,
	g_type_map,
	g_type_building,
	g_type_carmoto,
	g_type_smoke,
	g_type_bomb,
	g_type_bullet,
	g_type_flag,
	g_type_warrior,
}G_TYPE_ID;

typedef enum _G_DRIVE_MODE
{
	g_drive_manual,
	g_drive_automatic,
}G_DRIVE_MODE;

typedef enum _G_SMOKE_TYPE
{
	g_smoke_none,
	g_smoke_1,
	g_smoke_2,
}G_SMOKE_TYPE;

typedef enum _G_BOMB_TYPE
{
	g_bomb_none,
	g_bomb_1,
	g_bomb_2,
}G_BOMB_TYPE;

typedef enum _G_BULLET_TYPE
{
	g_bullet_none,
	g_bullet_1,
	g_bullet_2,
	g_arrow_1,
}G_BULLET_TYPE;

typedef enum _G_FLAG_TYPE
{
	g_flag_none,
	g_flag_1,
	g_flag_2,
}G_FLAG_TYPE;

typedef struct _G_TIME
{
	short year;
	short month;
	short week;
	short day;
	short hour;
	short minute;
	short second;
	short msec;
}G_TIME;

typedef enum _G_SHIFT_RESULT
{
	g_shift_ok,
	g_shift_boundary,
	g_shift_collide,
}G_SHIFT_RESULT;

typedef enum _G_ARM_TYPE
{
	g_arm_none,
	g_arm_1,
}G_ARM_TYPE;

typedef enum _G_WARRIOR_POSTURE
{
	g_warrior_standby,
	g_warrior_walk,
	g_warrior_stick,
	g_warrior_shoot,
	g_warrior_fire,
}G_WARRIOR_POSTURE;

typedef enum _G_BUTTON_STATUS
{
	g_button_up,
	g_button_down,
}G_BUTTON_STATUS;

typedef enum _G_MENU_OPERATE_TYPE
{
	g_menu_op_type_none,
	g_menu_op_type_1, // construct building
	g_menu_op_type_2, // produce car or person
}G_MENU_OPERATE_TYPE;

typedef enum _G_CONSTRUCT_ITEM
{
	g_item_fort,
	g_item_house,
	g_item_factory,
}G_CONSTRUCT_ITEM;

typedef enum _G_PRODUCE_ITEM
{
	g_item_none,
	g_item_car0,
	g_item_car1,
	g_item_car2,
	g_item_car3,
	g_item_swordman,
	g_item_cavalry,
	g_item_bowman,
	g_item_chariot,
}G_PRODUCE_ITEM;

typedef enum _G_NEXTSTEP_RESULT
{
	g_reach_none,
	g_reach_nextstop,
	g_reach_destination,
}G_NEXTSTEP_RESULT;

typedef enum _G_NEXTSTOP_RESULT
{
	g_stop_ok,
	g_stop_error,
}G_NEXTSTOP_RESULT;

typedef struct _G_MENU_OPERATE
{
	G_MENU_OPERATE_TYPE typeId;
	union
	{
	G_CONSTRUCT_ITEM	itemId;
	G_PRODUCE_ITEM		productId;
	};
}G_MENU_OPERATE;

#endif