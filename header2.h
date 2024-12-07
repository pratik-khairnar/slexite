#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "users.txt"
#define POSTFILE "posts.txt"
#define LIKESFILE "likes.txt"
#define COMMENTSFILE "comments.txt"
#define FOLLOWERFILE "followers.txt"
#define FOLLOWINGFILE "following.txt"

typedef struct listStruct { // Same struct for likes/followers/following
    int *ids;               // Array of IDs (e.g., user IDs)
    int count;              // Total count of IDs
} listStruct;

typedef struct commentStruct {
    int *ids;               // Array of user IDs who commented
    char **comments;        // Array of comments (each comment is a string)
    int count;              // Total number of comments
} commentStruct;

typedef struct Post {
    int postID;                 // Unique ID for the post
    int userID;                 // ID of the user who created the post
    char content[280];          // Post content (up to 280 characters)
    char timestamp[20];         // Timestamp in format: "DD/MM/YYYY HH:MM"
    int likesCount;             // Total number of likes
    listStruct likes;           // List of user IDs who liked the post
    commentStruct comments;     // Comments on the post
    struct Post* next;
    struct Post* prev;
} Post;

typedef struct User {
    int userID;                 // Unique user ID
    char username[16];          // Username
    char password[16];          // Password
    listStruct followers;       // List of followers
    listStruct following;       // List of following users
    int totalPost;              // Total number of posts made by the user
} User;

int checkUsernameExists(char username[]);
void AddUser(User user);
int getLastUser();
User SignUp();
void getCurrentTime(char *buffer, int bufferSize);
User LoadUser(char username[], char pass[]);
User login();
void logout(User user);
void freeUserData(User *user);
int generatePostID();
void addLike(int postID, int userID);
void deleteLikesForPost(int postID);
void deleteCommentsForPost(int postID);
void deletePost(User *loggedUser);
Post* readPostsFromFile();
void shuffleFeed(Post** head);
void freePostList(Post* head);
int compareByTime(const void *a, const void *b);
void sortFeedByTime(Post** head);
int compareByLikes(const void *a, const void *b);
void sortFeedByLikes(Post** head);
int compareByComments(const void *a, const void *b);
void sortFeedByComments(Post** head);
void printWrappedText(const char* text);
void displayPost(Post *head);
void deleteUserFromFile(int userID);
void deleteUserPosts(int userID);
void deleteUserLikes(int userID);
void deleteUserComments(int userID);
void deleteAccount(User *loggedUser);
void userMenu(User loggedUser);
void addPost(User *loggedUser);
Post* viewFeed(User loggedUser);
void displayPostsMenu(Post* feed, User* loggedUser);
Post* readPostsFromFile();
void displayPost(Post* post);
int getLikesCount(int postID);
void addComment(int postID, int userID, const char* comment);
int commentCount(int postID);
void viewComments(int postID);
char* getUsernameByID(int userID);
void deletePost(User* loggedUser);
void deleteLikesForPost(int postID);
void deleteCommentsForPost(int postID);
void followUser(int followerID, int followeeID);
int countFollowers(int userID);
int countFollowing(int userID);
void unfollowUser(int followerID, int followeeID);
void unlikePost(int postID, int userID);
int hasFollowed(int followerID, int followeeID);
int hasLiked(int postID, int userID);
void searchUser(User* loggedUser);
void viewProfile(int userID, User* loggedUser);
Post* readPostByID(int postID);
