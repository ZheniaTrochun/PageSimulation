#include "stdafx.h"

#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "pages.h"

int num_frames = 4; // Number of avaliable pages in page tables
int page_ref_upper_bound = 8; // Largest page reference
int max_page_calls = 1000; // Max number of page refs to test
int last_page_ref = -1; // Last ref
int hits = 0;
int misses = 0;
FILE *logfile = fopen("logfile.txt", "w");
Frame *clock_hand = NULL;

int main() {
	for (int i = 0; i < num_frames; ++i) {
		table.push_back(createEmptyFrame(i));
	}

	genRefs();

	for (int i = 0; i < max_page_calls; i++) {
		get(getRef());
	}

	showSummary();

	fclose(logfile);
	system("pause");
	return 0;
}


void genRefs() {
	for (int i = 0; i < max_page_calls; i++) {
		pages.push_back(genPage());
	}

	return;
}

Page_Ref genPage() {
	Page_Ref page;
	page.page_num = rand() % page_ref_upper_bound;
	return page;
}

Frame createEmptyFrame(int index) {
	Frame framep;
	framep.index = index;
	framep.page = -1;
	framep.r_bit = 0;

	return framep;
}

int getRef() {
	if (!pages.empty()) { // pop Page_Ref off page_refs
		int page_num = pages.begin()->page_num;
		pages.erase(pages.begin());

		return page_num;
	}
	else {
		return rand() % page_ref_upper_bound;
	}
}

void get(int page_ref) {

	last_page_ref = page_ref;
	clockAlgoSimulation();
	showStats();
}

void toSwap(Frame *frame) {

	printf("Page index: %d, Page: %d\n", frame->index, frame->page);

	Frame victim;
	victim = *frame;
	victim.index = 1;
	reverse(victims.begin(), victims.end());
	victims.push_back(victim);
	reverse(victims.begin(), victims.end());
}

void clockAlgoSimulation() {
	
	Frame *framep = &table[0];

	int fault = 0;
	int ind = 1;
	/* Find target (hit), empty page slot (miss), or victim to evict (miss) */
	while (framep != NULL && framep->page > -1 && framep->page != last_page_ref) {
		if (ind == num_frames)
			framep = NULL;
		else
			framep = &table[ind++];
	}

	if (framep != NULL) {
		if (framep->page == -1) {
			framep->page = last_page_ref;
			framep->r_bit = 0;
			fault = 1;
		}
		else { // Found the page, update its R bit to 0
			table.begin()->r_bit = 0;
		}
	}
	else {
		while (clock_hand == NULL || clock_hand->r_bit == 0) {
			if (clock_hand == NULL) {
				clock_hand = &table[0];
			}
			else {
				clock_hand->r_bit = 1;

				if (clock_hand->index == num_frames - 1) {
					clock_hand = NULL;
				} else {
					clock_hand = &table[clock_hand->index + 1];
				}
			}
		}

		toSwap(clock_hand);

		clock_hand->page = last_page_ref;
		clock_hand->r_bit = 0;

		fault = 1;
	}

	if (fault == 1) misses++;
	else hits++;

	return;
}

void showStats() {
	showSummary();
	showList();
}

void showSummary() {
	printf("Frames in Mem: %d, ", num_frames);
	printf("Hits: %d, ", hits);
	printf("Misses: %d, ", misses);
	printf("Hit Persent: %f\n", (double)hits / (double)(hits + misses));

	fprintf(logfile, "Frames in Mem: %d, ", num_frames);
	fprintf(logfile, "Hits: %d, ", hits);
	fprintf(logfile, "Misses: %d, ", misses);
	fprintf(logfile, "Hit Persent: %f\n", (double)hits / (double)(hits + misses));
}

void showList() {
	int colsize = 9;
	
	printf("%-*s: ", 9, "Page #");
	fprintf(logfile, "%-*s: ", 9, "Page #");

	for (int i(0); i < table.size(); i++) {
		printf("%*d", colsize, table[i]);
		fprintf(logfile, "%*d", colsize, table[i]);
	}

	printf("\n%-*s: ", 9, "Page Ref");
	fprintf(logfile, "\n%-*s: ", 9, "Page Ref");

	for (int i(0); i < table.size(); i++) {
		if (table[i].page == -1) {
			printf("%*s", colsize, "_");
			fprintf(logfile, "%*s", colsize, "_");
		} else {
			fprintf(logfile, "%*d", colsize, table[i].page);
			printf("%*d", colsize, table[i].page);
		}
	}

	printf("\n%-*s: ", 9, "R");
	fprintf(logfile, "\n%-*s: ", 9, "R");

	for (int i(0); i < table.size(); i++) {
		printf("%*d", colsize, table[i].r_bit);
		fprintf(logfile, "%*d", colsize, table[i].r_bit);
	}

	printf("\n\n");
	fprintf(logfile, "\n\n");
}

