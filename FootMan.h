#ifndef _FOOTMAN_H
#define _FOOTMAN_H


#include "cocos2d.h"

USING_NS_CC;			//奇怪，不加这个会报错，加这个在#include "cocos2d.h"前也会报错

enum Actor_face_to				//表示人物的面对方向

{
	UP,DOWN,LEFT,RIGHT
};
enum State_actor
{
	STATE_WALK,STATE_STAND
};//人物移动，人物静止
enum FootManClass						//该枚举类型用于表示兵种类型
{
	//十字攻击兵种,轻骑兵，重骑兵，拳师，武道家，拳圣，训虎师，训熊师，
	QQB,ZQB,QS,WDJ,QuanS,XHS,XXS,
	//3X3攻击兵种，轻步兵，重步兵，禁卫军，近卫骑兵，西凉骑兵，贼兵，义贼，豪杰
	
	//黄巾军，海盗，都督，
	QBB,ZBB,JWJ,JWQB,XLQB,ZB,YZ,HJ,HJJ,HD,DD,
	//马弓兵,弩骑兵，连弩骑兵
	MGB,NQB,LNQS,
	//弓兵，弩兵，连弩兵
	GB,NB,LNB,
	//砲车，重砲车，霹雳车
	PC,ZPC,PLC
};
class FootMan :public cocos2d::Node
{
public :
	FootMan():move_ability(5){};						//默认移动力5
	~FootMan();
	bool createFootMan(char *file_name,char *attack_path,char *spec_path,int x,int y,int identity);		//创建人物
	CREATE_FUNC(FootMan);
	bool setAnimation(char*path);						//设置动画,x为水平移动，y为竖直移动
	void stopAnimation();								//停止动画
	bool judgeclick();									//判断是否点击
	void changeSprite();
	
	Point getPositionFootMan();							//获得人物位置函数

	virtual void AttackAnimation(Actor_face_to orientation);								//执行攻击动画
	
	void setClicked(bool clicked);						//表示一个人物被点击已触发高亮
	void setMoveAbility(int ability);					//设置移动距离
	
	int getMoveAblity();								//获得移动距离
	void setState(State_actor state);									//设置人物状态
	State_actor getState();								//获得人物状态
	FootManClass getType();								//获得兵种类型
	void changeAttackAction();								//用于攻击动作的加锁

	void refreshCG();									//position变动之后使得cg移动

	void setPositionFootMan(Vec2 position);

	void strackbackAnimation(Actor_face_to orientation);								//反击动画

	void damageAnimation(Actor_face_to orientation);
protected:
	char *filename;									//每一个人物相关的图片
	char *attackPath;									//攻击图片路径
	char *movPath;										//移动图片路径
	char *specPath;										//反击图片路径
	Point position;										//人物位置
	Sprite *cg;											//cg替换
	
	bool isclicked;										//判断人物是否被点击
	int move_ability;									//人物移动力
	Sprite *cg1;										//替换cg
	State_actor _state;									//人物状态
	FootManClass classLabel;							//兵种
	//--------------------人物基本属性----------------//
	int identity;										//是否为友军,0为敌军，1为友军，2为可控制本军
	int hp;												//血量
	int mp;												//魔法值


};
class Path							//该类用于路径解析
{
public :
	Path(char _arrow,int _step);

	int step;
	char arrow;
};
class Offset						//该类用于处理可移动范围
{
public :
		int x_offset;
		int y_offset;
	
};
#endif