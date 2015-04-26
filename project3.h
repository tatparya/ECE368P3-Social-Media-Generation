#include <stdio.h>
#include <stdlib.h>

//	~~~~~~~~~~~~~~~~~~~ Structures ~~~~~~~~~~~~~~~~~~~
typedef struct Person {

	int id;				//	Person ID

	//	Person Attributes
	int age;
	int gender;
	int maritalStatus;
	int race;
	int birthPlace;
	int language;
	int occupation;
	int income;

}	User;

typedef struct RelationStruct {

	float Uab;
	float Lab;
	int isFriend;

}	Relation;

typedef struct GraphStruct {

	User * userList;
	Relation * * relationMatrix;
	float Lmax;
	float friendshipThreshold;
	int numUsers;

}	Graph;

//	~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~

//	Structure Funcitons

void createUser(  User * , int, int, int, int, int, int, int, int, int );
void destroyUserList( User * );
void printUser( User * );
void printUserList( User *, int );

void createRelationMatrix( Relation *, User, User );
float calculateULab( User, User );
void createFriendship( Relation *, float, float );

Graph * createGraph( User *, int, float );
void destroyGraph( Graph * );
void printGraph( Graph * );


//	Helper Functions
Graph * parseInputFile( const char *, const c	har * );
