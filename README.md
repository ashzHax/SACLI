# SACLI
Subversion Advanced Command Line Interface



# 사용 예시
svm add [file/dir] [commit group name]
- add file/dir from commit group (default if none is specified)
svm remove [file/dir] [commit group name]
- remove file/dir from commit group, does not delete it (default if none is specfified)
svm show [commit group name]
- show all files inside commit group
svm comment [commit group name]
- edit the commit comment of commit group
svm commit [commit group name]
- go through commit process
svm edit [commit group name] [all/select]
- edit files inside commit group, all or selective
svm help
- show help page
svm info [commit group name]
- show info of all files inside commit group
svm clean [group name]
- remove all files inside commit group
svm revert [group name]
- revert all modifications done to files inside group

# tools
svm rollback [revision] [file]
- get target revisiion version of file, and save it to latest version


