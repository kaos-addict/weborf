/*
Weborf
Copyright (C) 2010  Salvo "LtWorf" Tomaselli

Weborf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@author Salvo "LtWorf" Tomaselli <tiposchi@tiscali.it>
*/
#include "options.h"

#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>

#include "instance.h"
#include "types.h"
#include "myio.h"

/**
Copies count bytes from the file descriptor "from" to the
file descriptor "to".
It is possible to use lseek on the descriptors before calling
this function.
Will not close any descriptor

Returns
0 in case of success
-1 in case it is impossible to allocate the buffers
-2 in case of read/write failure
*/
int fd_copy(int from, int to, off_t count) {
    char *buf=malloc(FILEBUF);//Buffer to read from file
    int reads,wrote;
    int retval=0;

    if (buf==NULL) {
#ifdef SERVERDBG
        syslog(LOG_CRIT,"Not enough memory to allocate buffers");
#endif
        return -1;//If no memory is available
    }

    //Sends file
    while (count>0 && (reads=read(from, buf, FILEBUF<count? FILEBUF:count ))>0) {
        count-=reads;
        wrote=write(to,buf,reads);
        if (wrote!=reads) { //Error writing to the descriptor
            retval=-2;
            break;
        }
    }

    free(buf);
    return retval;
}

/**
Deletes a directory and its content.
This function is something like rm -rf

dir is the directory to delete
file is a buffer. Allocated outside because it
will be reused by every recoursive call.
It's size is file_s

Returns 0 on success
*/
int dir_remove(char * dir) {
    /*
    If it is a file, removes it
    Otherwise list the directory's content,
    then do a recoursive call and do
    rmdir on self
    */
    if (unlink(dir)==0)
        return 0;

    DIR *dp = opendir(dir); //Open dir
    struct dirent entry;
    struct dirent *result;
    int return_code;


    if (dp == NULL) {
        return 1;
    }

    char*file=malloc(PATH_LEN);//Buffer for path
    if (file==NULL)
        return HTTP_CODE_SERVICE_UNAVAILABLE;

    //Cycles trough dir's elements
    for (return_code=readdir_r(dp,&entry,&result); result!=NULL && return_code==0; return_code=readdir_r(dp,&entry,&result)) { //Cycles trough dir's elements

        //skips dir . and .. but not all hidden files
        if (entry.d_name[0]=='.' && (entry.d_name[1]==0 || (entry.d_name[1]=='.' && entry.d_name[2]==0)))
            continue;

        snprintf(file,PATH_LEN,"%s/%s",dir, entry.d_name);
        dir_remove(file);
    }

    closedir(dp);
    free(file);
    return rmdir(dir);
}

#ifdef WEBDAV
/**
Moves a file. If it is on the same partition it will create a new link and delete the previous link.
Otherwise it will create a new copy and delete the old one.

Returns 0 on success.
*/
int file_move(char* source, char* dest) {
    int retval=rename(source,dest);

    //Not the same device, doing a normal copy
    if (retval==-1 && errno==EXDEV) {
        retval=file_copy(source,dest);
        if (retval==0)
            unlink(source);
        else
            unlink(dest); //Might not exist, but we don't care
    }
    return retval;
}

/**
Copies a file into another file

Returns 0 on success
*/
int file_copy(char* source, char* dest) {
    int fd_from=-1;
    int fd_to=-1;
    int retval=0;

    //Open destination file
    if ((fd_to=open(dest,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))<0) {
        retval=HTTP_CODE_FORBIDDEN;
        goto escape;
    }

    //open source file
    if ((fd_from=open(source,O_RDONLY))<0) {
        retval = HTTP_CODE_PAGE_NOT_FOUND;
        goto escape;
    }

    //Find size of the source file
    struct stat stbuf;
    if (fstat(fd_from,&stbuf)!=0) {
        retval= HTTP_CODE_FORBIDDEN;
        goto escape;
    }

    retval=fd_copy(fd_from,fd_to,stbuf.st_size);

escape:
    if (fd_from>=0) close(fd_from);
    if (fd_to>=0) close(fd_to);
    return retval;
}

/**This function copies a directory.
The destination directory will be created and
will be filled with the same content of the source directory

Returns 0 on success
*/
int dir_copy (char* source, char* dest) {
    return dir_move_copy(source,dest,COPY);
}

/**This function moves a directory.
The destination directory will be created and
will be filled with the same content of the source directory.
Then, the source directory will be deleted.

Returns 0 on success
*/
int dir_move(char* source, char* dest) {
    int retval;

    if ((retval=rename(source,dest))==0) {
        return 0;
    } else if (retval==-1 && errno==EXDEV) {
        return dir_move_copy(source,dest,MOVE);
    } else {
        return 1;
    }

}

/**
Moves or copies a directory, depending on the method used

Returns 0 on success
*/
int dir_move_copy (char* source, char* dest,int method) {
    struct stat f_prop; //File's property
    int retval=0;

    if (mkdir(dest,S_IRWXU | S_IRWXG | S_IRWXO)!=0) {//Attemps to create destination directory
        return HTTP_CODE_FORBIDDEN;
    }

    DIR *dp = opendir(source); //Open dir
    struct dirent entry;
    struct dirent *result;
    int return_code;


    if (dp == NULL) {
        return HTTP_CODE_PAGE_NOT_FOUND;
    }

    char*src_file=malloc(PATH_LEN*2);//Buffer for path
    if (src_file==NULL)
        return HTTP_CODE_SERVICE_UNAVAILABLE;
    char* dest_file=src_file+PATH_LEN;

    //Cycles trough dir's elements
    for (return_code=readdir_r(dp,&entry,&result); result!=NULL && return_code==0; return_code=readdir_r(dp,&entry,&result)) {

        //skips dir . and .. but not all hidden files
        if (entry.d_name[0]=='.' && (entry.d_name[1]==0 || (entry.d_name[1]=='.' && entry.d_name[2]==0)))
            continue;

        snprintf(src_file,PATH_LEN,"%s/%s",source, entry.d_name);
        snprintf(dest_file,PATH_LEN,"%s/%s",dest, entry.d_name);

        stat(src_file, &f_prop);
        if (S_ISDIR(f_prop.st_mode)) {//Directory
            retval=dir_move_copy(src_file,dest_file,method);
        } else {//File
            if (method==MOVE) {
                retval=file_move(src_file,dest_file);
            } else {
                retval=file_copy(src_file,dest_file);
            }
        }

        if (retval!=0)
            goto escape;
    }

escape:
    closedir(dp);
    free(src_file);

    //Removing directory after that its content has been moved
    if (retval==0 && method==MOVE) {
        return rmdir(source);
    }
    return retval;
}

/**
 * Creates a temporary file, opened for read and write
 * and returns the file descriptor.
 *
 * The file will be deleted immediately after the close()
 **/
int myio_mktmp() {
    int fd;

    char template[] = "/tmp/weborf-temp-XXXXXX"; //TODO use env var for tmp directory
    fd=mkstemp(template);

    unlink(template);

    return fd;

}

#endif
