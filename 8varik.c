#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#define BUF_SIZE 1024
// typedef int           
// (*PFUNC)(const char*,         
//      const struct stat*);
int             
walk(const char*,       /* начальная директория */
char*, const char*);

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}
// int print_entry(const char* name, const struct stat* st)
// {
//     printf("%s\n", name);
//     return 0;
// }
static char* programName;
const char* pathnamefull;
int main(int agrc, char *agrv[]){
    char *dirc, *basec, *bname, *dname;
    int nameLength = strlen(agrv[0]);

    int slashPos = 0;

    for(int i = nameLength - 1; i > 0; --i){
        if(agrv[0][i] == '/'){
            slashPos = i;
            break;
        }
    }
    if(slashPos != 0){
        memcpy(agrv[0], &agrv[0][slashPos + 1], nameLength - slashPos - 1);
        agrv[0][nameLength - slashPos - 1] = '\0';
        programName = agrv[0];
    }
    else{
        programName = agrv[0];
    }
    pathnamefull = agrv[3];
if (walk(agrv[1],agrv[2],agrv[3]) ==1){
    perror(programName);
    return 0;
}

return 1;
}
char buf[BUF_SIZE];
char buf1[BUF_SIZE];
char *error;
int walk(const char* dirname,char* extens,const char* pathname)
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
        error = calloc(sizeof(programName)+len + +1,sizeof(char));
        strcat(error, programName);
        strcat(error, " ");
        strcat(error, buf);
        perror(error);
        retval = 1;
        free(error);
        break;
    }
  
    if(S_ISDIR(st.st_mode))//1 if catalog exist
    { 
        if(walk(buf,extens,buf1) == 1)
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
    //      pfunc(dirp->d_name, &st); //print file
          if(link(buf,buf1) == -1
          ){
              error = calloc(sizeof(programName)+len + sizeof(name)+1,sizeof(char));
            strcat(error, programName);
            strcat(error, " ");
            strcat(error, buf);
            perror(error);
             free(error);
          }
      }
        
        
    }
    buf[len] = '\0';
    buf1[len1] = '\0';
    }
 
    closedir(dp);
    return retval;
}