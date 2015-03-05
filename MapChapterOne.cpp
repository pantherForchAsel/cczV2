#include "MapChapterOne.h"
#include "FootMan.h"
#include <vector>
#include <strstream>
#include "cocos2d.h"
USING_NS_CC;
FootMan *now_actor = NULL;
FootMan *now_PC_actor = NULL;
FootMan *now_enemy_actor = NULL;
Sprite *NoteLabel;								//��Ϣ��ʾ��ǩ
bool NoteLabeling = false;						//��Ϣ�Ƿ���ʾ

bool isrunning = false;							//�Ƿ�����ɶ�������ʱ�ĵ��ȫ������
bool running_complete = false;					//���ﶯ���������������Ƿ���Լ�������Ͳ˵��ĳ���
bool menu_appear = false;						//���Ʋ˵��ĳ��ֺ��Ƿ��ڵ���д���˵��¼�
bool choosing_enemy = false;					//Ϊtrue��ʾ����ѡ�񹥻�����

float time_elapse = 0;							//��ʱ��,�����ʾ����ʱ��

Stage running_stage;

Point original_position = Vec2(0,0);							//��¼�����ʼλ��
std::vector<FootMan*> footman_array;		//��ͼ���Ҿ�����洢
std::vector<FootMan*> enemy_friend_array;			//��ͼ�ϵо����Ѿ��洢
std::vector<Sprite *> light_area;	//��������ͼƬ����
std::vector<Sprite *> attack_area;   //����ͼƬ����
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
	
	running_stage = STANDING;																	//��ʼ״̬
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
	footman_array.push_back(footman1);				//�������е�����ָ��
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
							//��ȡÿ�������λ��
		
							Point position = (*iter)->getPositionFootMan();
							if(location.x>position.x-48&&location.x<position.x+48&&location.y>position.y-48&&location.y<position.y+48)
							{
								
								(*iter)->setClicked(true);
								lightOnMap(position,*iter);				//���ø������Լ������־
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
						if(x>position.x-44&&y>position.y-61&&y<position.y+61&&x<position.x+44)	//�ڲ˵�������
						{
							if(x>position.x-44&&x<position.x+44&&y<position.y+61&&y>=position.y+38)		//����
							{
								if(judgeInAttackArea())						//������Χ������
								{
									
									showAttackArea();								//��ʾ������Χ
									running_stage = CHOOSING_ENEMY;						//���빥���׶�
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
							if(y<position.y+38&&y>position.y+15)						//����
							{

							}
							if(y<position.y+15&&y>position.y-7)						//����
							{

							}
							if(y<position.y-7&&y>position.y-30)						//����
							{

							}
							if(y<position.y-30&&y>position.y-61)						//ȡ��
							{
								returnToOrigin();
							}
						}
						
		}
		break;
	case LIGHTING:
		{
						
						auto iter = footman_array.begin();						//�����б������
						Point location = touch->getLocation();					//��õ��λ��
						while(iter!=footman_array.end())
						{
							if((*iter)->judgeclick()==true)						//�ҵ����������
							{
								temporary = *iter;								//��ø�����
								now_actor = temporary;
							}
							iter++;
						}
						Point position = temporary->getPositionFootMan();
						if(judgeLightOnSquare(position,temporary->getMoveAblity(),location))								//�ڸ���������
						{
							std::string path = graphToPath(position,location);

							const char *pathTran = path.c_str();					//ת��const char *��char*
							char *copy = new char[strlen(pathTran)+1];
							strcpy(copy,pathTran);
							temporary->setPosition(temporary->getPositionFootMan());		//��δ���ֻҪ��setAnimation���棬
							//�ͻᵼ�����ڶ���ִ��ǰִ��
							
							running_stage = RUNNING;
							temporary->setState(STATE_WALK);
							temporary->setAnimation(copy);
				
				
							//Sleep(1000);
							temporary->stopAnimation();
				
			
						}
		
		
							for(auto iter = light_area.begin();iter!=light_area.end();iter++)		//������������
								//�����������������򻺴�
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
			for(auto iter = array_pair.begin();iter!=array_pair.end();iter++)			//�ж��Ƿ��ڹ���������
			{
				if((actor_position.first+(*iter).first==graph_position.first&&
					actor_position.second+(*iter).second==graph_position.second)&&(graph[graph_position.first][graph_position.second].footman_on==3))				//ͨ����õ�������ڹ������ӵ������ڣ����ҵ�������ео�
					//�ڹ�����Χ��
				{
					//�ж����򣬵������﹥������
					if((*iter).first<0)
						 face_to = LEFT;
					else if((*iter).first>0)
						 face_to = RIGHT;
					else if((*iter).first==0&&(*iter).second>0)
						 face_to = UP;
					else
						 face_to = DOWN;
					AttackPicRelease();											//��������ͼƬ
					running_stage = ATTACKING;
					now_actor->AttackAnimation(face_to);			//����ִ�У���������isrunningΪtrue
					

					//���ñ���������
					for(auto iter:enemy_friend_array)
					{
						int attack_position_x = (iter->getPositionFootMan()).x;
						int attack_position_y = (iter->getPositionFootMan()).y;
						PAIR attack_position = PositionToGraph(iter->getPositionFootMan());
						if((attack_position.first==graph_position.first)&&(attack_position.second==graph_position.second))				//�����λ�ú�ĳ���о��ĸ���һ�£��ҵ��������о�
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
	if(isrunning==false)													//��ʾû�ж�������
	{
		if(true == choosing_enemy)												//û�ж�������ʾ�ǹ����׶�
		{
			Actor_face_to face_to;
			x = touch->getLocation().x;
			y = touch->getLocation().y;
			PAIR graph_position = PositionToGraph(Point(x,y));
			PAIR actor_position = PositionToGraph(now_actor->getPositionFootMan());
			std::vector<PAIR> array_pair = getAttackGraph();
			for(auto iter = array_pair.begin();iter!=array_pair.end();iter++)			//�ж��Ƿ��ڹ���������
			{
				if(actor_position.first+(*iter).first==graph_position.first&&
					actor_position.second+(*iter).second==graph_position.second)
					//�ڹ�����Χ��
				{
					//�ж����򣬵������﹥������
					if((*iter).first<0)
						 face_to = LEFT;
					else if((*iter).first>0)
						 face_to = RIGHT;
					else if((*iter).first==0&&(*iter).second>0)
						 face_to = UP;
					else
						 face_to = DOWN;
					AttackPicRelease();											//��������ͼƬ
					isrunning = true;
					now_actor->AttackAnimation(face_to);			//����ִ�У���������isrunningΪtrue
					choosing_enemy = false;								//�׶����
				}
			}
		}
		else                                                                 //�ǹ����׶Σ����ﲻ�ƶ�
		{
										//����û���ƶ���������δ�ƶ���ȷ���ƶ�λ�ã��˵����				  
					FootMan *temporary;
					x = touch->getLocation().x;
					y = touch->getLocation().y;
					if(false == menu_appear)						//�˵�δ���֣�ʲô������
					{
					}	
					else										//�˵����֣�����û�ƶ�
					{
						Point position = menu->getPosition();
						if(x>position.x-44&&y>position.y-61&&y<position.y+61&&x<position.x+44)	//�ڲ˵�������
						{
							if(x>position.x-44&&x<position.x+44&&y<position.y+61&&y>=position.y+36)		//����
							{
								if(judgeInAttackArea())						//������Χ������
								{
									choosing_enemy = true;
									showAttackArea();								//��ʾ������Χ
									
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
							if(y<position.y+36&&y>position.y+11)						//����
							{

							}
							if(y<position.y+11&&y>position.y-14)						//����
							{

							}
							if(y<position.y-14&&y>position.y-39)						//����
							{

							}
							if(y<position.y-39&&y>position.y-61)						//ȡ��
							{
								returnToOrigin();
							}
						}
						menu_appear = false;
					}



					if(islight_on==false)				//δ����
					{
						//ѭ������vector��Ȼ���ÿ�������ж����Ƿ񱻵����
						auto iter = footman_array.begin();
						Point location = touch->getLocation();
						while(iter!=footman_array.end())
						{
							//��ȡÿ�������λ��
		
							Point position = (*iter)->getPositionFootMan();
							if(location.x>position.x-48&&location.x<position.x+48&&location.y>position.y-48&&location.y<position.y+48)
							{
								if((*iter)->getState()!=STATE_WALK)
								{
								(*iter)->setClicked(true);
								lightOnMap(position,*iter);				//���ø������Լ������־
								islight_on = true;
								}
								else
								{
								}
			
							}
							iter++;
						}
					}
					else                                            //�Ѹ�������ȡ���λ���ƶ�
					{
						auto iter = footman_array.begin();						//�����б������
						Point location = touch->getLocation();					//��õ��λ��
						while(iter!=footman_array.end())
						{
							if((*iter)->judgeclick()==true)						//�ҵ����������
							{
								temporary = *iter;								//��ø�����
								now_actor = temporary;
							}
							iter++;
						}
						Point position = temporary->getPositionFootMan();
						if(judgeLightOnSquare(position,temporary->getMoveAblity(),location))								//�ڸ���������
						{
							std::string path = graphToPath(position,location);

							const char *pathTran = path.c_str();					//ת��const char *��char*
							char *copy = new char[strlen(pathTran)+1];
							strcpy(copy,pathTran);
							temporary->setPosition(temporary->getPositionFootMan());		//��δ���ֻҪ��setAnimation���棬
							//�ͻᵼ�����ڶ���ִ��ǰִ��
							isrunning = true;
				
							temporary->setState(STATE_WALK);
							temporary->setAnimation(copy);
				
				
							//Sleep(1000);
							temporary->stopAnimation();
				
			
						}
		
		
							for(auto iter = light_area.begin();iter!=light_area.end();iter++)		//������������
								//�����������������򻺴�
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
void MapChapterOne::touchMoved(Touch*touch,Event *event)			//��ͼ��������
{
	/**/
}
void MapChapterOne::touchEnded(Touch *touch,Event*event)
{
	if(running_stage==STANDING)						//�ƶ��в����ƶ���Ļ
	{
	


		auto children = this->getChildren();
		auto location  = touch->getLocation();
		float x_move = location.x-x;
		float y_move = location.y-y;
		if(x_move>0&&y_move>0)								//����
		{
				//ѭ������������position��Աλ���½�
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
		
		else if(y_move>0)							//�����ƶ�
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
void MapChapterOne::lightOnMap(Point per_position,FootMan*target)		//�ڼ���������л�ͼ
{
	assert(target->judgeclick());	//Ŀ�����ﱻ���
	now_actor = target;
	auto iter = light_area.begin();										//ѭ��vector��ӽ�ͼƬ����
	for(int i = 0;i<countWholeLightOn(target->getMoveAblity());i++)			//���������ƶ�����ӻ���
	{
		Sprite *lighter = Sprite::create("small/trans.png");
		light_area.push_back(lighter);								//�����Ӧ�ƶ�����ͼƬ
		
	}
	iter = light_area.begin();
	int ability = target->getMoveAblity();
	Point position  = target->getPositionFootMan();
		for(int j = 0;j<ability+1;j++)										//�������룬�����
		{
			for(int k = 0;k<2*j+1;k++)										//ÿһ�д�ֱ������
			{
				Sprite *pic = *iter;
				addChild(pic,1);
				pic->setPosition(position.x-(ability-j)*48,position.y+j*48-k*48);
				iter++;
			}
		}
		for(int j = ability+1;j<ability*2+1;j++)	//t=7,Ӧ����11�����ӣ��Ҳ���
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
	if(actor_graph_x+actor_graph_y-click_graph_x-click_graph_y<=ability||click_graph_x+click_graph_y-actor_graph_x-actor_graph_y<=ability)			//�ж����ƶ�������
	{
		return true;
	}
	else
		return false;
}
PAIR MapChapterOne::PositionToGraph(Point position)
{
	auto bg = this->getChildByTag(0);			//��ñ���ͼƬ
	auto position_1 = bg->getPosition();			//��ñ���ͼƬê�����½�

	int x = position_1.x;
	int y = position_1.y;

	//������ͼƬê��(0,-96),����λ�ã�100,15�������Ӿ��ǣ�2,2��
	int x_graph = (position.x-x)/48;
	int y_graph = (position.y-y)/48;								//�����Ǵ�0��ʼ��
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
	PAIR graph_position = PositionToGraph(position);			//���������ӣ������ǣ�6,6����������ʾ����
	//�ǣ�5,5����Ҫ���ݱ���ͼƬ�ı仯����λ��

	auto bgposition = this->getChildByTag(0)->getPosition();		//��ñ���ͼƬλ��
	//int bg_x = bgposition.x/48;
	int bg_y = bgposition.y/48;
	//��ñ���ͼƬ y�����ƶ�����
	if(bg_y<0)bg_y = -bg_y;
	auto attack_iter = attack_area.begin();
	if(isSide(graph_position))									//�ڱ߽�����
	{
		if(graph_position.first==0&&graph_position.second-bg_y==0)			//�����½���
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
		else if(graph_position.first==0&&graph_position.second==19)		//�����Ͻ�
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
		else if(graph_position.first==19&&graph_position.second==0)		//���½�
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
		else if(graph_position.first==19&&graph_position.second==19)		//���Ͻ�
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
		else if(graph_position.first==0)							//��߽�
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
		else if(graph_position.second==0)							//�±߽�
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
		else if(graph_position.first==19)							//�ұ߽�
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
		else if(graph_position.second == 19)						//�ϱ߽�	
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
	else                                                       //���ڱ߽�����
	{
		for(int i = 0;i<8;i++)											//����ͼƬ
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
	PAIR graph_position = PositionToGraph(position);				//����������
	std::vector<PAIR> attack_list = getAttackGraph();
	for(auto iter:attack_list)
	{
		if(graph[graph_position.first+iter.first][graph_position.second+iter.second].footman_on==3)	
			//�о�
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
	attack_area.clear();							//ÿһ�ι���������ɣ�����ʹ��������Ϊ��
}
FootMan* MapChapterOne::getAttackActor()
{
	return NULL;
}