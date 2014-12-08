//
//  main.c
//  shadow_parse
//
//  Created by Richard Lancia on 12/4/14.
//  Copyright (c) 2014 Richard Lancia. All rights reserved.
//
//test

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define shadowFile "shadow_example.txt"

int lines = 0;

typedef struct shadowDataNode{
	char *user;
	char *pw_hash;
	int numDays;
	int daysCanChange;
	int daysMustChange;
	int daysWarn;
	struct shadowDataNode *next, *prev;

}shadowDataNode;

typedef struct tempUser{
	char userinfo[100];
}tempUser;

tempUser *create(){
	int i;
	tempUser *myUsers = malloc(sizeof(tempUser));
	FILE *fp;
	char tempArray[100], tempArray_prev[100];
	lines = 0;
	fp = fopen(shadowFile, "rw");
	do{
		strcpy(tempArray_prev, tempArray);
		fgets(tempArray, 100, fp);
		if (strcmp(tempArray, tempArray_prev) != 0) {
			myUsers = realloc(myUsers, (lines+1)*sizeof(tempUser));
		strcpy(myUsers[lines].userinfo, tempArray);
		fseek(fp, 0, SEEK_CUR);
		lines++;
		}
	}
	while (strcmp(tempArray, tempArray_prev) != 0);
	return myUsers;
}

shadowDataNode *parse(shadowDataNode *head) {
	char *temp_str;
	int i;
	shadowDataNode *x;
	tempUser *users = create();
	head = malloc(sizeof(shadowDataNode));
	x = head;
	for (i = 0; i<lines; i++) {
		temp_str = strtok(users[i].userinfo, ":");
		x->user = temp_str;

		temp_str = strtok(NULL, ":");
		x->pw_hash = temp_str;

		temp_str = strtok(NULL, ":");
		x->numDays = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysCanChange = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysMustChange = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysWarn = atoi(temp_str);

		if(i != (lines-1)) {
			x->next = malloc(sizeof(shadowDataNode));
			x = x->next;
		}
	}
	return head;
}

char *deparse(shadowDataNode *x) {
	char *str_int = malloc(10*sizeof(char));
	char *shadow_line = malloc(100*sizeof(char));
    strcpy(shadow_line, x->user);
		strcat(shadow_line, ":");
		strcat(shadow_line, x->pw_hash);
		strcat(shadow_line, ":");
		strcat(shadow_line, itoa(x->numDays, str_int, 10));
		strcat(shadow_line, ":");
		strcat(shadow_line, itoa(x->daysCanChange, str_int, 10));
		strcat(shadow_line, ":");
		strcat(shadow_line, itoa(x->daysMustChange, str_int, 10));
		strcat(shadow_line, ":");
		strcat(shadow_line, itoa(x->daysWarn, str_int, 10));
		strcat(shadow_line, ":::");
return shadow_line;
}

int main() {
	shadowDataNode *head=parse(head);
	shadowDataNode *x=head;
	while(x!=NULL) {
    printf("%s\n", deparse(x));
		x = x->next;
	}
}
