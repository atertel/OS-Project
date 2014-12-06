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
#define shadowFile "/Users/rich/Desktop/shadow_example.txt"

char* createBigArray(FILE *fp){
    char* bigArray;
    fp = fopen(shadowFile, "rw");
    fgets(bigArray, sizeof(bigArray), fp);
    return bigArray;
}


typedef struct shadowDataNode{
    char *user;
    char hash[50];
    int numDays;
    int daysCanChange;
    int daysMustChange;
    int daysWarn;
    struct shadowDataNode *next, *prev;

}shadowDataNode;

typedef struct tempUser{
    char userinfo[100];
}tempUser;

tempUser* create(){
    FILE *fp;
    tempUser *users = malloc(33*sizeof(tempUser));
    char tempArray[100];
    int i;
    fp = fopen(shadowFile, "rw");

    for (i = 0; i<33; i++) {
        fgets(tempArray, 100, fp);
        strcpy(users[i].userinfo, tempArray);
        fseek(fp, 0, SEEK_CUR);
    }
    return users;
}

shadowDataNode *parse(shadowDataNode *head) {
	char *temp_str;
	int i;
	shadowDataNode *x;
	tempUser *users = create();
	head = malloc(sizeof(shadowDataNode));
	x = head;
	for (i = 0; i<33; i++) {
		temp_str = strtok(users[i].userinfo, ":");
		x->user = temp_str;
		printf("%s", temp_str);

		temp_str = strtok(users[i].userinfo, ":");
		strcpy(x->hash, temp_str);

		temp_str = strtok(users[i].userinfo, ":");
		x->numDays = (int)temp_str;

		temp_str = strtok(users[i].userinfo, ":");
		x->daysCanChange = (int)temp_str;

		temp_str = strtok(users[i].userinfo, ":");
		x->daysMustChange = (int)temp_str;

		temp_str = strtok(users[i].userinfo, ":");
		x->daysWarn = (int)temp_str;

		if(i != 32) {
			x->next = malloc(sizeof(shadowDataNode));
			x = x->next;
		}
	}
	return head;
}

typedef struct userParse {
	char indivUser[100];
}userParse;



/* userParse* parse(tempUser* users) {
	int i, j, k;
	char *temp_user;
	userParse *user = malloc(33*sizeof(userParse));
	for (k = 0; k<33; k++) {
		user[k] = malloc(6*sizeof(userParse));
	}
	for (i = 0; i<33; i++) {
		for(j = 0; j<6; j++) {
			temp_user = strtok(users[i].userinfo, ":");
			strcpy(user[i].indivUser, temp_user);
		}
	}
	return user;
} */



int main(int argc, const char * argv[]){
	/* int i;
    tempUser *rando = create();
	userParse* user = parse(rando);
	for (i = 0; i<70; i++) {
		printf("%s \n", user[i].indivUser);
    } */
	shadowDataNode *x;
	x = parse(x);
	while(x!=NULL) {
		printf("%s", x->user);
		x = x->next;
	}
    return 0;
}
