#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#define BUF_SIZE 1024
typedef int           
(*PFUNC)(const char*,         
     const struct stat*);
int             
walk(const char*,       /* начальная директория */
     PFUNC,char*, const char*);

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}
int print_entry(const char* name, const struct stat* st)
{
    printf("%s\n", name);
    return 0;
}
const char* pathnamefull;
int main(int agrc, char *agrv[]){
    char *dirc, *basec, *bname, *dname;
    dirc = strdup(agrv[0]);
    basec = strdup(agrv[0]);
    pathnamefull = agrv[3];
if (walk(agrv[1], print_entry,agrv[2],agrv[3]) ==1){
    perror(basec);
    return 0;
}

return 1;
}
char buf[BUF_SIZE];
char buf1[BUF_SIZE];
int walk(const char* dirname, PFUNC pfunc,char* extens,const char* pathname)
{
    DIR* dp;
    struct stat st;
    struct dirent* dirp;
    int retval = 0;
 
    size_t len = strlen(dirname);
    size_t len1 = strlen(pathname);

 if (!strcmp(dirname,pathnamefull)){
     return 1;
 }
    if((dp = opendir(dirname)) == NULL)
    {
    return 1;
    }
 
    strncpy(buf, dirname, len);
    
    strncpy(buf1, pathname, len1);
    while((dirp = readdir(dp)) != NULL)
    {
    if(strcmp(dirp->d_name, ".") == 0 ||
       strcmp(dirp->d_name, "..") == 0)
        continue;
    buf1[len1] = '/';
    buf1[len1+1] = '\0';
    buf[len] = '/';
    buf[len+1] = '\0';
 
    strcat(buf, dirp->d_name);
    
    if(stat(buf, &st) == -1)
    {
        perror(dirp->d_name);
        retval = 1;
        break;
    }
  
    if(S_ISDIR(st.st_mode))//1 if catalog exist
    {
        
        if(walk(buf, pfunc,extens,buf1) == 1)
        {
        retval = 1;
        break;
        }        
    }
    else 
    {
        
    const  char *name = dirp->d_name;
    strcat(buf1, name);
     const  char *ext = get_filename_ext(buf);
     if (strcmp(extens,ext) == 0){
         pfunc(dirp->d_name, &st); //print file
         if(link(buf,buf1) == -1){
           perror("gg");  
         }
     }
        
        
    }
    buf[len] = '\0';
    buf1[len1] = '\0';
    }
 
    closedir(dp);
    return retval;
}