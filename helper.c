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

void getCurrentTime(char *buffer, int bufferSize){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, bufferSize, "%d/%m/%Y-%H:%M", timeinfo);
}

void freePostList(Post* head) {
    Post* current = head;
    while (current != NULL) {
        Post* temp = current;
        current = current->next;
        free(temp);
    }
}

void printWrappedText(const char* text) {
    int len = strlen(text);
    int start = 0;

    while (start < len) {
        printf("| %-*.*s |\n", CONTENT_WIDTH, CONTENT_WIDTH, text + start);
        start += CONTENT_WIDTH;
    }
}
