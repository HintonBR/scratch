// FileDLL.cpp : Defines the entry point for the DLL application.
//



#include "stdafx.h"
#include "stdio.h"

FileSystem* filesys = new FileSystem();

//BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//{
//	filesys = new FileSystem();
//    return TRUE;
//}

extern "C" __declspec(dllexport)
int fdListDir(char **output) {
	return filesys->fdListDir(output);
}

extern "C" __declspec(dllexport)
int fdDelMem(char **output) {
	return filesys->fdDelMem(output);
}

extern "C" __declspec(dllexport)
int fdChangeDir(char *directory) {
	return filesys->fdChangeDir(directory);
}

extern "C" __declspec(dllexport)
int fdEraseFile(char *name) {
	return filesys->fdEraseFile(name);
}
extern "C" __declspec(dllexport)
int fdDelDir(char *directory) {
	return filesys->fdDelDir(directory);
}
extern "C" __declspec(dllexport)
int fdCopyFile(char *source, char *destination) {
	return filesys->fdCopyFile(source, destination);
}
extern "C" __declspec(dllexport)
int fdMakeDir(char *directory) {
	return filesys->fdMakeDir(directory);
}
extern "C" __declspec(dllexport)
int fdOpenFile(char *filename, int mode) {
	return filesys->fdOpenFile(filename, mode);
}
extern "C" __declspec(dllexport)
int fdCloseFile(int value) {
	return filesys->fdCloseFile(value);
}
extern "C" __declspec(dllexport)
int fdReadFile(int FDFileIndex, char *buffer, unsigned long length) {
	return filesys->fdReadFile(FDFileIndex, buffer, length);
}
extern "C" __declspec(dllexport)
int fdWriteFile(int FDFileIndex, char *buffer, unsigned long length) {
	return filesys->fdWriteFile(FDFileIndex, buffer, length);
}
extern "C" __declspec(dllexport)
int fdSeek(int FDFileIndex, unsigned long location) {
	return filesys->fdSeek(FDFileIndex, location);
}


int FileSystem::fdMakeDir(char *directory) {
	int retVal;
	int start;
	int old;
	BYTE buf[512];
	CString temp;
	temp.Format("%s",directory);
	if (temp.Find(".", 0) == -1 || temp.GetAt(0) == '.') temp += ".";
	retVal = findClusterNum(temp);
	if (retVal == -1) {
		if (getnumfree() <= 0) return 0;
		start = nextfreeentry();
		memcpy(&curDirectory, &dot, sizeof(directory_entry));
		if (WriteDirectoryEntry(temp,start,1) == 0) return 0;
		memset((void*) buf, 0, sizeof(buf));
		WriteSector(start + 31, buf, 1);
		SetCluster(start, 0xFFF);
		WriteSector(1, FAT1, 9);
		WriteSector(10, FAT2, 9);
		old = dir;
		dir = start;
		if (WriteDirectoryEntry(".", start, -1) == 0) return 0;
		if (WriteDirectoryEntry("..", old, -1) == 0) return 0;
		dir = old;
		return 1;
	}
	else return 0;

}

int FileSystem::getnextempty() {
int i = emptyfileindex;
	emptyfileindex++;
	if (emptyfileindex > 255) emptyfileindex = 0;
	while (fileTable[emptyfileindex].valid == 1) emptyfileindex++;
	return i;
}

int FileSystem::fdOpenFile(char *filename, int mode) {
	CString temp;
	int retVal;
	temp.Format("%s",filename);
	if (temp.Find(".", 0) == -1 || temp.GetAt(0) == '.') temp += ".";
	retVal = findClusterNum(temp);
	if ((retVal >= 0 || mode != 0) && !(curDirectory.Attributes & 0x10)) {
		char stuff[100];
		memset(stuff, 0, 100);
		sprintf(stuff, "We made it here-%d\n", retVal);
		fwrite(stuff, 100, 1, fp);
		if (retVal >= 0) {
			memset(stuff, 0, 100);
		sprintf(stuff, "We made it here1.5 -- %d\n", retVal);
		fwrite(stuff, 100, 1, fp);
			if (checkForOpen(retVal) != 0) return -1;
		}
		else {
			retVal = MakeFile(filename);
			if (retVal == -1) return -1;
		}
		memset(stuff, 0, 100);
		sprintf(stuff, "We made it here2\n", retVal);
		fwrite(stuff, 100, 1, fp);
		retVal = getnextempty();
		fileTable[retVal] = FileEntry();
		fileTable[retVal].startCluster = curDirectory.startCluster;
				memset(stuff, 0, 100);
		sprintf(stuff, "We made it here3\n", retVal);
		fwrite(stuff, 100, 1, fp);
		switch (mode) {
			case 1:	findClusterNum(temp);
					deleteFile(fileTable[retVal].startCluster, dir);
					retVal = MakeFile(filename);
			case 0:
			case 3:
				//if (retVal < 3000) {
					fileTable[retVal].curCluster = curDirectory.startCluster;
				//}
				//else fileTable[retVal].curCluster = 0;
					fileTable[retVal].bytePointer = 1;
					fileTable[retVal].filePosition = 1;													
					break;
			case 2:
					//if (retVal < 3000) {
					fileTable[retVal].curCluster = curDirectory.startCluster;
				//}
				//else fileTable[retVal].curCluster = 0;
					fileTable[retVal].bytePointer = (curDirectory.fileSize % 512) + 1;
					fileTable[retVal].filePosition = curDirectory.fileSize + 1;
					//if (fileTable[retVal].bytePointer == 1) {
					//	while (GetCluster(fileTable[retVal].curCluster) < 0xFF8) {
					//		fileTable[retVal].curCluster = GetCluster(fileTable[retVal].curCluster);
					//	}
					//	fileTable[retVal].curCluster = AllocateClusters(fileTable[retVal].curCluster,1);
					//}
										
					break;
		}//end of switch
		
		fileTable[retVal].valid = 1;
		strcpy(fileTable[retVal].Name, filename);
		fileTable[retVal].accessMode = mode;
		fileTable[retVal].fileSize = curDirectory.fileSize;
		fileTable[retVal].dirCluster = dir;
		fileTable[retVal].startCluster = curDirectory.startCluster;
		openFileCount++;
		memset(stuff, 0, 100);
		sprintf(stuff, "File Open is returning is %lu\n", retVal);
		fwrite(stuff, 100, 1, fp);
		return retVal;

	}
	
		else {
			char stuff[100];
			memset(stuff, 0, 100);
		sprintf(stuff, "File Open is returning is %d\n", -1);
		fwrite(stuff, 100, 1, fp);
			return -1;
		}
}
int FileSystem::checkForOpen(int retVal) {
	int i;
	for (i=0; i < 256; i++) {
		if (fileTable[i].valid != 0) {
			if (fileTable[i].startCluster == retVal) return 1;
		}
	}
	return 0;
}
int FileSystem::fdCloseFile(int FDFileIndex) {
	char stuff[100];
	memset(stuff, 0, 100);
		sprintf(stuff, "File Close receives %lu", FDFileIndex);
		fwrite(stuff, 100, 1, fp);
	if (FDFileIndex > 255 || FDFileIndex < 0) return -1;
	if (fileTable[FDFileIndex].valid != 0) {
		fileTable[FDFileIndex].valid = 0;
		openFileCount--;
		sprintf(stuff, "File Close is returning is %lu\n", 1);
		fwrite(stuff, 100, 1, fp);
		return 1;
	}
	//char stuff[100];
		sprintf(stuff, "The value we are returning is %d\n", -1);
		fwrite(stuff, 100, 1, fp);
	return -1;
}

int FileSystem::fdReadFile(int FDFileIndex, char *buffer, unsigned long length) {
	long bytesread = 0;
	int clusternum;
	long upper = 0;
	BYTE buf[512];
		memset(buffer, 0, length);
	if (FDFileIndex > 255 || FDFileIndex < 0) return -1;
	if (fileTable[FDFileIndex].valid == 0) return -1;
	if (fileTable[FDFileIndex].curCluster == 0) return -1;
	switch (fileTable[FDFileIndex].accessMode) {
	case 1:
	case 2:	return -1;
			break;
	case 0:
	case 3:
		memset(buffer, 0, length);
		clusternum = fileTable[FDFileIndex].curCluster;
		if (clusternum >= 0xFF8 || clusternum == 0 || fileTable[FDFileIndex].filePosition > fileTable[FDFileIndex].fileSize) return -1;
		if (fileTable[FDFileIndex].filePosition + length > fileTable[FDFileIndex].fileSize) upper = fileTable[FDFileIndex].fileSize;
		else upper = fileTable[FDFileIndex].filePosition + length - 1;
		bytesread = upper - fileTable[FDFileIndex].filePosition + 1;
		while (upper >= fileTable[FDFileIndex].filePosition) {
		if (upper - fileTable[FDFileIndex].filePosition > 512) {
			ReadSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			strncat(buffer, (char*) buf + fileTable[FDFileIndex].bytePointer - 1, 512 - fileTable[FDFileIndex].bytePointer + 1);
			fileTable[FDFileIndex].filePosition += 512 - fileTable[FDFileIndex].bytePointer + 2;
			fileTable[FDFileIndex].bytePointer = 1;
			fileTable[FDFileIndex].curCluster = GetCluster(fileTable[FDFileIndex].curCluster);
		}
		else {
			ReadSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			strncat(buffer, (char*) buf + fileTable[FDFileIndex].bytePointer - 1, (upper % 512) - fileTable[FDFileIndex].bytePointer + 1);
			fileTable[FDFileIndex].filePosition = upper + 1;
			fileTable[FDFileIndex].bytePointer = (upper % 512) + 1;
		}
		}
		
		break;
	}
	return bytesread;
}

int FileSystem::fdWriteFile(int FDFileIndex, char *buffer, unsigned long length) {

	long byteswritten = 0;
	int clusternum;
	long upper = 0;
	long temp, byte, pos;
	BYTE buf[512];
	
	
	if (FDFileIndex > 255 || FDFileIndex < 0) return -1;
	if (fileTable[FDFileIndex].valid == 0) return -1;
	//if (sizeof(buffer) < length) return -1;
	switch (fileTable[FDFileIndex].accessMode) {
	case 0:	return -1;
			break;
	case 1:
	case 2:
	case 3:
		temp = fileTable[FDFileIndex].curCluster;
		byte = fileTable[FDFileIndex].bytePointer;
		pos = fileTable[FDFileIndex].filePosition;
		if (fdSeek(FDFileIndex, fileTable[FDFileIndex].filePosition + length) == -1) {
			char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returned to write a -1 -- %lu + %lu = %lu\r\n", fileTable[FDFileIndex].filePosition, length, fileTable[FDFileIndex].filePosition + length);
		fwrite(stuff, 100, 1, fp);
			return -1;
		}
		fileTable[FDFileIndex].curCluster = temp;
		fileTable[FDFileIndex].bytePointer=byte;
		fileTable[FDFileIndex].filePosition=pos;
		
		char* mybuf = new char[length+1];
		char* beg = mybuf;
		strncpy(mybuf, buffer, length);
		clusternum = fileTable[FDFileIndex].curCluster;
		if (clusternum >= 0xFF8 || clusternum == 0 || fileTable[FDFileIndex].filePosition > fileTable[FDFileIndex].fileSize) return 0;
		upper = fileTable[FDFileIndex].filePosition + length - 1;
		byteswritten = upper - fileTable[FDFileIndex].filePosition + 1;
		char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "byte=%d,upper=%d\r\n", fileTable[FDFileIndex].bytePointer, upper);
		fwrite(stuff, 100, 1, fp);
		while (upper >= fileTable[FDFileIndex].filePosition) {
		if (upper - fileTable[FDFileIndex].filePosition > 512) {
			ReadSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			memmove(buf + fileTable[FDFileIndex].bytePointer - 1, (void*)mybuf, 512 - fileTable[FDFileIndex].bytePointer + 1);
			WriteSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			mybuf += (512 - fileTable[FDFileIndex].bytePointer + 1);
			fileTable[FDFileIndex].filePosition += 512 - fileTable[FDFileIndex].bytePointer + 2;
			fileTable[FDFileIndex].bytePointer = 1;
			fileTable[FDFileIndex].curCluster = GetCluster(fileTable[FDFileIndex].curCluster);
		}
		else {
			
			ReadSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			memmove(buf + fileTable[FDFileIndex].bytePointer - 1, (void*)mybuf, (upper % 512) - fileTable[FDFileIndex].bytePointer + 1);
			WriteSector(fileTable[FDFileIndex].curCluster + 31, buf, 1);
			mybuf += (512 - fileTable[FDFileIndex].bytePointer + 1);
			fileTable[FDFileIndex].filePosition = upper + 1;
			fileTable[FDFileIndex].bytePointer = (upper % 512) + 1;
		}
		}
		//mybuf = beg;
		//delete [] mybuf;
		break;
	}
	
	return byteswritten;
}

int FileSystem::fdSeek(int FDFileIndex, unsigned long location) {
	int numclusters = 0;
	int numclustersneeded = 0;
	int temp, count, old;
	temp = fileTable[FDFileIndex].startCluster;
	if (FDFileIndex > 255 || FDFileIndex < 0) {
		char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of invalid file index\r\n");
		fwrite(stuff, 100, 1, fp);
		return -1;
	}
	if (fileTable[FDFileIndex].valid == 0) {
		char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of invalid file handle\r\n");
		fwrite(stuff, 100, 1, fp);
		return -1;
	}
	if ((fileTable[FDFileIndex].fileSize < location) && (fileTable[FDFileIndex].accessMode == 0)) {
		char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of invalid access mode\r\n");
		fwrite(stuff, 100, 1, fp);
		return -1;
	}

	if (temp != 0) 	numclusters = GetClustersNeeded(temp);
	else numclusters = 0;
	if (((numclusters + getnumfree()) * 512) < location) {
		char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of lack of diskspace -- location is %lu\r\n", location);
		fwrite(stuff, 100, 1, fp);
		return -1;
	}
	if (temp ==0) {
		temp = SetStartCluster(fileTable[FDFileIndex].Name, FDFileIndex);
		numclusters = 1;
	}
	if ((numclusters < (location / 512)) || ((numclusters == (location / 512)) && (location % 512 != 0))) {
		if (numclusters == (location / 512)) {
			while (GetCluster(temp) < 0xFF8) {
				temp = GetCluster(temp);
			}
			temp = AllocateClusters(temp, 1);
			if (temp != -1) fileTable[FDFileIndex].curCluster = temp;
			else {
				char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of failed cluster allocation\r\n");
		fwrite(stuff, 100, 1, fp);
				return -1;
			}
		}
		else {
			if ((location % 512) == 0) numclustersneeded = 0;
			else numclustersneeded = 1;	
			numclustersneeded += ((location / 512) - numclusters); 
			while (GetCluster(temp) < 0xFF8) {
				temp = GetCluster(temp);
			}
			temp = AllocateClusters(temp, numclustersneeded);
			if (temp != -1) fileTable[FDFileIndex].curCluster = temp;
			else {
				char stuff[100];
			memset(stuff, 0 , 100);
		sprintf(stuff, "Seek returning -1 because of failed cluster allocation\r\n");
		fwrite(stuff, 100, 1, fp);
				return -1;
			}
		}
		fileTable[FDFileIndex].fileSize = location;
	}
	else if (fileTable[FDFileIndex].fileSize < location) {
		fileTable[FDFileIndex].fileSize = location;
		//Set curCluster
		count = location / 512;
		if (location %512 == 0) count--;
		temp = fileTable[FDFileIndex].startCluster;
		while (count > 0) {
			temp = GetCluster(temp);
			count--;
		}
		fileTable[FDFileIndex].curCluster = temp;
	}
	else {
		//Set curCluster
		count = location / 512;
		if (location %512 == 0) count--;
		temp = fileTable[FDFileIndex].startCluster;
		while (count > 0) {
			temp = GetCluster(temp);
			count--;
		}
		fileTable[FDFileIndex].curCluster = temp;
	}
	fileTable[FDFileIndex].filePosition = location;
	//Write out fileSize
	old = dir;
	dir = fileTable[FDFileIndex].dirCluster;
	findClusterNum(fileTable[FDFileIndex].Name);
	ChangeDirEntry(fileTable[FDFileIndex].Name, fileTable[FDFileIndex].fileSize);
	dir = old;
	//Set the byte pointer
	fileTable[FDFileIndex].bytePointer = location % 512;
	if (location ==0) {
		fileTable[FDFileIndex].bytePointer = 1;
		fileTable[FDFileIndex].filePosition = 1;
	}
	else if (fileTable[FDFileIndex].bytePointer == 0) {
		fileTable[FDFileIndex].bytePointer = 512;
		fileTable[FDFileIndex].filePosition = 512;
	}
	return fileTable[FDFileIndex].filePosition;
}

int FileSystem::fdListDir(char **output){
	CString temp;

	if (dir == 0) {
		temp = printRootDirectory();
	}
	else {
		temp = printDirectory(dir);
	}

	*output = new char[temp.GetLength() + 1];
	strcpy(*output, temp);
	return 1;
}

int FileSystem::fdChangeDir(char *directory){
	int retVal;
	CString temp;
	temp.Format("%s",directory);
	if (temp.Find(".", 0) == -1 || temp.GetAt(0) == '.') temp += ".";
	retVal = findClusterNum(temp);
	if (retVal >= 0) {
		dir = retVal;
		return 1;
	}
	else return 0;
}


int FileSystem::fdEraseFile(char *name){
	int retVal;
	CString temp;
	temp.Format("%s",name);
	if (temp.Find(".", 0) == -1) temp += ".";
	retVal = findClusterNum(temp);
	if (retVal >= 0 && !(curDirectory.Attributes & 0x10)) {
		deleteFile(retVal, dir);		
		return 1;
	}
	else return 0;
}

int FileSystem::fdDelDir(char *name){
	int retVal;
	CString temp;
	temp.Format("%s",name);
	if (temp.Find(".", 0) == -1) temp += ".";
	retVal = findClusterNum(temp);
	if (retVal >= 0 && (curDirectory.Attributes & 0x10)) {
		deleteDirectory(retVal, dir,temp);		
		return 1;
	}
	else return 0;
}


int FileSystem::fdCopyFile(char *source, char *destination){
	int retVal;
	int tempVal;
	CString temp;
	CString temp1;
	temp1.Format("%s",destination);
	if (temp1.Find(".", 0) == -1) temp1 += ".";
	tempVal = findClusterNum(temp1);
	temp.Format("%s",source);
	if (temp.Find(".", 0) == -1) temp += ".";
	retVal = findClusterNum(temp);
	
	if (retVal >= 0 && tempVal == -1 && !(curDirectory.Attributes & 0x10)) {
		if (GetClustersNeeded(retVal) <= numfreeentries) {
			if (copyFile(retVal, temp1, GetClustersNeeded(retVal))) return 1;
			else return 0;
		}
		else return 0;
	}
	else return 0;
	return 0;
}

//Deletes memory allocated to print out stuff
int FileSystem::fdDelMem(char **output) {
	delete[] *output;
	return 1;
}

//Create an empty file and insert a directory entry 
int FileSystem::MakeFile(char *directory) {
	int retVal;
	int start;
	CString temp;
	temp.Format("%s",directory);
	if (temp.Find(".", 0) == -1 || temp.GetAt(0) == '.') temp += ".";
	retVal = findClusterNum(temp);
	if (retVal == -1) {
		if (getnumfree() <= 0) return 0;
		start = 0;//nextfreeentry();
		memcpy(&curDirectory, &dot, sizeof(directory_entry));
		curDirectory.Attributes = 0x20;
		if (WriteDirectoryEntry(temp,start,1) == 0) return -1;
		//memset((void*) buf, 0, sizeof(buf));
		//WriteSector(start + 31, buf, 1);
		//SetCluster(start, 0xFFF);
		//WriteSector(1, FAT1, 9);
		//WriteSector(10, FAT2, 9);
		return getnextempty();
	}
	else return -1;
}

int FileSystem::SetStartCluster(char *directory, int FDFileIndex) {
	int start;
	BYTE buf[512];
	CString temp;
	temp.Format("%s",directory);
	if (temp.Find(".", 0) == -1 || temp.GetAt(0) == '.') temp += ".";
	deleteDirectoryName(temp, fileTable[FDFileIndex].dirCluster);
		if (getnumfree() <= 0) return 0;
		start = nextfreeentry();
		memcpy(&curDirectory, &dot, sizeof(directory_entry));
		curDirectory.Attributes = 0x20;
		if (WriteDirectoryEntry(temp,start,1) == 0) return -1;
		memset((void*) buf, 0, sizeof(buf));
		WriteSector(start + 31, buf, 1);
		SetCluster(start, 0xFFF);
		WriteSector(1, FAT1, 9);
		WriteSector(10, FAT2, 9);
		fileTable[FDFileIndex].startCluster = start;
		return start;
}


//Copy file
int FileSystem::copyFile(int clusternum, CString destination, int numclusters) {
	BYTE buf[512];
	char* output =0;
	int destCluster = nextfreeentry();
	int startCluster = destCluster;
	if (WriteDirectoryEntry(destination, destCluster, numclusters)) {
	while (clusternum < 0xFF8 && clusternum !=0) {
		if (ReadSector(clusternum + 31, buf, 1) == 1) {		
			WriteSector(destCluster + 31, buf, 1);
			clusternum = GetCluster(clusternum);
			if (clusternum >= 0xFF8 || clusternum == 0) {
				SetCluster(destCluster, 0xFFF);
			}
			else {
				SetCluster(destCluster, nextfreeentry());
				destCluster = GetCluster(destCluster);
			}
			WriteSector(1, FAT1, 9);
			WriteSector(10, FAT2, 9);
		}
	}
	return 1;
	}
	else {
		return 0;
	}


}


//Write a directory entry to the current directory - allocates new space if out of room - also
//uses curDirectory to get some attributes so set those in curDirectory before calling it
int FileSystem::WriteDirectoryEntry(CString destination, int startCluster, int numclusters) {
	int i,j,k;
	BYTE buf[512];
	int clusternum = dir;
	directory_entry tDirectory;
	fatdate d;
	fattime t;
	CString temp1="";
	CString temp2="";
	char* dest = new char[strlen(destination) + 1];
	if (destination.Find(".",0) == (destination.GetLength() -1)) destination += " ";
	if (destination.GetAt(0) != '.') {
	strcpy(dest, destination);
	temp1.Format(strtok(dest, "."));
	temp2.Format(strtok(NULL, "."));
	}
	else {
		temp2 = "";
		if (destination == "..") temp1 = "..";
		else temp1 = ".";
	}
	while (temp1.GetLength() < 8) {
		temp1 += " ";
	}
	while (temp2.GetLength() < 3) {
		temp2 += " ";
	}
	struct tm *newtime;
    time_t long_time;
    time( &long_time );                /* Get time as long integer. */
    newtime = localtime( &long_time ); /* Convert to local time. */
	d.day = newtime->tm_mday;
	d.month = newtime->tm_mon;
	d.year = newtime->tm_year;
	t.hour = newtime->tm_hour;
	t.min = newtime->tm_min;
	t.sec = newtime->tm_sec;

	if (clusternum == 0) {
			for (i=0; i<sizeOfRoot; i++) {
				if (ReadSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1) == 1) {
					for (j=0; j<(bs.BytesPerSector/32); j++) { 
						memcpy((void*)&tDirectory, (buf + (j*32)), sizeof(tDirectory));
						if (tDirectory.Name[0] == 0xE5 || tDirectory.Name[0] == 0x00) {
							for (k=0; k<8; k++) {
								tDirectory.Name[k] = toupper(temp1[k]);
							}
							for (k=0; k<3; k++) {
								tDirectory.Extension[k] = toupper(temp2[k]);
							}
							tDirectory.Attributes = curDirectory.Attributes;
							tDirectory.startCluster = (WORD) startCluster;
							tDirectory.fileSize = curDirectory.fileSize;
							memcpy(tDirectory.Reserved,curDirectory.Reserved,10);
							memcpy(&tDirectory.Time, &curDirectory.Time, sizeof(WORD));
							memcpy(&tDirectory.Date, &curDirectory.Date, sizeof(WORD));
							memcpy((void*)(buf + (j*32)), &tDirectory, sizeof(tDirectory));
							WriteSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1);
							return 1;
						}
					}//end of for
				}//end of if
			}//end of for
	}//end of if
	else {
	//Print Directory
		while (clusternum < 0xFF8) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
						memcpy((void*)&tDirectory, (buf + (j*32)), sizeof(tDirectory));
						if (tDirectory.Name[0] == 0xE5 || tDirectory.Name[0] == 0x00) {
							for (k=0; k<8; k++) {
								tDirectory.Name[k] = toupper(temp1[k]);
							}
							for (k=0; k<3; k++) {
								tDirectory.Extension[k] = toupper(temp2[k]);
							}
							tDirectory.Attributes = curDirectory.Attributes;
							tDirectory.startCluster = (WORD) startCluster;
							tDirectory.fileSize = curDirectory.fileSize;
						memcpy(tDirectory.Reserved,curDirectory.Reserved,10);
							memcpy(&tDirectory.Time, &curDirectory.Time, sizeof(WORD));
							memcpy(&tDirectory.Date, &curDirectory.Date, sizeof(WORD));
							memcpy((void*)(buf + (j*32)), &tDirectory, sizeof(tDirectory));
							WriteSector(clusternum + 31, buf, 1);
							return 1;
						}//end of if
				}//end of for
			}//end of if
			if (GetCluster(clusternum) >= 0xFF8) {
				if ((numclusters + 1) < numfreeentries) {
					SetCluster(clusternum, nextfreeentry());
					memset((void*) buf, 0, sizeof(buf));
					WriteSector(GetCluster(clusternum) + 31, buf, 1);
					clusternum = GetCluster(clusternum);
					SetCluster(clusternum, 0xFFF);
					WriteSector(1, FAT1, 9);
					WriteSector(10, FAT2, 9);
				}
				else return 0;
			}
			else clusternum = GetCluster(clusternum);
			
		}//end of while
	}
		return 0;
}

//Constructor of FileSystem object
FileSystem::FileSystem(){
	int i = 0;
	fp = fopen("c:\\mylog.txt", "w");
	diskHandle = CreateFile("\\\\.\\A:", GENERIC_READ+GENERIC_WRITE, FILE_SHARE_READ+FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	getbs();
	sizeOfRoot = (bs.MaxRootEntries * 32) / bs.BytesPerSector;
	//FAT1 = new BYTE[bs.TotalSectors1-30];
	FAT1 = new BYTE[512*bs.SectorsPerFAT];
	//FAT2 = new BYTE[bs.TotalSectors1-30];
	FAT2 = new BYTE[512*bs.SectorsPerFAT];
	freeentries = new int[512*bs.SectorsPerFAT];
	getFAT();
	nextfree = 0;
	numfreeentries = 0;
	getFree();
	dir = 0;
	emptyfileindex = 1;
	//memset((void*)blankDirectory, 0, 512);
	//memcpy((void*)blankDirectory, dot, 32);
	//memcpy((void*)(blankDirectory + 32), dotdot, 32);
	memcpy((void*)dot.Name, (void*)".       ", 8);
	memcpy((void*)dot.Extension, (void*)"   ",3);
	dot.Attributes = 0x10;
	memset((void*) dot.Reserved,0, 10);
	dot.Time = 0;
	dot.Date = 0;
	dot.startCluster = 0;
	dot.fileSize = 0;
	memcpy((void*)dotdot.Name, (void*)"..      ",8);
	memcpy((void*)dotdot.Extension, (void*)"   ",3);
	dotdot.Attributes = 0x10;
	memset((void*) dotdot.Reserved,0, 10);
	dotdot.Time = 0;
	dotdot.Date = 0;
	dotdot.startCluster = 0;
	dotdot.fileSize = 0;
	openFileCount = 0;

	for (i = 0; i < 256; i++) {
		fileTable[i] = FileEntry();
	}
}

//Destructor for FileSystem object
FileSystem::~FileSystem() {
	CloseHandle(diskHandle);
	delete[] FAT1;
	delete[] FAT2;
	delete[] freeentries;
}

//initialize free cluster tracker
void FileSystem::getFree() {
	int i;
	int val;
	for (i=2; i<=(2849);i++) {
		val = GetCluster(i);
		if (val == 0) {
			if (nextfree == 0) nextfree = i;
			freeentries[i] = 1;
			numfreeentries++;
		}
		else freeentries[i] = 0;
	}
}


//find the next free FAT entry and return pointer to current free entry
int FileSystem::nextfreeentry() {
	int i = nextfree;
	nextfree++;
	if (nextfree > 2849) nextfree = 2;
	while (freeentries[nextfree] == 0) nextfree++;
	return i;
}

//Check to see if diskHandle is valid
int FileSystem::isValid(){
	if (diskHandle == INVALID_HANDLE_VALUE) return 0;
	else return 1;
}

//Get FAT1 and FAT2
void FileSystem::getFAT() {
	BYTE* buf = new BYTE[512*bs.SectorsPerFAT];
	
	memset((void*)buf, 0, 512*bs.SectorsPerFAT);
	memset((void*)FAT1, 0, 512*bs.SectorsPerFAT);
	memset((void*)FAT2, 0, 512*bs.SectorsPerFAT);

	if (ReadSector(1, buf, bs.SectorsPerFAT) == 1) memcpy((void*)FAT1, buf, 512*bs.SectorsPerFAT);
	if (ReadSector((bs.SectorsPerFAT+1), buf, bs.SectorsPerFAT) == 1) {
		memcpy((void*)FAT2, buf, 512*bs.SectorsPerFAT);
	}
	delete[] buf;
}

//Gets the bootsector
void FileSystem::getbs() {
	BYTE buf[512];

	if (ReadSector(0, buf, 1) == 1) memcpy((void*)&bs, buf, sizeof(bs));
 
}

//Returns a printable string of the directory entry passed in
CString FileSystem::Get_Directory_Entry(directory_entry dirent) {
	unsigned char i;
	fatdate d;
	fattime t;
	CString toReturn="";
	CString temp;
	for (i=0; i<8; i++) {
		toReturn += dirent.Name[i];
	}
	toReturn += "\t";
	for (i=0; i<3; i++) {
		toReturn += dirent.Extension[i];
	}
	if (dirent.Attributes & 0x01) toReturn +="\tR"; else toReturn +="\t-";
	if (dirent.Attributes & 0x02) toReturn +="H"; else toReturn +="-";
	if (dirent.Attributes & 0x04) toReturn +="S"; else toReturn +="-";
	if (dirent.Attributes & 0x08) toReturn +="V"; else toReturn +="-";
	if (dirent.Attributes & 0x10) toReturn +="D"; else toReturn +="-";
	if (dirent.Attributes & 0x20) toReturn +="A\t"; else toReturn +="-\t";
	
	memcpy(&t, &dirent.Time, sizeof(WORD));
	temp.Format("%02d:%02d:%02d\t", t.hour, t.min, t.sec*2);
	toReturn += temp;
	memcpy(&d, &dirent.Date, sizeof(WORD));
	temp.Format("%02d/%02d/%04d\t", d.month, d.day, d.year+1980);
	toReturn += temp;
	temp.Format("%d\t", dirent.startCluster);
	toReturn += temp;
	temp.Format("%d", dirent.fileSize);
	toReturn += temp;
	toReturn += "\n";
	return toReturn;
}

//Print out the contents of the root Directory
CString FileSystem::printRootDirectory() {
	int i;	
	int j;
	BYTE buf[512];
	CString toReturn = "";
	//Print Root Directory
		for (i=0; i<sizeOfRoot; i++) {
			if (ReadSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					if ((curDirectory.Name[0] != 0x00) && (curDirectory.Name[0] != 0xE5)) {
						//Print_Directory_Entry(curDirectory);
						toReturn += Get_Directory_Entry(curDirectory);
					}
				}
			}
		}
		return toReturn;
}

//Print out the contents of a directory
CString FileSystem::printDirectory(int clusternum) {
	int j;
	BYTE buf[512];
	CString toReturn;
	//Print Directory
		while (clusternum < 0xFF8 && clusternum != 0) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					if ((curDirectory.Name[0] != 0x00) && (curDirectory.Name[0] != 0xE5)) {
						toReturn += Get_Directory_Entry(curDirectory);
					}
				}//end of for
			}//end of if
			clusternum = GetCluster(clusternum);
		}//end of while
		return toReturn;
}


//Delete a directory - also recursively deletes any entries
void FileSystem::deleteDirectory(int retVal, int direct, CString name) {
	int i,j;
	BYTE buf[512];
	int clusternum = retVal;
	int temp;
	CString temp1;
	//char test[13];
	if (clusternum != 0) {
		//Print Directory
		while (clusternum < 0xFF8 && clusternum != 0) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					if (curDirectory.Name[0] != '.' && curDirectory.Name[0] != 0xE5 && curDirectory.Attributes & 0x10) {
						for (i=0; i<8; i++) {
							temp1 += curDirectory.Name[i];
						}
						temp1 += ".";
						for (i=0; i<3; i++) {
							temp1 += curDirectory.Extension[i];
						}
						//strcpy(test, temp1);
						deleteDirectory(curDirectory.startCluster, retVal, temp1); 
					}
					else if (curDirectory.Name[0] != '.' && curDirectory.Name[0] != 0xE5 && curDirectory.Name[0] != 0x00) {
						deleteFile(curDirectory.startCluster, retVal); 
					}
				}//end of for
			}//end of if
			temp = clusternum;
			SetCluster(temp, 0);
			WriteSector(1, FAT1, 9);
			WriteSector(10, FAT2, 9);
			clusternum = GetCluster(clusternum);
		}//end of while
		deleteDirectoryName(name, direct);
	}
}

//Deletes a directory entry with the specified name (directory) and in the directory starting as cluster number = direct
int FileSystem::deleteDirectoryName(CString directory, int direct) {
int i,j,k;
	BYTE buf[512];
	CString temp;
	int clusternum = direct;
	if (clusternum == 0) {
			for (i=0; i<sizeOfRoot; i++) {
			if (ReadSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						buf[j*32] = 0xE5;
						WriteSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1);
						curDirectory.Name[0] = 0xE5;
						return 1;
					}
				}
			}
			}
	}
	else {
	//Print Directory
		while (clusternum < 0xFF8) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						buf[j*32] = 0xE5;
						WriteSector(clusternum + 31, buf, 1);
						curDirectory.Name[0] = 0xE5;
						return 1;
					}
				}//end of for
			}//end of if
			clusternum = GetCluster(clusternum);
		}//end of while
	}
		return 0;

}

//Finds the beginning cluster number of a file by searching for the file through all
// the directory entries of the current directory - also sets the curDirectory value to that of
//the directory entry we were looking for
int FileSystem::findClusterNum(CString directory) {
	int i,j,k;
	BYTE buf[512];
	CString temp;
	int clusternum = dir;
	if (clusternum == 0) {
			for (i=0; i<sizeOfRoot; i++) {
			if (ReadSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						return curDirectory.startCluster;
					}
				}
			}
			}
	}
	else {
	//Print Directory
		while (clusternum < 0xFF8 && clusternum != 0) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						return curDirectory.startCluster;
					}
				}//end of for
			}//end of if
			clusternum = GetCluster(clusternum);
		}//end of while
	}
		return -1;
}

int FileSystem::ChangeDirEntry(CString directory, int filesize) {
	int i,j,k;
	BYTE buf[512];
	CString temp;
	int clusternum = dir;
	if (clusternum == 0) {
			for (i=0; i<sizeOfRoot; i++) {
			if (ReadSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						curDirectory.fileSize = filesize;
						memcpy((buf + (j*32)),(void*)&curDirectory, sizeof(curDirectory));
						WriteSector((bs.SectorsPerFAT * 2) + (i+1), buf, 1);
						return 1;
					}
				}
			}
			}
	}
	else {
	//Print Directory
		while (clusternum < 0xFF8 && clusternum != 0) {
			if (ReadSector(clusternum + 31, buf, 1) == 1) {
				for (j=0; j<(bs.BytesPerSector/32); j++) { 
					memcpy((void*)&curDirectory, (buf + (j*32)), sizeof(curDirectory));
					temp = "";
					for (k=0; k<8; k++) {
						temp += curDirectory.Name[k];
					}
					temp += ".";
					for (k=0; k<3; k++) {
						temp += curDirectory.Extension[k];
					}
					temp.Replace(" ", "");
					if (directory.CompareNoCase(temp) == 0) {
						curDirectory.fileSize = filesize;
						memcpy((buf + (j*32)),(void*)&curDirectory, sizeof(curDirectory));
						WriteSector(clusternum + 31, buf, 1);
						return 1;
					}
				}//end of for
			}//end of if
			clusternum = GetCluster(clusternum);
		}//end of while
	}
		return -1;
}
//Find the number of clusters that a file uses 
int FileSystem::GetClustersNeeded(int retVal) {
	int count = 0;
	while (retVal < 0xFF8 && retVal != 0) {
		count++;
		retVal = GetCluster(retVal);
	}
	return count;
}

//Print out the cluster list for a file
void FileSystem::printClusterList(directory_entry dirent) {
	WORD cluster = dirent.startCluster;
	WORD nextcluster;
	
	while (cluster < 0xFF8 && (char) dirent.Name[0] != '.') {
		nextcluster = GetCluster(cluster);
		cout << "Current Cluster: " << cluster << "\t" << "Next Cluster: " << nextcluster << endl;
		cluster = nextcluster;
	}
}


//Delete a file including it's directory entry
void FileSystem::deleteFile(int cluster, int direct) {
	WORD nextcluster;
	CString temp;
	int k;
	
	if (curDirectory.Name[0] != 0xE5 && curDirectory.Name[0] != 0x00) {
	while (cluster < 0xFF8 && cluster != 0) {
		nextcluster = (WORD) GetCluster(cluster);
		SetCluster(cluster, 0);
		
		cluster = nextcluster;
	}
	WriteSector(1, FAT1, 9);
	WriteSector(10, FAT2, 9);
	temp = "";
		for (k=0; k<8; k++) {
			temp += curDirectory.Name[k];
		}
		temp += ".";
		for (k=0; k<3; k++) {
			temp += curDirectory.Extension[k];
		}
		temp.Replace(" ", "");
		deleteDirectoryName(temp, direct);
	}
}


//Print out a File
void FileSystem::printFile(directory_entry dirent) {
	WORD cluster = dirent.startCluster;
	WORD nextcluster;
	BYTE buf[512];
	int i;
	cout << endl;
	while (cluster < 0xFF8) {
		nextcluster = GetCluster(cluster);
		if (ReadSector(cluster + 31, buf, 1) == 1) {
			for (i=0; i < 512; i++) {
				cout << buf[i];
			}
		}
		cluster = nextcluster;
	}
	cout << endl;
}


//Print out the boot sector
void FileSystem::printbs() {
	cout << bs.jump << endl;
	cout << bs.SysName << endl;
	cout << bs.BytesPerSector << endl;
	cout << bs.SectorsPerCluster << endl;
	cout << bs.ReservedSectors << endl;
	cout << bs.FATcount << endl;
	cout << bs.MaxRootEntries << endl;
	cout << bs.TotalSectors1 << endl;
	cout << bs.MediaDescriptor << endl;
	cout << bs.SectorsPerFAT << endl;
	cout << bs.SectorsPerTrack << endl;
	cout << bs.HeadCount << endl;
	cout << bs.HiddenSectors << endl;
	cout << bs.TotalSectors2 << endl;
	cout << bs.DriveNumber << endl;
	cout << bs.Reserved1 << endl;
	cout << bs.ExtBootSignature << endl;
	cout << bs.VolumeSerial << endl;
	cout << bs.VolumeLabel << endl;
	cout << bs.Reserved2 << endl;

}


//Get the value of a cluster entry in the FAT table
WORD FileSystem::GetCluster(WORD fatEntry){
	WORD sector=0;
	
	int FatIndex = ((fatEntry * 3) /2 );
	if (fatEntry & 0x0001)	{
		sector = *((WORD *) (FAT1 + FatIndex));
		sector = sector >> 4 ;
	}
	else {
		    sector =  *((WORD*) (FAT1 + FatIndex));
			sector = sector & 0xfff;
	}
	return sector;
}

//Set a value of a cluster entry in the FAT table
void FileSystem::SetCluster(WORD cluster, WORD FAT12ClusEntryVal) {
	WORD ThisFATEntOffset = (cluster * 3) /2;
	
	if (cluster & 0x0001) {
		FAT12ClusEntryVal = FAT12ClusEntryVal << 4;
		*((WORD *) (FAT1 + ThisFATEntOffset)) = (*((WORD *) (FAT1 + ThisFATEntOffset))) & 0x000F;
		*((WORD *) (FAT2 + ThisFATEntOffset)) = (*((WORD *) (FAT2 + ThisFATEntOffset))) & 0x000F;
	}
	else {
		FAT12ClusEntryVal = FAT12ClusEntryVal & 0x0FFF;
		*((WORD *) (FAT1 + ThisFATEntOffset)) = (*((WORD *) (FAT1 + ThisFATEntOffset))) & 0xF000;
		*((WORD *) (FAT2 + ThisFATEntOffset)) = (*((WORD *) (FAT2 + ThisFATEntOffset))) & 0xF000;
	}
	*((WORD *) (FAT1 + ThisFATEntOffset)) = (*((WORD *) (FAT1 + ThisFATEntOffset))) | FAT12ClusEntryVal;
	*((WORD *) (FAT2 + ThisFATEntOffset)) = (*((WORD *) (FAT2 + ThisFATEntOffset))) | FAT12ClusEntryVal;
	if (FAT12ClusEntryVal != 0) {
		freeentries[cluster] = 0;
		numfreeentries--;
	}
	else {
		freeentries[cluster] = 1;
		numfreeentries++;
	}
	if (cluster == nextfree) nextfreeentry();
	
}

void FileSystem::writeFAT() {
	WriteSector(1, FAT1, 9);
	WriteSector(10, FAT2, 9);
}
//Takes a cluster and chains on numclusters more
int FileSystem::AllocateClusters(int begcluster, int numclusters) {
	int i;
	int temp = begcluster;
	BYTE buf[512];
	memset(buf, 0, 512);
	if (getnumfree() < numclusters) return -1;
	for (i = 0; i <= numclusters; i++) {
		if (i != numclusters) {
			SetCluster(temp, nextfreeentry());
			temp = GetCluster(temp);
		}
		else SetCluster(temp, 0xFFF);
		//WriteSector(temp + 31, buf, 512);
	}
	writeFAT();
	return temp;
}

//Read the count sectors from the index sector into the buffer passed in
int FileSystem::ReadSector(int index, BYTE *buffer, int count) {
	
	int err;
	OVERLAPPED overlap;
	DWORD dwRead;
	memset(&overlap, 0, sizeof(OVERLAPPED));

	overlap.Offset = index * 512;

	if (ReadFile(diskHandle, buffer, 512 * count, &dwRead, &overlap)) err=1;
	else  {
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Default Language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);
		printf("Error: %s\n", lpMsgBuf);
		err=0;

	}
	return err;
}

//Write count sectors from the index sector using data in the buffer passed in
int FileSystem::WriteSector(int index, BYTE *buffer, int count) {
	int err;
	OVERLAPPED overlap;
	DWORD dwWrite;
	memset(&overlap, 0, sizeof(OVERLAPPED));
	overlap.Offset = index * 512;

	if (WriteFile(diskHandle, buffer, 512 * count, &dwWrite, &overlap)) err = 1;
	else {
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Default Language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);
		printf("Error: %s\n", lpMsgBuf);
		err=0;
	}
	return err;
}

//Constructor for a FileEntry object
FileEntry::FileEntry() {
	valid = 0;
}
