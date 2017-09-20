#include "stdafx.h"
#include "Page.h"
#define RETURN "\r\n"

//Implementation of a CBuffer
CBuffer::CBuffer() {
	int i;
	mutex = (HWND) CreateSemaphore(NULL, 1, 1, NULL);
	full = (HWND) CreateSemaphore(NULL, 20, 20, NULL);
	empty = (HWND) CreateSemaphore(NULL, 0,20, NULL);
	for (i =0; i<20; i++) {
		values[i] = "";
	}
	numvalues = 0;
}

CString CBuffer::pop() {
	CString temp;
	WaitForSingleObject(empty, INFINITE);
	WaitForSingleObject(mutex, INFINITE);
	numvalues -= 1;
	temp =  values[numvalues];
	ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(full, 1, NULL);
	return temp;
}
void CBuffer::push(CString value) {
	WaitForSingleObject(full, INFINITE);
	WaitForSingleObject(mutex, INFINITE);
	values[numvalues] = value;
	numvalues += 1;
	ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(empty, 1, NULL);
}

CString CBuffer::print() {
	int i;
	CString temp = "";

	WaitForSingleObject(mutex, INFINITE);
	for (i = 0; i<numvalues; i++) {
		temp += values[i];
	}
	ReleaseSemaphore(mutex, 1, NULL);
	return temp;
}


//PAGETABLE CLASS IMPLEMENTATION
Pagetable::Pagetable() {
	arrSwapPages = new int[1];
	arrFramePages = new int[1];
	size = 1;
}


Pagetable::Pagetable(long siz) {
	int loop;
	arrSwapPages = new int[siz];
	arrFramePages = new int[siz];
	size = siz;

	for (loop = 0; loop < size; loop++) {
		arrFramePages[loop] = -1;
		arrSwapPages[loop] = -1;
	}
	
}

void Pagetable::setValBit(int pagenum, int framenum) {
	arrFramePages[pagenum] = framenum;
}

int Pagetable::getValBit(int pagenum) {
	return arrFramePages[pagenum];
}

void Pagetable::setSwap(int pagenum, int swapnum) {
	arrSwapPages[pagenum] = swapnum;
}

int Pagetable::getSwap(int pagenum) {
	return arrSwapPages[pagenum];
}


//PAGE CLASS IMPLEMENTATION
Page::Page() {
	arrVals = new int[1];
	size = 1;
}

Page::Page(int siz) {
	int i;
	arrVals = new int[siz / 4];
	size = siz / 4;
	for (i=0; i < (siz /4); i++) {
		arrVals[i] = -1;
	}
}


void Page::setvalue(long offset, long value) {
	arrVals[(offset / 4)] = value;
}

int Page::getvalue(int offset) {
	return arrVals[(offset / 4)];
}

CString Page::printpage() {
	CString myString;
	char tempString[10];
	CString val;
	CString page;
	int i;
	for (i = 0; i < size; i++) {
		itoa(arrVals[i], tempString, 10);
		val.Format(tempString);
		itoa(i, tempString, 10);
		page.Format(tempString);
		myString += page + "   " + val + RETURN;
	}
	return myString;
}



//FRAMETABLE CLASS IMPLEMENTATION
Frametable::Frametable() {
	arrPages = new Page[1];
	size = 1;
	used = 0;
	process = new int[1];
	virtualPage  = new int[1];
	arrRefBits = new int[1];
	arrModBits = new int[1];
}


Frametable::Frametable(int siz, int pageSize) {
	arrPages = new Page[siz];
	size = siz;
	used = 0;
	pageToConsider = 0;
	process = new int[siz];
	virtualPage  = new int[siz];
	arrRefBits = new int[siz];
	arrModBits = new int[siz];
	int loop;
	for (loop = 0; loop < siz; loop++) {
		//arrPages[loop] = *(new Page(pageSize));
		virtualPage[loop] = -1;
		arrRefBits[loop] = -1;
		arrModBits[loop] = 0;
	}
}

int Frametable::getPageToReplace() {
	int retVal = 0;
	while (1) {
		if (arrRefBits[pageToConsider] == 0) {
			retVal = pageToConsider;
			pageToConsider++;
			if (pageToConsider == size) pageToConsider = 0;
			return retVal;
		}
		else {
			arrRefBits[pageToConsider] = 0;
			pageToConsider++;
			if (pageToConsider == size) pageToConsider = 0;
		}
	}
}

void Frametable::setRefBit(int pagenum, int value) {
	arrRefBits[pagenum] = value;
}

void Frametable::setModBit(int pagenum, int value) {
	arrModBits[pagenum] = value;
	
}

Page Frametable::get(int framenum) {
	return arrPages[framenum];	
};

void Frametable::set(int framenum, Page page) {
	arrPages[framenum] = page;
}

bool Frametable::full() {
	if (used == size) return true;
	else return false;
}

int Frametable::emptyframe() {
	int i=0;
	for (i=0; i < size;i++) {
		if (virtualPage[i] == -1) return i;
	}
	return -1;
}

CString Frametable::printpage(int pagenum) {
	if (pagenum < size) return arrPages[pagenum].printpage();
	return "";
}

CString Frametable::printStatus() {
	int loop;
	CString message = "Proc  Frame  ref  dirty";
	char temp[256];
	message += RETURN;
	for (loop = 0; loop < size; loop++) {
		if (arrRefBits[loop] == -1) {
			message += "Not Allocated";
			message += RETURN;
		}
		else {
			sprintf(temp, " %d      %d      %d    %d", process[loop],virtualPage[loop],arrRefBits[loop] , arrModBits[loop]);
			message += temp;
			message += RETURN;
		}
	}
	return message;
}

void Frametable::setprocess(int pagenum, int value) {
	process[pagenum] = value;
}

void Frametable::setvirtualPage(int pagenum, int value) {
	virtualPage[pagenum] = value;
}

int Frametable::getprocess(int pagenum) {
	return process[pagenum];
}

int Frametable::getvirtualPage(int pagenum) {
	return virtualPage[pagenum];
}
//SWAPSPACE CLASS IMPLEMENTATION
Swapspace::Swapspace() {
	arrPages = new Page[1];
	size = 1;
	nextFreeChunk = 0;
}

Swapspace::Swapspace(int siz, int pageSize) {
	arrPages = new Page[siz];
	size = siz;
	nextFreeChunk = 0;
	int loop;
	for (loop = 0; loop < siz; loop++) {
		arrPages[loop] = *(new Page(pageSize));
	}
}

Page Swapspace::get(int swapnum) {
	return arrPages[swapnum];	
}


CString Swapspace::printpage(int pagenum) {
	if (pagenum < size) return arrPages[pagenum].printpage();
	return "";
} 

int Swapspace::allocate() {
	if (nextFreeChunk < (size -1)) {
		nextFreeChunk++;
		return (nextFreeChunk-1);
	}
	else return -1;
}

void Swapspace::set(int swapnum, Page page) {
	arrPages[swapnum] = page;
}


//PROC CLASS IMPLEMENTATION
Proc::Proc() {
	IDNum = -1;
	myPT = NULL;
}


Proc::Proc(int ID, int PageSize) {
	IDNum = ID;
	myPT = Pagetable(PageSize);
}