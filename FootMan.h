#ifndef _FOOTMAN_H
#define _FOOTMAN_H


#include "cocos2d.h"

USING_NS_CC;			//��֣���������ᱨ���������#include "cocos2d.h"ǰҲ�ᱨ��

enum Actor_face_to				//��ʾ�������Է���

{
	UP,DOWN,LEFT,RIGHT
};
enum State_actor
{
	STATE_WALK,STATE_STAND
};//�����ƶ������ﾲֹ
enum FootManClass						//��ö���������ڱ�ʾ��������
{
	//ʮ�ֹ�������,��������������ȭʦ������ң�ȭʥ��ѵ��ʦ��ѵ��ʦ��
	QQB,ZQB,QS,WDJ,QuanS,XHS,XXS,
	//3X3�������֣��Ჽ�����ز�����������������������������������������������
	
	//�ƽ����������������
	QBB,ZBB,JWJ,JWQB,XLQB,ZB,YZ,HJ,HJJ,HD,DD,
	//����,��������������
	MGB,NQB,LNQS,
	//����������������
	GB,NB,LNB,
	//�h�����سh����������
	PC,ZPC,PLC
};
class FootMan :public cocos2d::Node
{
public :
	FootMan():move_ability(5){};						//Ĭ���ƶ���5
	~FootMan();
	bool createFootMan(char *file_name,char *attack_path,char *spec_path,int x,int y,int identity);		//��������
	CREATE_FUNC(FootMan);
	bool setAnimation(char*path);						//���ö���,xΪˮƽ�ƶ���yΪ��ֱ�ƶ�
	void stopAnimation();								//ֹͣ����
	bool judgeclick();									//�ж��Ƿ���
	void changeSprite();
	
	Point getPositionFootMan();							//�������λ�ú���

	virtual void AttackAnimation(Actor_face_to orientation);								//ִ�й�������
	
	void setClicked(bool clicked);						//��ʾһ�����ﱻ����Ѵ�������
	void setMoveAbility(int ability);					//�����ƶ�����
	
	int getMoveAblity();								//����ƶ�����
	void setState(State_actor state);									//��������״̬
	State_actor getState();								//�������״̬
	FootManClass getType();								//��ñ�������
	void changeAttackAction();								//���ڹ��������ļ���

	void refreshCG();									//position�䶯֮��ʹ��cg�ƶ�

	void setPositionFootMan(Vec2 position);

	void strackbackAnimation(Actor_face_to orientation);								//��������

	void damageAnimation(Actor_face_to orientation);
protected:
	char *filename;									//ÿһ��������ص�ͼƬ
	char *attackPath;									//����ͼƬ·��
	char *movPath;										//�ƶ�ͼƬ·��
	char *specPath;										//����ͼƬ·��
	Point position;										//����λ��
	Sprite *cg;											//cg�滻
	
	bool isclicked;										//�ж������Ƿ񱻵��
	int move_ability;									//�����ƶ���
	Sprite *cg1;										//�滻cg
	State_actor _state;									//����״̬
	FootManClass classLabel;							//����
	//--------------------�����������----------------//
	int identity;										//�Ƿ�Ϊ�Ѿ�,0Ϊ�о���1Ϊ�Ѿ���2Ϊ�ɿ��Ʊ���
	int hp;												//Ѫ��
	int mp;												//ħ��ֵ


};
class Path							//��������·������
{
public :
	Path(char _arrow,int _step);

	int step;
	char arrow;
};
class Offset						//�������ڴ�����ƶ���Χ
{
public :
		int x_offset;
		int y_offset;
	
};
#endif