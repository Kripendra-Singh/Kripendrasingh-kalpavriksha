SinglyLinkedListNode* reverse(SinglyLinkedListNode* llist) {
    SinglyLinkedListNode* curr=llist;
    SinglyLinkedListNode* prev=NULL;
    SinglyLinkedListNode*next=NULL;
  while (curr != NULL) {
        next = curr->next;

        curr->next = prev;

        prev = curr;
        curr = next;
    }

    return prev;
}