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
};//��һغϣ�AI�غ�

enum Stage
{
	RUNNING,					//�ƶ���
	STANDING,					//��ʼ״̬
	LIGHTING,					//����״̬
	MENUING,					//�˵���ʾ״̬
	CHOOSING_ENEMY,             //�о�ѡ��׶�
	ATTACKING,			          //�����׶�
	ENEMY_DAMAGE,						//�˺��׶�
	STRACK_BACK,					//�����׶�
	DAMAGE							//�Ѿ����˽׶�
};
enum graph_type
{
	GRAPH_NULL,						//�յ�
	GRAPH_GRASS,					//�ݵ�
	GRAPH_WATER,					//ˮ��
	GRAPH_MOUNTAIN,					//ɽ
	GRAPH_FORSET,					//ɭ��
	GRAPH_SOFT_GROUND,				//���
	GRAPH_HOUSE,					//��ʾ����
	GRAPH_WALL,						//��ʾ�ǹ�
	GRAPH_CANNOT_MOVE				//���β����ƶ�
};
class square						//������
{
public:
	
	void setSquare(int a,int b,int c,graph_type d)					//��ʼ��������ÿһ�ŵ�ͼ����������Լ���create_graph���������setSquare����ɳ�ʼ��
	{
		l_corner_x = a;
		l_corner_y = b;
		footman_on = c;
		ground_type = d;
	}
	int l_corner_x;					//�ø������½�����
	int l_corner_y;					//�ø������½�����
	int footman_on;					//�ø����Ƿ�������,Ϊ0��ʾû�ˣ�1��ʾ������2��ʾ�Ѿ���3��ʾ�о�
	graph_type ground_type;			//��ʾ�ø��ӵ���

};
class MapChapterOne:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();									//��ʼ������

	CREATE_FUNC(MapChapterOne);
	
	bool virtual touchBegan(Touch*touch,Event*event);			//��������
	void virtual touchMoved(Touch *touch,Event *event);			//�����ƶ�����
	void virtual touchEnded(Touch *touch,Event *event);			//������������
	void virtual touchCancelled(Touch*touch,Event *event);

	void lightOnMap(Point per_position,FootMan*footman);			//��ͼ��������

	int countWholeLightOn(int move_ability);				//�����ƶ����������������Ŀ

	bool judgeLightOnSquare(Point position,int ability,Point click);								//�жϵ���Ƿ��ڸ���������
	//����Ϊ��������������ƶ����͵��λ��

	std::string graphToPath(Point click,Point position);							//������ת����·���ַ���

	PAIR virtual PositionToGraph(Point position);						//ת�����굽����λ��

	void virtual create_graph();										//400�����ӵĳ�ʼ��

	virtual void update(float dt);									//֡ˢ�º���

	bool virtual judgeInAttackArea();						//�ж��Ƿ��ڹ���������
	std::vector<PAIR> getAttackGraph();						//��ù�����Χ����

	void virtual showAttackArea();							//��ʾ��������3X3

	void showAttackAreaSZ();						//��ʾʮ�ֹ�������

	void AttackPicRelease();						//������������

	void returnToOrigin();									//ʹ��ǰ���������ƶ�������ص�ԭʼλ��
	bool isSide(PAIR position);								//�жϵ�ǰ�����Ƿ��Ǳ߽�

	FootMan* getAttackActor();									//��ù�������
private:
	float x;										//��ʾһ�δ���λ��
	float y;				
	Point bgPosition;								//��ʾ���½�λ��
	int x_offset;									//��ʾ����ͼƬ���ƶ��ľ��ο򳤶̣�ÿ�ŵ�ͼ��ͬ
	int y_offset;									

	bool islight_on;								//��ʾ����ͼ���Ƿ���������ƶ����򱻸���
	int width;										//��ͼ��С����ȣ��߶�
	int height;		

	square graph[20][20];								//��ͼ�����࣬ÿһ����4����Ա�����½����꣬���Σ��Ƿ�����

	Point choose_position;							//ÿһ�ζ�������ʼλ��
	Sprite *menu;										//�˵�
	Sprite *noAttackLabel;
};







#endif
