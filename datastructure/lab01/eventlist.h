#pragma once
#pragma once
#include"structure.h"
using std::cout;
class eventlist
{
public:
	eventlist();
	~eventlist();
	bool isempty();
	void show();
	littleevent pop();
	void inserfun(littleevent toadd);
	void inserfun(int occurtime, int ntype);
	int lasttime[100];
	void minus_num(int num, int ntype);//专用于处理移除的情况
private:
	event* head;
};

eventlist::eventlist()
{
	head = nullptr;
	for (int i = 0; i < handle_num; i++) {
		lasttime[i] = 0;
	}
}
eventlist::~eventlist()
{
}
bool eventlist::isempty() {
	if (head == nullptr) {
		return true;
	}
	else {
		return false;
	}

}
void eventlist::show() {
	cout << "evlist:";
	event* temp = head;
	while (temp != nullptr) {
		cout << temp->occurtime << "\t" << temp->ntype << "\t";
		temp = temp->next;
	}
	cout << std::endl;
}
littleevent eventlist::pop() {
	littleevent toclear;
	event* temp;
	toclear.ntype = head->ntype;
	toclear.occurtime = head->occurtime;
	temp = head;
	head = head->next;
	delete temp;
	return toclear;
}
void eventlist::inserfun(littleevent toadd) {
	event* temp = new event;
	temp->occurtime = toadd.occurtime;
	temp->ntype = toadd.ntype;
	event* current, * prev;
	current = prev = head;
	while (current && current->occurtime < temp->occurtime) {
		prev = current;
		current = current->next;
	}
	if (current == head) {
		temp->next = current;
		head = temp;
	}
	else {
		temp->next = current;
		prev->next = temp;
	}
	if (toadd.ntype >= 0) {
		this->lasttime[toadd.ntype] = toadd.occurtime;
	}
}
void eventlist::inserfun(int occurtime, int ntype) {
	littleevent temp = { occurtime,ntype };
	this->inserfun(temp);
}
void eventlist::minus_num(int num, int ntype) {
	lasttime[ntype] = lasttime[ntype] - num;
	event* prev ,*current,*temp=nullptr;
	prev = current = head;
	while (current != nullptr) {
		if (current->ntype == ntype) {
			if (current == head) {
				current->occurtime = current->occurtime - num;
				prev = current;
				current = current->next;
			}
			else {
				prev->next = current->next;
				this->inserfun(current->occurtime - num, current->ntype);
				delete current;
				current = prev->next;
			}
		}
		else {
			prev = current;
			current = current->next;
		}


	}


}