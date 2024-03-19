# SACLI
Subversion Advanced Command Line Interface
  
### About this project
I've been working for 4 years using Subversion, and the amount of time I wished the `svn` command to be more helpful is uncountable.  
  
This project is to add advanced features to the `svn` command.  
The project itself won't be too fancy, since I'm just going to system call the `svn` command when I need to, rather than fork the thing (if it exists). 
  
Project will be built based on the svn command version 1.14.1 (r1886195)  
Using jansson library v2.14  
  
# Usage
**svm { options } \[ action \] { arguments }**
| Action    | Main Argument              | Explaination                                                                                                                                           |
| :---      | :---                       | :---                                                                                                                                                   |
| add       | {file/directory}           | Add file to the commit group.                                                                                                                          |
| remove    | {file/directory}           | Remove file from the commit group.                                                                                                                     |
| show      |                            | Show all files inside commit group.                                                                                                                    |
| info      |                            | Show a list of existing group.                                                                                                                         |
| comment   | {comment}                  | Edit the comment of the commit group.                                                                                                                  |
| commit    |                            | Start the commit process!                                                                                                                              |
| clear     |                            | Remove all files inside the specified group.                                                                                                           |
| overwrite | {dest\_group} {src\_group} | Overwrite destination group with source group (Use with caution!)                                                                                      |
| edit      | \[all/selective\]          | Edit files inside the commit group. (Default: 'all')                                                                                                   |
| help      |                            | Show help page.                                                                                                                                        |
| revert    |                            | Revert all modifications done to the files inside specified group.                                                                                     |
| rollback  | {revision} {files}         | Get the specified file at specified revision.                                                                                                          |
| auto      |                            | Automatically get modified files via 'svn status' (Reference paths only specified in '/.c.target' if file exists, otherwise runs 'svn status' at root) |
| target    | {path}                     | Path to search for when using the command 'auto' (This command creates and opens '/.c.target')                                                         |

# Options
| Action  | Option | Explaination                                          |
| :---    | :---   | :---                                                  |
| -       | -h     | Show help page.                                       |
| add     | -g     | Name of the target commit group. (Default: 'default') |
| remove  | -g     | Name of the target commit group. (Default: 'default') |
| show    | -g     | Name of the target commit group. (Default: 'default') |
| comment | -g     | Name of the target commit group. (Default: 'default') |
| commit  | -g     | Name of the target commit group. (Default: 'default') |
| edit    | -g     | Name of the target commit group. (Default: 'default') |
| clean   | -g     | Name of the target commit group. (Default: 'default') |
| revert  | -g     | Name of the target commit group. (Default: 'default') |

# Compiling
Using gcc(11.4.0) and make(4.3).
```
make
sudo make install
```
