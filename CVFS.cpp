////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about process file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to initialise UAREA members
//  Author :            Atharva Samadhan Khairnar
//  Date :              13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
   strcpy(uareaobj.ProcessName,"Myexe");
   
   int i = 0;

   for(i = 0; i < MAXOPENFILES; i++)
   {
        uareaobj.UFDT[i] = NULL;
   }
    printf("ASKs CVFS : UAREA gets initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock
//  Description :       It is used to initialise Super block members
//  Author :            Atharva Samadhan Khairnar
//  Date :              13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("ASKs CVFS : Super block gets initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of inodes
//  Author :            Atharva Samadhan Khairnar
//  Date :              13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("ASKs CVFS : DILB created succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitilisation
//  Description :       It is used to call all such functions which are
//                      used to initialise auxillary data
//  Author :            Atharva Samadhan Khairnar
//  Date :              13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of ASKs CVFS is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("ASKs CVFS : Auxillary data initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to display the help page
//  Author :            Atharva Samadhan Khairnar
//  Date :              14/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-----------------------------------------------\n");
    printf("---------- ASKs CVFS Help Page ----------\n");
    printf("-----------------------------------------------\n");

    printf("man    : It is used to display manual page\n");
    printf("ls     : It is used to display the list of files made in the directory.\n");
    printf("clear  : It is used to clear the terminal\n");
    printf("creat  : It is used to create new file\n");
    printf("write  : It is used to write the data into file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to display statistical information\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate ASKs CVFS\n");

    printf("-----------------------------------------------\n");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to display man page
//  Author :            Atharva Samadhan Khairnar
//  Date :              14/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls",Name) == 0)
    {
        printf("About : It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man",Name) == 0)
    {
        printf("About : It is used to display manual page\n");
        printf("Usage : man command_name\n");
        printf("command_name : It is the name of command\n");        
    }
    else if(strcmp("exit",Name) == 0)
    {
        printf("About : It is used to terminate the shell.\n");
        printf("Usage : exit\n");        
    }
    else if(strcmp("clear",Name) == 0)
    {
        printf("About : It is used to clear the shell.\n");
        printf("Usage : clear\n");        
    }
    else if(strcmp("write",Name) == 0)
    {
        printf("About : It is used to write the data into a file through its fd.\n");
        printf("Usage : write fd\n");
        printf("fd    : A file descriptor (fd) is a non-negative integer that serves as a unique identifier for an open file or input/output (I/O) resource.\n");        
    }
    else if(strcmp("creat",Name) == 0)
    {
        printf("About   : It is used to creat a file at a free fd.\n");
        printf("Usage   : creat FileName\n");
        printf("FileName: The name of file which the user wants to make.\n");        
    }
    else if(strcmp("read",Name) == 0)
    {
        printf("About   : It is used to read a files data ata certain fd.\n");
        printf("Usage   : read fd Bytes\n");
        printf("FileName: The name ofr file which the user wants to make.\n");
        printf("Bytes   : Number of Bytes to be read from the file at fd.\n");    
    }
    else if(strcmp("stat",Name) == 0)
    {
        printf("About   : It is used to display statistical information about a file or file system.\n");
        printf("Usage   : stat FileName\n");
        printf("FileName: The name of file whose statistics you want to display.\n");
        printf("          If no filename is provided, displays file system statistics.\n");
        printf("\nDisplayed Information:\n");
        printf("  - File Name\n");
        printf("  - Inode Number\n");
        printf("  - File Size (Maximum and Actual)\n");
        printf("  - Available Space\n");
        printf("  - File Type (Regular/Special)\n");
        printf("  - Reference Count\n");
        printf("  - File Permissions (Read/Write/Execute)\n");
        printf("  - File Status (Open/Closed)\n");
        printf("  - File Descriptor (if open)\n");
        printf("  - Read and Write Offsets (if open)\n");
        printf("  - Access Mode (if open)\n");
        printf("\nFile System Statistics (when used without filename):\n");
        printf("  - Total Inodes\n");
        printf("  - Free Inodes\n");
        printf("  - Used Inodes\n");
        printf("  - Maximum File Size\n");
        printf("  - Currently Open Files\n");
    }
    else
    {
        printf("No manual entry for %s\n",Name);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check whether file is already exist or not
//  Input :             It accepts file name
//  Output :            It returns the true or false
//  Author :            Atharva Samadhan Khairnar
//  Date :              16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name      // File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    
    return bFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input :             It accepts file name and permissions
//  Output :            It returns the file descriptor
//  Author :            Atharva Samadhan Khairnar
//  Date :              16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char *name,         // Name of new file
                    int permission      // Permission for that file
                )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    // If name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the permission value is wrong
    // permission -> 1 -> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // If file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;    
        }
        temp = temp -> next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // Note : 0,1,2 are reserved
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate ememory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise File table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;
    
    // Connect File table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate ememory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;   // File descriptor
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all files
//  Input :             Nothing
//  Output :            Nothing
//  Author :            Atharva Samadhan Khairnar
//  Date :              16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("-----------------------------------------------\n");
    printf("------ ASKs CVFS Files Information ------\n");
    printf("-----------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%d\t%s  %d bytes\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        
        temp = temp -> next;
    }
    
    printf("-----------------------------------------------\n");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete the file
//  Input :             File name
//  Output :            Nothing
//  Author :            Atharva Samadhan Khairnar
//  Date :              22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Reset all values of inode
                // Dont deallocate memmory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Dealloacte memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // // Increment free inodes count
                superobj.FreeInodes++;

                break;  // IMP
            }// End of if
        }// End of if
    }// End of for

    return EXECUTE_SUCCESS;

}// End of function

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into the file
//  Input :             File descriptor
//                      Address of buffer which contains data
//                      Size of data that we want to write
//  Output :            Number of bytes succesfully written
//  Author :            Atharva Samadhan Khairnar
//  Date :              22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(
                    int fd,
                    char *data,
                    int size
            )
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data from the file
//  Input :             File descriptor
//                      Address of empty buffer
//                      Size of data that we want to read
//  Output :            Number of bytes succesfully read
//  Author :            Atharva Samadhan Khairnar
//  Date :              22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{

    //  Invaid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insuuficeint data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    // Reset for next read
    uareaobj.UFDT[fd]->ReadOffset = 0;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StatFile()
//  Description :       Display statistical info about a file
//  Input :             File name
//  Output :            Error code or success
//  Author :            Atharva Samadhan Khairnar
//  Date :              09/02/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int StatFile(char *name)
{
    PINODE temp = head;
    int i = 0;
    int fd = -1;
    
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    
    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }
    
    // Find the inode
    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0)
        {
            break;
        }
        temp = temp->next;
    }
    
    // Check if file is currently open
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                fd = i;
                break;
            }
        }
    }
    
    printf("-----------------------------------------------\n");
    printf("---------- Statistical Information ------------\n");
    printf("-----------------------------------------------\n");
    
    printf("File Name           : %s\n", temp->FileName);
    printf("Inode Number        : %d\n", temp->InodeNumber);
    printf("File Size           : %d bytes\n", temp->FileSize);
    printf("Actual File Size    : %d bytes\n", temp->ActualFileSize);
    printf("Available Space     : %d bytes\n", temp->FileSize - temp->ActualFileSize);
    printf("File Type           : %s\n", (temp->FileType == REGULARFILE) ? "Regular" : "Special");
    printf("Reference Count     : %d\n", temp->ReferenceCount);
    
    // Display permissions
    printf("File Permissions    : ");
    if(temp->Permission & READ)
        printf("Read ");
    if(temp->Permission & WRITE)
        printf("Write ");
    if(temp->Permission & EXECUTE)
        printf("Execute");
    printf("\n");
    
    // Display open file info
    if(fd != -1)
    {
        printf("File Status         : Open\n");
        printf("File Descriptor     : %d\n", fd);
        printf("Read Offset         : %d\n", uareaobj.UFDT[fd]->ReadOffset);
        printf("Write Offset        : %d\n", uareaobj.UFDT[fd]->WriteOffset);
        printf("Access Mode         : ");
        if(uareaobj.UFDT[fd]->Mode & READ)
            printf("Read ");
        if(uareaobj.UFDT[fd]->Mode & WRITE)
            printf("Write ");
        printf("\n");
    }
    else
    {
        printf("File Status         : Closed\n");
    }
    
    printf("-----------------------------------------------\n");
    
    return EXECUTE_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StatFileSystem()
//  Description :       Display file system statistics
//  Input :             Nothing
//  Output :            Nothing
//  Author :            Atharva Samadhan Khairnar
//  Date :              09/02/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StatFileSystem()
{
    int openFiles = 0;
    int i = 0;
    
    // Count open files
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            openFiles++;
        }
    }
    
    printf("-----------------------------------------------\n");
    printf("-------- File System Statistics ---------------\n");
    printf("-----------------------------------------------\n");
    
    printf("Total Inodes        : %d\n", superobj.TotalInodes);
    printf("Free Inodes         : %d\n", superobj.FreeInodes);
    printf("Used Inodes         : %d\n", superobj.TotalInodes - superobj.FreeInodes);
    printf("Maximum File Size   : %d bytes\n", MAXFILESIZE);
    printf("Maximum Open Files  : %d\n", MAXOPENFILES);
    printf("Currently Open Files: %d\n", openFiles);
    printf("Available FDs       : %d\n", MAXOPENFILES - openFiles);
    
    printf("-----------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point function of the project
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitilisation();

    printf("-----------------------------------------------\n");
    printf("-------- ASKs CVFS started succesfully --------\n");
    printf("-----------------------------------------------\n");
    
    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nASKs CVFS : > ");
        fgets(str,sizeof(str),stdin);
        
        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3], Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // ASKs CVFS : > exit
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using ASKs CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            // ASKs CVFS : > ls
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            // ASKs CVFS : > help
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            // ASKs CVFS : > clear
            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            else if(strcmp("stat", Command[0]) == 0)
            {
                StatFileSystem();
            }     
        } // End of else if 1
        else if(iCount == 2)
        {
            // ASKs CVFS : > man ls
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // Asks CVFS : > stat Demo.txt
            else if(strcmp("stat", Command[0]) == 0)
            {
                iRet = StatFile(Command[1]);
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File does not exist\n");
                }
            }
            // ASKs CVFS : > unlink Demo.txt
            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }
            }
            // ASKs CVFS : > write 2
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no space\n");
                }
                else
                {
                    printf("%d bytes gets succesfully written...\n",iRet);
                }
            }
            else
            {
                printf("There is no such command\n");
            }
        } // End of else if 2
        else if(iCount == 3)
        {
            // ASKs CVFS : > creat Ganesh.txt 3
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there is no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("File gets succesfully created with FD : %d\n",iRet);
            } 
            // ASKs CVFS : > read 3 10
            else if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }
                else
                {
                    printf("Read operation is succesfull.\n");
                    printf("Data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);

                }
            }
            else
            {
                printf("There is no such command.\n");
            }
        } // End of else 3
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        } // End of else
    } // End of while

    return 0;
} // End of main