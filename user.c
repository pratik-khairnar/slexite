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

int checkUsernameExists(char username[]){
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL) {
        return 0;
    }
    User user;
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF) {
        if (strcmp(user.username, username) == 0){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void AddUser(User user){
    FILE *fp = fopen(USERFILE, "a");
    if (fp == NULL){
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%d,%s,%s\n", user.userID, user.username, user.password);
    fclose(fp);
}

int getLastUser(){
    User user;
    int lastID = 10000000;
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL) {
        return lastID;
        ;
    } while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF) {
        lastID = user.userID;
    }
    fclose(fp);
    return lastID;
}

User SignUp() {
    User newUser;
    char username[16], password[16];

    // ask user to enter details
    while (1) {
        printf("Enter a username (minimum 4 characters, max 15): ");
        scanf("%s", username);

        if (strlen(username) < 4) {
            printf("Error: Username must be at least 4 characters long.\n");
            continue;
        }

        if (checkUsernameExists(username)) {
            printf("Error: Username '%s' already exists. Try a different one.\n", username);
            continue;
        }

        strncpy(newUser.username, username, sizeof(newUser.username) - 1);
        newUser.username[sizeof(newUser.username) - 1] = '\0';
        break;
    }

    // ask user to enter passwrd
    while (1) {
        printf("Enter a password (minimum 8 characters, max 15): ");
        scanf("%s", password);

        if (strlen(password) < 8) {
            printf("Error: Password must be at least 8 characters long.\n");
            continue;
        }

        strncpy(newUser.password, password, sizeof(newUser.password) - 1);
        newUser.password[sizeof(newUser.password) - 1] = '\0';
        break;
    }

    // create new userID
    newUser.userID = getLastUser() + 1;
    newUser.followers.count = 0;
    newUser.following.count = 0;
    newUser.totalPost = 0;

    AddUser(newUser);

    printf("User created successfully! Your username is '%s'.\n",
           newUser.username, newUser.userID);

    return newUser;
}

User LoadUser(char username[], char pass[]){
    User user;
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL){
        printf("Error opening file\n");
        user.userID = -1;
        return user;
    } while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) == 3) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, pass) == 0) {
            fclose(fp);
            return user;
        }
    }
    fclose(fp);
    user.userID = -1;
    return user;
}

User login(){
    User userAcc;
    char username[16], password[16];
    while (1){
        printf("Please enter your username: ");
        scanf("%s", username);
        if (!checkUsernameExists(username)){
            printf("Username is incorrect. Try again\n");
            continue;
        } else {
            break;
        }
    }
    printf("Please enter your password: ");
    scanf("%s", password);
    userAcc = LoadUser(username, password);
    if (userAcc.userID == -1) {
        printf("Invalid username or password. Try Again\n");
    } else {
        printf("Welcome, %s!\n", username);
    }
    return userAcc;
}

void logout(User user){
    user.userID = -1;
    return;
}

void deleteUserFromFile(int userID) {
    FILE *fp = fopen(USERFILE, "r+");
    if (fp == NULL) {
        printf("Error opening user file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp_users.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary user file.\n");
        fclose(fp);
        return;
    }

    User user;
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF) {
        if (user.userID != userID) {
            fprintf(temp_fp, "%d,%s,%s\n", user.userID, user.username, user.password);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(USERFILE);
    rename("temp_users.txt", USERFILE);
    printf("User deleted successfully.\n");
}

void deleteAccount(User *loggedUser) {
    // Delete user data and its data
    deleteUserFromFile(loggedUser->userID);
    deleteUserPosts(loggedUser->userID);
    deleteUserLikes(loggedUser->userID);
    deleteUserComments(loggedUser->userID);

    printf("Your account has been deleted successfully.\n");
}

char* getUsernameByID(int userID) {
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL) {
        printf("Error opening user file.\n");
        return NULL;
    }

    static char username[16];  // buffer to store the username

    User user;
    // Search for the userID in the users file
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF) {
        if (user.userID == userID) {
            strcpy(username, user.username);  // store the username in buffer
            fclose(fp);
            return username;
        }
    }

    fclose(fp);
    return NULL;  //userID is not found
}

void searchUser(User* loggedUser) {
    char searchUsername[16];
    printf("Enter the username to search: ");
    scanf("%s", searchUsername);

    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL) {
        printf("Error opening user file.\n");
        return;
    }

    int userID;
    char username[16], password[16];
    int found = 0;

    // Search for the username in users.txt file
    while (fscanf(fp, "%d,%[^,],%s", &userID, username, password) == 3) {
        if (strcmp(username, searchUsername) == 0) {
            found = 1;
            fclose(fp);
            printf("\nUser '%s' found!\n", username);
            viewProfile(userID, loggedUser);  // Call viewProfile func for found userID
            return;
        }
    }

    fclose(fp);

    if (!found) {
        printf("User '%s' not found.\n", searchUsername);
    }
}

void viewProfile(int userID, User* loggedUser) {
    char* username = getUsernameByID(userID);
    if (username == NULL) {
        printf("Error: User not found in user database.\n");
        return;
    }

    // get follower and following counts
    int followerCount = countFollowers(userID);
    int followingCount = countFollowing(userID);

    // Display profile
    printf("\n");
    printf("+-------------------------------------------------------------+\n");
    printf("|                      User Profile                           |\n");
    printf("+-------------------------------------------------------------+\n");
    printf("| USERNAME: %-49s |\n", username);
    printf("| FOLLOWERS: %-48d |\n", followerCount);
    printf("| FOLLOWING: %-48d |\n", followingCount);
    printf("+-------------------------------------------------------------+\n");

    // Filter posts by the user
    Post* allPosts = readPostsFromFile();  // Load all posts in a linkedlist
    Post* filteredPosts = NULL;
    Post* tail = NULL;

    while (allPosts != NULL) {
        if (allPosts->userID == userID) {
            Post* newPost = (Post*)malloc(sizeof(Post)); //alloc memory 
            if (newPost == NULL) {
                printf("Memory allocation failed.\n");
                freePostList(filteredPosts);
                return;
            }

            *newPost = *allPosts;  // Copy the post data
            newPost->next = NULL;

            if (tail != NULL) {
                tail->next = newPost;
            } else {
                filteredPosts = newPost;  // First post in the list
            }
            tail = newPost;
        }
        allPosts = allPosts->next;
    }

    // Call displayPostsMenu to interact with the filtered posts
    displayPostsMenu(filteredPosts, loggedUser);

    // free memory
    freePostList(filteredPosts);
}

void followUser(int followerID, int followeeID) {
    // if the follower is trying to follow themselves
    if (followerID == followeeID) {
        printf("You cannot follow yourself.\n");
        return;
    }

    // Get the username of the followee
    char* followeeUsername = getUsernameByID(followeeID);
    if (followeeUsername == NULL) {
        printf("Error: Could not find user.\n");
        return;
    }

    // if already following
    FILE *followingFp = fopen(FOLLOWINGFILE, "r");
    if (followingFp != NULL) {
        int id1, id2;
        while (fscanf(followingFp, "%d,%d", &id1, &id2) == 2) {
            if (id1 == followerID && id2 == followeeID) {
                printf("You are already following %s.\n", followeeUsername);
                fclose(followingFp);
                return;
            }
        }
        fclose(followingFp);
    }

    // add to the `following.txt` file
    followingFp = fopen(FOLLOWINGFILE, "a");
    if (followingFp == NULL) {
        printf("Error opening following file.\n");
        return;
    }
    fprintf(followingFp, "%d,%d\n", followerID, followeeID);
    fclose(followingFp);

    // Add to `followers.txt` file
    FILE *followersFp = fopen(FOLLOWERFILE, "a");
    if (followersFp == NULL) {
        printf("Error opening followers file.\n");
        return;
    }
    fprintf(followersFp, "%d,%d\n", followeeID, followerID);
    fclose(followersFp);

    printf("You are now following %s.\n", followeeUsername);
}

int countFollowers(int userID) {
    FILE *followersFp = fopen(FOLLOWERFILE, "r");
    if (followersFp == NULL) {
        printf("Error opening followers file.\n");
        return -1;  // error
    }

    int id1, id2;
    int followerCount = 0;

    // Read the followers file and counts how many times userID appears as a followee
    while (fscanf(followersFp, "%d,%d", &id1, &id2) == 2) {
        if (id1 == userID) {
            followerCount++;
        }
    }

    fclose(followersFp);
    return followerCount;
}

int countFollowing(int userID) {
    FILE *followingFp = fopen(FOLLOWINGFILE, "r");
    if (followingFp == NULL) {
        printf("Error opening following file.\n");
        return -1;
    }

    int id1, id2;
    int followingCount = 0;

    // Read the following file and counts how many times userID appears as a follower
    while (fscanf(followingFp, "%d,%d", &id1, &id2) == 2) {
        if (id1 == userID) {
            followingCount++;
        }
    }

    fclose(followingFp);
    return followingCount;
}

void unfollowUser(int followerID, int followeeID) {
    // if the follower is trying to unfollow themselves
    if (followerID == followeeID) {
        printf("You cannot unfollow yourself.\n");
        return;
    }

    // Get the username of the followee
    char* followeeUsername = getUsernameByID(followeeID);
    if (followeeUsername == NULL) {
        printf("Error: Could not find usern.\n");
        return;
    }

    int foundInFollowing = 0, foundInFollowers = 0;

    // Open the following.txt file and remove the data
    FILE *followingFp = fopen(FOLLOWINGFILE, "r");
    if (followingFp == NULL) {
        printf("Error opening following file.\n");
        return;
    }

    FILE *tempFp = fopen("temp.txt", "w");
    if (tempFp == NULL) {
        printf("Error opening temporary file.\n");
        fclose(followingFp);
        return;
    }

    char line[1024];
    int id1, id2;

    // Read the following file and filter out the relationship
    while (fgets(line, sizeof(line), followingFp)) {
        sscanf(line, "%d,%d", &id1, &id2);  // get userIDs

        // If the relationship is found, dont write that line to temp
        if (id1 == followerID && id2 == followeeID) {
            foundInFollowing = 1;
        } else {
            fputs(line, tempFp);  // Keep the line if not the one to delete
        }
    }

    fclose(followingFp);
    fclose(tempFp);

    if (foundInFollowing) {
        // Replace the original file with the updated one
        remove(FOLLOWINGFILE);
        rename("temp.txt", FOLLOWINGFILE);
    } else {
        // if not found, delete the temp file
        remove("temp.txt");
        printf("You are not following %s.\n", followeeUsername);
        return;
    }

    // Now open the followers.txt file to remove the relationship
    FILE *followersFp = fopen(FOLLOWERFILE, "r");
    if (followersFp == NULL) {
        printf("Error opening followers file.\n");
        return;
    }

    tempFp = fopen("temp.txt", "w");
    if (tempFp == NULL) {
        printf("Error opening temporary file.\n");
        fclose(followersFp);
        return;
    }

    // Read the followers file and filter data
    while (fgets(line, sizeof(line), followersFp)) {
        sscanf(line, "%d,%d", &id1, &id2);

        // If the relationship is found, skip writing that line to temp
        if (id1 == followeeID && id2 == followerID) {
            foundInFollowers = 1;
        } else {
            fputs(line, tempFp);  // everyline except the one to delete.
        }
    }

    fclose(followersFp);
    fclose(tempFp);

    if (foundInFollowers) {
        // Replace the original file with the updated one
        remove(FOLLOWERFILE);
        rename("temp.txt", FOLLOWERFILE);
        printf("You have unfollowed %s.\n", followeeUsername);
    } else {
        remove("temp.txt");
        printf("You were not following %s.\n", followeeUsername);
    }
}

int hasFollowed(int followerID, int followeeID) {
    FILE *fp = fopen(FOLLOWINGFILE, "r");
    if (fp == NULL) {
        printf("Error opening following file.\n");
        return 0;
    }

    int id1, id2;
    while (fscanf(fp, "%d,%d", &id1, &id2) == 2) {
        if (id1 == followerID && id2 == followeeID) {
            fclose(fp);
            return 1;  // Found follow entry
        }
    }

    fclose(fp);
    return 0;  // Not found
}

void deleteUserPosts(int userID) {
    FILE *fp = fopen(POSTFILE, "r+");
    if (fp == NULL) {
        printf("Error opening post file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp_posts.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary post file.\n");
        fclose(fp);
        return;
    }

    Post post;
    while (fscanf(fp, "%d,%d,%[^,],%s", &post.postID, &post.userID, post.content, post.timestamp) != EOF) {
        if (post.userID != userID) {
            fprintf(temp_fp, "%d,%d,%s,%s\n", post.postID, post.userID, post.content, post.timestamp);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(POSTFILE);
    rename("temp_posts.txt", POSTFILE);
    printf("User's posts deleted successfully.\n");
}

void deleteUserLikes(int userID) {
    FILE *fp = fopen(LIKESFILE, "r+");
    if (fp == NULL) {
        printf("Error opening likes file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp_likes.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary likes file.\n");
        fclose(fp);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int currentPostID;
        char *token = strtok(line, ",");
        sscanf(token, "%d", &currentPostID);
        fprintf(temp_fp, "%d", currentPostID);

        token = strtok(NULL, ",");
        while (token != NULL) {
            int likeUserID;
            sscanf(token, "%d", &likeUserID);
            if (likeUserID != userID) {
                fprintf(temp_fp, ",%d", likeUserID);
            }
            token = strtok(NULL, ",");
        }
        fprintf(temp_fp, "\n");
    }

    fclose(fp);
    fclose(temp_fp);

    remove(LIKESFILE);
    rename("temp_likes.txt", LIKESFILE);
    printf("User's likes deleted successfully.\n");
}

void deleteUserComments(int userID) {
    FILE *fp = fopen(COMMENTSFILE, "r+");
    if (fp == NULL) {
        printf("Error opening comments file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp_comments.txt", "w");
    if (temp_fp == NULL) {
        printf("Error opening temporary comments file.\n");
        fclose(fp);
        return;
    }

    int currentPostID;
    int currentUserID;
    char currentComment[1000];
    while (fscanf(fp, "%d,%d,%[^\n]\n", &currentPostID, &currentUserID, currentComment) != EOF) {
        if (currentUserID != userID) {
            fprintf(temp_fp, "%d,%d,%s\n", currentPostID, currentUserID, currentComment);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(COMMENTSFILE);
    rename("temp_comments.txt", COMMENTSFILE);
    printf("User's comments deleted successfully.\n");
}
