
#include <afxwin.h>
#ifndef _FILESYS
#define _FILESYS
#define READONLY 0x01
#define HIDDEN 0x02
#define SYSTEM 0x04
#define VOLUME 0x08
#define DIRECT 0x10
#define ARCHIVE 0x20

#pragma pack(push, 1)


struct fatdate {
	WORD day:5;
	WORD month:4;
	WORD year:7;
};

struct fattime {
	WORD sec:5;
	WORD min:6;
	WORD hour:5;
};

struct directory_entry {
	BYTE Name[8];
	BYTE Extension[3];
	BYTE Attributes;
	BYTE Reserved[10];
	WORD Time;
	WORD Date;
	WORD startCluster;
	DWORD fileSize;

};

struct bootsector {
	BYTE jump[3];
	BYTE SysName[8];
	WORD BytesPerSector;
	BYTE SectorsPerCluster;
	WORD ReservedSectors;
	BYTE FATcount;
	WORD MaxRootEntries;
	WORD TotalSectors1;
	BYTE MediaDescriptor;
	WORD SectorsPerFAT;
	WORD SectorsPerTrack;
	WORD HeadCount;
	DWORD HiddenSectors;
	DWORD TotalSectors2;
	BYTE DriveNumber;
	BYTE Reserved1;
	BYTE ExtBootSignature;
	DWORD VolumeSerial;
	BYTE VolumeLabel[11];
	BYTE Reserved2[8];
};


#pragma pack(pop)



class FileEntry {

public:
	int valid;
	int dirCluster;
	char Name[12];
	int curCluster;
	DWORD fileSize;
	int accessMode;
	long bytePointer;
	long filePosition;
	long startCluster;
	FileEntry();
};

class FileSystem {

public:

FileSystem();
int fdMakeDir(char *directory);
int fdOpenFile(char *filename, int mode);
int fdCloseFile(int FDFileIndex);
int fdReadFile(int FDFileIndex, char *buffer, unsigned long length);
int fdWriteFile(int FDFileIndex, char *buffer, unsigned long length);
int fdSeek(int FDFileIndex, unsigned long location);
int fdListDir(char **output);
int fdChangeDir(char *directory);
int fdEraseFile(char *name);
int fdDelDir(char *directory);
int fdCopyFile(char *source, char *destination);
int fdDelMem(char **output);
int WriteDirectoryEntry(CString destination, int startCluster, int numclusters);
int copyFile(int clusternum, CString destination, int numclusters);
int ReadSector(int index, BYTE *buffer, int count);
int WriteSector(int index, BYTE *buffer, int count);
WORD GetCluster(WORD fatEntry);
void SetCluster(WORD cluster, WORD FAT12ClusEntryVal);
void printbs();
void getbs();
void deleteFile(int cluster, int direct);
CString printRootDirectory();
CString printDirectory(int clusternum);
~FileSystem();
int isValid();
void getFAT();
void printClusterList(directory_entry dirent);	
void printFile(directory_entry dirent);
CString Get_Directory_Entry(directory_entry dirent);
int findClusterNum(CString directory);
int getnumfree(){return numfreeentries;}
int nextfreeentry();
void getFree();
int deleteDirectoryName(CString name, int direct);
void deleteDirectory(int retVal, int direct, CString name);
int GetClustersNeeded(int retVal);
directory_entry curDirectory;
int MakeFile(char *directory);
int AllocateClusters(int begcluster, int numclusters);
void writeFAT();
int ChangeDirEntry(CString directory, int filesize);
int SetStartCluster(char *directory, int FDFileIndex);
int getnextempty();
int checkForOpen(int retVal);
private:

	int numfreeentries;
	int sizeOfRoot;
	int dir;
	
	int openFileCount;
	FileEntry fileTable[256];
	directory_entry dot;
	directory_entry dotdot;
	char blankDirectory[512];
	bootsector bs;	
	HANDLE diskHandle;	
	BYTE* FAT1;
	BYTE* FAT2;
	int* freeentries;
	int nextfree;
	int emptyfileindex;
	FILE *fp;
};


#endif