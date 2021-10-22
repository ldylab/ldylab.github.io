21、合并两个有序链表
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* dummy = new ListNode(-1);
        ListNode* p = dummy; // 作为前面一个虚拟节点，其中的p是在不断移动的

        // 输入的两个节点给输入进去（其头指针的位置）
        ListNode* p1 = l1;
        ListNode* p2 = l2;

        while(p1 != nullptr && p2 != nullptr){
            if(p1->val > p2->val){
                p->next = p2;
                p2 = p2->next;
            }else{
                p->next = p1;
                p1 = p1->next;
            }

            p = p->next;
        }

        // 要是输入其中的有一个是空指针
        if(p1 != nullptr){
            p->next = p1;
        }

        if(p2 != nullptr){
            p->next = p2;
        }

        return dummy->next;
    }
};

// 元素插入
class arrayInsert
{
public:
    int array[];
    int size;

public:
    // 构造函数
    MyArray(int capacity)
    {
        this->array = new int[capacity]; // 开辟其中的新空间
        this->size = 0；
    }

    // 数组中插入元素
    void insert(int element, int index){
        if(index < 0 || index > size){
            throw "超出数组实际元素范围";

        }

        // 从右向左循环，将元素逐个向右挪一位
        for(int i = size-1; i >= index; i--){
            this->array[i+1] = this->array[i];
        }

        // 腾出的位置放置新元素
        this->arrayInsert[index] = element;
        size++;
    }

    // 输出数组
    void output(){
        for(int i = 0; i < size; i++){
            cout << this->array[i];
        }
    }
}

// 数组的扩容与插入
class arrayExtendInsert{
private:
    int size = 0;
    int array[];

public:
    MyArray(int capacity){
        this->array = new int[capacity];
        size = 0;
    }

    void insert(int element, int index){
        if(index < 0 || index > size){
            throw "超出数组实际元素范围！";
        }

        if(index >= this->array.lenght){
            resize();
        }

        for(int i = this->size - 1; i > index; i--){
            this->array[i + 1] = this->array[i];
        }

        // 把腾出的位置放入新元素
        this->array[index] = element;
        this->size++;
    }

    // 数组扩容
    void resize(){
        int *arrayNew = new int[this->array->lenght * 2];
        // 从旧数组复制到新数组
        array = arrayNew;
    }
}

class Sort{
private:
    vector<int> array_to_sort; // 创建一个容器

public:
    void bubbleSortOpt1(vector<int> array){
        for(int i = 0; i < array.size() - 1; i++){
            boolean isSorted = true; // 要是每次轮训时更新，要是没有改变就说明已经排列完毕

            // 逐渐的递进
            for(int j = 0; j < array.size() - i - 1; j++){
                int temp = 0;
                // 前面比后面大就做一次交换
                if(array[j] > array[j+1]){
                    tmp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = tmp;

                    // 因为有元素进行交换，所以不是有序的，标记变成false
                    isSorted = false;
                }
            }
            if(isSorted){
                break;
            }
        }
    }

    void bubbleSortOpt2(vector<int> array){
        
    }
}

// 优先队列的实现
#include <queue>
// 创建大顶堆
priority_queue<int> a;
priority_queue<int, vector<int>, less<int>> a;
// 创建小顶堆
priority_queue<int, vector<int>, greater<int> > c; 
// 向堆中压入元素，在压如之后，其会在堆中自动排列
a.push(i);
// 访问堆定
a.top();
// 堆顶弹出
a.pop();
// 判断堆是否已经空了
a.empty();

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
 
class Solution{
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        std::priority_queue<ListNode*, std:: vector<ListNode*>, ListNodeCompare> pq;

        // 遍历lists中的所有元素，加塞到优先队列中
        for(const auto& node: lists){
            if(node){ // 可能有些lists中的有些元素是null的
                pq.push(node);
            }
        }

        // 虚拟节点的构建
        ListNode* dummy = new ListNode();
        ListNode* curr = dummy;
        while(pq.size()){ // 只要其中的优先队列不为空
            ListNode* top = pq.top();
            pq.pop();
            curr->next = top;
            curr = curr->next;
            if(top->next){
                pq.push(top->next);
            }
        }
        return dummy->next;
    }
private:
    // 这种写法还没有学过，要注意记一下，在原有的list中是采用传入地址，但是传入的地址并不能比较大小，只能是将地址翻译为实际的数的大小并做比较
    struct ListNodeCompare{
        bool operator()(const ListNode* lhs, const ListNode* rhs){
            return lhs -> val > rhs->val; // 这种函数就是返回其中的比较结果而已
        }
    }
}