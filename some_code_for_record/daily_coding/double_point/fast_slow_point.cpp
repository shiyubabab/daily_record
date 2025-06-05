/*************************************************************************
	> File Name: fast_slow_point.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 05 Jun 2025 02:20:56 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
struct ListNode{
	int val;
	ListNode *next;

	ListNode() : val(0),next(nullptr) {}
	ListNode(int x) : val(x),next(nullptr) {}
	ListNode(int x,ListNode *next) : val(x),next(next) {}
};

ListNode *middleNode(ListNode* head){
	if(!head) return NULL;
	ListNode *fast,*slow;
	fast = head;
	slow = head;
	while(fast->next){
		fast = fast -> next;
		if(fast->next) fast = fast->next;
		slow = slow->next;
	}
	return slow;
}

int main(void){
	ListNode *head = new ListNode(1);
	head->next = new ListNode(2);
	cout << head->val<<endl;
	cout << head->next->val<<endl;
	return 0;
}

