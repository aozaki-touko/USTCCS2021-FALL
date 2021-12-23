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
	int ntype;//ntype是-1的时候为到达，其他按存钱和取钱的去分配。
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
	int duration;//这个客户所需时间
	int intertime;//下个客户到达时间
	int amount;//钱数
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
