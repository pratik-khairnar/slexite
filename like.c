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

void addLike(int postID, int userID) {
    // Check if user has already liked the post
    if (hasLiked(postID, userID)) {
        printf("You have already liked this post.\n");
        return;
    }

    // open the likes file to add the new like
    FILE *fp = fopen(LIKESFILE, "a");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return;
    }

    // add the like to the file (one line for each like)
    fprintf(fp, "%d,%d\n", postID, userID);

    fclose(fp);
    printf("You have liked this post.\n");
}

int getLikesCount(int postID) {
    FILE *fp = fopen(LIKESFILE, "r");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return 0;  // Return 0 if the file cant be opened
    }

    char line[1024];
    int postCount = 0;

    // read the likes.txt file line by line
    while (fgets(line, sizeof(line), fp)) {
        int id1;
        sscanf(line, "%d", &id1);  // Get the postID from each line
        if (id1 == postID) {
            postCount++;  // inc the count if the postID matched
        }
    }

    fclose(fp);
    return postCount;  //return the no. of likes for the post
}

int hasLiked(int postID, int userID) {
    FILE *fp = fopen(LIKESFILE, "r");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return 0;
    }

    char line[1024];
    int id1, id2;

    // search likes file for the userID associated with the postID
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%d", &id1, &id2);
        if (id1 == postID && id2 == userID) {
            fclose(fp);
            return 1;  // user already liked this post
        }
    }

    fclose(fp);
    return 0;  // user has not liked this post
}

void unlikePost(int postID, int userID) {
    // open the likes file for reading
    FILE *fp = fopen(LIKESFILE, "r");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return;
    }

    // create a temp file to write the modified data
    FILE *tempFp = fopen("temp_likes.txt", "w");
    if (tempFp == NULL) {
        printf("Error opening temporary likes file.\n");
        fclose(fp);
        return;
    }

    char line[1024];
    int id1;
    int found = 0;

    //go through each line in the likes file
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d", &id1);  //Read the postID from the line

        // If correct post found
        if (id1 == postID) {
            // Process the line to remove the userID. split in three token. 1st postID, 2nd ',', 3rd userID
            char *token = strtok(line, ",");  // Read the postID (first token)
            token = strtok(NULL, ",");  // Skip postID and start reading liked user ID
            int isFirst = 1;
            int isUserFound = 0;  // flag to check if the user has been removed
            int remainingLikes = 0;  // Track if there are any other likes left

            // write the other userIDs, skipping the one that is unliking
            while (token != NULL) {
                int likedUserID = atoi(token);

                if (likedUserID != userID) {
                    if (!isFirst) {
                        fprintf(tempFp, ",");
                    }
                    fprintf(tempFp, "%d", likedUserID);
                    isFirst = 0;
                    remainingLikes++;  // count the remaining likes
                } else {
                    isUserFound = 1;  // user has been removed
                }

                token = strtok(NULL, ",");
            }

            if (remainingLikes > 0) {
                // If there are remaining likes, write the updated line with likes
                fprintf(tempFp, "\n");
            }

            // If no likes remain, dont write this line at all (remove the post entirely)
            if (!isUserFound || remainingLikes == 0) {
                found = 1;
            }
        } else {
            // If it's not the correct post, just copy the line to the temp file
            fputs(line, tempFp);
        }
    }

    fclose(fp);
    fclose(tempFp);

    // If the post was found and updated
    if (found) {
        remove(LIKESFILE);
        rename("temp_likes.txt", LIKESFILE);
        printf("You have unliked the post.\n");
    } else {
        remove("temp_likes.txt");
        printf("Post not found in likes file.\n");
    }
}
