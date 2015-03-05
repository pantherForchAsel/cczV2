#include "FootMan.h"
#include <vector>
#include "MapChapterOne.h"
USING_NS_CC;
extern Stage running_stage;
extern bool isrunning;
extern bool running_complete;
extern bool menu_appear;
FootMan::~FootMan()
{
}
Path::Path(char _arrow,int _step)
{
	arrow =_arrow;
	step = _step;
}
bool FootMan::createFootMan(char *file_name,char *attack_path,char *spec_path,int x,int y,int identity)
{
	filename = file_name;
	attackPath = attack_path;
	movPath = file_name;
	specPath = spec_path;
	cg = Sprite::create(filename,Rect(0,288,48,48));
	position = Point(x,y);
	this->addChild(cg,1,4);																		//cg��1��,tagΪ4
	this->setPosition(position);
	this->setState(STATE_STAND);
	this->identity = identity;
	isclicked = false;
	
	
	if(cg!= NULL)
		return true;
	else return false;
}
/*void FootMan::changeSprite()
{
	if(isrunning==true)
	isrunning = false;
	Point position = this->getPositionFootMan();
	running_complete = true;
	menu_appear = true;
	//this->AttackAnimation(static_cast<Actor_face_to>(2));
	
	
}*/
void FootMan::changeSprite()
{
	if(running_stage == RUNNING)
		running_stage = MENUING;
}
bool FootMan::setAnimation(char *path)				//�ú������ݴ���Ĳ������ж��ߵķ���ִ�еĶ���,�ߵķ���
{	
	
	if(getChildByTag(4)==NULL)
	this->removeChild(cg1);
	else removeChild(cg);
	std::vector<Path> path_element;					//����·��˳��
	int step = 0;
	int count_for_once = 0;						//�洢һ��element��Ҫ�ƶ���λ�ã���"X:3"��Ϊ3��"Y=13"��Ϊ4
	char *element = &path[0];
	bool flag = false;							//�Ƿ�Ϊ����
	assert(path[0]=='X'||path[0]=='Y');
	while(*element !='\0')
	{
		if(*element == 'X')
		{
			if(*(element+1)==':')
			{
				char *p = element+2;					//���X:֮��ĵ�ַ
				while(isdigit(*p)||*p=='-')
				{
					if(*p!='-')
					step = atoi(p)+10*step;
					else flag = true;					//Ϊ����				
					 p++;
					count_for_once++;
				}
				//if(isdigit(step))						//���벽��
				//{
				if(flag==false)
				{
				Path temporary_path('X',step);
				path_element.push_back(temporary_path);
				}
				else 
				{
				Path temporary_path('X',-step);
				path_element.push_back(temporary_path);
				}
				
				//}
			}
			element+=(2+count_for_once);				//element�ƶ�����һ���ƶ���Ϣ��ȡλ��

			count_for_once = 0;
		}
		else if(*element=='Y')
		{
			if(*(element+1)==':')
			{
				char *p = element+2;
				while(isdigit(*p)||*p=='-')
				{
					if(*p!='-')
					step = atoi(p)+10*step;
					else flag = true;
					p++;
					count_for_once++;
				}
				//if(isdigit(step))
				//{
				if(flag==false)
				{
				Path temporary_path('Y',step);
				path_element.push_back(temporary_path);
				}
				else 
				{
				Path temporary_path('Y',-step);
				path_element.push_back(temporary_path);
				}
				//}
				element+=(2+count_for_once);

				count_for_once = 0;
			}
		}
		step = 0;
		flag = false;
	}

	cocos2d::Sequence *sequence = NULL;
	cocos2d::Spawn *spawn = NULL;
	auto iter = path_element.begin();
	while(iter!=path_element.end())			//ѭ����������
	{
		Path temporary = *iter;
		if(temporary.arrow=='X')
		{
			if(temporary.step>0)				//������
			{
				auto right = MoveBy::create(iter->step,Point(iter->step*48,0));
				position.x+=48*iter->step;
				Animation *animation_right = Animation::create();
				animation_right->addSpriteFrameWithFile("small/1-1-13.png");
				animation_right->addSpriteFrameWithFile("small/1-1-14.png");
				animation_right->setDelayPerUnit(2.8f/14.0f);							//2.8���14֡
				animation_right->setRestoreOriginalFrame(true);

				auto action = Animate::create(animation_right);
				auto Repeat_right = Repeat::create(action,2.5*temporary.step);

				if(spawn == NULL)
				{
					spawn = Spawn::create(right,Repeat_right,NULL);
					sequence = Sequence::create(spawn,NULL);
				}
				else 
					sequence= Sequence::create(sequence,Spawn::create(right,Repeat_right,NULL),NULL);
			}
			else if(temporary.step<0)			//������
			{
				auto left = MoveBy::create((-iter->step),Point(iter->step*(48),0));
				position.x+=iter->step*48;
				Animation *animation_left = Animation::create();
				animation_left->addSpriteFrameWithFile("small/1-1-5.png");
				animation_left->addSpriteFrameWithFile("small/1-1-6.png");
				animation_left->setDelayPerUnit(2.8f/14.0f);
				animation_left->setRestoreOriginalFrame(true);

				auto action = Animate::create(animation_left);
				auto Repeat_left = Repeat::create(action,-2.5*temporary.step);			//Ϊ������һֱ��

				if(spawn==NULL)
				{
					spawn = Spawn::create(left,Repeat_left,NULL);
					sequence = Sequence::create(spawn,NULL);
				}
				else
					sequence = Sequence::create(sequence,Spawn::create(left,Repeat_left,NULL),NULL);

			}
			else
			{
				
			}
			
		}
		else if(temporary.arrow =='Y')
		{
			if(temporary.step>0)				//������
			{
				auto up = MoveBy::create(iter->step,Point(0,iter->step*48));
				position.y+=iter->step*48;
				Animation *animation_up = Animation::create();
				animation_up->addSpriteFrameWithFile("small/1-1-3.png");
				animation_up->addSpriteFrameWithFile("small/1-1-4.png");
				animation_up->setDelayPerUnit(2.8f/14.0f);
				animation_up->setRestoreOriginalFrame(true);

				auto action = Animate::create(animation_up);
				auto Repeat_up = Repeat::create(action,2.5*temporary.step);

				if(spawn==NULL)
				{
					spawn = Spawn::create(up,Repeat_up,NULL);
					sequence = Sequence::create(spawn,NULL);
				}
				else
					sequence = Sequence::create(sequence,Spawn::create(up,Repeat_up,NULL),NULL);

			}
			else if(temporary.step<0)			//������
			{
				auto down = MoveBy::create((-iter->step),Point(0,iter->step*(48)));
				position.y+=iter->step*48;
				Animation *animation_down = Animation::create();
				animation_down->addSpriteFrameWithFile("small/1-1-1.png");
				animation_down->addSpriteFrameWithFile("small/1-1-2.png");
				animation_down->setDelayPerUnit(2.8f/14.0f);
				animation_down->setRestoreOriginalFrame(true);

				auto action = Animate::create(animation_down);
				auto Repeat_down = Repeat::create(action,-2.5*temporary.step);

				if(spawn==NULL)
				{
					spawn = Spawn::create(down,Repeat_down,NULL);
					sequence = Sequence::create(spawn,NULL);
				}
				else
					sequence = Sequence::create(sequence,Spawn::create(down,Repeat_down,NULL),NULL);
			}
			else
			{
				
			}
		}
		iter++;
	}
	if(this->getChildByTag(8)==NULL)				//cg1Ŀǰû����
	{
	cg1 = Sprite::create(filename,Rect(0,288,48,48));
	addChild(cg1,1,8);
	if(sequence!=NULL)
	{
		sequence = Sequence::create(sequence,CallFunc::create(CC_CALLBACK_0(
			FootMan::changeSprite,this)),NULL);
		cg1->runAction(sequence);
		return true;
	}
	else if(spawn!=NULL)
	{
		spawn = Spawn::create(sequence,CallFunc::create(CC_CALLBACK_0(FootMan::changeSprite,this)),NULL);
		cg1->runAction(spawn);
		return true;
	}
	else changeSprite();
	}
	else
	{
		cg = Sprite::create(filename,Rect(0,288,48,48));
		addChild(cg,1,4);
	if(sequence!=NULL)
	{
		sequence = Sequence::create(sequence,CallFunc::create(CC_CALLBACK_0(FootMan::changeSprite,this)),NULL);
		cg->runAction(sequence);
		return true;
	}
	else if(spawn!=NULL)
	{
		spawn = Spawn::create(spawn,CallFunc::create(CC_CALLBACK_0(FootMan::changeSprite,this)),NULL);
		cg->runAction(spawn);
		return true;
	}
	else changeSprite();
	}
	this->setState(STATE_STAND);
}

/*void FootMan::onEnter()
{
	Node::onEnter();															//������ø����onEnter��������Ȼ������붯��ִ��
	auto listener = EventListenerTouchOneByOne::create();						//���ü���
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(FootMan::onTouchBegan,this);			//���ü�������
	listener->onTouchMoved = CC_CALLBACK_2(FootMan::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(FootMan::onTouchEnded,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}
void FootMan::onExit()
{
	_eventDispatcher->removeAllEventListeners();
	Node::onExit();																	//ͬ��
}*/



void FootMan::stopAnimation()							//������״̬�ı�
{
	
	this->setState(STATE_STAND);

}
bool FootMan::judgeclick()
{
	return isclicked;
}

void FootMan::setClicked(bool clicked)
{
	this->isclicked = clicked;
}
int FootMan::getMoveAblity()
{
	return move_ability;
}

Point FootMan::getPositionFootMan()
{
	return position;
}
void FootMan::setMoveAbility(int ability)
{
	move_ability = ability;
}

void FootMan::setState(State_actor state)
{
	_state = state;
}

State_actor FootMan::getState()
{
	return _state;
}
void FootMan::AttackAnimation(Actor_face_to orientation)
{
	Animation *attack;
	Texture2D *pic = TextureCache::getInstance()->addImage(attackPath);
	if(orientation == UP)
	{
		 
		SpriteFrame *frame1 = SpriteFrame::createWithTexture(pic,CCRectMake(0,448,64,64));
		SpriteFrame *frame2 = SpriteFrame::createWithTexture(pic,CCRectMake(0,384,64,64));
		SpriteFrame *frame3 = SpriteFrame::createWithTexture(pic,CCRectMake(0,320,64,64));
		SpriteFrame *frame4 = SpriteFrame::createWithTexture(pic,CCRectMake(0,256,64,64));
		
		Vector<SpriteFrame*> arr;
		arr.pushBack(frame4);
		arr.pushBack(frame3);
		arr.pushBack(frame2);
		arr.pushBack(frame1);
		attack = Animation::createWithSpriteFrames(arr,0.2f);
		
	}
	if(orientation == DOWN)
	{
		SpriteFrame *frame1 = SpriteFrame::createWithTexture(pic,CCRectMake(0,192,64,64));
		SpriteFrame *frame2 = SpriteFrame::createWithTexture(pic,CCRectMake(0,128,64,64));
		SpriteFrame *frame3 = SpriteFrame::createWithTexture(pic,CCRectMake(0,64,64,64));
		SpriteFrame *frame4 = SpriteFrame::createWithTexture(pic,CCRectMake(0,0,64,64));

		Vector<SpriteFrame*> arr;
		arr.pushBack(frame4);
		arr.pushBack(frame3);
		arr.pushBack(frame2);
		arr.pushBack(frame1);
		attack = Animation::createWithSpriteFrames(arr,0.2f);
		

		
	}
	if(orientation == LEFT)
	{
		SpriteFrame *frame1 = SpriteFrame::createWithTexture(pic,CCRectMake(0,704,64,64));
		SpriteFrame *frame2 = SpriteFrame::createWithTexture(pic,CCRectMake(0,640,64,64));
		SpriteFrame *frame3 = SpriteFrame::createWithTexture(pic,CCRectMake(0,576,64,64));
		SpriteFrame *frame4 = SpriteFrame::createWithTexture(pic,CCRectMake(0,512,64,64));
		
		Vector<SpriteFrame*> arr;
		arr.pushBack(frame4);
		arr.pushBack(frame3);
		arr.pushBack(frame2);
		arr.pushBack(frame1);
		attack = Animation::createWithSpriteFrames(arr,0.2f);
		

		
	}
	if(orientation == RIGHT)
	{
		SpriteFrame *frame1 = SpriteFrame::createWithTexture(pic,CCRectMake(0,704,64,64));
		SpriteFrame *frame2 = SpriteFrame::createWithTexture(pic,CCRectMake(0,640,64,64));
		SpriteFrame *frame3 = SpriteFrame::createWithTexture(pic,CCRectMake(0,576,64,64));
		SpriteFrame *frame4 = SpriteFrame::createWithTexture(pic,CCRectMake(0,512,64,64));
		
		Vector<SpriteFrame*> arr;
		arr.pushBack(frame4);
		arr.pushBack(frame3);
		arr.pushBack(frame2);
		arr.pushBack(frame1);
		attack = Animation::createWithSpriteFrames(arr,0.2f);
		

		
	}
	//attack->setDelayPerUnit(3.0f/4);
	attack->setRestoreOriginalFrame(true);
	auto action = Animate::create(attack);
	auto callback = CallFunc::create(CC_CALLBACK_0(FootMan::changeAttackAction,this));
	if(this->getChildByTag(4)==NULL)
	{
		auto runaction  = Sequence::create(action,callback,NULL);
		if(orientation==RIGHT)
		{
			cg1->setFlippedX(true);
		}
		cg1->runAction(runaction);
	}
	else
	{
		auto runaction = Sequence::create(action,callback,NULL);
		if(orientation==RIGHT)
		{
			cg->setFlippedX(true);
		}
		cg->runAction(runaction);
	}

}
FootManClass FootMan::getType()
{
	return classLabel;
}
void FootMan::changeAttackAction()
{
	if(running_stage==ATTACKING)
		running_stage = STRACK_BACK;										//����������ɺ���뷴���׶�
}
/*
void FootMan::changeAttackAction()
{
	if(isrunning==true)
	isrunning = false;
}*/
void FootMan::setPositionFootMan(Vec2 position)
{
	this->position = position;
	
}
void FootMan::refreshCG()
{
	if(this->getChildByTag(4)!=0)							//cg��Ϊ��
	{
		removeChild(cg);
		cg1 = Sprite::create(filename,Rect(0,288,48,48));
		addChild(cg1,1,8);
	}
	if(this->getChildByTag(8)!=0)							//cg1��Ϊ��
	{
		removeChild(cg1);
		cg = Sprite::create(filename,Rect(0,288,48,48));
		addChild(cg,1,4);
	}
}
void FootMan::strackbackAnimation(Actor_face_to orientation)
{
	Texture2D *pic = TextureCache::getInstance()->addImage(specPath);
	if(UP==orientation)
	{
		SpriteFrame *frame1 = SpriteFrame::createWithTexture(pic,CCRectMake(0,48,48,48));
	}
	else if(DOWN==orientation)
	{
		SpriteFrame *frame2 = SpriteFrame::createWithTexture(pic,CCRectMake(0,0,48,48));
	}
	else if(LEFT==orientation)
	{
		SpriteFrame *frame3 = SpriteFrame::createWithTexture(pic,CCRectMake(0,96,48,48));
	}
	else if(RIGHT==orientation)
	{
		SpriteFrame *frame4 = SpriteFrame::createWithTexture(pic,CCRectMake(0,96,48,48));
	}


}
void FootMan::damageAnimation(Actor_face_to orientation)
{
	Texture2D *pic = TextureCache::getInstance()->addImage(specPath);
}