---

# Social Media Feed Organizer

This project is a **Terminal-based Social Media Feed Organizer** that simulates basic social media functionalities such as user management, post creation, and organizing feeds based on various metrics like time, likes, and comments. The project is implemented in **C** and demonstrates the use of data structures like doubly linked lists, arrays, and file I/O for persistent storage.

---

## Table of Contents

- [Features](#features)  
- [Setup](#setup)  
- [Usage](#usage)  
- [Files and Directories](#files-and-directories)  
- [Data Structures](#data-structures)  
- [Sorting Algorithms](#sorting-algorithms)  
- [Future Enhancements](#future-enhancements)

---

## Features

1. **User Management**
   - Create, search, and delete user profiles.
   - Users have unique usernames and passwords.
   - Users can follow/unfollow others, with relationships tracked in `followers.txt` and `following.txt`.

2. **Post Management**
   - Users can create posts (up to 280 characters).
   - Posts include timestamps, likes, and comments.
   - Persistent storage for posts in `posts.txt`.

3. **Feed Organization**
   - Sort posts by:
     - **Time**: Most recent first.
     - **Likes**: Posts with the highest likes first.
     - **Comments**: Posts with the highest comments first.
   - Shuffle feed randomly for variety.

4. **Likes and Comments**
   - Fetch and display dynamic like and comment counts using external files:
     - `likes.txt` for tracking likes.
     - `comments.txt` for tracking comments.

5. **Follow System**
   - Track followers and following relationships using:
     - `followers.txt`: Stores user IDs of followers for each user.
     - `following.txt`: Stores user IDs of users followed by a specific user.

6. **File I/O**
   - Persistent storage of posts, likes, comments, and user relationships in external files.
   - Dynamic fetching ensures data remains updated.

---

## Setup

### Prerequisites
- A C compiler (e.g., GCC).
- Basic understanding of C and data structures.

### Steps
1. Clone the repository:  
   ```bash
   git clone <repository_url>
   cd social-media-feed-organizer
   ```
2. Compile the program:  
   ```bash
   gcc -o organizer main.c -std=c99
   ```
3. Run the program:  
   ```bash
   ./organizer
   ```

---

## Usage

1. **Log in or Register**: Start by creating a new user profile or logging in with existing credentials.  
2. **Create Posts**: Add posts with content and automatically generate timestamps.  
3. **Like/Comment**: Simulate liking and commenting on posts.  
4. **Follow/Unfollow**: Build a network by following or unfollowing other users.  
5. **Organize Feed**: Choose between sorting options or shuffle the feed.  
6. **Exit**: Exit the program, saving all data for future use.

---

## Files and Directories

| File             | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `main.c`         | Contains the main logic for user interaction and feed organization.         |
| `users.txt`      | Stores user data, including usernames, passwords, and basic information.    |
| `posts.txt`      | Stores post data, including content, timestamps, and post IDs.              |
| `likes.txt`      | Tracks likes associated with post IDs and user IDs.                         |
| `comments.txt`   | Tracks comments associated with post IDs and user IDs.                      |
| `followers.txt`  | Tracks followers of each user, storing user relationships.                  |
| `following.txt`  | Tracks users followed by each user.                                         |

---

## Data Structures

### 1. **Doubly Linked List**  
   - Used to store and manage posts dynamically.
   - Allows efficient traversal in both forward and backward directions.

### 2. **Arrays**
   - Used in `listStruct` for tracking likes, followers, and following relationships.

### 3. **Custom Structs**
   - `Post`: Represents a post, containing fields for content, timestamp, likes, comments, etc.  
   - `User`: Represents a user, containing fields for username, password, and follow relationships.  
   - `listStruct`: Tracks IDs for likes, followers, and following.  
   - `commentStruct`: Tracks user IDs and comments associated with posts.

### 4. **File I/O**
   - Used for storing and retrieving persistent data for users, posts, likes, comments, and follow relationships.

---

## Sorting Algorithms

### Sorting by Time
- Posts are sorted using timestamps in descending order (most recent posts first).

### Sorting by Likes
- Posts are dynamically sorted by fetching like counts from `likes.txt`.

### Sorting by Comments
- Posts are dynamically sorted by fetching comment counts from `comments.txt`.

---

## Future Enhancements

1. **Search Feature**: Add functionality to search posts by keywords.
2. **User Recommendations**: Suggest users to follow based on shared interests.
3. **Hashtags and Trends**: Introduce hashtags and display trending topics.
4. **Enhanced Performance**: Optimize file I/O operations for large datasets.
5. **Graphical UI**: Migrate from terminal-based to a graphical interface.

---

## Contributors

- **Pratik**: Concept, development, and implementation.  

Feel free to fork and contribute to this project. Suggestions and pull requests are always welcome!

--- 
