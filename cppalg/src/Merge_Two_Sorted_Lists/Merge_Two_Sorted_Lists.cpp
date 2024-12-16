#include <iostream>


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1){
            return list2;
        }
        if (!list2){
            return list1;
        }

        ListNode* head = nullptr;
        head = init_head(head, list1, list2);
        ListNode* tail = head;

        while (list1 && list2)
            (list1->val <= list2->val) ? update_tail(tail, list1) : update_tail(tail, list2);

        (!list1) ? update_tail(tail, list2) : update_tail(tail, list1);

        return head;
    }

private:

    ListNode* init_head(ListNode* head, ListNode* &src1head, ListNode* &src2head){
        if (src1head->val <= src2head->val){
            head = src1head;
            src1head = src1head->next;
        } else {
            head = src2head;
            src2head = src2head->next;
        }
        return head;
    }

    ListNode* update_tail(ListNode* &src, ListNode* &dist){
        src->next = dist;
        dist = dist->next;
        src = src->next;
        return src; 
    }

    #if 0
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1){
            return list2;
        }
        if (!list2){
            return list1;
        }


        ListNode* tail = new ListNode();
        ListNode* head = new ListNode(0, tail);

        if (list1->val <= list2->val){
            head->val = list1->val;
            list1 = list1->next;
        } else {
            head->val = list2->val;
            list2 = list2->next;
        }


        while(list1 && list2){
            if (list1->val <= list2->val){
                tail->val = list1->val;
                tail->next = new ListNode();
                tail = tail->next;

                list1 = list1->next; 
            } else {
                tail->val = list2->val;
                tail->next = new ListNode();
                tail = tail->next;

                list2 = list2->next; 
            }
        }

        if (!list1){
            tail->val   = list2->val;
            tail->next  = list2->next;
        } else {
            tail->val   = list1->val;
            tail->next  = list1->next;
        }
    
        return head;
    }
    #endif
};


int main(int argc, char* argv[]){

    ListNode* tmp = new ListNode(1);
    ListNode* list1 = tmp;


    for (int i: {2,4}){
        tmp->next = new ListNode(i);
        tmp = tmp->next;
    }

    ListNode* tmp_2 = new ListNode(1);
    ListNode* list2 = tmp_2;

    for (int i: {3,4}){
        tmp_2->next = new ListNode(i);
        tmp_2 = tmp_2->next;
    }

    # if 0
    std::cout << "list1" << std::endl;
    while(list1){
        std::cout << list1->val << std::endl;
        list1 = list1->next;
    }

    std::cout << "list2" << std::endl;
    while(list2){
        std::cout << list2->val << std::endl;
        list2 = list2->next;
    }
    #endif 

    Solution sol;
    ListNode* merged = new ListNode();
    merged = sol.mergeTwoLists(list1, list2);
    

    std::cout << "merged" << std::endl;
    while(merged){
        std::cout << merged->val << std::endl;
        merged = merged->next;
    }

    return 0;
}
