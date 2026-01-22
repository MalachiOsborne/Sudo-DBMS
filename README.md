# Pseudo-DBMS
### Video Demo: https://www.youtube.com/watch?v=t0hPnMyjVKo
### A Pseudo Database Management System Written in C
A simple DBMS that accepts a `.csv` file as a command-line input. The `.csv` must include the headers `id`,`name` and `age`. The reason it's called "pseudo" is because it doesn't allow for columns to be created/inserted, updated, searched and deleted. 
It has 4 modes: Search, Insert, Update and Delete (along with an exit)
### Search Mode
In search mode, you can search using an id, name, age or display all entries.
### Insert Mode
In insert mode, you can insert a name and an age and an id will automatically be assigned.
### Update Mode
In update mode, you can update an id, name or age. An error message displays if: the id being updated doesn't exist, the new id is already being used, name wasn't found, age wasn't found. If naming/age conflicts exist, the program handles that by asking for the id of the desired entry to be altered.
### Delete Mode
In delete mode, you delete an entry using its id, name or age. An error message displays if id/name/age aren't found. If naming/age conflicts exist, the program also handles it by asking for an id. Then a confirmation message appears.

## History of Making the Project
The project started out as a simple tracker: it loads the `.csv` into a linked list and it allows you to see what's going on in the `.csv` file. I started out with 3 files: `main.c`, `llf.c` where I have all the linked list functions and `llf.h`. As I was making the create node function, I realised that I could expand the project to not only be able to search but to insert, delete and update the `.csv` as well. This lead to the idea of making a pseudo DBMS in C. 

## Difficulties During the Making
### Load function
The load function was one of the hardest parts of this project because I wouldn't be able to debug anything if I couldn't even get access to the `.csv` file. I wanted to use `fread` for reading the headers, but after i utilized `fgets` it made everything simpler. However, to try and stay true to my objective, I read the rest of the file using `fread` (which upon looking back is bad since I'm doing a syscall every time I call `fread` but that's an improvement to make later). In the current project, the `headers` array is obsolete. This was unintentional. Unfortunately, I didn't have the time nor the knowledge to be able to configure the struct to include the headers whatever their permutation. 
### File size
As i was implementing more and more functions into `llf.c`, it was becoming clearer and clearer not all the functions belong in the same file. When the file got to 1000+ lines, it became quite tedious to debug. At that point, I decided to split it into two files: `llf.c` where the helper linked list functions lie and the `dbmsf.c` where the DBMS functions that use the linked list functions lie. 
### Compatibility
Since I own two systems that I use frequently: Windows 10 and Arch Linux, switching back and forth was a nightmare to say the least. Refactoring my code to fit both systems was downright unacceptable. Therefore, I added the `windows_compatibility.h` file for my project to work on both Windows based systems as well as UNIX based ones. Note: I haven't tested it on MacOS so I am not sure whether it works there or not.

# Technical Aspects of the Project
## `llf.c`

### `create_node` function and the `node` struct
In this file, I implemented all the linked list functions necessary for my project to work. First and foremost, the struct that builds upon everything. It used to contain char buffers namely `char name[100]` and `char age[4]` to be able to include the age and name. I decided to make the age a string because there's no need to perform calculations using the number once it gets inserted into the struct. The char buffers were decent but I wanted as much flexibility as possible. This lead to making them both `char*` and `malloc`ing the memory needed. I made sure to free everything when deleting.

### `append_node` and `insert_after_x`
Those two functions are essential in building up the linked list, especially `append_node`. `insert_after_x` function was really important keeping the order in the linked list once there was a node deletion as well as keeping the `.csv` file organized by id.

### `delete_node` and `delete_first_node`
The `delete_node` function is important for deleting an entry. I was very careful to free any `malloc`s I made previously. The `delete_first_node` function was really only used for the `unload` function in `dbmsf.c`. 

### Display functions and update functions
The display functions are used by `search_options`, `update_options` and `delete_options` functions. They helped print the entry to the terminal by id, name or age respectively. They also helped the error handling logic by returning a boolean value (for `display_entry_by_id` function, since there may only be 1 of any id) or an int (for the other two functions because there may be multiple entries with the same name/age). 

### `generate_id`
This function was very important in keeping the id count low. Because deletion of an entry removes the id, the `.csv` would reach upwards of thousands of ids if there was constant deletion and insertion of the same entry. The `generate_id` function generates an id that is not a part of the `ids` array and filling the gaps if there are any.

## `dbmsf.c`

### Macros
Most of the macros are used to `#include` libraries necessary to perform the tasks the project demands. The most glaring macro is the `#define` one. This is a super interesting C property that I used to replicate a default argument in C++.

### Global Variables
There are 4 global variables: `ids` int array to store the ids in the linked list (looking back it was a nightmare to try and sync the `ids` array with the linked list, so this was a poor design decision but I digress), `ids_counter` to store the number of ids, `headers` char* array which is currently unused, but the purpose was to use it in the linked list struct to have as many headers as there were in any permutation; however, I wasn't able to implement it due to it being so early in the project that I didn't even implement the load function yet, and I didn't want to get stuck on an unnecessarily complicated task. For the most important global variable: `headsofheads`. `headofheads` is where the linked list structure lives that gives rise to everything in the project. The name comes from a naming collision in a function since I named a parameter "head" and the original global variable also "head". This sparked the idea that the global variable is the "head" of all the other (temporary) heads. Finally, `loaded` global bool variable is used to determine whether the `.csv` file was fully loaded or not in the `unload` function. 

### `search_options`
I implemented 3 ways to search (namely id, name and age) and an exit. The very outer while loop always runs until the user decides to exit search mode. I then use `scanf` to read a single character case-insensitively (although multiple characters can be input which I didn't handle unfortunately. This is true for all the other `scanf` uses). The next while loop is used to handle any incorrect input. Afterwards, a switch statement is used to redirect the user to whichever one of the options they chose. An interesting part of the switch statement is `case 'n'`. Since the function `display_entry_by_name` accepts 3 parameters, I used a macro to simulate a default argument (as discussed earlier). The reason `display_entry_by_name` has 3 parameters will be discussed in Update options later.

### `insert_options`
When choosing the insert option, I didn't really handle any exit strategy. The assumption is that if an incorrect entry is input, the user can delete it. During the making of `insert_options`, I learned the existence of scansets, which were a super interesting use of `scanf`. Unfortunately, it is possible that a user inputs a name as an age and an age as a name because of how the `node` struct is defined and there is no error handling for that. Afterwards, I call `generate_id` to generate an `id` that isn't a part of the list, and if there were any gaps between ids, it would fill it. This prevents any user misinput, keeps the number of ids relatively low and doesn't prompt the user with providing a unique identifier (imagine having a `.csv` with a thousand entries and having to add an id that doesn't conflict with any of them). Because of how `create_node` was defined as a result of the `load` function, one of the parameters is an integer array. This is why the generated id is stored in an int array with only 1 element. At this point, I didn't implement `update_csv` function yet so I manually opened a file and appended the entry myself. I didn't get rid of that part because I didn't feel like it. I still call `update_csv` silently at the end of the function just to keep the order in the `.csv`. Finally, the user is prompted whether they want to add any more entries.

### `update_options`
One of the hardest parts of the project, every update/display function and `ids` int array aswell as `ids_counter` was a result of this part of my project. The user is prompted with a couple options just like search mode. However, this time I was going to have to do a plethora of checks to make sure no duplicate/incorrect entry is updated. `case 'i'` was pretty straightforward: store the id to be updated in a variable, check if it already exists in `ids` and display a message accordingly. When it finally gets accepted, `ids`, the linked list and the `.csv` file is updated. I believe the for loop is unnecessary because I call `update_id` but I didn't check. The fun starts when we get to `case 'n'`. The beginning is basically the same: prompts the user for a name, checks if there is an entry with the name and displays a message accordingly. The complicated part is when there are multiple entries with the exact same name. This is why `display_entry_by_name` accepts 3 parameters: node*, char* and int array. The int array is passed into the parameter so it can store the ids of all the matches it found and then return that number. In search mode, there was no need for the 3rd parameter to store the ids when searching by name, that's why the macro was made to manually fill in the gaps. Afterwards, it checks how many entries had a match. If there were more than two, the program would ask the user to specify which one based on the id. That's also why I just used the id to display id, name and age (because there wouldn't be any conflicts). Finally the `.csv` is updated accordingly. The other case is practically the same just with age.

### `delete_options`
Extremely similar to `update_options`; however, incredibly easier now that I had completed `update_options`. The only real difference between the two is that the second prompt is just a verification on whether the user meant to delete that entry or not. Most of the error handling is the same as `update_options`.

### `load`
The first hurdle in this project: actually loading the `.csv` file into a linked list. In there, it was my first use of `fgets` to store the headers in the `headers` char* array (which was never used unfortunately), and my first memory leak with `malloc` because I forgot to free it in `unload`. Anyway, after the headers are stored, I used `fread`. At the time, I didn't know that syscalls were a thing and didn't think much about them since I used the same technique in pset5: speller which didn't present much of a performance issue. I later learned that it is more optimized to do the minimum amount of `fread`s as possible to reduce expensive syscalls. I should've done the entire thing with `fgets` or store the result of `fread` into a large buffer from which I could extract the contents. Anywho, I then make a couple buffers to store the result of `fread` (maybe that should've been the clue that I was doing `fread` all wrong). All the buffers had me rework the `create_node` function into what it is right now. It's the reason it has the parameters it has, and the reason that `generate_id`'s return value was stored inside an integer array as opposed to being passed normally.

### `update_csv`
The final time I had to pluck out my hair during this project. The entire project's idea was based on wrong assumptions which was also a reason why `update_options` was so hard to implement. I had to constantly make copies of the new file and delete the old ones to be able to successfully update a `.csv` file. One of the reasons I used `malloc` in `create_node` and a char* in the `node` struct is because I wanted to exchange memory size for performance. I could've as easily used a char buffer for the name but I wanted as much flexibiliy as possible. The point is, I knew that there could be a variable number of memory needed, yet I still didn't realize that I couldn't magically update a 1KB file to include a 300 byte name for example. When I finally learned that the hard way, I implemented `update_csv` correctly: opening the `.csv` and a temporary file, copying everything over with the updates, deleting the old one and renaming the new one to the old file's name. I had two options when copying everything over: use the `.csv` or use the linked list. I went with the linked list because that ties nicey with what I wanted the project to achieve in the first place.

### `unload`
Possibly one of the most boring functions I implemented. It's entire purpose is to delete everything such that there is no allocated memory left over. All it does is call `delete_first_node` and free the headers (that I forgot about) and that's it.

## 'main.c'
Underwhelming as per usual compared to the other `.c` files. The main purpose is to accept the `.csv` file as a command line argument and handles any problems doing that. It presents the program and calls the 4 big functions: search, insert, update and delete option functions while passing in the `.csv` file into them.

## Header files
In the beginning when it was all inside `llf.c`, there were no problems with the global variables, but when I split the file into `llf.c` and `dbmsf.c`, it taught me the existence of `extern`. Other than that its quite self explanatory: write the prototypes/forward declarations of the functions in them so other files can access them and voila. The only weird one is `windows_portability` because all it does is replace `#include <strings.h>` with `stricmp` because windows is full of itself.

## Makefile
This was kind of a nightmare to configure in windows because neither `clang` nor `mingw` was working for me when I ran `make`. Most of the makefile is just checking which operating system it's being run on and making the changes accordingly. I copied almost all of the flags from the makefiles in the cs50 course.

## `compile_commands.json`
This file was just for my lsp to recognize the other files. They compiled gracefully, but when debugging, the lsp was freaking out.
