# SACLI
Subversion Advanced Command Line Interface

### About this project
I've been working for 4 years using Subversion, and the amount of time  
I wished the svn command to be more helpful is uncountable.  
  
This project is to add advanced features to the svn command.
The project itself won't be too fancy, since I'm just going to system call the svn command  
when I need to than rather fork the thing (if it exists).  
  
Project will be built based on the svn command version 1.14.1 (r1886195)  
  
# Usage
svm { options } \[ action \] \[ ... \]
| Actions | Arguments | Explaination |
| --- | --- | --- |
| add | \[Target File/Directory\] | Add file to the commit group. (Added to 'default' group if no group is specified) |
| remove | \[Target File/Directory\] | Remove file from the commit group. (Removed from 'default' group if no group is specified) |
| show | \[Group\] | Show all files inside commit group. (The commit group 'default' is shown if no group is specified) |
| comment | \[Group\] | Edit the commment of the commit group. (The comment of the commit group 'default' is edited if no group is specified) |
| overwrite | \[Destination Group\] \[Source Group\] | Overwrite destination group with source group (Use with caution!) |
| commit | \[Group\] | Start the commit process! |
| edit | \[Group\] | Edit files inside the commit group. (Default: all) |
| help | --- | Show help page. |
| info | \[Group\] | Show all files inside the commit group. (Files inside the commit group 'default' is shown if no group is specified) |
| clean | \[Group\] | Remove all files inside the commit group specified. |
| revert | \[Group\] | Revert all modifications done to the files inside group. |
| rollback | \[Revision\] \[Files\] | Get the specified file at specified revision. |
  
# Note
edit [commit group name] [all/selective/vimtab]
