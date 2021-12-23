#pragma once
#pragma once
#include"structure.h"
using std::cout;
class customerqueue {
private:
	customer* head;
	customer* rear;
	int lenth;
public:
	int howmuch();
	customerqueue();
	void display(int mode );
	void addpeople(int arrivaltime, int durationtime, int amount);
	void leave();
	int howlong();
	~customerqueue();
	void scanmode_2(int& custom, int& totaltime, int& bankmoney, int lastmoney, int& nowtime);
	int time();
	int getarr();
	void clearallpeople(int& totaltime, int& custom, int now);
};
void customerqueue::clearallpeople(int& totaltime, int& custom, int now) {
	customer* current, * temp;
	current = temp = head;
	while (current != NULL) {
		temp = current;
		current = current->next;
		totaltime += now - temp->arrivaltime;
		custom++;
		delete temp;
	}

}
int customerqueue::time() {
	return head->durationtime;


}
int customerqueue::getarr() {
	return head->arrivaltime;
}
customerqueue::customerqueue() {
	lenth = 0;
	head = rear = nullptr;
}
void customerqueue::display(int mode=0) {
	cout << "queue:";
	customer* temp = head;
	if (mode == 0) {
		while (temp != nullptr) {
			cout << temp->arrivaltime << "\t" << temp->durationtime << "\t";
			temp = temp->next;
		}
	}
	else {
		cout << "ex��:";
		while (temp != nullptr) {
			cout << "���׶�:" << temp->amount << "\t";
			temp = temp->next;
		}
	}
	cout << std::endl;
}
void customerqueue::addpeople(int arrivaltime, int durationtime, int amount) {
	customer* temp = new customer;
	temp->arrivaltime = arrivaltime;
	temp->durationtime = durationtime;
	temp->amount = amount;
	temp->next = nullptr;
	if (head == nullptr) {
		lenth++;
		head = rear = temp;
	}
	else {
		rear->next = temp;
		rear = temp;
		lenth++;
	}
}
void customerqueue::leave() {
	if (lenth > 0) {
		customer* temp = head;
		head = head->next;
		lenth--;
		delete temp;
	}

}
int customerqueue::howlong() {
	return lenth;
}
int findmin(customerqueue list[], int lenth) {
	int i, result = 0;
	for (i = 0; i < lenth; i++) {
		if (list[result].howlong() > list[i].howlong())
		{
			result = i;
		}
		
	}
	return result;
}
customerqueue::~customerqueue() {

}

int customerqueue::howmuch() {
	return head->amount;
}

void customerqueue::scanmode_2(int& custom, int& totaltime, int& bankmoney, int lastmoney, int& nowtime) {//�����2����
	customer* current, * prev;
	prev = current = head;
	int temptime = nowtime;
	while (bankmoney > lastmoney && current) {
		//�����һ��ܴ���
		if (bankmoney + current->amount >= 0) {
			cout << "�����뿪ex��,���׶���" << current->amount<<std::endl;
			if (current == head) {
				//ͷָ��ֱ�ӵ�����ӷ���
				custom++;
				totaltime = (temptime - current->arrivaltime + current->durationtime) + totaltime;
				temptime += current->durationtime;
				bankmoney += current->amount;
				this->leave();//����
				prev = current = head;
			}
			else
			{
				//��������������
				custom++;
				totaltime = (nowtime - current->arrivaltime + current->durationtime) + totaltime;
				temptime += current->durationtime;
				bankmoney += current->amount;
				prev->next = current->next;
				if (current == rear) {
					rear = prev;
				}
				delete current;
				current = prev->next;
				lenth--;
			}

		}
		/*ʵ�����㷨*/
		/*
		else if (nowtime - current->arrivaltime >= 2 * maxduration) {
				custom++;
				totaltime = (nowtime - current->arrivaltime) + totaltime;
				if (current == head) {
					this->leave();//����
					prev = current = head;
				}
				else {
					prev->next = current->next;
					if (current == rear) {
						rear = prev;
					}
					delete current;
					current = prev->next;
					lenth--;
				}
			}*/
		else
		{
			prev = current;
			current = current->next;
		}
	}
	//�����꣬���޷���������ڶ�β
	if (current != nullptr && current != prev) {
		prev->next = nullptr;
		rear->next = head;
		head = current;
		rear = prev;

	}
}
