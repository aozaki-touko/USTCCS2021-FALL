#pragma once
#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
#define maxduration mxdur
#define maxintertime mxint
#define maxmoney mxmon
//store in eventlist
struct event
{
	int occurtime;
	int ntype;//ntype��-1��ʱ��Ϊ�����������Ǯ��ȡǮ��ȥ���䡣
	event* next;
};
//store in queue
struct customer
{

	int arrivaltime;
	int durationtime;
	int amount;
	customer* next;
};
struct littleevent
{
	int occurtime;
	int ntype;
};
struct randomtime
{
	int duration;//����ͻ�����ʱ��
	int intertime;//�¸��ͻ�����ʱ��
	int amount;//Ǯ��
};
void random(randomtime& ran) {
	
	ran.duration = rand() % mxdur;
	ran.intertime = rand() % mxint;
	int temp=rand()%100;
	if (temp<minus_percent*100) {
		ran.amount = (-1) * rand() % mxmon;
	}
	else {
		ran.amount = rand() % mxmon;
	}

}
