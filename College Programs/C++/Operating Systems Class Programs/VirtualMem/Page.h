
#if !defined(_Page)
#define _Page


class Page {
public:
	Page();
	Page(int size);
	void setvalue(long offset, long value);
	int getvalue(int offset);
	CString printpage();
private:
	int* arrVals;
	int size;
};

class Frametable  {
public:
	Frametable();
	Frametable(int size, int pageSize);
	Page get(int framenum);
	void set(int framenum, Page page);
	bool full();
	int emptyframe();
	CString printpage(int pagenum);
	void setRefBit(int pagenum, int value);
	void setModBit(int pagenum, int value);
	void setprocess(int pagenum, int value);
	void setvirtualPage(int pagenum, int value);
	int getprocess(int pagenum);
	int getvirtualPage(int pagenum);
	int getPageToReplace();
	CString printStatus();
private:
	Page* arrPages;
	int* process;
	int* virtualPage;
	int* arrRefBits;
	int* arrModBits;
	int pageToConsider;
	int size;
	int used;
};

class Swapspace  {
public:
	Swapspace();
	Swapspace(int size, int pageSize);
	Page get(int swapnum);
	void set(int swapnum, Page page);
	int allocate();
	CString printpage(int pagenum);
private:
	Page* arrPages;
	int size;
	int nextFreeChunk;

};



class Pagetable {
public:
	Pagetable();
	Pagetable(long size);
	void setValBit(int pagenum, int framenum);
	int getValBit(int pagenum);
	void setSwap(int pagenum, int swapnum);
	int getSwap(int pagenum);
private:
	int* arrSwapPages;
	int* arrFramePages;
	int size;
};

class Proc  {
public:
	Proc();
	Proc(int ID, int PageSize);	
	Pagetable myPT;
private:
	int IDNum;
};

class CBuffer {

public:
	CBuffer();
	CString pop();
	void push(CString value);
	CString print();
	HWND mutex;
	HWND full;
	HWND empty;


private:
	CString values[20];
	int numvalues;

};

#else
#define stuff
#endif