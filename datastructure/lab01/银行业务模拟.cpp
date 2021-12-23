extern int mxdur = 30, mxint = 12, mxmon = 1000;float minus_percent = 0.5;;//mxdur是最大处理时间，mxint是最大间隔时间，mxmon是最大取钱数目,minus是取钱的概率
static int handle_num = 1;//最大队伍数目
#include"structure.h"
#include"queue.h"
#include"eventlist.h"
#include<Windows.h>
#include<WinBase.h>
#define closetime 600
using std::endl;
//--------------------------------------------------
/*模拟器原理
*1.先初始化事件链表，即使用type=-1，occurtime=0的结点插入
*2.程序运行
* 3.先从事件链表取出头结点，进行判断。
* 3.5对于ntype=-1的事件,这表明有新客人来,利用随机数生成是存钱还是取钱，正数是存钱，负数是取钱,customer++,生成一个customer，将其插入到1号窗口。
* 4.1号窗口进行判断，如果存钱，直接结束，如果取钱，钱数小于银行当前有的资金，离队，如果大于银行所有的钱，进入窗口0（专用于处理大额取款）。
* 5.事件链表只储存在窗口>=1的人，不处理在0号窗口的人。
*
* 6.事件链表中提取事件，进行处理
* 每次处理完事件链表中在窗口>=1的人，进行0号窗口的搜索，发现可以满足，则离队，这离队事件的durationtime=上一个的arrivaltime+durationtime-此窗口的人的arrivaltime
*
*/
//允许进行多条队列的排队
static int custom = 0;
static int totaltime = 0;
static int totalmoney = 10000, lastmoney = 10000;
static int successpeople = 0;


void similator() {
	//初始化
	srand((unsigned)time(NULL));
	eventlist evlist;
	customerqueue *handle_queue, waitqueue;//handle处理正在做的，wait处理大额取款
	handle_queue = new customerqueue[handle_num];
	littleevent happen = { 0,-1 }, detect = { 0,0 };//happen是一个中间变量,detect用于提取出的事件,并对其分析
	evlist.inserfun(happen);
	detect = evlist.pop();
	int now;
	randomtime ran;//储存随机数
	//开始模拟
	while (detect.occurtime < closetime) {
		if (detect.ntype == -1) {//入队事件
			//cout << totalmoney<<"\t";
			random(ran);
			happen.ntype =-1, happen.occurtime = ran.intertime + detect.occurtime;
			evlist.inserfun(happen);//下一个来的人
			handle_queue[now=findmin(handle_queue,handle_num)].addpeople(detect.occurtime, ran.duration, ran.amount);//把这个人插到队伍后面
			evlist.inserfun(detect.occurtime + ran.duration, now);//要处理的人
			if (detect.occurtime < evlist.lasttime[detect.ntype]) {
				//到的时间小于lasttime
				evlist.inserfun(evlist.lasttime[detect.ntype]+ ran.duration, now);
			}
			else
			{
				evlist.inserfun(detect.occurtime-evlist.lasttime[detect.ntype] + ran.duration, now);
			}
			cout << now << "队来了人,交易额是" << ran.amount << "\n";//log
		}
		else if (detect.ntype != -1)
		{
			if (handle_queue[detect.ntype].howmuch() >= 0) {//存款直接过去
				totalmoney += handle_queue[detect.ntype].howmuch();
				totaltime += handle_queue[detect.ntype].time()+detect.occurtime- handle_queue[detect.ntype].getarr();
				custom++;
				cout << "队" << detect.ntype << "的人离队了，交易额是" << handle_queue[detect.ntype].howmuch() << endl;//log
				handle_queue[detect.ntype].leave();
				waitqueue.scanmode_2(custom, totaltime, totalmoney, lastmoney, detect.occurtime);//处理第二条队
				lastmoney = totalmoney;
				
			}
			else if (handle_queue[detect.ntype].howmuch() < 0) {//取款的情况
				if (handle_queue[detect.ntype].howmuch() * (-1) <= totalmoney)//可取
				{

					totalmoney += handle_queue[detect.ntype].howmuch();
					lastmoney = totalmoney;
					custom++;
					totaltime += handle_queue[detect.ntype].time() + detect.occurtime - handle_queue[detect.ntype].getarr();
					cout << "队" << detect.ntype << "的人离队了，交易额是" << handle_queue[detect.ntype].howmuch()<< endl;//log
					handle_queue[detect.ntype].leave();//处理完这个人
					
				}
				else if (handle_queue[detect.ntype].howmuch() * (-1) > totalmoney) {//取款额大于银行的钱
					//压到第二条队,且第detect.ntype条队的人occurtime全部减去detect.duration
					waitqueue.addpeople(handle_queue[detect.ntype].getarr(), handle_queue[detect.ntype].time(), handle_queue[detect.ntype].howmuch());
					cout << "队" << detect.ntype << "的人由于钱不够进入ex队,交易额是" << handle_queue[detect.ntype].howmuch() << endl;
					handle_queue[detect.ntype].leave();
					waitqueue.display(1);
				}
			}
		}
		//处理完上面的情况，继续读取时间链表。
		detect = evlist.pop();

	}
	//关门以后清退(还在wait，handle以及detect)
	for (int i = 0; i < handle_num; i++) {
		handle_queue[i].clearallpeople(totaltime, custom, closetime);
	}
	
	waitqueue.clearallpeople(totaltime, custom, closetime);

}
int main() {
	//参数输入部分
	cout << "输入最大队伍数:";
	(std::cin >> handle_num).get();
	while (handle_num <= 0) {
		cout << "错误,请输入一个正数";
		(std::cin >> handle_num).get();
	}


	cout << "输入最大处理业务时间";
	(std::cin >> mxdur).get();
	while (mxdur <= 1) {
		cout << "错误,请输入一个大于1的数";
		(std::cin >> mxdur).get();
	}

	cout << "输入最大到达间隔时间";
	(std::cin >> mxint).get();
	while (mxint <= 1) {
		cout << "错误,请输入一个大于1的数";
		(std::cin >> mxint).get();
	}


	cout << "输入最大单次交易额";
	(std::cin >> mxmon).get();
	while (mxmon <= 1) {
		cout << "错误,请输入一个大于1的数";
		(std::cin >> mxmon).get();
	}


	cout << "输入取钱的概率，用小数";
	(std::cin >> minus_percent).get();
	while (minus_percent > 1||minus_percent<0) {
		cout << "错误,请输入一个小于1大于0的数";
		(std::cin >> minus_percent).get();
	}


	similator();
	cout << endl<<"接待顾客总数为" << custom << "\t" << "逗留总时长为" << totaltime << std::endl;
	cout << "最终钱数为:" << totalmoney << "\t" <<"平均停留时间为" << (float)totaltime / custom;
	system("pause");
}






