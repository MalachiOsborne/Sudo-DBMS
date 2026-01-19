# Pseudo-DBMS
### Video Demo: https://www.youtube.com/watch?v=t0hPnMyjVKo
### A pseudo database management system written in c
A simple DBMS that accepts a `.csv` file as a command-line input. The `.csv` must include the headers `id`,`name` and `age`. The reason it's called "pseudo" is because it doesn't allow for columns to be created/inserted, updated, searched and deleted. 
It has 4 modes: Search, Insert, Update and Delete (along with an exit)
### Search mode
In search mode, you can search using an id, name, age or display all entries.
### Insert mode
In insert mode, you can insert a name and an age and an id will automatically be assigned.
### Update mode
In update mode, you can update an id, name or age. An error message displays if: the id being updated doesn't exist, the new id is already being used, name wasn't found, age wasn't found. If naming/age conflicts exist, the program handles that by asking for the id of the desired entry to be altered.
### Delete mode
In delete mode, you delete an entry using its id, name or age. An error message displays if id/name/age aren't found. If naming/age conflicts exist, the program also handles it by asking for an id. Then a confirmation message appears.
