#include "teamstyle18-my-1.h"
#include <math.h>
#include <vector>
#include <time.h>
#define min(x, y) ((x) > (y)? (y) : (x))
#define max(x, y) ((x) < (y)? (y) : (x))



//下面开始命名各种变量
//首先声明敌我标志-敌方后缀en，我方后缀me，全部是all
//敌我后缀之前是类型说明-数目为num，地点为pos，单位为unit
//类型说明-兵为mover，建筑为building
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这些声明哈		 │
*           │   我感觉写的挺好的		 ├─┐
*           │   如果要改的话呢		 ┌─┘
*           │   直接搜索替换就行		 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************下面为变量声明区*****************************************/
const int maxSize = 300;
Unit* Unit_All;//所有单位数组指针，用于遍历
Unit* Base_Unit_Me;//我方基地
Unit* Base_Unit_En;//敌方基地
Unit* Meats_Mover_Unit_Me[maxSize];//我方小鲜肉
Unit* Meats_Mover_Unit_En[maxSize];//敌方小鲜肉
Unit* Produce_Building_pUnit_All[maxSize];//所有生产性建筑
Unit* Produce_Building_Unit_Me[maxSize];//我方生产性建筑
Unit* Skill_Building_pUnit_All[maxSize];//所有技能型建筑
Unit* Bank_Unit_NotMe[maxSize];//非我方银行
Unit* School_Unit_NotMe[maxSize];//非我方教学楼
Unit* Weapon_Unit_Me[maxSize];//我方武器
Unit* Weapon_Unit_En[maxSize];//敌方武器

int Turn_Num = 1;//回合数
int Meats_Mover_Num_Me;//我方小鲜肉数量
int Meats_Mover_Num_En;//敌方小鲜肉数量
int Team_Flag;//队伍标志，判断是哪一队
int Bank_Num_Me;//我方银行数目
int School_Num_Me;//我方学校数目
int Skill_Building_Num_Me;//我方技能建筑数目
int Produce_Building_Num_Me;//我方生产建筑数目
int Produce_Building_Num_All;//所有生产建筑最大值
int Ski_Building_Num_All;//所有技能建筑数目
int Bank_Num_NotMe;//非我方银行数目
int School_Num_NotMe;//非我方教学楼数目
int Meat_Mover_Num_Max_Me;//我方小鲜肉最大数目
						  //int Base_Health;//基地情况
						  //int Task_Num_Me;//任务总数
int Weapon_Num_Me;//我方可攻击单位数目
int Weapon_Num_En;//敌方攻击单位数目
int Task_Produce_Num_Now;//现在派去占领生产性建筑的小鲜肉数目
int UAV_Num_Me;//无人机个数
int HACKER_Num_Me;//黑客个数
int BOLT_TANK_Num_Me;//电磁坦克个数
int BATTLE_TANK_Num_Me;//主战坦克个数
int NUKE_TANK_Num_Me;//核子坦克个数
int SUPERMAN_Num_Me;//改造人战士个数
int EAGLE_Num_Me;//鹰式飞机个数

bool If_Arranged[Type_num];//为了在打对手的时候步调一致设计的
Position Arranged_Position[Type_num];//同上，和上面的一起使用
Unit* Arranged_Tasks[Type_num] = { 0 };//同上，给每个单位分配任务

vector<int> Available_Task;//任务分配的时候需要用这个vector来判断什么任务还没有被分配（可以被分配）
vector<Unit*> Available_Meat;//没有被分配任务的小鲜肉
vector<Unit*> Available_NotMe;//没有被分配去占领的非我建筑
vector<Unit*>::iterator it, it2, ittemp;//在vector遍历的时候用
vector<Unit*> mySkill2Build;//新写的二技能数组

string Name_Types[] = { "__BASE",

"MEAT", "HACKER", "SUPERMAN",

"BATTLE_TANK", "BOLT_TANK", "NUKE_TANK",

"UAV", "EAGLE",

"HACK_LAB", "BID_LAB", "CAR_LAB", "ELEC_LAB", "RADIATION_LAB",
"UAV_LAB", "AIRCRAFT_LAB", "BUILD_LAB", "FINANCE_LAB", "MATERIAL_LAB",
"NANO_LAB", "TEACH_BUILDING", "BANK",
"Type_num"

};
/*****************************************变量声明区已结束*****************************************/


/*****************************************自定义数据结构开始*****************************************/
struct Task
{
	Unit* Meat_Unit;//小鲜肉
	Unit* Aim_Unit;//目标
	int Aim_Type;//目标类型
} Task_Meat[300];

/*****************************************自定义数据结构已结束*****************************************/


/*****************************************函数声明区开始*****************************************/
void Get_Info();//获取信息
int Get_Len(Position, Position);//获取长度
void Base_Update();//基地更新
int Meat_Update();//小鲜肉活动并返回需要小鲜肉数目
void Produce_Building_Update();//生产建筑开始生产
void Weapon_Update();//武器更新
int Get_Meat_Max_Num();//获取最大的可能小鲜肉数目
void Meat_Capture(Unit* Meat, Unit* Target);//让小鲜肉占领目标
bool Farther(Unit* a, Unit* b);//如果a比b离基地远，则返回TRUE
void Get_Way(Unit* a);//用于a去敌方基地
void Go_To_Hit_Enemy(Unit* a);//用于去攻击对方前来进攻单位
void Hit(Unit* a);//用于指挥单位攻击
Position Get_Next_Pos(Position Unit_Position, Position Target_Position, int Lenth, int Speed_Max);//获取去距离某个点一定距离的路径

																								  /*****************************************函数声明区已结束*****************************************/

																								  /*****************************************获取长度开始*****************************************/
int Get_Len(Position X, Position Y)
{
	return abs(X.x - Y.x) + abs(X.y - Y.y);
}
/*****************************************获取长度结束*****************************************/

/*****************************************获取小鲜肉最大数目开始*****************************************/
int Get_Meat_Max_Num()
{
	return Meat_Update();
}
/*****************************************获取小鲜肉最大数目结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   要用的都写上了		 ├─┐
*           │   用不到的也写上了		 ┌─┘
*           │   可以增删，无所谓的	 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************获取信息开始******************************************/
void Get_Info()
{
	Available_NotMe.clear();
	Available_Meat.clear();
	mySkill2Build.clear();
	for (int LoopTemp = 0; LoopTemp < maxSize; LoopTemp++)
	{
		Meats_Mover_Unit_Me[LoopTemp] = NULL;//我方小鲜肉
		Meats_Mover_Unit_En[LoopTemp] = NULL;//敌方小鲜肉
		Produce_Building_pUnit_All[LoopTemp] = NULL;//所有生产性建筑
		Produce_Building_Unit_Me[LoopTemp] = NULL;//我方生产性建筑
		Skill_Building_pUnit_All[LoopTemp] = NULL;//所有技能型建筑
		Weapon_Unit_Me[LoopTemp] = NULL;;//我方武器
		Weapon_Unit_En[LoopTemp] = NULL;;//敌方武器
		Bank_Unit_NotMe[LoopTemp] = NULL;//非我方银行
		School_Unit_NotMe[LoopTemp] = NULL;//非我方教学楼
	}
	for (int LoopTemp = 0; LoopTemp < Type_num; LoopTemp++)
	{
		If_Arranged[LoopTemp] = false;
		Arranged_Position[LoopTemp] = NULL;
	}
	Meats_Mover_Num_Me = 0;//我方小鲜肉数量
	Meats_Mover_Num_En = 0;//敌方小鲜肉数量
	Team_Flag = 0;//队伍标志，判断是哪一队
	Bank_Num_Me = 0;//我方银行数目
	Bank_Num_NotMe = 0;//非我方银行数目
	School_Num_NotMe = 0;//非我方教学楼数目
	School_Num_Me = 0;//我方学校数目
	Skill_Building_Num_Me = 0;//我方技能建筑数目
	Produce_Building_Num_Me = 0;//我方生产建筑数目
	Produce_Building_Num_All = 0;//所有生产建筑最大值
	Ski_Building_Num_All = 0;//所有技能建筑数目
	Meat_Mover_Num_Max_Me = 0;//我方小鲜肉最大数目
							  //Task_Num_Me = 0;//所有非空任务数目
	Weapon_Num_Me = 0;//我方可攻击单位数目
	Weapon_Num_En = 0;//敌方攻击单位数目
	Task_Produce_Num_Now = 0;//现在派去占领生产性建筑的小鲜肉数目
	UAV_Num_Me = 0;//无人机个数
	HACKER_Num_Me = 0;//黑客个数
	BOLT_TANK_Num_Me = 0;//电磁坦克个数
	BATTLE_TANK_Num_Me = 0;//主战坦克个数
	NUKE_TANK_Num_Me = 0;
	SUPERMAN_Num_Me = 0;
	EAGLE_Num_Me = 0;

	Unit_All = getUnit();
	Team_Flag = getTeamId();
	for (int LoopTemp = 0; LoopTemp < getUnitSize(); LoopTemp++)
	{
		switch (Unit_All[LoopTemp].type_name)
		{
		case BASE://基地登记
			if (Unit_All[LoopTemp].flag == Team_Flag)
				Base_Unit_Me = &Unit_All[LoopTemp];
			else
				Base_Unit_En = &Unit_All[LoopTemp];
			break;
		case MEAT://小鲜肉登记
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Available_Meat.push_back(&Unit_All[LoopTemp]);
				Meats_Mover_Unit_Me[Meats_Mover_Num_Me++] = &Unit_All[LoopTemp];
			}
			else
			{
				Meats_Mover_Unit_En[Meats_Mover_Num_En++] = &Unit_All[LoopTemp];
			}
			break;
		case ELEC_LAB:
		case HACK_LAB:
		case UAV_LAB:
		case CAR_LAB:
		case RADIATION_LAB:
		case AIRCRAFT_LAB:
		case BID_LAB:
			Produce_Building_pUnit_All[Produce_Building_Num_All++] = &Unit_All[LoopTemp];
			if (Unit_All[LoopTemp].flag == Team_Flag)
				Produce_Building_Unit_Me[Produce_Building_Num_Me++] = &Unit_All[LoopTemp];
			else
			{
				//CONFUSING
				if (Available_NotMe.empty())
					Available_NotMe.push_back(&Unit_All[LoopTemp]);
				else
				{
					for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Farther(*it, &Unit_All[LoopTemp]); it++);
					if (it != Available_NotMe.end())
						Available_NotMe.insert(it, &Unit_All[LoopTemp]);
					else
						Available_NotMe.push_back(&Unit_All[LoopTemp]);
				}
			}
			//	Pro_Building_Num_En++;
			//如果需要敌方数据，就声明并且在函数前部初始化
			break;
		case TEACH_BUILDING:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				School_Num_Me++;
			}
			else
			{
				School_Unit_NotMe[School_Num_NotMe++] = &Unit_All[LoopTemp];
				if (Available_NotMe.empty())
					Available_NotMe.push_back(&Unit_All[LoopTemp]);
				else
				{
					for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Farther(*it, &Unit_All[LoopTemp]); it++);
					if (it != Available_NotMe.end())
						Available_NotMe.insert(it, &Unit_All[LoopTemp]);
					else
						Available_NotMe.push_back(&Unit_All[LoopTemp]);
				}
			}
			break;
		case BANK:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Bank_Num_Me++;
			}
			else
			{
				Bank_Unit_NotMe[Bank_Num_NotMe++] = &Unit_All[LoopTemp];
				if (Available_NotMe.empty())
					Available_NotMe.push_back(&Unit_All[LoopTemp]);
				else
				{
					for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Farther(*it, &Unit_All[LoopTemp]); it++);
					if (it != Available_NotMe.end())
						Available_NotMe.insert(it, &Unit_All[LoopTemp]);
					else
						Available_NotMe.push_back(&Unit_All[LoopTemp]);
				}
			}
			break;
		case BUILD_LAB:
			//skill_2(Unit_All[LoopTemp].unit_id, -1, Base_Unit_En->position);
		case FINANCE_LAB:

		case NANO_LAB:
		case MATERIAL_LAB:
			Skill_Building_pUnit_All[Ski_Building_Num_All++] = &Unit_All[LoopTemp];
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Skill_Building_Num_Me++;
				mySkill2Build.push_back(&Unit_All[LoopTemp]);
			}
			else
				if (Available_NotMe.empty())
					Available_NotMe.push_back(&Unit_All[LoopTemp]);
				else
				{
					for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Farther(*it, &Unit_All[LoopTemp]); it++);
					if (it != Available_NotMe.end())
						Available_NotMe.insert(it, &Unit_All[LoopTemp]);
					else
						Available_NotMe.push_back(&Unit_All[LoopTemp]);
				}
			break;
		case UAV:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				UAV_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case BATTLE_TANK:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				BATTLE_TANK_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case HACKER:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				HACKER_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case BOLT_TANK:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				BOLT_TANK_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case NUKE_TANK:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				NUKE_TANK_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case SUPERMAN:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				SUPERMAN_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		case EAGLE:
			if (Unit_All[LoopTemp].flag == Team_Flag)
			{
				Weapon_Unit_Me[Weapon_Num_Me++] = &Unit_All[LoopTemp];
				EAGLE_Num_Me++;
			}
			else
				Weapon_Unit_En[Weapon_Num_En++] = &Unit_All[LoopTemp];
			break;
		default:
			break;
		}
	}
}
/*****************************************获取信息已结束*****************************************/


/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   不多说了				 ├─┐
*           │   没感觉有什么要改的	 ┌─┘
*           │   大神要改就自便吧		 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************主函数开始*****************************************/
void player_main(void)
{
	srand((unsigned int)time(NULL));
	if (Turn_Num == 1)//回合计算
	{
		for (int LoopTemp = 0; LoopTemp < 300; LoopTemp++)
		{
			Task_Meat[LoopTemp].Aim_Unit = Task_Meat[LoopTemp].Meat_Unit = NULL;
			Task_Meat[LoopTemp].Aim_Type = NULL;
		}
	}
	Turn_Num++;
	Get_Info();
	Base_Update();
	Produce_Building_Update();
	Weapon_Update();
	resourse NowRes = getResourse();
	////cout << " Bank_Num_Me:" << Bank_Num_Me << " School_Num_Me:" << School_Num_Me << "  Skill_Building_Num_Me:" << Skill_Building_Num_Me << " Produce_Building_Num_Me:" << Produce_Building_Num_Me << " MyHealth:" << pBase_Unit_Me->health_now << ", EnHealth" << pBase_Unit_En->health_now << "  Meats_Me" << Meats_Mover_Num_Me << "  NowMoney" << NowRes.money_1 << "  NowMoney" << NowRes.money_2 << " NowTec" << NowRes.tech_1<< "  NowTec2" << NowRes.tech_2 << endl;
	//////cout << " MyHealth:" << pBase_Unit_Me->health_now << ", EnHealth" << pBase_Unit_En->health_now << endl;
}
/*****************************************主函数已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   我感觉写的挺好的		 ├─┐
*           │   如果要改的话呢		 ┌─┘
*           │   可以加上判断打哪个敌方 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************基地行为开始*****************************************/
void Base_Update()
{
	Meat_Mover_Num_Max_Me = Get_Meat_Max_Num();
	if (Meat_Mover_Num_Max_Me > 0)
		produce(Base_Unit_Me->unit_id);
	for (int LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)
	{
		//之后可以加入判断优先级
		int Lenth = Get_Len(Weapon_Unit_En[LoopTemp]->position, Base_Unit_Me->position);
		if (Lenth < Base_Unit_Me->shot_range_now)
		{
			skill_1(Base_Unit_Me->unit_id, -1, Weapon_Unit_En[LoopTemp]->position, none_position);
			break;
		}
	}//进行基地的进攻操作

	for (Unit* my : mySkill2Build)
	{
		switch (my->type_name)
		{
		case BUILD_LAB:
			skill_2(my->unit_id, -1, Base_Unit_En->position);
			break;
		case FINANCE_LAB:
			skill_2(my->unit_id, Meats_Mover_Unit_En[0]->unit_id);
			break;
		case NANO_LAB:
			skill_2(my->unit_id, Base_Unit_Me->unit_id);
			break;
		case MATERIAL_LAB:
			skill_2(my->unit_id, 1);
		default:
			break;
		}
	}
}
/*****************************************基地行为已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   我感觉写的挺好的		 ├─┐
*           │   如果要改的话呢		 ┌─┘
*           │ 可以加上占领东西优先级的 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************小鲜肉行为并返回需要小鲜肉数目开始*****************************************/
int Meat_Update()
{
	Available_Task.clear();
	for (int LoopTemp = 0; LoopTemp < 300; LoopTemp++)
	{
		if (Task_Meat[LoopTemp].Aim_Unit != NULL && Task_Meat[LoopTemp].Aim_Unit->flag != Team_Flag && (Task_Meat[LoopTemp].Aim_Type == ELEC_LAB || Task_Meat[LoopTemp].Aim_Type == HACK_LAB || Task_Meat[LoopTemp].Aim_Type == UAV_LAB || Task_Meat[LoopTemp].Aim_Type == CAR_LAB || Task_Meat[LoopTemp].Aim_Type == RADIATION_LAB || Task_Meat[LoopTemp].Aim_Type == AIRCRAFT_LAB || Task_Meat[LoopTemp].Aim_Type == BID_LAB))
			Task_Produce_Num_Now++;
		if (Task_Meat[LoopTemp].Aim_Unit != NULL && (Task_Meat[LoopTemp].Aim_Unit->flag == Team_Flag || Task_Meat[LoopTemp].Meat_Unit->health_now <= 0))//占领成功或者小鲜肉出师未捷身先死
		{
			Task_Meat[LoopTemp].Aim_Unit = Task_Meat[LoopTemp].Meat_Unit = NULL;
			Task_Meat[LoopTemp].Aim_Type = NULL;
		}
		if (Task_Meat[LoopTemp].Aim_Unit == NULL)
			Available_Task.push_back(LoopTemp);
		else
		{
			Meat_Capture(Task_Meat[LoopTemp].Meat_Unit, Task_Meat[LoopTemp].Aim_Unit);
			for (it = Available_Meat.begin(); it != Available_Meat.end();)
			{
				if (*it == Task_Meat[LoopTemp].Meat_Unit)//找出已经被用过的小鲜肉，删除之
				{
					Available_Meat.erase(it++);
					break;
				}
				else
					it++;
			}
			for (it = Available_NotMe.begin(); it != Available_NotMe.end();)
			{
				if (*it == Task_Meat[LoopTemp].Aim_Unit || (*it)->unit_id < 6)//找出已经被用过的非我建筑，删除之
				{
					Available_Meat.erase(it++);
					break;
				}
				else
					it++;
			}
		}
	}
	int ans = Available_NotMe.size() - Available_Meat.size();
	if (ans < 0)
		ans = 0;
	Unit* Min = NULL;
	int MinLen, NowLen;
	if (Task_Produce_Num_Now < 3)
	{
		/*for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Available_Meat.empty(); it++)
		{
		if ((*it)->type_name == UAV_LAB)
		{
		MinLen = 99999;
		for (it2 = Available_Meat.begin(); it2 != Available_Meat.end(); it2++)
		{
		NowLen = Get_Len((*it)->position, (*it2)->position);
		if (NowLen < MinLen)
		{
		MinLen = NowLen;
		Min = *it2;
		ittemp = it2;
		}
		}
		Meat_Capture(Min, *it);
		Available_Meat.erase(ittemp);
		it = Available_NotMe.erase(it);
		break;
		}
		}*/
		for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Available_Meat.empty(); it++)
		{
			if ((*it)->type_name == HACK_LAB || (*it)->type_name == CAR_LAB || (*it)->type_name == AIRCRAFT_LAB)
			{
				MinLen = 99999;
				for (it2 = Available_Meat.begin(); it2 != Available_Meat.end(); it2++)
				{
					NowLen = Get_Len((*it)->position, (*it2)->position);
					if (NowLen < MinLen)
					{
						MinLen = NowLen;
						Min = *it2;
						ittemp = it2;
					}
				}
				Meat_Capture(Min, *it);
				Available_Meat.erase(ittemp);
				it = Available_NotMe.erase(it);
				break;
			}
		}
		for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Available_Meat.empty(); it++)
		{
			if ((*it)->type_name == CAR_LAB || (*it)->type_name == RADIATION_LAB || (*it)->type_name == AIRCRAFT_LAB || (*it)->type_name == BID_LAB || (*it)->type_name == UAV_LAB)
			{
				MinLen = 99999;
				for (it2 = Available_Meat.begin(); it2 != Available_Meat.end(); it2++)
				{
					NowLen = Get_Len((*it)->position, (*it2)->position);
					if (NowLen < MinLen)
					{
						MinLen = NowLen;
						Min = *it2;
						ittemp = it2;
					}
				}
				Meat_Capture(Min, *it);
				Available_Meat.erase(ittemp);
				it = Available_NotMe.erase(it);
				break;
			}
		}
	}
	for (it = Available_NotMe.begin(); it != Available_NotMe.end() && !Available_Meat.empty();)
	{
		MinLen = 99999;
		for (it2 = Available_Meat.begin(); it2 != Available_Meat.end(); it2++)
		{
			NowLen = Get_Len((*it)->position, (*it2)->position);
			if (NowLen < MinLen)
			{
				MinLen = NowLen;
				Min = *it2;
				ittemp = it2;
			}
		}
		Meat_Capture(Min, *it);
		Available_Meat.erase(ittemp);
		it = Available_NotMe.erase(it);
	}
	return ans;
}
/*****************************************小鲜肉行为并返回需要小鲜肉数目已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   我感觉写的挺好的		 ├─┐
*           │   如果要改的话呢		 ┌─┘
*           │   可以加上躲避敌方攻击的 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************小鲜肉占领目标开始*****************************************/
void Meat_Capture(Unit* Meat_Unit, Unit* Target)
{
	int Lenth = Get_Len(Meat_Unit->position, Target->position);
	if (Lenth == 1)//可以占领
	{
		capture(Meat_Unit->unit_id, Target->unit_id);
		return;
	}
	Position TarPos;
	//if (Lenth < Meat_Unit->max_speed_now)//可以一步就位
	//{
	//	if (Meat_Unit->position.x < Target->position.x)
	//	{
	//		TarPos.y = Target->position.y;
	//		TarPos.x = Target->position.x - 1;
	//	}
	//	else if (Meat_Unit->position.x > Target->position.x)
	//	{
	//		TarPos.y = Target->position.y;
	//		TarPos.x = Target->position.x + 1;
	//	}
	//	else if (Meat_Unit->position.y > Target->position.y)
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Target->position.y + 1;
	//	}
	//	else
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Target->position.y - 1;
	//	}
	//	Move(Meat_Unit->unit_id, TarPos);
	Move(Meat_Unit->unit_id, Get_Next_Pos(Meat_Unit->position, Target->position, 1, Meat_Unit->max_speed_now));
	if (Lenth == 1)//可以占领
		capture(Meat_Unit->unit_id, Target->unit_id);
	return;
	//}
	//if (Meat_Unit->position.x < Target->position.x)//如果小鲜肉在左边
	//{
	//	if (-Meat_Unit->position.x + Target->position.x >= Meat_Unit->max_speed_now)
	//	{
	//		TarPos.x = Meat_Unit->position.x + Meat_Unit->max_speed_now;
	//		TarPos.y = Meat_Unit->position.y;
	//	}
	//	else if (Meat_Unit->position.y >= Target->position.y)
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Meat_Unit->position.y - Meat_Unit->max_speed_now - Meat_Unit->position.x + Target->position.x;
	//	}
	//	else
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Meat_Unit->position.y + Meat_Unit->max_speed_now + Meat_Unit->position.x - Target->position.x;
	//	}
	//}
	//else
	//{
	//	if (Meat_Unit->position.x - Target->position.x >= Meat_Unit->max_speed_now)
	//	{
	//		TarPos.x = Meat_Unit->position.x - Meat_Unit->max_speed_now;
	//		TarPos.y = Meat_Unit->position.y;
	//	}
	//	else if (Meat_Unit->position.y >= Target->position.y)
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Meat_Unit->position.y - Meat_Unit->max_speed_now + Meat_Unit->position.x - Target->position.x;
	//	}
	//	else
	//	{
	//		TarPos.x = Target->position.x;
	//		TarPos.y = Meat_Unit->position.y + Meat_Unit->max_speed_now - Meat_Unit->position.x + Target->position.x;
	//	}
	//}
	//Move(Meat_Unit->unit_id, TarPos);
}
/*****************************************小鲜肉占领目标已结束*****************************************/

/*****************************************判断距离远近开始*****************************************/
bool Farther(Unit* a, Unit* b)
{
	return Get_Len(a->position, Base_Unit_Me->position) > Get_Len(b->position, Base_Unit_Me->position);
}
/*****************************************判断距离远近已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │               		 │
*           │   下面这段不多说了		 ├─┐
*           │   连控制生产顺序都没有	 ┌─┘
*           │   连判断对方情况都没有	 │
*           │   大神求加代码！！！	 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************生产开始*****************************************/
void Produce_Building_Update()
{
	/*for (int i = 0; i < Produce_Building_Num_Me; i++)
	{
	switch (pProduce_Building_Unit_Me[i]->type_name)
	{
	//case ELEC_LAB:
	//case HACK_LAB:
	case UAV_LAB:
	//case CAR_LAB:
	case RADIATION_LAB:
	//case AIRCRAFT_LAB:
	//case BID_LAB:
	////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
	produce(pProduce_Building_Unit_Me[i]->unit_id);
	break;
	default:
	break;
	}
	}
	for (int i = 0; i < Produce_Building_Num_Me; i++)
	{
	switch (pProduce_Building_Unit_Me[i]->type_name)
	{
	//case ELEC_LAB:
	//case HACK_LAB:
	//case UAV_LAB:
	//case CAR_LAB:
	//case RADIATION_LAB:
	//case AIRCRAFT_LAB:
	case BID_LAB:
	////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
	produce(pProduce_Building_Unit_Me[i]->unit_id);
	break;
	default:
	break;
	}
	}
	for (int i = 0; i < Produce_Building_Num_Me; i++)
	{
	switch (pProduce_Building_Unit_Me[i]->type_name)
	{
	//case ELEC_LAB:
	//case HACK_LAB:
	//case UAV_LAB:
	case CAR_LAB:
	//case RADIATION_LAB:
	//case AIRCRAFT_LAB:
	//case BID_LAB:
	////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
	produce(pProduce_Building_Unit_Me[i]->unit_id);
	break;
	default:
	break;
	}
	}
	for (int i = 0; i < Produce_Building_Num_Me; i++)
	{
	switch (pProduce_Building_Unit_Me[i]->type_name)
	{
	case ELEC_LAB:
	case HACK_LAB:
	//case UAV_LAB:
	//case CAR_LAB:
	//case RADIATION_LAB:
	case AIRCRAFT_LAB:
	//case BID_LAB:
	////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
	produce(pProduce_Building_Unit_Me[i]->unit_id);
	break;
	default:
	break;
	}
	}*/
	resourse Rs = getResourse();
	//if (BATTLE_TANK_Num_Me >= 1)
	if ((Rs.money_2 > 500 && Team_Flag) || (Rs.money_1 > 500 && Team_Flag))
	{
		for (int i = 0; i < Produce_Building_Num_Me; i++)
		{
			if (Produce_Building_Unit_Me[i]->type_name == HACK_LAB || Produce_Building_Unit_Me[i]->type_name == ELEC_LAB)
			{
				switch (Produce_Building_Unit_Me[i]->type_name)
				{
					//case UAV_LAB:
					//	if (UAV_Num_Me < 8)
					//		////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
					//	produce(pProduce_Building_Unit_Me[i]->unit_id);
					//	break;
				case HACK_LAB:
					if (HACKER_Num_Me < 10)
						////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
						produce(Produce_Building_Unit_Me[i]->unit_id);
					break;
				default:
					if (BOLT_TANK_Num_Me < 3)
						////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
						produce(Produce_Building_Unit_Me[i]->unit_id);
					break;
				}
			}
		}
	}
	for (int i = 0; i < Produce_Building_Num_Me; i++)
	{
		if (Produce_Building_Unit_Me[i]->type_name == UAV_LAB || Produce_Building_Unit_Me[i]->type_name == HACK_LAB || Produce_Building_Unit_Me[i]->type_name == ELEC_LAB)
			continue;
		////cout << Name_Types[pProduce_Building_Unit_Me[i]->type_name] << " is Producing!!" << endl;
		produce(Produce_Building_Unit_Me[i]->unit_id);
	}
}
/*****************************************生产已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码  		 │
*           │   现在只是加了分兵种	 ├─┐
*           │   之后可以一定比例过去	 ┌─┘
*           │ 加个根据比赛进程改变策略 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************武器更新开始*****************************************/
void Weapon_Update()
{
	for (int LoopTemp = 0; LoopTemp < 300; LoopTemp++)
	{
		if (Weapon_Unit_Me[LoopTemp] == NULL)
			return;
		if (Weapon_Unit_Me[LoopTemp]->type_name == HACKER || Weapon_Unit_Me[LoopTemp]->type_name == BOLT_TANK || Weapon_Unit_Me[LoopTemp]->type_name == UAV || Weapon_Unit_Me[LoopTemp]->type_name == NUKE_TANK)
			Go_To_Hit_Enemy(Weapon_Unit_Me[LoopTemp]);
		else
			Get_Way(Weapon_Unit_Me[LoopTemp]);
		Hit(Weapon_Unit_Me[LoopTemp]);
	}
}
/*****************************************武器更新已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │   下面这段代码哈		 │
*           │   我感觉写的挺好的		 ├─┐
*           │   如果要改的话呢		 ┌─┘
*           │   可以加上躲避敌方攻击的 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************获取去距离某个点一定距离的路径下一个点开始*****************************************/
Position Get_Next_Pos(Position Unit_Position, Position Target_Position, int Lenth_Need, int Speed_Max)//获取去距离某个点一定距离的路径
{
	int Already_Moved;//分两步走的时候需要先计算已经走过的
	int Lenth = Get_Len(Unit_Position, Target_Position);
	int Xp, Yp;//当可以一步走到的时候在目的地附近的偏差移动，这里需要用随机数初始化，在后面有写
	Position TarPos;


	if (Lenth + Speed_Max >= Lenth_Need && Lenth - Speed_Max <= Lenth_Need)//可以一步就位
	{
		switch (rand() % 4)//为了之后写更多种写的switch
		{
		case 0:// Xp >= 0 ; Yp < 0 的情况
			for (Xp = rand() % Lenth_Need; Xp <= Lenth_Need; Xp++)
			{
				Yp = Xp - Lenth_Need;
				TarPos.x = Target_Position.x + Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}//故意不写break，为了之后能够返回！
		case 1://Xp >= 0; Yp >= 0;
			for (Xp = rand() % Lenth_Need; Xp <= Lenth_Need; Xp++)
			{
				Yp = Lenth_Need - Xp;
				TarPos.x = Target_Position.x + Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
		case 2://Xp <= 0; Yp >= 0;
			for (Xp = rand() % Lenth_Need; Xp <= Lenth_Need; Xp++)
			{
				Yp = Lenth_Need - Xp;
				TarPos.x = Target_Position.x - Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
		default:
			for (Xp = rand() % Lenth_Need; Xp <= Lenth_Need; Xp++)
			{
				Yp = Xp - Lenth_Need;
				TarPos.x = Target_Position.x - Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}//如果之后还没有返回，就遍历一下呗
			for (Xp = 0; Xp <= Lenth_Need; Xp++)
			{
				Yp = Xp - Lenth_Need;
				TarPos.x = Target_Position.x + Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
			for (Xp = 0; Xp <= Lenth_Need; Xp++)
			{
				Yp = Lenth_Need - Xp;
				TarPos.x = Target_Position.x + Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
			for (Xp = 0; Xp <= Lenth_Need; Xp++)
			{
				Yp = Lenth_Need - Xp;
				TarPos.x = Target_Position.x - Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
			for (Xp = 0; Xp <= Lenth_Need; Xp++)
			{
				Yp = Xp - Lenth_Need;
				TarPos.x = Target_Position.x - Xp;
				TarPos.y = Target_Position.y + Yp;
				if (Get_Len(Unit_Position, TarPos) <= Speed_Max)
					return TarPos;
			}
			break;
		}
	}
	else if (Lenth - Speed_Max >= Lenth_Need)//目前距离太远时
	{
		switch (rand() % 2)
		{
		case 0://先左右走
			if (Unit_Position.x < Target_Position.x)//应该向右走
			{
				if (Target_Position.x - Unit_Position.x >= Speed_Max)//如果目前直接向右不会越过去，那就直接走
				{
					TarPos.x = Unit_Position.x + Speed_Max;
					TarPos.y = Unit_Position.y;
				}
				else//此时先把x变成和Target一样的，之后再走y
				{
					TarPos.x = Target_Position.x;
					Already_Moved = Target_Position.x - Unit_Position.x;
					if (Unit_Position.y < Target_Position.y)
						TarPos.y = Unit_Position.y + Speed_Max - Already_Moved;
					else
						TarPos.y = Unit_Position.y - (Speed_Max - Already_Moved);
				}
			}
			else
			{
				if (Unit_Position.x - Target_Position.x >= Speed_Max)
				{
					TarPos.x = Unit_Position.x - Speed_Max;
					TarPos.y = Unit_Position.y;
				}
				else
				{
					TarPos.x = Target_Position.x;
					Already_Moved = Unit_Position.x - Target_Position.x;
					if (Unit_Position.y < Target_Position.y)
						TarPos.y = Unit_Position.y + Speed_Max - Already_Moved;
					else
						TarPos.y = Unit_Position.y - (Speed_Max - Already_Moved);
				}
			}
			return TarPos;
		default:
			if (Unit_Position.y < Target_Position.y)//应该向右走
			{
				if (Target_Position.y - Unit_Position.y >= Speed_Max)//如果目前直接向右不会越过去，那就直接走
				{
					TarPos.y = Unit_Position.y + Speed_Max;
					TarPos.x = Unit_Position.x;
				}
				else//此时先把x变成和Target一样的，之后再走y
				{
					TarPos.y = Target_Position.y;
					Already_Moved = Target_Position.y - Unit_Position.y;
					if (Unit_Position.x < Target_Position.x)
						TarPos.x = Unit_Position.x + Speed_Max - Already_Moved;
					else
						TarPos.x = Unit_Position.x - (Speed_Max - Already_Moved);
				}
			}
			else
			{
				if (Unit_Position.y - Target_Position.y >= Speed_Max)
				{
					TarPos.y = Unit_Position.y - Speed_Max;
					TarPos.x = Unit_Position.x;
				}
				else
				{
					TarPos.y = Target_Position.y;
					Already_Moved = Unit_Position.y - Target_Position.y;
					if (Unit_Position.x < Target_Position.x)
						TarPos.x = Unit_Position.x + Speed_Max - Already_Moved;
					else
						TarPos.x = Unit_Position.x - (Speed_Max - Already_Moved);
				}
			}
			return TarPos;
		}
	}//在这里直接写第一部分，后面都是复制粘贴之后替换x与y得到的
};
/*****************************************获取去距离某个点一定距离的路径下一个点已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │						 │
*           │   记得改一下攻击范围	 ├─┐
*           │   现在是为了能达到而-4	 ┌─┘
*           │   bug改完之后可以删掉	 │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************获取路径下一个点开始*****************************************/
void Get_Way(Unit* a)//在这里需要让所有的攻击敌方基地的单位能够在尽量远的距离进行攻击，并且不能全部到同一个位置，否则对这个位置发起攻击就会导致他们被同时打到
{
	int Lenth = Get_Len(a->position, Base_Unit_En->position);
	int Lenth_Need = max(a->shot_range_now - 4, 1);
	Position TarPos;
	TarPos = Get_Next_Pos(a->position, Base_Unit_En->position, Lenth_Need, a->max_speed_now);
	Move(a->unit_id, TarPos);
}
/*****************************************获取路径下一个点已结束*****************************************/
/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │						 │
*           │   记得改一下下面的代码	 ├─┐
*           │   加上不同兵种的攻击目标 ┌─┘
*           │                        │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/
/*****************************************获取打敌人下一个点开始*****************************************/
void Go_To_Hit_Enemy(Unit* a)
{
	Position TarPos;
	if (If_Arranged[a->type_name])
	{
		TarPos = Get_Next_Pos(a->position, Arranged_Position[a->type_name], 2, a->max_speed_now);
		Move(a->unit_id, TarPos);
		return;
	}
	int LoopTemp;
	Unit* Target_Unit = NULL;
	if (a->type_name != UAV)
	{
		for (LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)
		{
			if (a->type_name == HACKER && (Weapon_Unit_En[LoopTemp]->type_name != BATTLE_TANK && Weapon_Unit_En[LoopTemp]->type_name != BOLT_TANK && Weapon_Unit_En[LoopTemp]->type_name != NUKE_TANK))
				continue;
			if (a->type_name == NUKE_TANK && (Weapon_Unit_En[LoopTemp]->type_name == UAV || Weapon_Unit_En[LoopTemp]->type_name == EAGLE))
				continue;
			if (Weapon_Unit_En[LoopTemp]->shot_range_now <= Get_Len(Weapon_Unit_En[LoopTemp]->position, Base_Unit_Me->position))
				continue;//在这里优先选择可以打到的，但是也存在问题，因为不同的同种单位不会一直在同一位置
			if (Target_Unit == NULL || Get_Len(Weapon_Unit_En[LoopTemp]->position, a->position) < Get_Len(Target_Unit->position, a->position))
				Target_Unit = Weapon_Unit_En[LoopTemp];
		}
		if (Target_Unit != NULL)
		{
			If_Arranged[a->type_name] = true;
			TarPos = Get_Next_Pos(a->position, Target_Unit->position, 2, a->max_speed_now);
			Arranged_Position[a->type_name] = Target_Unit->position;
			Arranged_Tasks[a->type_name] = Target_Unit;
			Move(a->unit_id, TarPos);
			return;
		}
		for (LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)//后面这一部分去掉了对可以打到的论述，在这里通过削弱条件继续寻找最近的目标
		{
			if (a->type_name == HACKER && (Weapon_Unit_En[LoopTemp]->type_name != BATTLE_TANK && Weapon_Unit_En[LoopTemp]->type_name != BOLT_TANK && Weapon_Unit_En[LoopTemp]->type_name != NUKE_TANK))
				continue;
			if (a->type_name == NUKE_TANK && (Weapon_Unit_En[LoopTemp]->type_name == UAV || Weapon_Unit_En[LoopTemp]->type_name == EAGLE))
				continue;
			if (Target_Unit == NULL || Get_Len(Weapon_Unit_En[LoopTemp]->position, a->position) < Get_Len(Target_Unit->position, a->position))
				Target_Unit = Weapon_Unit_En[LoopTemp];
		}
		if (Target_Unit != NULL)
		{
			If_Arranged[a->type_name] = true;
			TarPos = Get_Next_Pos(a->position, Target_Unit->position, 2, a->max_speed_now);
			Arranged_Position[a->type_name] = Target_Unit->position;
			Arranged_Tasks[a->type_name] = Target_Unit;
			Move(a->unit_id, TarPos);
			return;
		}
		else
		{
			If_Arranged[a->type_name] = true;
			TarPos = Get_Next_Pos(a->position, Base_Unit_Me->position, 2, a->max_speed_now);
			Arranged_Position[a->type_name] = Base_Unit_Me->position;
			Move(a->unit_id, TarPos);
			return;
		}
	}
	else
	{
		//for (LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)
		//{
		//	if (pWeapon_Unit_En[LoopTemp]->type_name != HACKER && pWeapon_Unit_En[LoopTemp]->type_name != MEAT && pWeapon_Unit_En[LoopTemp]->type_name != SUPERMAN)
		//		continue;
		//	if (Target_Unit == NULL || Get_Len(pWeapon_Unit_En[LoopTemp]->position, a->position) < Get_Len(Target_Unit->position, a->position))
		//		Target_Unit = pWeapon_Unit_En[LoopTemp];
		//}
		//if (Target_Unit != NULL)
		//{
		//	If_Arranged[a->type_name] = true;
		//	TarPos = Get_Next_Pos(a->position, Target_Unit->position, 2, a->max_speed_now);
		//	Arranged_Position[a->type_name] = Target_Unit->position;
		//	Arranged_Tasks[a->type_name] = Target_Unit;
		//	Move(a->unit_id, TarPos);
		//	return;
		//}
		//else
		//{
		if (Arranged_Tasks[a->type_name] != NULL && Arranged_Tasks[a->type_name]->type_name == MEAT && Arranged_Tasks[a->type_name]->health_now > 0)
		{
			If_Arranged[a->type_name] = true;
			TarPos = Get_Next_Pos(a->position, Arranged_Tasks[a->type_name]->position, 2, a->max_speed_now);
			Arranged_Position[a->type_name] = Arranged_Tasks[a->type_name]->position;
			Move(a->unit_id, TarPos);
			return;
		}
		//cout << "OK" << endl;
		Target_Unit = NULL;
		for (LoopTemp = 0; LoopTemp < Meats_Mover_Num_En; LoopTemp++)
		{
			if (Meats_Mover_Unit_En[LoopTemp] == NULL)
				continue;
			if (Target_Unit == NULL || Get_Len(Meats_Mover_Unit_En[LoopTemp]->position, Base_Unit_En->position) < Get_Len(Target_Unit->position, Base_Unit_En->position))
			{
				Target_Unit = Meats_Mover_Unit_En[LoopTemp];
				//cout << "Getting!" << endl;
			}
		}
		//cout << "Got:" << Target_Unit->unit_id << endl;
		if (Target_Unit == NULL)
		{
			If_Arranged[a->type_name] = true;
			Target_Unit = Base_Unit_En;
			TarPos = Get_Next_Pos(a->position, Base_Unit_En->position, 4, a->max_speed_now);
			Arranged_Position[a->type_name] = Base_Unit_En->position;
		}
		else
		{
			If_Arranged[a->type_name] = true;
			TarPos = Get_Next_Pos(a->position, Target_Unit->position, 2, a->max_speed_now);
			Arranged_Position[a->type_name] = Target_Unit->position;
			//cout << "Meat!" << endl;
		}
		Arranged_Tasks[a->type_name] = Target_Unit;
		Move(a->unit_id, TarPos);
		return;
		//}
	}
}
/*****************************************获取打敌人下一个点已结束*****************************************/

/*
*
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │						 │
*           │   记得改一下攻击代码	 ├─┐
*           │   加上不同兵种的攻击目标 ┌─┘
*           │   谁去打谁什么的	     │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘
*             │ ─┤ ─┤       │ ─┤ ─┤
*             └──┴──┘       └──┴──┘
*                 神兽保佑
*                 代码无BUG!
*/

/*****************************************攻击开始*****************************************/
void Hit(Unit* a)
{
	if (a->type_name != UAV)
	{			//case HACKER:
				//case SUPERMAN:
				//case BATTLE_TANK:
				//case BOLT_TANK:
				//case NUKE_TANK:
				//case UAV:
				//case EAGLE:
				//	if (Get_Len(pBase_Unit_En->position, a->position) < a->shot_range_now || (a->type_name == EAGLE && (a->skill_last_release_time2 < 0 || Turn_Num - a->skill_last_release_time2 > 71)))
				////cout << Name_Types[a->type_name] << " will beat pBase_Unit_En! Length is " << Get_Len(pBase_Unit_En->position, a->position) << ".Shot range is " << a->shot_range_now << endl;
		if (Arranged_Tasks[a->type_name] != NULL && Get_Len(Arranged_Tasks[a->type_name]->position, a->position) <= a->shot_range_now)
		{
			if (a->type_name == NUKE_TANK && (a->skill_last_release_time2 <= 0 || Turn_Num - a->skill_last_release_time2 > 71))
			{
				skill_2(a->unit_id, -1, Arranged_Tasks[a->type_name]->position);
				return;
			}
			skill_1(a->unit_id, Arranged_Tasks[a->type_name]->unit_id);
		}
		if (Get_Len(Base_Unit_En->position, a->position) <= a->shot_range_now)
		{
			////cout << Name_Types[a->type_name] << " is beating pBase_Unit_En! Length is " << Get_Len(pBase_Unit_En->position, a->position) << ".Shot range is " << a->shot_range_now << endl;
			switch (a->type_name)
			{
			case NUKE_TANK:
				////cout << Name_Types[a->type_name]  << "'s skill_last_release_time2:" << a->skill_last_release_time2 << endl;
				if (a->skill_last_release_time2 <= 0 || Turn_Num - a->skill_last_release_time2 > 71)
				{
					skill_2(a->unit_id, -1, Base_Unit_En->position);
					return;
				}
			case EAGLE:
				////cout << Name_Types[a->type_name] << "'s skill_last_release_time2:" << a->skill_last_release_time2 << endl;
				if (a->skill_last_release_time2 <= 0 || Turn_Num - a->skill_last_release_time2 > 71)
				{
					skill_2(a->unit_id, -1, Base_Unit_En->position, Base_Unit_En->position);
					return;
				}
				else if (a->type_name == EAGLE)
				{
					skill_1(a->unit_id, -1, Base_Unit_En->position);
					return;
				}
			case UAV:
			case SUPERMAN:
			case BATTLE_TANK:
				skill_1(a->unit_id, Base_Unit_En->unit_id, none_position, none_position);
				return;
			default:
				break;
			}
		}
		for (int LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)
		{
			switch (a->type_name)
			{
			case NUKE_TANK:
				////cout << Name_Types[a->type_name] << "'s skill_last_release_time2:" << a->skill_last_release_time2 << endl;
				if (a->skill_last_release_time2 <= 0 || Turn_Num - a->skill_last_release_time2 > 71)
				{
					skill_2(a->unit_id, -1, Weapon_Unit_En[LoopTemp]->position);
					return;
				}
				break;
			case SUPERMAN:
				////cout << Name_Types[a->type_name] << "'s skill_last_release_time2:" << a->skill_last_release_time2 << endl;
				if (a->skill_last_release_time2 <= 0 || Turn_Num - a->skill_last_release_time2 > 51)
				{
					skill_2(a->unit_id);
					return;
				}
			default:
				break;
			}
			//之后可以加入判断优先级
			switch (a->type_name)
			{
			case HACKER:
				if (Weapon_Unit_En[LoopTemp]->type_name != BATTLE_TANK && Weapon_Unit_En[LoopTemp]->type_name != BOLT_TANK && Weapon_Unit_En[LoopTemp]->type_name != NUKE_TANK)
					continue;
				break;
			case SUPERMAN:
			case BATTLE_TANK:
			case NUKE_TANK:
				if (Weapon_Unit_En[LoopTemp]->type_name == UAV || Weapon_Unit_En[LoopTemp]->type_name == EAGLE)
					continue;
				break;
			case BOLT_TANK:
				if (Weapon_Unit_En[LoopTemp]->type_name == UAV || Weapon_Unit_En[LoopTemp]->type_name == EAGLE || Weapon_Unit_En[LoopTemp]->type_name == MEAT || Weapon_Unit_En[LoopTemp]->type_name == HACKER || Weapon_Unit_En[LoopTemp]->type_name == SUPERMAN)
					continue;
				break;
			default:
				break;
			}
			int Lenth = Get_Len(Weapon_Unit_En[LoopTemp]->position, a->position);
			if (Lenth <= a->shot_range_now)
			{
				skill_1(a->unit_id, Weapon_Unit_En[LoopTemp]->unit_id);
				break;
			}
		}//进行进攻操作
	}
	else
	{
		if (Arranged_Tasks[a->type_name] != NULL && Get_Len(Arranged_Tasks[a->type_name]->position, a->position) <= a->shot_range_now)
		{
			skill_1(a->unit_id, Arranged_Tasks[a->type_name]->unit_id);
			return;
		}
		for (int LoopTemp = 0; LoopTemp < Weapon_Num_En; LoopTemp++)
		{
			if (Weapon_Unit_En[LoopTemp]->type_name == MEAT && Get_Len(Weapon_Unit_En[LoopTemp]->position, a->position) <= a->shot_range_now)
			{
				skill_1(a->unit_id, Weapon_Unit_En[LoopTemp]->unit_id);
				return;
			}
		}
		if (Get_Len(Base_Unit_En->position, a->position) <= a->shot_range_now)
		{
			skill_1(a->unit_id, Base_Unit_En->unit_id);
			return;
		}
	}
}
/*****************************************攻击已结束*****************************************/