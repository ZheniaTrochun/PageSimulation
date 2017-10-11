#pragma once
#include <vector>
using namespace std;

// List for page tables and victim lists
//LIST_HEAD(Page_Ref_List, Page_Ref) page_refs;
// List for page tables and victim lists
//LIST_HEAD(Frame_List, Frame);


// stuct to hold Frame info
typedef struct Page_Ref {
//	LIST_ENTRY(Page_Ref) pages;
	
	int page_num;
} Page_Ref;



typedef struct Frame {
//	LIST_ENTRY(Frame) frames;
	int index;
	int page; // page frame points to, -1 is empty
	int r_bit; // R bit
} Frame;
vector<Frame> frames;
//	struct Frame_List victim_list; // List to hold frames that were replaced in page table
vector<Frame> victims;

//	struct Frame_List page_table; // List to hold frames in page table
vector<Frame> table;
vector<Page_Ref> pages;

void genRefs();
Page_Ref genPage();
Frame createEmptyFrame(int index);
void get(int page_ref); // page with page ref
int getRef(); // get next page ref however you like
void toSwap(struct Frame *frame); // add victim frame to a victim list

void showList();
void showStats();
void showSummary();

void clockAlgoSimulation();
