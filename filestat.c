#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h>
#include <stdio.h>

int main() {
    DIR *dir;
    struct dirent *ndir;
    struct stat info;
    struct passwd *uid;
    struct group *gid;
    dir = opendir("./");
    if (dir == NULL) {
        perror("bad opendir");
        return -1;
    }
    while((ndir = readdir(dir))) {
        printf("File: %s\n", ndir->d_name);
        if (stat(ndir->d_name, &info) < 0) {
            perror("bad stat");
            closedir(dir);
            return -1;
        }
        printf("Size: %lu\n", info.st_size);
        if (!(uid = getpwuid(info.st_uid))) {
            perror("bad getuid");
            return -1;
        }
        printf("%s\n", uid->pw_name);
        if (!(gid = getgrgid(info.st_uid))) {
            perror("bad getgrid");
            return -1;
        }
        printf("%s\n", gid->gr_name);
    }
}
