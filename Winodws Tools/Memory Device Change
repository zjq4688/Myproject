/*游戏修改器
功能说明：
1.列出现有进程。
2.指定进程并修改该进程目标内存。
1)打开进程，获取内核对象句柄。
2）查找首轮目标。
3）列出查找结果。
4）结果大于1时，查找二轮目标。
5）修改二轮目标中所有内存值。
3.杀死指定进程。
4.退出。
*/

#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<TlHelp32.h>
#include<iostream>
using namespace std;

const DWORD KONEK = 1024;  //定义K
const DWORD KPAGE = 4 * KONEK;  //定义4K为一页
const DWORD KONEG = KONEK*KONEK*KONEK;

void ShowMenu();
void ShowProcessList();
void EditProcessData();
void KillProcess();
bool CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax);
void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax);
void SecondRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD dwAddrListCounter, DWORD *pTargetList, DWORD *pTargetCounter);
void ShowAddrList(DWORD *pDwAddrList, DWORD dwAddrCount);

int main(void)
{
	int select = 0;
	while (true)
	{
		//显示菜单
		ShowMenu();

		while (!scanf_s("%d", &select))
		{
			/*fflush(stdin);*/
			rewind(stdin);
			cout << "Please input your choice...";
		}
		switch (select)
		{
		case 1:
			ShowProcessList();
			break;
		case 2:
			EditProcessData();
			break;
		case 3:
			KillProcess();
			break;
		case 4:
			printf("Thank you using ...\n");
			system("pause");
			return false;
		default:
			printf("Please select again 1-4...\n");
		}
	}

	return 0;
}

void ShowMenu()
{
	cout << "\n-------------------------------------------------------------\n";
	cout << "Menu：\n";
	cout << "\t1.Show system process list.\n";
	cout << "\t2.Input process id which you want to edit.\n";
	cout << "\t3.Input process id which you want to kill.\n";
	cout << "\t4.Exit\n\n";
	cout << "-------------------------------------------------------------\n";
}

void ShowProcessList() //功能1显示进程
{
	PROCESSENTRY32 pc;//定义一个32接收变量
	pc.dwSize = sizeof(pc);

	int count = 0;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bMore = Process32First(hProcessSnap, &pc);
	while (bMore)
	{
		cout << "\n---------------------------------------\n";
		cout << "id: " << pc.th32ProcessID << endl;
		wprintf(L"name:%s\n", pc.szExeFile);
		bMore = Process32Next(hProcessSnap, &pc);
		count++;
	}
	CloseHandle(hProcessSnap);
	cout << "目前进程数：" << count << endl;

}

void EditProcessData() //功能2 编辑进程
{
	DWORD dwId = 0;
	DWORD dwSearchValue = 0;
	DWORD dwAddrList[4 * KONEK] = { 0 };
	DWORD dwAddrCount = 0;
	BOOL bRet = false;

	printf("Please input process id which you want to edit:");
	while (!scanf_s("%d", &dwId))
	{
		rewind(stdin);
		printf("Please input again:");
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if (NULL == hProcess)//打开进程失败
	{
		printf("Open process failed...\n");
		return;
	}
	else   //打开进程成功
	{
		printf("Open process success...\n");
		system("pause");
		printf("Please input the value which you want search first round:");
		scanf_s("%u", &dwSearchValue);

		//首轮查找  进程       查找目标值    存放数组    找到目标计数   空间
		FirstRound(hProcess, dwSearchValue, dwAddrList, &dwAddrCount, 4 * KONEK);

		ShowAddrList(dwAddrList, dwAddrCount);
		if (dwAddrCount == 0)
		{
			return;
		}
		else if (dwAddrCount == 1)
		{
			DWORD value;

			printf("input the value which you want to set");
			scanf_s("%u", &value);
			//改写内存数据
			bRet = WriteProcessMemory(hProcess, (LPVOID)dwAddrList[0], (LPVOID)&value, sizeof(DWORD), NULL);
			if (bRet)
			{
				printf("Edit Success!!!");
			}
			else
			{
				printf("Edit Failed!!!");
			}
		}
		else
		{
			//找到目标过多 二轮查找
			DWORD dwSecondRoundSearchValue = 0;
			DWORD dwTargetList[KONEK] = { 0 };
			DWORD dwTargetCounter = 0;

			printf("input the value which you need to second round find: ");
			scanf_s("%u", &dwSecondRoundSearchValue);
			//二轮查找   进程      二轮查找目标值          原（1轮）地址     原（1轮）目标计数     新目标地址       新目标计数   
			SecondRound(hProcess, dwSecondRoundSearchValue, dwAddrList, dwAddrCount, dwTargetList, &dwTargetCounter);
			ShowAddrList(dwTargetList, dwTargetCounter);

			DWORD value;

			printf("input the value which you want to set");
			scanf_s("%u", &value);
			for (DWORD i = 0; i < dwTargetCounter; i++)
			{
				bRet = WriteProcessMemory(hProcess, (LPVOID)dwTargetList[i], (LPCVOID)&value, sizeof(DWORD), NULL);
				if (bRet)
				{
					printf("Edit Success!!!");
				}
				else
				{
					printf("Edit Failed!!!");
				}
			}
		}
	}
	CloseHandle(hProcess);
}
void KillProcess() //功能3 结束进程
{
	BOOL bRet = FALSE;
	DWORD dwId = 0;
	cout << "Please input process id which you want to kill...\n";
	while (!scanf_s("%d", &dwId))
	{
		/*fflush(stdin);*/
		rewind(stdin);
		cout << "Please input process id which you want to kill";
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);

	if (hProcess != NULL)
	{
		bRet = TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);

	if (bRet)
	{
		cout << "kill process success\n";
	}
	else
	{
		cout << "kill process filed\n";
	}
	return;
}

void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax) //首轮内存查找
{
	DWORD dwBaseAddr = 64 * KONEK;  //查找首地址
	DWORD dwPageCount = (2 * KONEG - 64 * KONEK * 2) / KPAGE; //查找计数 用以确定合法范围  |          |    。。。   |       |                          |
																		  //               |前64k     |用户程序区   |64k禁区|       2G内核区           |
	printf("%u pages\n", dwPageCount);                                    //               |NULL指针区|             |       |                          |

	printf("Start searching...\n");


	DWORD dwBeginAddr = dwBaseAddr;
	for (; dwBaseAddr< 2 * KONEG - 64 * KONEK; dwBaseAddr += KPAGE)
	{
		if (!CompareOnePage(hProcess, dwBaseAddr, dwValue, pAddrList, pAddrListCounter, addrListMax))
		{
			return;
		}
		//计算进度百分比
		DWORD page = (dwBaseAddr - dwBeginAddr) / KPAGE + 1;
		printf("current is %u page\n", page);
		double temp = ((double)page / dwPageCount) * 100;
		printf("-----------%%%f---------\n", temp);
	}
	printf("\nSearch finished...\n");
	system("pause");
}
bool CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax) //对单页内数据进行遍历 比对
{
	BYTE byPage[KPAGE] = { 0 };
	if (!ReadProcessMemory(hProcess, (LPCVOID)dwBaseAddr, (LPVOID)byPage, KPAGE, NULL))
	{
		printf("Read Memory error!!!\n");
		return true;
	}

	DWORD *pdwPointer = NULL;
	pdwPointer = (DWORD*)byPage;

	for (DWORD i = 0; i < KONEK; i++)
	{
		if (*pAddrListCounter >= addrListMax)
		{
			printf("Too many data, can not save...\n");
			return false;
		}
		if (pdwPointer[i] == dwValue)
		{
			pAddrList[*pAddrListCounter] = dwBaseAddr + i*sizeof(DWORD);
			(*pAddrListCounter)++;
		}
	}
	return true;
}

void ShowAddrList(DWORD *pDwAddrList, DWORD dwAddrListCount)
{
	printf("\n--------------Address list begin---------------\n");
	for (DWORD i = 0; i < dwAddrListCount; i++)
	{
		printf("%X\n", pDwAddrList[i]);
	}
	printf("\n--------------Address list end---------------\n");
}

void SecondRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD dwAddrListCounter, DWORD *pTargetList, DWORD *pTargetCounter)
{
	DWORD dwTemp = 0;
	for (DWORD i = 0; i < dwAddrListCounter; i++)
	{
		if (ReadProcessMemory(hProcess, (LPVOID)pAddrList[i], &dwTemp, sizeof(dwTemp), NULL))
		{
			if (dwTemp == dwValue)
			{
				pTargetList[*pTargetCounter] = pAddrList[i];
				(*pTargetCounter)++;
			}
		}
	}
}
