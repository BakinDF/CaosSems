#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

int getMinIdx(struct ListNode **lists, int listsSize) {
    int cur_min = INT_MAX;
    int cur_idx = -1;
    for (int i = 0; i < listsSize; ++i) {
        if (lists[i] == NULL) {
            continue;
        }
        if (lists[i]->val < cur_min || cur_idx == -1) {
            cur_min = lists[i]->val;
            cur_idx = i;
        }
    }
    return cur_idx;
}

struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {
    if (listsSize == 0) {
        return NULL;
    }
    struct ListNode *root_node =
        (struct ListNode *)calloc(1, sizeof(struct ListNode));
    struct ListNode *cur_node = root_node;
    int cur_idx = getMinIdx(lists, listsSize);
    while (cur_idx != -1) {
        cur_node->next = (struct ListNode *)calloc(1, sizeof(struct ListNode));
        cur_node = cur_node->next;
        cur_node->val = lists[cur_idx]->val;
        cur_node->next = NULL;
        struct ListNode *tmp = lists[cur_idx];
        lists[cur_idx] = lists[cur_idx]->next;
        free(tmp);
        cur_idx = getMinIdx(lists, listsSize);
    }
    struct ListNode *node_to_ret = root_node->next;
    free(root_node);

    return node_to_ret;
}
