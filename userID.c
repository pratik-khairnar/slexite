#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

#define USERFILE "users.txt"
#define MAX_POST_LENGTH 256
#define MAX_COMMENT_LENGTH 256
#define POST_FILE_PREFIX "posts_"

typedef struct User
{
    int userID;
    char username[16];
    char password[16];
} User;

typedef struct post {
    char content[MAX_POST_LENGTH];
    int like;
    int commentCount;
    char timestamp[20];
} Post;

int checkUsernameExists(char username[])
{
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL)
    {
        return 0;
    }

    User user;
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void AddUser(User user)
{
    FILE *fp = fopen(USERFILE, "a");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%d,%s,%s\n", user.userID, user.username, user.password);
    fclose(fp);
}

int getLastUser()
{
    User user;
    int lastID = 10000000;
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL)
    {
        return lastID;
        ;
    }
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF)
    {
        lastID = user.userID;
    }
    fclose(fp);
    return lastID;
}

User SignUp()
{
    User userAcc;
    int userID = getLastUser();
    char tempPass[16], tempPass2[16];
    userAcc.userID = userID + 1;
    while (1)
    {
        printf("Please enter a username: ");
        scanf("%s", userAcc.username);
        if (checkUsernameExists(userAcc.username))
        {
            printf("Username is already taken. Try another username.\n");
            continue;
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("Please enter a password: ");
        scanf("%s", tempPass);
        printf("Please re-enter the password: ");
        scanf("%s", tempPass2);

        if (strcmp(tempPass, tempPass2) == 0)
        {
            strcpy(userAcc.password, tempPass);
            printf("User signed up successfully!\n");
            break;
        }
        else
        {
            printf("Passwords do not match. Please try again.\n");
        }
    }
    AddUser(userAcc);
    return userAcc;
}

void getCurrentTime(char *buffer, int bufferSize) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, bufferSize, "%d/%m/%Y, %H:%M", timeinfo);
}

User LoadUser(char username[], char pass[])
{
    User user;
    FILE *fp = fopen(USERFILE, "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        user.userID = -1;
        return user;
    }
    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) == 3)
    {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, pass) == 0)
        {
            fclose(fp);
            return user;
        }
    }
    fclose(fp);
    user.userID = -1;
    return user;
}

User login()
{
    User userAcc;
    char username[16], password[16];
    while (1)
    {
        printf("Please enter your username: ");
        scanf("%s", username);
        if (!checkUsernameExists(username))
        {
            printf("Username is incorrect. Try again\n");
            continue;
        }
        else
        {
            break;
        }
    }
    printf("Please enter your password: ");
    scanf("%s", password);
    userAcc = LoadUser(username, password);
    if (userAcc.userID == -1)
    {
        printf("Invalid username or password. Try Again\n");
    }
    else
    {
        printf("Welcome, %s!\n", username);
    }
    return userAcc;
}

void logout(User user)
{
    user.userID = -1;
    return;
}

void deleteUser()
{
    User loggedUser = login();
    int confirm;
    if (loggedUser.userID == -1)
    {
        printf("You must be logged in to delete an account.\n");
        return;
    }
    printf("Are you sure you want to delete the account ?\nType 1 to confirm, 0 to exit\n");
    scanf("%d", &confirm);
    if (confirm == 0)
    {
        printf("Account deletion canceled.\n");
        return;
    }
    FILE *fp = fopen(USERFILE, "r+");
    if (fp == NULL)
    {
        printf("Error opening user file!\n");
        return;
    }

    User user;
    FILE *temp_fp = fopen("temp_users.txt", "w");
    if (temp_fp == NULL)
    {
        printf("Error opening temporary file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d,%[^,],%s", &user.userID, user.username, user.password) != EOF)
    {
        if (user.userID != loggedUser.userID)
        {
            fprintf(temp_fp, "%d,%s,%s\n", user.userID, user.username, user.password);
        }
    }

    fclose(fp);
    fclose(temp_fp);
    remove(USERFILE);
    rename("temp_users.txt", USERFILE);
    printf("Account for user %s deleted successfully.\n", loggedUser.username);
}

// int main(){
//     User user = SignUp();
//     printf("New user created; userID: %d, username: %s\n", user.userID, user.username);

//     char currentTime[20];
//     getCurrentTime(currentTime, sizeof(currentTime));
//     printf("Current Time: %s\n", currentTime);
//     return 0;

// }

void displayPosts(int userID) {
    char postFileName[32];
    sprintf(postFileName, "%s%d.txt", POST_FILE_PREFIX, userID);  // Generate the filename posts_<userID>.txt

    FILE *fp = fopen(postFileName, "r");
    if (fp == NULL) {
        printf("No posts found for user %d.\n", userID);
        return;
    }

    char line[512];
    printf("\n--- Your Posts ---\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);  // Display each line from the file
    }
    printf("\n--- End of Posts ---\n");

    fclose(fp);  // Close the file
}

void addPost(int userID, char postContent[]) {
    char postFileName[32];
    sprintf(postFileName, "%s%d.txt", POST_FILE_PREFIX, userID);  // Generate the filename posts_<userID>.txt

    FILE *fp = fopen(postFileName, "a");
    if (fp == NULL) {
        printf("Error opening post file for user %d!\n", userID);
        return;
    }

    // Create a new post
    Post newPost;
    strcpy(newPost.content, postContent);
    newPost.like = 0;  // Initialize likes to 0
    newPost.commentCount = 0;  // Initialize comment count to 0
    getCurrentTime(newPost.timestamp, sizeof(newPost.timestamp));  // Get the current timestamp

    // Write the post details to the file
    fprintf(fp, "Post: %s\n", newPost.content);
    fprintf(fp, "Timestamp: %s\n", newPost.timestamp);
    fprintf(fp, "Likes: %d\n", newPost.like);
    fprintf(fp, "Comments: %d\n\n", newPost.commentCount);

    fclose(fp);  
    printf("Post added successfully for user %d!\n", userID);
}

// Assume the functions: SignUp(), login(), deleteUser(), addPost() are already defined

int main() {
    int choice;
    User loggedInUser;  // Store the logged-in user's data
    int isLoggedIn = 0;  // Flag to check if user is logged in

    while (1) {
        if (!isLoggedIn) {
            // Menu for Sign Up, Login, Delete Account when not logged in
            printf("\nMenu:\n1. Sign Up\n2. Login\n3. Delete Account\n4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1:
                SignUp();  // Call SignUp function to register a new user
                break;
            case 2:
                loggedInUser = login();  // Call login function to authenticate the user
                if (loggedInUser.userID != -1) {  // If login is successful
                    isLoggedIn = 1;  // Mark user as logged in
                } else {
                    printf("Login failed. Please try again.\n");
                }
                break;
            case 3:
                deleteUser();  // Call function to delete user account
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);  // Exit the program
            default:
                printf("Invalid choice. Please try again.\n");
            }
        } else {
            // Menu for users after they have logged in
            printf("\nWelcome, %s!\n", loggedInUser.username);
            printf("\nLogged-In Menu:\n1. Add Post\n2. Display My Posts\n3. Logout\n4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1: {
                // Add a new post
                char postContent[MAX_POST_LENGTH];
                printf("Enter your post: ");
                getchar();  // Consume the newline character left by the previous input
                fgets(postContent, MAX_POST_LENGTH, stdin);
                postContent[strcspn(postContent, "\n")] = 0;  // Remove the newline character from the input

                addPost(loggedInUser.userID, postContent);  // Call the addPost function to save the post
                break;
            }
            case 2:
                // Display all posts of the logged-in user
                displayPosts(loggedInUser.userID);
                break;
            case 3:
                // Logout user
                printf("Logging out...\n");
                isLoggedIn = 0;  // Reset the logged-in flag
                break;
            case 4:
                // Exit the program
                printf("Exiting...\n");
                exit(0);  // Exit the application
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
}
