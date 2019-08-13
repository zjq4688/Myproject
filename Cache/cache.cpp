#include"head.h"
#include<math.h>

class FIFOCache
{
public:

	FIFOCache(int capacity)
	{
		FIFOcapacity = capacity;
		FIFOsize = 0;
		list.Listcapcity = capacity;

	}

	int get(int key)
	{
		if (!FIFOmap.count(key))
			return -1;
		Node* node = FIFOmap[key];
		return node->val;
	}
	void put(int key, int value)
	{
		if (FIFOcapacity == 0)
			return;
		if (FIFOmap.count(key))
		{
			Node* node = FIFOmap[key];
			list.remove(node);
			node->val = value;
			list.append(node);
		}
		else
		{
			if (FIFOsize == FIFOcapacity)
			{
				Node* node = list.pop();
				int x = node->val;
				map<int, Node*>::iterator it3 = FIFOmap.find(x);
				//通过find拿到 node.val 为关键字的的迭代器
				FIFOmap.erase(it3);
				--FIFOsize;
			}
			Node* node = new Node(value);

			list.append(node);
			FIFOmap[key] = node;
			++FIFOsize;

		}
	}

	void printFIFO()
	{
		list.printList();
	}
	//数据成员
	int FIFOsize;
	int FIFOcapacity;
	DoubleList list;
	map<int, Node*>FIFOmap;
};


class LRUcache
{
public:
	LRUcache(int capacity)
	{
		LRUcapacity = capacity;
		list.Listcapcity = capacity;
		LRUsize = 0;
	}
	int get(int key)
	{
		if (LRUmap.count(key))
		{
			Node* node = LRUmap[key];
			list.remove(node);
			list.append_front(node);
			return node->val;
		}
		else
			return - 1;
	}

	void put(int key, int value)
	{
		if (LRUcapacity == 0)
			return;
		if (LRUmap.count(key))
		{
			Node* node = NULL;
			node = list.remove(LRUmap[key]);
			node->val = value;
			list.append_front(node);
		}
		else
		{
			if (LRUsize == LRUcapacity)
			{
				Node* node = NULL;
				node= list.remove();
				int x = node->val;
				map<int, Node*>::iterator it = LRUmap.find(x);
				LRUmap.erase(it);
				--LRUsize;
			}
			Node* node = new Node(value);
			list.append_front(node);
			LRUmap[key] = node;
			++LRUsize;
		}
	}

	void printLRU()
	{
		list.printList();
	}
	//数据成员
	int LRUsize;
	int LRUcapacity;
	DoubleList list;
	map<int, Node*>LRUmap;
};

class LFUNode :public Node
{
public:
	LFUNode() {}
	
	LFUNode(int x)
	{
		freq = 0;
		val = x;
	}
	int freq;
};

class LFUcache
{
public:
	LFUcache(int capacity)
	{
		LFUcapacity = capacity;
		LFUsize = 0;
		
	}

	void update(LFUNode*node) //更新节点 每次get或者put 访问频率改变
	{
		int freq = node->freq;
		//删除
		LFUNode* node1 = NULL;
		node1=(LFUNode*)freq_map[freq]->remove(node); ///
		if (freq_map[freq]->Listsize == 0)//如果该节点为最后一个节点，则删除双向链表
		{
			map<int, DoubleList*>::iterator it = freq_map.find(freq);
			freq_map.erase(it);
		}

		//更新
		++freq;
		node1->freq = freq;
		if (freq_map.count(freq))//该频率已存在，则尾部加入
		{
			freq_map[freq]->append(node1);//static_cast<Node*>(node)
		}
		else  //该频率未存在，则创建新双向链表插入
		{
			DoubleList* Dlist = new DoubleList();
			Dlist->append(node1);//
			freq_map[freq] = Dlist;
		}
	}
	int get(int key)
	{
		if (!LFUmap.count(key))
			return -1;
		else
		{
			LFUNode* node = LFUmap[key];
			update(node);
			return node->val;
		}
	}

	void put(int key,int value)
	{
		if (LFUcapacity == 0)
			return;
		if (LFUmap.count(key))//缓存命中
		{
			LFUNode* node = LFUmap[key];
			node->val = value;
			update(node);
		}
		else//缓存没有命中
		{
			if (LFUsize == LFUcapacity)
			{
				Node* node = NULL;
				map<int, DoubleList*>::iterator it1 = freq_map.begin();
				node =it1->second->pop();
				map<int, LFUNode*>::iterator it2 = LFUmap.find(node->val);
				LFUmap.erase(it2);
				--LFUsize;
			}
			LFUNode* lfNode = new LFUNode(value);
			lfNode->freq = 1;
			LFUmap[key] = lfNode;
			if (!freq_map.count(lfNode->freq))
			{
				freq_map[lfNode->freq] = new DoubleList();
			}
			freq_map[lfNode->freq]->append(static_cast<Node*>(lfNode));//
			++LFUsize;
		}
	}

	void printLFU()
	{
		cout << "---------Start-------------------"<< endl;
		for (map<int ,DoubleList*>::iterator it= freq_map.begin();  it != freq_map.end(); ++it)
		{
			cout << "Freq = " << it->first << " :" << endl;
			it->second->printList(); 
		}
		cout << "---------End-------------------"<< endl<<endl;
	}


	int LFUsize;
	int LFUcapacity;
	
	map<int, DoubleList*>freq_map;
	map<int, LFUNode*>LFUmap;

};
int main()
{
	/*DoubleList list(10);
	Node node1(1);
	Node node2(2);
	Node node3(3);
	Node node4(4);

	list.add_head(&node1);
	list.add_head(&node2);

	list.add_tail(&node3);
	list.add_tail(&node4);
	list.printList();

	list.remove_tail();
	list.printList();

	list.remove_head();
	list.printList();


	list.remove_head();
	list.printList();

	list.remove_head();
	list.printList();*/

	//////////////////////doublelist 测试

	/*FIFOCache cache(2);
	cache.put(1,1);
	cache.printFIFO();

	cache.put(2, 2);
	cache.printFIFO();

	cout << cache.get(1) << endl;

	cache.put(3, 3);
	cache.printFIFO();

	cout << cache.get(2) << endl;
	cache.printFIFO();

	cache.put(4, 4);
	cache.printFIFO();

	cout << cache.get(1) << endl;*/

	/////////////////////////////FIFOcache 测试

	/*LRUcache cache2(2);
	cache2.put(2, 2);
	cache2.printLRU();

	cache2.put(1, 1);
	cache2.printLRU();

	cache2.put(3, 3);
	cache2.printLRU();

	cout << cache2.get(1) << endl;
	cache2.printLRU();

	cout << cache2.get(2) << endl;
	cache2.printLRU();

	cout << cache2.get(3) << endl;
	cache2.printLRU();*/



	////////////////////////////   LFU 测试

	LFUcache cache3(2);
	cache3.put(1, 1);
	cache3.printLFU();

	cache3.put(2, 2);
	cache3.printLFU();

	cout << cache3.get(1) << endl;
	cache3.printLFU();

	cache3.put(3, 3);
	cache3.printLFU();

	cout<<cache3.get(2) << endl;
	cache3.printLFU();
	cout << cache3.get(3) << endl;
	cache3.printLFU();

	cache3.put(4, 4);
	cache3.printLFU();

	cout << cache3.get(1) << endl;
	cache3.printLFU();
	cout << cache3.get(3) << endl;
	cache3.printLFU();
	cout << cache3.get(4) << endl;
	cache3.printLFU();

	getchar();
	return 0;
}
