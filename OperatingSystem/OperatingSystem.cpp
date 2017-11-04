// OperatingSystem.cpp : Defines the entry point for the console application.
//

// OperatinfSystem.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iomanip>


#include <stdlib.h>
using namespace std;

int PID_f = 0;//初始进程PID
typedef struct NODE{
	int PID;
	int priority;
	struct NODE *next;
}process_node;
process_node *runingPro;

void DisplayRunningProcess(){
	printf("Running process:\n");
	if (runingPro != NULL){
		printf("PID = %d\n\n", runingPro->PID);
	}
	else
	{
		printf("No running process.\n");
	}
}
//从就绪进程队列中选择队首进程运行
void Run(process_node *head){
	runingPro = head->next;
	if (head->next != NULL){
		head->next = head->next->next;
		printf("Running process:\n");
		printf("PID = %d\n\n",runingPro->PID);
	}
	else
	{
		printf("No running process.\n");
	}
	
}

//唤醒阻塞进程队列中处于队首的进程
process_node *Wakeup(process_node *headBlock,process_node *headReady){

	process_node *wakeup_node = headBlock->next;
	headBlock->next = wakeup_node->next;
	wakeup_node->next = headReady->next;
	headReady->next = wakeup_node;

	printf("PID = %d is woken up;\n", wakeup_node->PID);
	return runingPro;
}

//创建一个进程，将其加入就绪进程队列
process_node *Creat(process_node *tail){
	process_node *node = (process_node *)malloc(sizeof(process_node) * 1);
	tail->next = node;
	node->next = NULL;
	node->PID = PID_f + 1;
	PID_f = PID_f + 1;
	node->priority = rand() % 5;
	while (node->priority == 0){
		int i = rand() % 5;
		node->priority = i;

	}
	tail = node;
	printf("PID = %d  ", node->PID);
	printf("priority = %d\n", node->priority);
	return tail;
}

//从进程队列中查找某一进程
process_node *searchBefore(process_node *head,int PID){
	process_node *node = head;
	while (node->next != NULL && node->next->PID != PID){
		node = node->next;
	}	
	return node;
}

//撤销某进程
void Undo(process_node *headBlock, process_node *headReady){
	printf("输入撤销进程的PID\n");
	int PID;
	int leap = 0;
	scanf("%d", &PID);
	if (runingPro->PID == PID){
		process_node *node4 = runingPro;
		Run(headReady);
		free(node4);
	}
	process_node *node = searchBefore(headBlock,PID);
	if (node->next != NULL){
		printf("headBlock\n");
		process_node *node2 = node->next;
		node->next = node2->next;
		free(node2);
	}
	process_node *node1 = searchBefore(headReady, PID);
	if (node1->next != NULL){
		printf("headReady\n");
		process_node *node3 = node1->next;
		node1->next = node3->next;
		free(node3);
	}
	

}

//阻塞当前正在运行的进程
process_node *Block(process_node *tail){
	if (runingPro != NULL){
		tail->next = runingPro;
		tail = runingPro;
		tail->next = NULL;
		printf("\nPID = %d is blocked.\n\n", tail->PID);
	}
	else
	{
		printf("\nThere is not any running process to block.\n\n");
	}
	
	return tail;
}

//按进程优先级对队列中的进程进行排序
process_node *sort_link(process_node *head)
{
	process_node *temp, *q, *p;
	for (q = head; q->next != NULL; q = q->next)
		for (p = q->next; p->next != NULL; p = p->next)
		{
			if (((p->next)->priority) < ((q->next)->priority))
				if (q->next == p)
				{
					temp = p->next;
					p->next = p->next->next;
					temp->next = p;
					q->next = temp;
					p = temp;
				}
				else
				{
					temp = p->next;
					p->next = q->next;
					q->next = temp;
					temp = p->next->next;
					p->next->next = q->next->next;
					q->next->next = temp;
				}
		}
	return head;
}

//输出队列中的进程
void Display(process_node *first){
	process_node *node = first->next;
	while (node != NULL){
		printf(" PID=%d,prio=%d ", node->PID, node->priority);
		printf("->");
		node = node->next;
	}
	printf("NULL");


}
//撤销所有进程
void UndoAllPro(process_node *headReady, process_node *headBlock){
	process_node *node = headReady->next;
	while (node != NULL){
		process_node *node1 = node;
		node = node->next;
		free(node1);
	}
	free(headReady);
	node = headBlock->next;
	while (node != NULL){
		process_node *node1 = node;
		node = node->next;
		free(node1);
	}
	free(headBlock);
}


int _tmain(int argc, _TCHAR* argv[])
{
	process_node *headReady = (process_node *)malloc(sizeof(process_node));
	headReady->next = NULL;//就绪进程队列的队首节点
	process_node *tailReady = (process_node *)malloc(sizeof(process_node));
	tailReady->next = NULL;//就绪进程队列的队尾节点
	headReady = tailReady;

	process_node *headBlock = (process_node *)malloc(sizeof(process_node));
	headBlock->next = NULL;//阻塞进程队列的队首节点
	process_node *tailBlock = (process_node *)malloc(sizeof(process_node));
	tailBlock->next = NULL;//阻塞进程队列的队尾节点
	headBlock = tailBlock;

	//-------------------------------------------
	/*printf("创建一个新的进程\n");
	tailReady = Creat(tailReady);
	headReady = sort_link(headReady);

	tailReady = Creat(tailReady);
	headReady = sort_link(headReady);

	tailReady = Creat(tailReady);
	headReady = sort_link(headReady);

	tailReady = Creat(tailReady);
	headReady = sort_link(headReady);

	tailReady = Creat(tailReady);
	headReady = sort_link(headReady);

	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");

	Run(headReady);

	tailBlock = Block(tailBlock);
	headBlock = sort_link(headBlock);
	Run(headReady);

	printf("Block queue:\n");
	Display(headBlock);
	printf("\n");
	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");

	tailBlock = Block(tailBlock);
	headBlock = sort_link(headBlock);
	Run(headReady);

	printf("Block queue:\n");
	Display(headBlock);
	printf("\n");
	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");

	Wakeup(headBlock, headReady);
	headReady = sort_link(headReady);
	printf("Block queue:\n");
	Display(headBlock);
	printf("\n");
	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");

	tailBlock = Block(tailBlock);
	headBlock = sort_link(headBlock);
	Run(headReady);

	printf("Block queue:\n");
	Display(headBlock);
	printf("\n");
	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");

	tailBlock = Block(tailBlock);
	headBlock = sort_link(headBlock);
	Run(headReady);

	printf("Block queue:\n");
	Display(headBlock);
	printf("\n");
	printf("Ready queue:\n");
	Display(headReady);
	printf("\n");*/

	//----------------------------------------
	
	char option = '1';
	
	while (true){
		
		
		printf("  C   创建一个进程\n");
		printf("  R   运行\n");
		printf("  B   阻塞正在运行的进程\n");
		printf("  W   唤醒进程\n");
		printf("  U   撤销进程\n");
		printf("  0   退出程序\n");
		
		
		printf("输入相应的字母，对进程进行操作\n");
		
		scanf("%c", &option);
		printf("\n\n");
		switch (option)
		{
		case 'C':
			tailReady = Creat(tailReady);
			headReady = sort_link(headReady);
			DisplayRunningProcess();
			printf("Block queue:\n");
			Display(headBlock);
			printf("\n");
			printf("Ready queue:\n");
			Display(headReady);
			printf("\n");
			break;
		case 'R':
			Run(headReady);
			printf("Block queue:\n");
			Display(headBlock);
			printf("\n");
			printf("Ready queue:\n");
			Display(headReady);
			printf("\n");
			break;
		case 'B':
			tailBlock = Block(tailBlock);
			headBlock = sort_link(headBlock);
			if (runingPro != NULL){
				Run(headReady);
			}
			else
			{
				DisplayRunningProcess();
			}
			printf("Block queue:\n");
			Display(headBlock);
			printf("\n");
			printf("Ready queue:\n");
			Display(headReady);
			printf("\n");
			break;
		case 'W':
		
			
			Wakeup(headBlock, headReady);
			headReady = sort_link(headReady);
			DisplayRunningProcess();
			printf("Block queue:\n");
			Display(headBlock);
			printf("\n");
			printf("Ready queue:\n");
			Display(headReady);
			printf("\n");
			break;
		case 'U':
			Undo( headBlock, headReady);
			DisplayRunningProcess();
			printf("Block queue:\n");
			Display(headBlock);
			printf("\n");
			printf("Ready queue:\n");
			Display(headReady);
			printf("\n");
			break;
		case '0':
			break;
		default:
			break;
		}
		if (option == '0')
		{
			break;
		}
		printf("\n-----------------------------------\n");
		getchar();
	
	}

	
	printf("\n");
	UndoAllPro(headReady, headBlock);
	
	system("pause");
	return 0;
}



