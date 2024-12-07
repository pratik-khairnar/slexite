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

void userMenu(User loggedUser) {
    int choice;
    Post* feed = NULL;  // feed pointer

    while (1) {
        printf("\nUser Menu:\n");
        printf("1. View Feed\n");
        printf("2. Add Post\n");
        printf("3. Delete Post\n");
        printf("4. Search User\n");
        printf("5. Delete Account\n");
        printf("6. Log Out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (feed != NULL) {
                freePostList(feed);  // free any previously loaded feed
                feed = NULL;         // pointer
            }
            feed = viewFeed(loggedUser);  // load the feed
            break;

        case 2:
            addPost(&loggedUser);  //add post
            break;

        case 3:
            deletePost(&loggedUser);  //delete post
            break;

        case 4:
            searchUser(&loggedUser);  //search for a user
            break;

        case 5:
            deleteAccount(&loggedUser);
            if (feed != NULL) {
                freePostList(feed);
                feed = NULL;
            }
            return;

        case 6:
            if (feed != NULL) {
                freePostList(feed);
                feed = NULL;
            }
            logout(loggedUser);  //log out the user
            return;  //return to main menu

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    int choice;
    User loggedUser = { .userID = -1 }; // not logged in initially
    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            SignUp();  //handle user sign up
            break;
        case 2:
            loggedUser = login();  //user login
            if (loggedUser.userID != -1) {
                userMenu(loggedUser);  // Redirect to user menu if login successful
            }
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}