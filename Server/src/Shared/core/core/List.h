#pragma once


struct ListNode
{
	ListNode*	prev;
	ListNode*	next;
	void*		value;
};

struct List
{
	ListNode*	head;
	ListNode*	tail;
	int32		count;
	void(*free)(void* ptr) = NULL;
};


List* listCreate();
void listRelease(List* list);
void listClear(List* list);
void* listValue(ListNode* node);
ListNode* listAddNodeHead(List* list, void* value);
ListNode* listAddNodeTail(List* list, void* value);
ListNode* listInsertNode(List *list, ListNode *old_node, void* value, int after = 0);

ListNode* listPushFront(List* list, ListNode* node);
ListNode* listPushBack(List* list, ListNode* node);
ListNode* listInsertNode(List *list, ListNode *old_node, ListNode* node, int after = 0);

void listRemoveNode(List* list, ListNode* node);
void listRemoveNode(List* list, int32 index);
ListNode* listGetNode(List* list, int32 index);
void listNodeRelease(List* list, ListNode* node);

ListNode* listFront(List* list);
ListNode* listBack(List* list);