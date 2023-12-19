#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};
struct ListNode *mergeKLists(struct ListNode **lists, int listsSize);

// Helper function to print a linked list.
void printList(struct ListNode *node) {
    while (node != NULL) {
        printf("%d ", node->val);
        node = node->next;
    }
    printf("\n");
}

// Helper function to create a new list node.
struct ListNode *createListNode(int val) {
    struct ListNode *newNode =
        (struct ListNode *)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

void freeList(struct ListNode *node) {
    struct ListNode *temp;

    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

void validateResult(struct ListNode *result, struct ListNode *expected, int i) {
    for (struct ListNode *tempA = result, *tempB = expected;
         tempA != NULL || tempB != NULL;
         tempA = tempA->next, tempB = tempB->next) {
        if (tempA == NULL || tempB == NULL) {
            printf("Test Case %d failed. Result list size differs from "
                   "expected.\n",
                   i);
            return;
        }
        if (tempA->val != tempB->val) {
            printf("Test Case %d failed. List isn't sorted.\n", i);
            return;
        }
    }
    printf("Test Case %d passed!\n", i);
}

void testMergeKLists() {
    struct ListNode *result, *expected;
    // Test Case 1: No linked lists
    struct ListNode **lists1 = NULL;
    result = mergeKLists(lists1, 0);
    assert(result == NULL);  // Expect 'result' to be NULL.
    printf("Test Case 1 passed!\n");

    // Test Case 2: A single linked list
    struct ListNode **lists2 =
        (struct ListNode **)malloc(sizeof(struct ListNode *));
    lists2[0] = createListNode(1);
    lists2[0]->next = createListNode(2);
    lists2[0]->next->next = createListNode(3);
    result = mergeKLists(lists2, 1);

    expected = createListNode(1);
    expected->next = createListNode(2);
    expected->next->next = createListNode(3);

    validateResult(result, expected, 2);
    freeList(result);
    freeList(expected);
    free(lists2);

    // Test Case 3: Multiple linked lists
    struct ListNode **lists3 =
        (struct ListNode **)malloc(3 * sizeof(struct ListNode *));

    // List 1: 1 -> 4 -> 5
    lists3[0] = createListNode(1);
    lists3[0]->next = createListNode(4);
    lists3[0]->next->next = createListNode(5);

    // List 2: 1 -> 3 -> 4
    lists3[1] = createListNode(1);
    lists3[1]->next = createListNode(3);
    lists3[1]->next->next = createListNode(4);

    // List 3: 2 -> 6
    lists3[2] = createListNode(2);
    lists3[2]->next = createListNode(6);

    result = mergeKLists(lists3, 3);

    // Create expected result list: 1 -> 1 -> 2 -> 3 -> 4 -> 4 -> 5-> 6
    expected = createListNode(1);
    expected->next = createListNode(1);
    expected->next->next = createListNode(2);
    expected->next->next->next = createListNode(3);
    expected->next->next->next->next = createListNode(4);
    expected->next->next->next->next->next = createListNode(4);
    expected->next->next->next->next->next->next = createListNode(5);
    expected->next->next->next->next->next->next->next = createListNode(6);

    validateResult(result, expected, 3);

    // Free all dynamically allocated memory
    freeList(result);
    freeList(expected);
    free(lists3);
}

int main() {
    testMergeKLists();
    return 0;
}
