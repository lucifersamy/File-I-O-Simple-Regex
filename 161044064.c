#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int checkCommandValidity(char* commands){
    int i=0;
    int delimCount=0;

    for(i=0; commands[i] != '?' ; ++i)
    {
        if(i == 0){
            if(commands[i] != '/')
            {
                return -1;
            }
            delimCount++;
        }
        else if(commands[i+1] == '?'){
            
            if(commands[i] == '/')
                delimCount++;

            else if(commands[i] == 'i'){
                if(commands[i-1] != '/'){
                    return -1;
                }
            }
            else{
                return -1;
            }

        }

        else if(commands[i] == '/')
            delimCount++;

        if(delimCount > 3){
            return -1;
        }

    }
    if(delimCount != 3){
        return -1;
    }
    int phFlag=0;
    int mesafe =0;
    for(i=0; commands[i] != '?' ; ++i)
    {   
        if(phFlag == 1)
            mesafe++;
        if(commands[i] == '^' &&  commands[i-1] != '/' ) 
            return -1;
        if(commands[i] == '$' && commands[i+1] != '/')
            return -1;
        if(commands[i] == '*' &&  commands[i-1] == '/')
            return -1;


        if(commands[i] == '[' && phFlag==0){
            phFlag = 1;
        }
        else if(commands[i] == '[' && phFlag == 1 ){
            return -1;
        }
        else if(commands[i] == ']' && phFlag == 0){
            return -1;
        }
        else if(commands[i] == ']' && phFlag == 1){
            if(mesafe>=2){
                phFlag=0;
                mesafe=0;
            }
            else{
                return -1;
            }
        }

    }
    if(phFlag == 1)
        return -1;

    return 0;
}
    char newBuf[100000];


void replace(char* newBuf, char* originalText2, char* str12, char* str2, int iCase){
    int j1=0;
    char charCompareAll[1000]; /////whyyy????
    int tempi;
    int lineStart = 1;
    int a=0;
    int kelimeninBasi;
    int finded=0;
    int i=0;
    int newB=0;    

    char originalText[100000];
    char str1[100000];
    int x=0;
    if(iCase){
        for (x = 0; originalText2[x]!='?'; x++) {
          if(originalText2[x] >= 'a' && originalText2[x] <= 'z') {
             originalText[x] = originalText2[x] -32;
          }
          else
            originalText[x] = originalText2[x];
        }
        originalText[x] = '?';
        for (x = 0; str12[x]!='?'; x++) {
          if(str12[x] >= 'a' && str12[x] <= 'z') {
             str1[x] = str12[x] -32;
          }
          else
            str1[x] = str12[x];
        }
        str1[x] = '?';
    }
    else{

        for (x = 0; originalText2[x]!='?'; x++) {
            originalText[x] = originalText2[x];
        }
        originalText[x] = '?';
        for (x = 0; str12[x]!='?'; x++) {
            str1[x] = str12[x];
        }
        str1[x] = '?';
    }

    int asterixFlag=0, startFlag=0, endFlag=0;

    if(strstr(str1,"^"))
        startFlag = 1;

    if(strstr(str1,"$"))
        endFlag = 1;

    int breaked = 0;
    int kacHarf=0;
    for(i=0; i<strlen(originalText); ++i){
        kacHarf=0;
        breaked = 0;
        finded = 0;
        tempi = i;
        j1 = 0;
        kelimeninBasi = i;
        lineStart = 1;

        asterixFlag=0;

        if(startFlag == 1){
            if(i != 0 && originalText[i-1] != '\n'){
                lineStart = 0;
            }
        }


        while(lineStart && !finded && !breaked) { 
            if(str1[j1] == '?'){                
                finded = 1;
                break;
            }
            asterixFlag=0;
            if(str1[j1+1] == '*'){ 
                asterixFlag = 1;
            }
            if(str1[j1] == '*' || str1[j1] == '^' || str1[j1] == '$'){
                j1++;
            }
            if(str1[j1] == '?'){
                finded = 1;
                break;
            }  

            
            if(str1[j1] == '['){
                a=0;
                j1++;
                while(str1[j1] != ']'){
                    charCompareAll[a] = str1[j1];
                    a++;
                    j1++; 
                }
                
                if(str1[j1+1] == '*'){ 
                    asterixFlag = 1;
                }
                j1++;

                charCompareAll[a] = '?';
            }                                  
            else{
                charCompareAll[0] = str1[j1];
                charCompareAll[1] = '?';
                j1++;

            }  

            int compare=0;
            int harfBulundu=0;
            while(!breaked && !finded){
                harfBulundu=0;
                if(charCompareAll[compare] == '?'){
                    finded = 1;
                }

                while(charCompareAll[compare] != '?'){
                    if(charCompareAll[compare] == originalText[tempi]){
                        harfBulundu = 1;
                        break;
                    }

                    if(charCompareAll[compare] != originalText[tempi]){
                        compare++;
                    }

                    else if(charCompareAll[compare] == '?'){ 
                        break;
                    }
                }
                if(asterixFlag == 0 && harfBulundu==1){
                    kacHarf++;
                    tempi++;
                    break;
                }
                if(asterixFlag == 1 && harfBulundu == 0){                    
                    harfBulundu = 1;
                    break;
                }
                else if(asterixFlag == 1 && harfBulundu == 1){ 
                    while(charCompareAll[compare] == originalText[tempi]){
                        tempi++; 
                        kacHarf++;
                    }
                    break;

                }
                else if(asterixFlag == 0 && harfBulundu == 0){
                    j1 = 0;
                    breaked = 1;

                }
                
                tempi++;
            }
        }
        if(finded){
           // printf("%d %d %d",tempi+1 , strlen(originalText), originalText[tempi+1]);
            if(endFlag == 1 && (originalText[tempi] != '\n' && tempi != strlen(originalText)  && originalText[tempi] != '\0' && originalText[tempi] != '\r')){
                finded = 0;
            }
        }
        if(finded){
            int l=0;
            int g=kelimeninBasi;
            while(str2[l] != '?'){
                newBuf[newB] = str2[l];
                newB++;
                l++;
            }
            for(g=0; g<kacHarf; ++g){
                i++;
            }
            --i;
        }
        
        else{
            newBuf[newB] = originalText2[i];
            newB++;
        }
    }
    newBuf[newB] = '?';
}

int fileOperations(char* str1, char* str2, int iCase, char* progName, char* inputFilepath, int inputFd){
    char *bp;
    char buffer[100000];
    int bytesRead;

   
    while( (bytesRead = read(inputFd, buffer, 100000) == -1) && (errno == EINTR))
    if(bytesRead< 0){
        perror("Read error");
        exit(EXIT_FAILURE);
    }

    
    bp = buffer;
    int r=0;

    replace(newBuf,bp, str1, str2, iCase);
    while(newBuf[r] != '?'){
        r++;
    }

    if(lseek(inputFd, 0, SEEK_SET)<0)
    {
        perror("lseek() error");
        exit(EXIT_FAILURE);
    }    
    if (ftruncate(inputFd, r) != 0){
        perror("ftruncate() error");
        exit(EXIT_FAILURE);
    }
    

    int bytesWritten=0;
    bytesRead = r;
    
    while(bytesRead > 0){
        while( (bytesWritten = write(inputFd, newBuf, bytesRead)) == -1 && ((errno == EINTR)))
        if(bytesWritten < 0){
            perror("Write error");
            exit(EXIT_FAILURE);
        }
        bytesRead -= bytesWritten;
       //sonBuf += bytesWritten;
    }
    return 0;

}

int parseCommands(char* allCommands, char* progName, char* inputFilepath){

    int init_size = strlen(allCommands);
    char delim[] = ";";
    struct flock lock;


    char *ptr = strtok(allCommands, delim);
    char commands[init_size][init_size]; 

    int i=0, j=0;

    while(ptr != NULL)
    { 
        for(j=0; j<strlen(ptr); ++j){
            commands[i][j] = ptr[j];
        }
        commands[i][j] = '?';
        ptr = strtok(NULL, delim);
        ++i;
    }

    j=0;
    while(j<i){
        if(checkCommandValidity(commands[j]) == -1){
            fprintf(stderr, "Wrong command format.\n" );
            fprintf(stderr, "Usage: %s \'/^Window[sz]*/Linux/i;/close[dD]$/open/\' pathname\n", progName);
            exit(EXIT_FAILURE);
        }
        ++j;
    }

    char delim2[] = "/";
    char *ptr2; 

    char strings[init_size][init_size]; 

    int a=0, b=0;
    j = 0;

    while(j<i){ // commands[j] /str1/str2/i   command1(/str1/str2/i) command2(/str2/str3/) command3 
        ptr2 = strtok(commands[j], delim2);

        while(ptr2 != NULL)
        { 
            for(a=0; a<strlen(ptr2); ++a){
                strings[b][a] = ptr2[a];
            }
            strings[b][a] = '?';
            ptr2 = strtok(NULL, delim2);
            ++b;
        }

        ++j;
    }
    j=0;

    int iCase;
    int inputFd;

    inputFd = open(inputFilepath, O_RDWR);
    if(inputFd == -1){
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    memset (&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    /* Place a write lock on the file. */
    fcntl (inputFd, F_SETLKW, &lock);

    while(j<b){
        //printf(" %s %s %s \n", strings[j], strings[j+1], strings[j+2]);
        if(strings[j+2][0] == 'i')
            iCase = 1;
        else if(strings[j+2][0] == '?')
            iCase = 0;
        else{
            iCase = 0;
        }

        fileOperations(strings[j], strings[j+1], iCase, progName, inputFilepath, inputFd);
        j += 3;
    }

    /* Release the lock. */
    lock.l_type = F_UNLCK;
    fcntl (inputFd, F_SETLKW, &lock);

    if (close(inputFd) == -1){
        perror("Failed to close input file");
        exit(EXIT_FAILURE);
    }

    return 0;
}




int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(stderr, "Wrong command format.\n" );
        fprintf(stderr, "Usage: %s \'/^Window[sz]*/Linux/i;/close[dD]$/open/\' pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int size = strlen(argv[1])+2;
    char allCommands[size]; 

    allCommands[0] = ';';
    int j=0;
    int k=1;
    for(j=0; j<strlen(argv[1]); ++j){
        allCommands[k] = argv[1][j];
        ++k;
    }
    allCommands[k] = ';';
    
 
    parseCommands(allCommands, argv[0], argv[2]);

 

 return 0;
}