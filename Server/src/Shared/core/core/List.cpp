#include "Shared.hpp"

List* listCreate()
{
	List* list = new List;

	list->head = NULL;
	list->tail = NULL;
	list->count = 0;
	return list;
}

void listRelease(List* list)
{
	listClear(list);
	delete list;
}

void listClear(List* list)
{
	while (list->head)
		listRemoveNode(list, list->head);
}

void* listValue(ListNode* node)
{
	return node->value;
}

ListNode* listAddNodeHead(List* list, void* value)
{
	ListNode* node = new ListNode;
	node->value = value;
	return listPushFront(list, node);
}

ListNode* listPushFront(List* list, ListNode* node)
{
	node->prev = NULL;
	node->next = list->head;

	if (list->head)
		list->head->prev = node;

	if (list->tail)
		list->tail = node;

	list->head = list->head == NULL ? node : list->head;
	list->head = node;

	list->count++;
	return node;
}

ListNode*  listAddNodeTail(List* list, void* value)
{
	ListNode* node = new ListNode;
	node->value = value;
	
	return listPushBack(list, node);
}

ListNode* listPushBack(List* list, ListNode* node)
{
	list->head = list->head == NULL ? node : list->head;
	node->prev = list->tail;
	node->next = NULL;
	if (list->tail)
		list->tail->next = node;

	list->tail = node;
	list->count++;
	return node;
}

ListNode* listInsertNode(List *list, ListNode *old_node, void* value, int after /*= 0*/)
{
	ListNode* node = new ListNode;
	node->value = value;
	return listInsertNode(list, old_node, node, after);
}

ListNode* listInsertNode(List *list, ListNode *old_node, ListNode* node, int after /*= 0*/)
{
	if (after)
	{
		node->prev = old_node;
		node->next = old_node->next;
		if (list->tail == old_node) {
			list->tail = node;
		}
	}
	else
	{
		node->next = old_node;
		node->prev = old_node->prev;
		if (list->head == old_node) {
			list->head = node;
		}
	}

	if (node->prev != NULL) {
		node->prev->next = node;
	}

	if (node->next != NULL) {
		node->next->prev = node;
	}
	list->count++;
	return node;
}

void listRemoveNode(List* list, ListNode* node)
{
	ListNode* head = list->head;

	while (head)
	{
		if (head == node)
		{
			if (head == list->head && head == list->tail)
			{
				head->next = NULL;
				head->prev = NULL;
				list->head = NULL;
				list->tail = NULL;
			}
			else if (head == list->head)
			{
				head->next->prev = NULL;
				list->head = head->next;

				head->next = NULL;
			}
			else if (head == list->tail)
			{
				head->prev->next = NULL;
				list->tail = head->prev;
				head->prev = NULL;
			}
			else
			{
				head->prev->next = head->next;
				head->next->prev = head->prev;
			}

			listNodeRelease(list, node);
			list->count--;

			break;
		}
		head = head->next;
	}
}

void listRemoveNode(List* list, int32 index)
{
	ListNode* node = list->head;
	int32 idx = 0;
	while (node)
	{
		if (index == idx)
		{
			listNodeRelease(list, node);
			list->count--;
			break;
		}
		idx++;
		node = node->next;
	}
}

ListNode* listGetNode(List* list, int32 index)
{
	ListNode* node = list->head;
	int32 idx = 0;
	while (node)
	{
		if (index == idx)
			return node;
		idx++;
		node = node->next;
	}
	return NULL;
}

void listNodeRelease(List* list, ListNode* node)
{
	if (list->free)
		list->free(node->value);
	delete node;
}

ListNode* listFront(List* list)
{
	return list->head;
}

ListNode* listBack(List* list)
{
	return list->tail;
}

