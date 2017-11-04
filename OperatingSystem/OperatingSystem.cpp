// OperatingSystem.cpp : Defines the entry point for the console application.
//

// OperatinfSystem.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iomanip>


#include <stdlib.h>
using namespace std;

int PID_f = 0;//��ʼ����PID
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
//�Ӿ������̶�����ѡ����׽�������
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

//�����������̶����д��ڶ��׵Ľ���
process_node *Wakeup(process_node *headBlock,process_node *headReady){

	process_node *wakeup_node = headBlock->next;
	headBlock->next = wakeup_node->next;
	wakeup_node->next = headReady->next;
	headReady->next = wakeup_node;

	printf("PID = %d is woken up;\n", wakeup_node->PID);
	return runingPro;
}

//����һ�����̣��������������̶���
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

//�ӽ��̶����в���ĳһ����
process_node *searchBefore(process_node *head,int PID){
	process_node *node = head;
	while (node->next != NULL && node->next->PID != PID){
		node = node->next;
	}	
	return node;
}

//����ĳ����
void Undo(process_node *headBlock, process_node *headReady){
	printf("���볷�����̵�PID\n");
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

//������ǰ�������еĽ���
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

//���������ȼ��Զ����еĽ��̽�������
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

//��������еĽ���
void Display(process_node *first){
	process_node *node = first->next;
	while (node != NULL){
		printf(" PID=%d,prio=%d ", node->PID, node->priority);
		printf("->");
		node = node->next;
	}
	printf("NULL");


}
//�������н���
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
	headReady->next = NULL;//�������̶��еĶ��׽ڵ�
	process_node *tailReady = (process_node *)malloc(sizeof(process_node));
	tailReady->next = NULL;//�������̶��еĶ�β�ڵ�
	headReady = tailReady;

	process_node *headBlock = (process_node *)malloc(sizeof(process_node));
	headBlock->next = NULL;//�������̶��еĶ��׽ڵ�
	process_node *tailBlock = (process_node *)malloc(sizeof(process_node));
	tailBlock->next = NULL;//�������̶��еĶ�β�ڵ�
	headBlock = tailBlock;

	//-------------------------------------------
	/*printf("����һ���µĽ���\n");
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
		
		
		printf("  C   ����һ������\n");
		printf("  R   ����\n");
		printf("  B   �����������еĽ���\n");
		printf("  W   ���ѽ���\n");
		printf("  U   ��������\n");
		printf("  0   �˳�����\n");
		
		
		printf("������Ӧ����ĸ���Խ��̽��в���\n");
		
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



