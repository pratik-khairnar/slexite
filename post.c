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

void addPost(User* loggedUser) {
    char content[280];
    char timestamp[20];
    FILE* fp = fopen(POSTFILE, "a");
    if (fp == NULL) {
        printf("Error opening post file.\n");
        return;
    }

    printf("Enter post content (max 280 characters): ");
    getchar();  //clear the input buffer
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = '\0';  //remove trailing newline

    // replace commas in content
    for (int i = 0; content[i] != '\0'; i++) {
        if (content[i] == ',') {
            content[i] = ';';  // Replace commas with semicolons
        }
    }

    // Generate a timestamp
    getCurrentTime(timestamp, sizeof(timestamp));

    // write the post to the file
    fprintf(fp, "%d,%d,%s,%s\n", generatePostID(), loggedUser->userID, content, timestamp);

    fclose(fp);
    printf("Post added successfully.\n");
}

int generatePostID() {
    FILE *fp = fopen(POSTFILE, "r");
    if (fp == NULL) {
        // if the file doesn't exist, start with the first Post ID
        return 10000001;
    }

    int lastPostID = 10000000; //starting point
    int tempPostID, tempUserID;
    char tempContent[280], tempTimestamp[20];

    // read through the file to find the last Post ID
    while (fscanf(fp, "%d,%d,%[^,],%[^\n]", &tempPostID, &tempUserID, tempContent, tempTimestamp) == 4) {
        lastPostID = tempPostID; // Update to the latest valid Post ID
    }

    fclose(fp);

    // inc to get the new Post ID
    return lastPostID + 1;
}

Post* viewFeed(User loggedUser) {
    int choice;
    Post* feed = readPostsFromFile();

    // Check if posts were successfully loaded
    if (feed == NULL) {
        printf("No posts to display.\n");
        return NULL;
    }

    while (1) {
        printf("\n--- View Feed ---\n");
        printf("1. Sort by Time\n");
        printf("2. Sort by Likes Count\n");
        printf("3. Sort by Comments Count\n");
        printf("4. Shuffle Feed\n");
        printf("5. Return to main menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            sortFeedByTime(&feed);
            printf("Feed sorted by Time.\n");
            displayPostsMenu(feed, &loggedUser);
            break;
        case 2:
            sortFeedByLikes(&feed);
            printf("Feed sorted by Likes Count.\n");
            displayPostsMenu(feed, &loggedUser);
            break;
        case 3:
            sortFeedByComments(&feed);
            printf("Feed sorted by Comments Count.\n");
            displayPostsMenu(feed, &loggedUser);
            break;
        case 4:
            shuffleFeed(&feed);
            printf("Feed shuffled randomly.\n");
            displayPostsMenu(feed, &loggedUser);
            break;
        case 5:
            freePostList(feed);
            return NULL;  // return to main menu
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void displayPostsMenu(Post* feed, User* loggedUser) {
    int choice;
    Post* currentPost = feed;

    while (currentPost != NULL) {
        // display the current post
        displayPost(currentPost);

        printf("\n--- Post Menu ---\n");
        printf("1. Like this post\n");
        printf("2. Unlike this post\n");
        printf("3. Go to next post\n");
        printf("4. Go to previous post\n");
        printf("5. Add Comment\n");
        printf("6. View All Comments\n");
        printf("7. Follow Post Owner\n");
        printf("8. Unfollow Post Owner\n");
        printf("9. Return to Feed Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addLike(currentPost->postID, loggedUser->userID);
            break;

        case 2:
            if (hasLiked(currentPost->postID, loggedUser->userID)) {
                unlikePost(currentPost->postID, loggedUser->userID);
            } else {
                printf("You have not liked this post yet.\n");
            }
            break;

        case 3:
            currentPost = currentPost->next;  //move to the next post
            if (currentPost == NULL) {
                printf("You are at the last post. No next post.\n");
            }
            break;

        case 4:
            currentPost = currentPost->prev;  //move to the previous post
            if (currentPost == NULL) {
                printf("You are at the first post. No previous post.\n");
            }
            break;

        case 5: {
            char comment[100];
            printf("Enter your comment (max 100 characters): ");
            getchar();  // clear the newline from the input buffer
            fgets(comment, sizeof(comment), stdin);
            comment[strcspn(comment, "\n")] = '\0'; //remove trailing newline

            if (strlen(comment) == 0 || strspn(comment, " \t") == strlen(comment)) {
                printf("Error: Comment cannot be blank.\n");
            } else {
                addComment(currentPost->postID, loggedUser->userID, comment);  // Add comment
            }
            break;
        }

        case 6:
            viewComments(currentPost->postID);  //view all comments for the post
            break;

        case 7:
            if (currentPost->userID == loggedUser->userID) {
                printf("You cannot follow yourself.\n");
            } else if (hasFollowed(loggedUser->userID, currentPost->userID)) {
                printf("You are already following this user.\n");
            } else {
                followUser(loggedUser->userID, currentPost->userID); //follow the poster
            }
            break;

        case 8:
            if (hasFollowed(loggedUser->userID, currentPost->userID)) {
                unfollowUser(loggedUser->userID, currentPost->userID);  //unfollow the poster
            } else {
                printf("You are not following this user.\n");
            }
            break;

        case 9:
            return;     //return to the feed menu

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    printf("No more posts to display.\n");
}

void displayPost(Post* post) {
    if (post != NULL) {
        // get the username using userID
        char* username = getUsernameByID(post->userID);
        if (username == NULL) {
            username = "Unknown";  //username is not found
        }

        // get likes count using the existing getLikesCount func
        int likesCountValue = getLikesCount(post->postID);

        // get comments count dynamically
        int commentCountValue = commentCount(post->postID);

        printf("\n");
        printf("+-------------------------------------------------------------+\n");
        printf("|                         Post Details                        |\n");
        printf("+-------------------------------------------------------------+\n");

        // Print the username of the poster
        printf("| USERNAME: %-49s |\n", username);

        // Print the content
        printf("| CONTENT:                                                    |\n");
        printWrappedText(post->content);

        printf("| TIME : %-52s |\n", post->timestamp);
        printf("| LIKES: %-52d |\n", likesCountValue);  // from getLikesCount 
        printf("| COMMENTS: %-49d |\n", commentCountValue);  // from commentCount
        printf("+-------------------------------------------------------------+\n");
    } else {
        printf("\nNo post to display.\n");
    }
}

Post* readPostsFromFile() {
    FILE* fp = fopen(POSTFILE, "r");
    if (fp == NULL) {
        printf("Error opening post file.\n");
        return NULL;
    }

    Post* head = NULL;
    Post* tail = NULL;
    char line[1024];

    // read each line of the file
    while (fgets(line, sizeof(line), fp)) {
        Post* newPost = (Post*)malloc(sizeof(Post));
        if (newPost == NULL) {
            printf("Memory allocation failed.\n");
            fclose(fp);
            return NULL;
        }

        int postID, userID;
        char content[280], timestamp[20];
        sscanf(line, "%d,%d,%[^,],%s", &postID, &userID, content, timestamp);

        //convert semicolons back to commas
        for (int i = 0; content[i] != '\0'; i++) {
            if (content[i] == ';') {
                content[i] = ',';  //replace semicolons with commas
            }
        }
        //fill data in the newPost node
        newPost->postID = postID;
        newPost->userID = userID;
        strncpy(newPost->content, content, sizeof(newPost->content) - 1);
        strncpy(newPost->timestamp, timestamp, sizeof(newPost->timestamp) - 1);
        newPost->likesCount = 0;
        newPost->comments.count = 0;
        newPost->next = NULL;
        newPost->prev = tail;

        if (tail != NULL) {
            tail->next = newPost;
        } else {
            head = newPost;
        }

        tail = newPost;
    }

    fclose(fp);
    return head;
}

void deletePost(User* loggedUser) {
    FILE* fp = fopen(POSTFILE, "r");
    if (fp == NULL) {
        printf("Error opening posts file.\n");
        return;
    }

    FILE* temp_fp = fopen("temp_posts.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary posts file.\n");
        fclose(fp);
        return;
    }

    Post tempPost;
    int postIDs[100];  // array to store post IDs
    int postCount = 0;

    printf("\n--- Your Posts ---\n");

    // show all posts of the logged-in user
    while (fscanf(fp, "%d,%d,%[^,],%[^\n]", &tempPost.postID, &tempPost.userID, tempPost.content, tempPost.timestamp) == 4) {
        if (tempPost.userID == loggedUser->userID) {
            printf("%d. Content: %s\n   Time: %s\n\n", postCount + 1, tempPost.content, tempPost.timestamp);
            postIDs[postCount++] = tempPost.postID;
        }
    }

    // no posts were found for the user
    if (postCount == 0) {
        printf("You have no posts to delete.\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp_posts.txt");
        return;
    }

    //select a post to delete
    int choice;
    printf("\nEnter the number of the post to delete (1-%d): ", postCount);
    scanf("%d", &choice);

    if (choice < 1 || choice > postCount) {
        printf("Invalid choice. No post deleted.\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp_posts.txt");
        return;
    }

    int postIDToDelete = postIDs[choice - 1];

    // Reset fp
    rewind(fp);

    // write all posts except the one being deleted to the temp file
    while (fscanf(fp, "%d,%d,%[^,],%[^\n]", &tempPost.postID, &tempPost.userID, tempPost.content, tempPost.timestamp) == 4) {
        if (tempPost.postID != postIDToDelete) {
            fprintf(temp_fp, "%d,%d,%s,%s\n", tempPost.postID, tempPost.userID, tempPost.content, tempPost.timestamp);
        }
    }

    fclose(fp);
    //delete corresponding likes and comments
    deleteLikesForPost(postIDToDelete);
    deleteCommentsForPost(postIDToDelete);

    fclose(temp_fp);

    // replace the file
    remove(POSTFILE);
    rename("temp_posts.txt", POSTFILE);
    printf("Post deleted.\n");
}

void deleteLikesForPost(int postID) {
    FILE* fp = fopen(LIKESFILE, "r");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return;
    }

    FILE* temp_fp = fopen("temp_likes.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary likes file.\n");
        fclose(fp);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int currentPostID;
        sscanf(line, "%d,", &currentPostID);

        if (currentPostID != postID) {
            fprintf(temp_fp, "%s", line);  // write all other likes to the temp file
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(LIKESFILE);
    rename("temp_likes.txt", LIKESFILE);
}

void deleteCommentsForPost(int postID) {
    FILE* fp = fopen(COMMENTSFILE, "r");
    if (fp == NULL) {
        printf("Error opening comments file.\n");
        return;
    }

    FILE* temp_fp = fopen("temp_comments.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary comments file.\n");
        fclose(fp);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int currentPostID;
        sscanf(line, "%d,", &currentPostID);

        if (currentPostID != postID) {
            fprintf(temp_fp, "%s", line);  // write all other comments to the temp file
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(COMMENTSFILE);
    rename("temp_comments.txt", COMMENTSFILE);
}

Post* readPostByID(int postID) {
    FILE *fp = fopen(POSTFILE, "r");
    if (fp == NULL) {
        printf("Error opening posts file.\n");
        return NULL;
    }

    Post tempPost;
    Post* post = NULL;

    // Search for the post with the ID
    while (fscanf(fp, "%d,%d,%[^,],%[^\n]", &tempPost.postID, &tempPost.userID, tempPost.content, tempPost.timestamp) == 4) {
        if (tempPost.postID == postID) {
            post = (Post*)malloc(sizeof(Post));
            if (post != NULL) {
                *post = tempPost;  // store the post data
            }
            break;
        }
    }

    fclose(fp);
    return post;
}
