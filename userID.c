#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

#define USERFILE "users.txt"

typedef struct User
{
    int userID;
    char username[16];
    char password[16];
} User;

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

void getCurrentTime(char *buffer, int bufferSize)
{
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

int main()
{
    int choice;
    while (1)
    {
        printf("\nMenu:\n1. Sign Up\n2. Login\n3. Delete Account\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            SignUp();
            break;
        case 2:
            login();
            break;
        case 3:
            deleteUser();
            break;
        case 4:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}
