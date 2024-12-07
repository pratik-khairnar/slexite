#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header2.h"

#define USERFILE "users.txt"
#define POSTFILE "posts.txt"
#define LIKESFILE "likes.txt"
#define COMMENTSFILE "comments.txt"
#define FOLLOWERFILE "followers.txt"
#define FOLLOWINGFILE "following.txt"
#define BOX_WIDTH 63
#define CONTENT_WIDTH (BOX_WIDTH - 4)

void shuffleFeed(Post** head) {
    // count total number of posts
    int count = 0;
    Post* current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count <= 1) return; // No need to shuffle if there's 1 or less posts

    // Convert the list to an array
    Post* postsArray[count];
    current = *head;
    for (int i = 0; i < count; i++) {
        postsArray[i] = current;
        current = current->next;
    }

    // Shuffle the posts array (Fisher-Yates shuffle)
    srand(time(NULL)); // random seed based on current time
    for (int i = 0; i < count; i++) {
        int j = rand() % count;  // Pick a random index from 0 to count-1
        Post* temp = postsArray[i];
        postsArray[i] = postsArray[j];
        postsArray[j] = temp;
    }

    // form the doubly linked list again
    *head = postsArray[0];
    (*head)->prev = NULL;
    for (int i = 1; i < count; i++) {
        postsArray[i]->prev = postsArray[i - 1];
        postsArray[i - 1]->next = postsArray[i];
    }
    postsArray[count - 1]->next = NULL;
}

int compareByTime(const void *a, const void *b) {
    Post* postA = *(Post**)a;
    Post* postB = *(Post**)b;

    // Compare(latest first)
    return strcmp(postB->timestamp, postA->timestamp);
}


int compareByLikes(const void *a, const void *b) {
    Post* postA = *(Post**)a;
    Post* postB = *(Post**)b;

    // compare posts based on likes count
    return postB->likesCount - postA->likesCount;  // highest like count first
}


int compareByComments(const void *a, const void *b) {
    Post* postA = *(Post**)a;
    Post* postB = *(Post**)b;

    // compare posts based on comment count (highest first)
    return postB->comments.count - postA->comments.count;
}

void sortFeedByLikes(Post** head) {
    if (*head == NULL) {
        return;  // Empty list, nothing to sort
    }

    int swapped;
    Post *ptr1;
    Post *lptr = NULL;

    // Bubble sort based on likesCount (dynamically fetched from file)
    do {
        swapped = 0;
        ptr1 = *head;

        // Traverse the list
        while (ptr1->next != lptr) {
            // Fetch likes count dynamically
            int likesA = getLikesCount(ptr1->postID);
            int likesB = getLikesCount(ptr1->next->postID);

            // Compare likes count
            if (likesA < likesB) {  // Sort in descending order (highest likes first)
                // Swap the entire post nodes based on likesCount
                int tempLikesCount = ptr1->likesCount;
                ptr1->likesCount = ptr1->next->likesCount;
                ptr1->next->likesCount = tempLikesCount;

                char tempContent[280];
                strcpy(tempContent, ptr1->content);
                strcpy(ptr1->content, ptr1->next->content);
                strcpy(ptr1->next->content, tempContent);

                // Swap the post IDs (or any other fields if necessary)
                int tempPostID = ptr1->postID;
                ptr1->postID = ptr1->next->postID;
                ptr1->next->postID = tempPostID;

                // Swap the next pointers
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;  // Move the boundary of the unsorted part of the list
    } while (swapped);  // Repeat until no more swaps are made
}

void sortFeedByComments(Post** head) {
    if (*head == NULL) {
        return;  // Empty list, nothing to sort
    }

    int swapped;
    Post *ptr1;
    Post *lptr = NULL;

    // Bubble sort based on comment count (dynamically fetched from file)
    do {
        swapped = 0;
        ptr1 = *head;

        // Traverse the list
        while (ptr1->next != lptr) {
            // Fetch comments count dynamically
            int commentCountA = commentCount(ptr1->postID);
            int commentCountB = commentCount(ptr1->next->postID);

            // Compare comment count
            if (commentCountA < commentCountB) {  // Sort in descending order (highest comments first)
                // Swap the entire post nodes based on comment count
                int tempCommentCount = ptr1->comments.count;
                ptr1->comments.count = ptr1->next->comments.count;
                ptr1->next->comments.count = tempCommentCount;

                char tempContent[280];
                strcpy(tempContent, ptr1->content);
                strcpy(ptr1->content, ptr1->next->content);
                strcpy(ptr1->next->content, tempContent);

                // Swap the post IDs (or any other fields if necessary)
                int tempPostID = ptr1->postID;
                ptr1->postID = ptr1->next->postID;
                ptr1->next->postID = tempPostID;

                // Swap the next pointers
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;  // Move the boundary of the unsorted part of the list
    } while (swapped);  // Repeat until no more swaps are made
}

void sortFeedByTime(Post** head) {
    int count = 0;
    Post* current = *head;

    // count the total number of posts
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // convert list to array
    Post* postsArray[count];
    current = *head;
    for (int i = 0; i < count; i++) {
        postsArray[i] = current;
        current = current->next;
    }

    // Sort the array of posts by time(latest first)
    qsort(postsArray, count, sizeof(Post*), compareByTime);

    // form the doubly linked list again
    *head = postsArray[0];
    (*head)->prev = NULL;  // The first post should have no previous node
    for (int i = 1; i < count; i++) {
        postsArray[i]->prev = postsArray[i-1];
        postsArray[i-1]->next = postsArray[i];
    }

    postsArray[count - 1]->next = NULL; // The last post should have no next node
}
