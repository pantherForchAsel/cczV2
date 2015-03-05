#include "MapChapterOne.h"
#include "FootMan.h"
#include <vector>
#include <strstream>
#include "cocos2d.h"
USING_NS_CC;
FootMan *now_actor = NULL;
FootMan *now_PC_actor = NULL;
FootMan *now_enemy_actor = NULL;
Sprite *NoteLabel;								//消息提示标签
bool NoteLabeling = false;						//消息是否提示

bool isrunning = false;							//是否在完成动作，此时的点击全部无视
bool running_complete = false;					//人物动作完成与否，来决定是否可以继续点击和菜单的出现
bool menu_appear = false;						//控制菜单的出现和是否在点击中处理菜单事件
bool choosing_enemy = false;					//为true表示正在选择攻击对象

float time_elapse = 0;							//计时器,标记显示文字时间

Stage running_stage;

Point original_position = Vec2(0,0);							//记录人物初始位置
std::vector<FootMan*> footman_array;		//地图上我军人物存储
std::vector<FootMan*> enemy_friend_array;			//地图上敌军与友军存储
std::vector<Sprite *> light_area;	//高亮区域图片缓存
std::vector<Sprite *> attack_area;   //攻击图片缓存
Scene *MapChapterOne::createScene()
{
	auto scene = Scene::create();

	auto layer = MapChapterOne::create();

	scene->addChild(layer);

	return scene;
}
bool MapChapterOne::init()
{
	if(!Layer::init())
		return false;

	Size visiblesize = Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	running_stage = STANDING;																	//起始状态
	time_elapse = 0;
	//add picture
	Sprite *bg = Sprite::create("1.png");

	this->addChild(bg,0,0);
	bg->setAnchorPoint(Point(0,0));
	bg->setPosition(Vec2(0,0));
	islight_on = false;
	isrunning = false;
	this->create_graph();
	//add person
	//------------------------------------//
	auto footman1 = FootMan::create();
	footman1->createFootMan("resource/mov/1-1.png","resource/ATT/1-1.png",456,312,2);
	PAIR position1 = PositionToGraph(footman1->getPositionFootMan());	
	graph[position1.first][position1.second].footman_on = 1;
	footman_array.push_back(footman1);				//保存所有的人物指针
	footman1->setState(STATE_STAND);
	
	this->addChild(footman1,3);
	//-------------------------------------//
	auto footman2 = FootMan::create();
	footman2->createFootMan("resource/mov/9-1.png","resource/ATT/9-1.png",408,408,0);
	PAIR position2 = PositionToGraph(footman2->getPositionFootMan());
	graph[position2.first][position2.second].footman_on = 3;
	enemy_friend_array.push_back(footman2);
	footman2->setState(STATE_STAND);
	this->addChild(footman2,3);
	//------------------------------------//
	


	menu = Sprite::create("menu.png");
	addChild(menu,5);
	menu->setVisible(false);

	auto _touchLister = EventListenerTouchOneByOne::create();
	_touchLister->setSwallowTouches(false);
	_touchLister->onTouchBegan = CC_CALLBACK_2(MapChapterOne::touchBegan,this);
	_touchLister->onTouchMoved = CC_CALLBACK_2(MapChapterOne::touchMoved,this);

	_touchLister->onTouchEnded = CC_CALLBACK_2(MapChapterOne::touchEnded,this);
	_touchLister->onTouchCancelled = CC_CALLBACK_2(MapChapterOne::touchCancelled,this);


	_eventDispatcher->addEventListenerWithFixedPriority(_touchLister,-255);

	this->scheduleUpdate();

	return true;
		
	

}
bool MapChapterOne::touchBegan(Touch* touch,Event *event)
{
	FootMan *temporary;
	x = touch->getLocation().x;
	y = touch->getLocation().y;
	switch(running_stage)
	{
	case STANDING:
		{
					auto iter = footman_array.begin();
					Point location = touch->getLocation();
					while(iter!=footman_array.end())
						{
							//获取每个人物的位置
		
							Point position = (*iter)->getPositionFootMan();
							if(location.x>position.x-48&&location.x<position.x+48&&location.y>position.y-48&&location.y<position.y+48)
							{
								
								(*iter)->setClicked(true);
								lightOnMap(position,*iter);				//设置高亮，以及点击标志
								original_position = (*iter)->getPositionFootMan();
								running_stage = LIGHTING;
							}
							iter++;
						}

					
					
		}
		break;
	case RUNNING:
		{

		}
		break;
	case MENUING:
		{
		
						Point position = menu->getPosition();
						if(x>position.x-44&&y>position.y-61&&y<position.y+61&&x<position.x+44)	//在菜单区域内
						{
							if(x>position.x-44&&x<position.x+44&&y<position.y+61&&y>=position.y+38)		//攻击
							{
								if(judgeInAttackArea())						//攻击范围内有人
								{
									
									showAttackArea();								//显示攻击范围
									running_stage = CHOOSING_ENEMY;						//进入攻击阶段
								}
								else 
								{
									noAttackLabel = Sprite::create("attackarealabel.png");
									addChild(noAttackLabel,5);
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									Size visiblesize = Director::getInstance()->getVisibleSize();
									noAttackLabel->setPosition(origin.x+visiblesize.width/2,origin.y+visiblesize.height/2);
									NoteLabeling= true;
									noAttackLabel->setVisible(true);
									time_elapse = 0;
									running_stage = STANDING;
								}
														
							}
							if(y<position.y+38&&y>position.y+15)						//策略
							{

							}
							if(y<position.y+15&&y>position.y-7)						//道具
							{

							}
							if(y<position.y-7&&y>position.y-30)						//待命
							{

							}
							if(y<position.y-30&&y>position.y-61)						//取消
							{
								returnToOrigin();
							}
						}
						
		}
		break;
	case LIGHTING:
		{
						
						auto iter = footman_array.begin();						//人物列表迭代器
						Point location = touch->getLocation();					//获得点击位置
						while(iter!=footman_array.end())
						{
							if((*iter)->judgeclick()==true)						//找到被点击人物
							{
								temporary = *iter;								//获得该人物
								now_actor = temporary;
							}
							iter++;
						}
						Point position = temporary->getPositionFootMan();
						if(judgeLightOnSquare(position,temporary->getMoveAblity(),location))								//在高亮区域内
						{
							std::string path = graphToPath(position,location);

							const char *pathTran = path.c_str();					//转换const char *到char*
							char *copy = new char[strlen(pathTran)+1];
							strcpy(copy,pathTran);
							temporary->setPosition(temporary->getPositionFootMan());		//这段代码只要在setAnimation下面，
							//就会导致其在动画执行前执行
							
							running_stage = RUNNING;
							temporary->setState(STATE_WALK);
							temporary->setAnimation(copy);
				
				
							//Sleep(1000);
							temporary->stopAnimation();
				
			
						}
		
		
							for(auto iter = light_area.begin();iter!=light_area.end();iter++)		//高亮后无论如
								//点击都会清理高亮区域缓存
							{
								if((*iter)!=NULL)
								{
									removeChild(*iter);
								}
							}
							light_area.clear();
							temporary->setClicked(false);	
		}
		break;
	case ATTACKING:
		{
		}
		break;

	case CHOOSING_ENEMY:
		{
			Actor_face_to face_to;
			x = touch->getLocation().x;
			y = touch->getLocation().y;
			PAIR graph_position = PositionToGraph(Point(x,y));
			PAIR actor_position = PositionToGraph(now_actor->getPositionFootMan());
			std::vector<PAIR> array_pair = getAttackGraph();
			for(auto iter = array_pair.begin();iter!=array_pair.end();iter++)			//判断是否在攻击区域内
			{
				if((actor_position.first+(*iter).first==graph_position.first&&
					actor_position.second+(*iter).second==graph_position.second)&&(graph[graph_position.first][graph_position.second].footman_on==3))				//通过获得点击格子在攻击格子的区域内，并且点击格子有敌军
					//在攻击范围内
				{
					//判断面向，调用人物攻击动画
					if((*iter).first<0)
						 face_to = LEFT;
					else if((*iter).first>0)
						 face_to = RIGHT;
					else if((*iter).first==0&&(*iter).second>0)
						 face_to = UP;
					else
						 face_to = DOWN;
					AttackPicRelease();											//消除攻击图片
					running_stage = ATTACKING;
					now_actor->AttackAnimation(face_to);			//动作执行，必须设置isrunning为true
					

					//设置被攻击对象
					for(auto iter:enemy_friend_array)
					{
						int attack_position_x = (iter->getPositionFootMan()).x;
						int attack_position_y = (iter->getPositionFootMan()).y;
						PAIR attack_position = PositionToGraph(iter->getPositionFootMan());
						if((attack_position.first==graph_position.first)&&(attack_position.second==graph_position.second))				//点击的位置和某个敌军的格子一致，找到被攻击敌军
						{
							now_enemy_actor = iter;

						}
					}

				}
			}
		}
		break;
	
	case STRACK_BACK:
		{
			now_enemy_actor->
		}
	return true;
	}
}
/*bool MapChapterOne::touchBegan(Touch* touch,Event*event)
{
	if(isrunning==false)													//表示没有动作运行
	{
		if(true == choosing_enemy)												//没有动作，表示是攻击阶段
		{
			Actor_face_to face_to;
			x = touch->getLocation().x;
			y = touch->getLocation().y;
			PAIR graph_position = PositionToGraph(Point(x,y));
			PAIR actor_position = PositionToGraph(now_actor->getPositionFootMan());
			std::vector<PAIR> array_pair = getAttackGraph();
			for(auto iter = array_pair.begin();iter!=array_pair.end();iter++)			//判断是否在攻击区域内
			{
				if(actor_position.first+(*iter).first==graph_position.first&&
					actor_position.second+(*iter).second==graph_position.second)
					//在攻击范围内
				{
					//判断面向，调用人物攻击动画
					if((*iter).first<0)
						 face_to = LEFT;
					else if((*iter).first>0)
						 face_to = RIGHT;
					else if((*iter).first==0&&(*iter).second>0)
						 face_to = UP;
					else
						 face_to = DOWN;
					AttackPicRelease();											//消除攻击图片
					isrunning = true;
					now_actor->AttackAnimation(face_to);			//动作执行，必须设置isrunning为true
					choosing_enemy = false;								//阶段完成
				}
			}
		}
		else                                                                 //非攻击阶段，人物不移动
		{
										//人物没有移动，可能是未移动，确定移动位置，菜单点击				  
					FootMan *temporary;
					x = touch->getLocation().x;
					y = touch->getLocation().y;
					if(false == menu_appear)						//菜单未出现，什么都不做
					{
					}	
					else										//菜单出现，人物没移动
					{
						Point position = menu->getPosition();
						if(x>position.x-44&&y>position.y-61&&y<position.y+61&&x<position.x+44)	//在菜单区域内
						{
							if(x>position.x-44&&x<position.x+44&&y<position.y+61&&y>=position.y+36)		//攻击
							{
								if(judgeInAttackArea())						//攻击范围内有人
								{
									choosing_enemy = true;
									showAttackArea();								//显示攻击范围
									
								}
								else 
								{
									noAttackLabel = Sprite::create("attackarealabel.png");
									addChild(noAttackLabel,5);
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									Size visiblesize = Director::getInstance()->getVisibleSize();
									noAttackLabel->setPosition(origin.x+visiblesize.width/2,origin.y+visiblesize.height/2);
									NoteLabeling= true;
									noAttackLabel->setVisible(true);
									time_elapse = 0;
								}
														
							}
							if(y<position.y+36&&y>position.y+11)						//策略
							{

							}
							if(y<position.y+11&&y>position.y-14)						//道具
							{

							}
							if(y<position.y-14&&y>position.y-39)						//待命
							{

							}
							if(y<position.y-39&&y>position.y-61)						//取消
							{
								returnToOrigin();
							}
						}
						menu_appear = false;
					}



					if(islight_on==false)				//未高亮
					{
						//循环人物vector，然后对每个人物判断其是否被点击到
						auto iter = footman_array.begin();
						Point location = touch->getLocation();
						while(iter!=footman_array.end())
						{
							//获取每个人物的位置
		
							Point position = (*iter)->getPositionFootMan();
							if(location.x>position.x-48&&location.x<position.x+48&&location.y>position.y-48&&location.y<position.y+48)
							{
								if((*iter)->getState()!=STATE_WALK)
								{
								(*iter)->setClicked(true);
								lightOnMap(position,*iter);				//设置高亮，以及点击标志
								islight_on = true;
								}
								else
								{
								}
			
							}
							iter++;
						}
					}
					else                                            //已高亮，获取点击位置移动
					{
						auto iter = footman_array.begin();						//人物列表迭代器
						Point location = touch->getLocation();					//获得点击位置
						while(iter!=footman_array.end())
						{
							if((*iter)->judgeclick()==true)						//找到被点击人物
							{
								temporary = *iter;								//获得该人物
								now_actor = temporary;
							}
							iter++;
						}
						Point position = temporary->getPositionFootMan();
						if(judgeLightOnSquare(position,temporary->getMoveAblity(),location))								//在高亮区域内
						{
							std::string path = graphToPath(position,location);

							const char *pathTran = path.c_str();					//转换const char *到char*
							char *copy = new char[strlen(pathTran)+1];
							strcpy(copy,pathTran);
							temporary->setPosition(temporary->getPositionFootMan());		//这段代码只要在setAnimation下面，
							//就会导致其在动画执行前执行
							isrunning = true;
				
							temporary->setState(STATE_WALK);
							temporary->setAnimation(copy);
				
				
							//Sleep(1000);
							temporary->stopAnimation();
				
			
						}
		
		
							for(auto iter = light_area.begin();iter!=light_area.end();iter++)		//高亮后无论如
								//点击都会清理高亮区域缓存
							{
								if((*iter)!=NULL)
								{
									removeChild(*iter);
								}
							}
							light_area.clear();
						temporary->setClicked(false);
		    
						islight_on = false;
					}
					return true;
				}
		}
		else
		{
			return true;
		}
	}
	*/
void MapChapterOne::touchMoved(Touch*touch,Event *event)			//地图滚动函数
{
	/**/
}
void MapChapterOne::touchEnded(Touch *touch,Event*event)
{
	if(running_stage==STANDING)						//移动中不能移动屏幕
	{
	


		auto children = this->getChildren();
		auto location  = touch->getLocation();
		float x_move = location.x-x;
		float y_move = location.y-y;
		if(x_move>0&&y_move>0)								//向上
		{
				//循环人物设置其position成员位置下降
		for(auto iter:footman_array)
		{
			iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y+48));
		}
		for(auto iter:enemy_friend_array)
		{
			iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y+48));
		}
			if(this->getChildByTag(0)->getPosition().y<0)
			{
				auto iter = children.begin();
				for(int i = 0;i<children.size();i++)
				{
				
					(*iter)->setPosition((*iter)->getPosition().x,(*iter)->getPosition().y+48);
					iter++;
				}
			
			}
		}
		else if(x_move>0);
		
		else if(y_move>0)							//向上移动
		{
			for(auto iter:footman_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y+48));
			}
			for(auto iter:enemy_friend_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y+48));
			}
			if(this->getChildByTag(0)->getPosition().y<0)
			{
				auto iter = children.begin();
				for(int i = 0;i<children.size();i++)
					{
				
					(*iter)->setPosition((*iter)->getPosition().x,(*iter)->getPosition().y+48);
					iter++;
					}
			
			}
		}
		else if(x_move<0&&y_move<0)
		{
			for(auto iter:footman_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y-48));
			}
			for(auto iter:enemy_friend_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y-48));
			}
			if(this->getChildByTag(0)->getPosition().y>-336)
			{
				auto iter = children.begin();
				for(int i = 0;i<children.size();i++)
				{
				
					(*iter)->setPosition((*iter)->getPosition().x,(*iter)->getPosition().y-48);
					iter++;
				}
			
			}
		}
		else if(x_move<0)
		{
		
		}
		else if(y_move<0)
		{
			for(auto iter:footman_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y-48));
			}
			for(auto iter:enemy_friend_array)
			{
				iter->setPositionFootMan(Vec2(iter->getPositionFootMan().x,iter->getPositionFootMan().y-48));
			}
			if(this->getChildByTag(0)->getPosition().y>-336)
			{
				auto iter = children.begin();
			for(int i = 0;i<children.size();i++)
				{
				
					(*iter)->setPosition((*iter)->getPosition().x,(*iter)->getPosition().y-48);
					iter++;
			
				}
			}
		}
	}
	else
	{
	}
}
void MapChapterOne::touchCancelled(Touch*touch,Event *event)
{
}
void MapChapterOne::lightOnMap(Point per_position,FootMan*target)		//在计算的区域中绘图
{
	assert(target->judgeclick());	//目标人物被点击
	now_actor = target;
	auto iter = light_area.begin();										//循环vector添加进图片缓存
	for(int i = 0;i<countWholeLightOn(target->getMoveAblity());i++)			//根据人物移动力添加缓存
	{
		Sprite *lighter = Sprite::create("small/trans.png");
		light_area.push_back(lighter);								//缓存对应移动力的图片
		
	}
	iter = light_area.begin();
	int ability = target->getMoveAblity();
	Point position  = target->getPositionFootMan();
		for(int j = 0;j<ability+1;j++)										//按列载入，左侧列
		{
			for(int k = 0;k<2*j+1;k++)										//每一列垂直的载入
			{
				Sprite *pic = *iter;
				addChild(pic,1);
				pic->setPosition(position.x-(ability-j)*48,position.y+j*48-k*48);
				iter++;
			}
		}
		for(int j = ability+1;j<ability*2+1;j++)	//t=7,应该有11个格子，右侧列
		{
			for(int k = 0;k<(ability*2-j)*2+1;k++)
			{
					Sprite *pic = *iter;
					addChild(pic,1);
					pic->setPosition(position.x+(j-ability)*48,position.y+(2*ability-j-k)*48);
					iter++;
			}
		}
		
	
}
int MapChapterOne::countWholeLightOn(int move_ability)
{
	int step = move_ability*2+1;
	for(int i = move_ability-1;i>=0;i--)
	{
		step+=(i*2+1)*2;
	}
	return step;
}
bool MapChapterOne::judgeLightOnSquare(Point position,int ability,Point click)
{
	int actor_graph_x = this->PositionToGraph(position).first;
	int actor_graph_y = this->PositionToGraph(position).second;
	int click_graph_x = this->PositionToGraph(click).first;
	int click_graph_y = this->PositionToGraph(click).second;
	if(actor_graph_x+actor_graph_y-click_graph_x-click_graph_y<=ability||click_graph_x+click_graph_y-actor_graph_x-actor_graph_y<=ability)			//判断在移动区域内
	{
		return true;
	}
	else
		return false;
}
PAIR MapChapterOne::PositionToGraph(Point position)
{
	auto bg = this->getChildByTag(0);			//获得背景图片
	auto position_1 = bg->getPosition();			//获得背景图片锚点左下角

	int x = position_1.x;
	int y = position_1.y;

	//若背景图片锚点(0,-96),传入位置（100,15），格子就是（2,2）
	int x_graph = (position.x-x)/48;
	int y_graph = (position.y-y)/48;								//格子是从0开始的
	PAIR result = std::make_pair(x_graph,y_graph);
	return result;
}
void MapChapterOne::create_graph()
{
	int inarray[20][20] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5},
		{0,0,0,0,0,8,8,8,8,0,0,8,8,8,8,0,0,0,5,5},
		{0,0,0,0,0,8,0,0,0,0,0,0,0,6,8,0,5,5,5,5},
		{5,0,0,0,0,8,0,0,0,0,0,0,0,6,8,5,5,5,5,5},
		{5,5,0,0,0,0,0,0,0,7,6,0,0,0,8,5,5,5,5,5},
		{5,5,5,0,0,0,0,0,0,8,8,0,0,0,8,5,5,5,5,5},
		{5,5,5,5,0,8,0,0,0,0,0,0,0,0,8,0,5,5,5,5},
		{5,5,5,5,5,8,0,0,0,0,0,0,0,0,8,0,5,5,5,5},
		{5,5,5,5,5,8,0,0,0,0,0,0,0,0,8,0,5,5,5,5},
		{5,5,5,5,5,8,8,8,8,0,0,8,8,8,8,0,0,5,5,5},
		{5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5},
		{5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5},
		{5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5},
		{5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	for(int i = 0;i<20;i++)
	{
		for(int j = 0;j<20;j++)
		{
			graph[i][j].setSquare(i*48,j*48,0,(static_cast<graph_type>(inarray[i][j])));
		}
	}
	int t = 0;

}
std::string MapChapterOne::graphToPath(Point position,Point click)
{
	int actor_graph_x = this->PositionToGraph(position).first;
	int actor_graph_y = this->PositionToGraph(position).second;
	int click_graph_x = this->PositionToGraph(click).first;
	int click_graph_y = this->PositionToGraph(click).second;

	int x_offset = click_graph_x-actor_graph_x;
	int y_offset = click_graph_y-actor_graph_y;
	std::string tem = "";
	std::string path = "";
	path+="X:";
	std::strstream ss; 
	ss<<x_offset;
	ss>>tem;
	path+=tem;
	ss.clear();
	path+="Y:";
	ss<<y_offset;
	ss>>tem;
	path+=tem;

	return path;
}
void MapChapterOne::update(float dt)
{
	FootMan *temporay;
	//if((running_complete==true)&&menu_appear == true)
	if(running_stage == MENUING)
	{
		
		menu->setVisible(true);
		
		if(now_actor!=NULL)
			menu->setPosition(now_actor->getPositionFootMan().x+67,now_actor->getPositionFootMan().y);
		running_complete = false;
	}
	else if(false == menu_appear)
	{
		menu->setVisible(false);
	}
	if(true==NoteLabeling)
	{
		time_elapse++;
		if(time_elapse>120)
		{
			NoteLabeling = false;
			noAttackLabel->setVisible(false);
		}
	}
}
void MapChapterOne::returnToOrigin()
{
	Point position = original_position;
	auto temporary = now_actor;
	temporary->setPosition(position);
	temporary->setPositionFootMan(position);
	temporary->refreshCG();
	running_stage = STANDING;

}
void MapChapterOne::showAttackArea()
{
	FootMan *temporary;
	temporary = now_actor;
	/*for(auto iter = footman_array.begin();iter!=footman_array.end();iter++)
	{
		if((*iter)->judgeclick())
		{
			temporary = *iter;
		}

	}*/
	auto position = temporary->getPositionFootMan();
	PAIR graph_position = PositionToGraph(position);			//获得人物格子，可能是（6,6），但是显示可能
	//是（5,5），要根据背景图片的变化设置位置

	auto bgposition = this->getChildByTag(0)->getPosition();		//获得背景图片位置
	//int bg_x = bgposition.x/48;
	int bg_y = bgposition.y/48;
	//获得背景图片 y方向移动距离
	if(bg_y<0)bg_y = -bg_y;
	auto attack_iter = attack_area.begin();
	if(isSide(graph_position))									//在边界区域
	{
		if(graph_position.first==0&&graph_position.second-bg_y==0)			//在左下角上
		{
			for(int i = 0;i<3;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);

			}
			for(auto i = graph_position.first;i<=graph_position.first+1;i++)
			{
				for(auto j = graph_position.second-bg_y;j<=graph_position.second-bg_y+1;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second-bg_y)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24-bg_y*48);
					}
				}
			}
		}
		else if(graph_position.first==0&&graph_position.second==19)		//在左上角
		{
			for(int i = 0;i<3;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first;i<=graph_position.first+1;i++)
			{
				for(auto j = graph_position.second-1;j<=graph_position.second;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.first==19&&graph_position.second==0)		//右下角
		{
			for(int i = 0;i<3;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first-1;i<=graph_position.first;i++)
			{
				for(auto j = graph_position.second;j<=graph_position.second+1;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.first==19&&graph_position.second==19)		//右上角
		{
			for(int i = 0;i<3;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first-1;i<=graph_position.first;i++)
			{
				for(auto j = graph_position.second-1;j<=graph_position.second;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.first==0)							//左边界
		{
			for(int i = 0;i<5;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first;i<=graph_position.first+1;i++)
			{
				for(auto j = graph_position.second-1;j<=graph_position.second+1;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.second==0)							//下边界
		{
			for(int i = 0;i<5;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first-1;i<=graph_position.first+1;i++)
			{
				for(auto j = graph_position.second;j<=graph_position.second+1;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.first==19)							//右边界
		{
			for(int i = 0;i<5;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first-1;i<=graph_position.first;i++)
			{
				for(auto j = graph_position.second-1;j<=graph_position.second+1;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
		}
		else if(graph_position.second == 19)						//上边界	
			for(int i = 0;i<5;i++)
			{
				Sprite *pic = Sprite::create("attack.png");
				attack_area.push_back(pic);
			}
			for(auto i = graph_position.first-1;i<=graph_position.first+1;i++)
			{
				for(auto j = graph_position.second-1;j<=graph_position.second;j++)
				{
					if(i!=graph_position.first&&j!=graph_position.second)
					{
						Sprite *pic = *attack_iter;
						addChild(pic,2);
						pic->setPosition(i*48+24,j*48+24);
					}
				}
			}
	}
	else                                                       //不在边界区域
	{
		for(int i = 0;i<8;i++)											//载入图片
		{
			Sprite *pic = Sprite::create("attack.png");
			attack_area.push_back(pic);
		}
		auto iter = attack_area.begin();
		for(auto i = graph_position.first-1;i<=graph_position.first+1;i++)
		{
			for(auto j = graph_position.second-bg_y-1;j<=graph_position.second-bg_y+1;j++)
			{
				if((j==graph_position.second-bg_y)&&graph_position.first==i);
				else
				{
					(*iter)->setPosition(i*48+24,j*48+24);
					addChild(*iter,2);
					
					iter++;
				}
			}
		}
	}
}
bool MapChapterOne::isSide(PAIR position)
{
	if(position.first==0||position.second==0||position.first==19||position.second==19)
	{
		return true;
	}
	else return false;
}
bool MapChapterOne::judgeInAttackArea()
{
	Point position = now_actor->getPositionFootMan();
	PAIR graph_position = PositionToGraph(position);				//获得人物格子
	std::vector<PAIR> attack_list = getAttackGraph();
	for(auto iter:attack_list)
	{
		if(graph[graph_position.first+iter.first][graph_position.second+iter.second].footman_on==3)	
			//敌军
		{
			return true;
		}
		
	}
	return false;

	
}
std::vector<PAIR> MapChapterOne::getAttackGraph()
{
	std::vector<PAIR> return_list;
	Point position = now_actor->getPositionFootMan();
	FootManClass type = now_actor->getType();
	PAIR graph_position = PositionToGraph(position);
	if(0<=type<=6)
	{
		return_list.push_back(std::make_pair(-1,0));
		return_list.push_back(std::make_pair(1,0));
		return_list.push_back(std::make_pair(0,1));
		return_list.push_back(std::make_pair(0,-1));
	}
	else if(7<=type<=17)
	{
		return_list.push_back(std::make_pair(-1,-1));
		return_list.push_back(std::make_pair(-1,0));
		return_list.push_back(std::make_pair(-1,1));
		return_list.push_back(std::make_pair(0,-1));
	
		return_list.push_back(std::make_pair(0,1));
		return_list.push_back(std::make_pair(1,-1));
		return_list.push_back(std::make_pair(1,0));
		return_list.push_back(std::make_pair(1,1));
	}
	else if(18<=type<=20)
	{
		
		return_list.push_back(std::make_pair(-2,0));
		return_list.push_back(std::make_pair(-1,1));
		return_list.push_back(std::make_pair(2,0));
		return_list.push_back(std::make_pair(1,1));
		return_list.push_back(std::make_pair(0,-2));
		return_list.push_back(std::make_pair(-1,-1));
		return_list.push_back(std::make_pair(0,2));
		return_list.push_back(std::make_pair(1,-1));
		if(20==type)
		{
			return_list.push_back(std::make_pair(-3,0));
			return_list.push_back(std::make_pair(3,0));
			return_list.push_back(std::make_pair(0,3));
			return_list.push_back(std::make_pair(0,-3));
			return_list.push_back(std::make_pair(-2,1));
			return_list.push_back(std::make_pair(-2,-1));
			return_list.push_back(std::make_pair(2,1));
			return_list.push_back(std::make_pair(2,-1));
			return_list.push_back(std::make_pair(1,-2));

			return_list.push_back(std::make_pair(1,2));
			return_list.push_back(std::make_pair(-1,2));
			return_list.push_back(std::make_pair(-1,-2));
			
			
		}
		
	}
	else if(21<=type<=23)
	{
			return_list.push_back(std::make_pair(-2,0));
			return_list.push_back(std::make_pair(-1,1));
			return_list.push_back(std::make_pair(2,0));
			return_list.push_back(std::make_pair(1,1));
			return_list.push_back(std::make_pair(0,-2));
			return_list.push_back(std::make_pair(-1,-1));
			return_list.push_back(std::make_pair(0,2));
			return_list.push_back(std::make_pair(1,-1));
			return_list.push_back(std::make_pair(-3,0));
			return_list.push_back(std::make_pair(3,0));
			return_list.push_back(std::make_pair(0,3));
			return_list.push_back(std::make_pair(0,-3));
			return_list.push_back(std::make_pair(-2,1));
			return_list.push_back(std::make_pair(-2,-1));
			return_list.push_back(std::make_pair(2,1));
			return_list.push_back(std::make_pair(2,-1));
			return_list.push_back(std::make_pair(1,-2));

			return_list.push_back(std::make_pair(1,2));
			return_list.push_back(std::make_pair(-1,2));
			return_list.push_back(std::make_pair(-1,-2));
			if(23==type)
			{
				return_list.push_back(std::make_pair(-4,0));
				return_list.push_back(std::make_pair(4,0));
				return_list.push_back(std::make_pair(0,-4));
				return_list.push_back(std::make_pair(0,4));
				return_list.push_back(std::make_pair(-3,1));
				return_list.push_back(std::make_pair(-3,-1));
				return_list.push_back(std::make_pair(-2,2));
				return_list.push_back(std::make_pair(-2,-2));
				return_list.push_back(std::make_pair(-1,-3));
				return_list.push_back(std::make_pair(-1,3));
				
				return_list.push_back(std::make_pair(1,3));
				return_list.push_back(std::make_pair(1,-3));
				return_list.push_back(std::make_pair(2,-2));
				return_list.push_back(std::make_pair(2,2));
				return_list.push_back(std::make_pair(3,1));
				return_list.push_back(std::make_pair(3,-1));
			}
	}
	else if(24<=type<=26)
	{
			return_list.push_back(std::make_pair(-4,0));
			return_list.push_back(std::make_pair(-3,1));
			return_list.push_back(std::make_pair(-3,-1));
			return_list.push_back(std::make_pair(-2,-2));
			return_list.push_back(std::make_pair(-2,2));
			return_list.push_back(std::make_pair(-1,3));
			return_list.push_back(std::make_pair(-1,-3));
			return_list.push_back(std::make_pair(0,-4));
			return_list.push_back(std::make_pair(0,4));
			return_list.push_back(std::make_pair(1,-3));
			return_list.push_back(std::make_pair(1,3));
			return_list.push_back(std::make_pair(2,2));
			return_list.push_back(std::make_pair(2,-2));
			return_list.push_back(std::make_pair(3,1));
			return_list.push_back(std::make_pair(3,-1));
			return_list.push_back(std::make_pair(4,0));
			if(25==type||26==type)
			{
				return_list.push_back(std::make_pair(-5,0));
				return_list.push_back(std::make_pair(-4,-1));
				return_list.push_back(std::make_pair(-4,1));
				return_list.push_back(std::make_pair(-3,2));
				return_list.push_back(std::make_pair(-3,-2));
				return_list.push_back(std::make_pair(-2,-3));
				return_list.push_back(std::make_pair(-2,3));
				return_list.push_back(std::make_pair(-1,4));
				return_list.push_back(std::make_pair(-1,-4));
				return_list.push_back(std::make_pair(0,5));
				return_list.push_back(std::make_pair(0,-5));
				return_list.push_back(std::make_pair(1,4));
				return_list.push_back(std::make_pair(1,-4));
				return_list.push_back(std::make_pair(2,-3));
				return_list.push_back(std::make_pair(2,3));
				return_list.push_back(std::make_pair(3,2));
				return_list.push_back(std::make_pair(3,-2));
				return_list.push_back(std::make_pair(4,1));
				return_list.push_back(std::make_pair(4,-1));
				return_list.push_back(std::make_pair(5,0));
			
			}
	}
	return return_list;
}
void MapChapterOne::AttackPicRelease()
{
	for(auto iter:attack_area)
	{
		removeChild(iter);
	}
	attack_area.clear();							//每一次攻击动作完成，都会使攻击区域为空
}
FootMan* MapChapterOne::getAttackActor()
{
	return NULL;
}