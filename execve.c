//
//  main.c
//  nadobotat
//
//  Created by Anton Suslov on 30/05/2017.
//  Copyright © 2017 Anton Suslov. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>

int main(int argc, const char * argv[], const char * envp[]) {
    char *say_argv[3] = {
        "/usr/bin/say",
        "кек",
        NULL
    };
    execve("/usr/bin/say", say_argv, NULL);
    perror(NULL);
    scanf("");
    return 0;
}
