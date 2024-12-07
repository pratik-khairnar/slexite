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

void viewComments(int postID) {
    FILE *fp = fopen(COMMENTSFILE, "r");
    if (fp == NULL) {
        printf("Error opening comments file.\n");
        return;
    }

    char line[1024];
    int found = 0;

    printf("\n--- Comments ---\n");

    // read comments.txt line by line
    while (fgets(line, sizeof(line), fp)) {
        int currentPostID, userID;
        char comment[1024];

        if (sscanf(line, "%d,%d,%[^\n]", &currentPostID, &userID, comment) == 3) {
            if (currentPostID == postID) {
                // get username by using userID
                char* username = getUsernameByID(userID);
                if (username != NULL) {
                    printf("%s: %s\n", username, comment);
                } else {
                    printf("User ID %d: %s\n", userID, comment); // username is not found
                }
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No comments for this post.\n");
    }

    fclose(fp);
}

void addComment(int postID, int userID, const char* comment) {
    FILE *fp = fopen(COMMENTSFILE, "a");
    if (fp == NULL) {
        printf("Error opening comments file.\n");
        return;
    }

    // Write the comment to the file
    fprintf(fp, "%d,%d,%s\n", postID, userID, comment);

    fclose(fp);

    printf("Comment added successfully.\n");
}

int commentCount(int postID) {
    FILE *fp = fopen(COMMENTSFILE, "r");
    if (fp == NULL) {
        printf("Error opening comments file.\n");
        return 0; // If file can't be opened, return 0 comments
    }

    int count = 0;
    int currentPostID;
    char line[1024];

    // read each line from the file and count occurrences of the given postID
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,", &currentPostID); // read the postID from the line
        if (currentPostID == postID) {
            count++;
        }
    }

    fclose(fp);

    return count; // Return the total number of comments for the postID
}
