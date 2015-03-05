#ifndef _MAPCHAPTERONE_
#define _MAPCHAPTERONE_

#include "cocos2d.h"
#include "FootMan.h"
#include <string>
#include <utility>
USING_NS_CC;
typedef std::pair<int,int> PAIR;
enum Game_round
{
	PLAYER_ROUND,AI_ROUND
};//玩家回合，AI回合

enum Stage
{
	RUNNING,					//移动中
	STANDING,					//起始状态
	LIGHTING,					//高亮状态
	MENUING,					//菜单显示状态
	CHOOSING_ENEMY,             //敌军选择阶段
	ATTACKING,			          //攻击阶段
	ENEMY_DAMAGE,						//伤害阶段
	STRACK_BACK,					//反击阶段
	DAMAGE							//友军受伤阶段
};
enum graph_type
{
	GRAPH_NULL,						//空地
	GRAPH_GRASS,					//草地
	GRAPH_WATER,					//水域
	GRAPH_MOUNTAIN,					//山
	GRAPH_FORSET,					//森林
	GRAPH_SOFT_GROUND,				//软地
	GRAPH_HOUSE,					//表示帐篷
	GRAPH_WALL,						//表示城郭
	GRAPH_CANNOT_MOVE				//地形不能移动
};
class square						//格子类
{
public:
	
	void setSquare(int a,int b,int c,graph_type d)					//初始化函数，每一张地图都必须完成自己的create_graph，里面调用setSquare来完成初始化
	{
		l_corner_x = a;
		l_corner_y = b;
		footman_on = c;
		ground_type = d;
	}
	int l_corner_x;					//该格子左下角坐标
	int l_corner_y;					//该格子右下角坐标
	int footman_on;					//该格子是否有人在,为0表示没人，1表示己方，2表示友军，3表示敌军
	graph_type ground_type;			//表示该格子地形

};
class MapChapterOne:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();									//初始化函数

	CREATE_FUNC(MapChapterOne);
	
	bool virtual touchBegan(Touch*touch,Event*event);			//触摸函数
	void virtual touchMoved(Touch *touch,Event *event);			//触摸移动函数
	void virtual touchEnded(Touch *touch,Event *event);			//触摸结束函数
	void virtual touchCancelled(Touch*touch,Event *event);

	void lightOnMap(Point per_position,FootMan*footman);			//地图高亮函数

	int countWholeLightOn(int move_ability);				//根据移动力计算高亮格子数目

	bool judgeLightOnSquare(Point position,int ability,Point click);								//判断点击是否在高亮区域内
	//参数为人物坐标和人物移动力和点击位置

	std::string graphToPath(Point click,Point position);							//将格子转换到路径字符串

	PAIR virtual PositionToGraph(Point position);						//转换坐标到格子位置

	void virtual create_graph();										//400个格子的初始化

	virtual void update(float dt);									//帧刷新函数

	bool virtual judgeInAttackArea();						//判断是否在攻击区域内
	std::vector<PAIR> getAttackGraph();						//获得攻击范围格子

	void virtual showAttackArea();							//显示攻击区域3X3

	void showAttackAreaSZ();						//显示十字攻击区域

	void AttackPicRelease();						//消除攻击区域

	void returnToOrigin();									//使当前激活人物移动结束后回到原始位置
	bool isSide(PAIR position);								//判断当前格子是否是边界

	FootMan* getAttackActor();									//获得攻击对象
private:
	float x;										//表示一次触点位置
	float y;				
	Point bgPosition;								//表示左下角位置
	int x_offset;									//表示背景图片可移动的矩形框长短，每张地图不同
	int y_offset;									

	bool islight_on;								//表示本地图上是否有人物的移动区域被高亮
	int width;										//地图大小，宽度，高度
	int height;		

	square graph[20][20];								//地图格子类，每一格有4个成员，左下角坐标，地形，是否有人

	Point choose_position;							//每一次动作的起始位置
	Sprite *menu;										//菜单
	Sprite *noAttackLabel;
};







#endif
