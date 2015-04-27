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

typedef struct DijStruct {
	int visited;
	float distance;
}	DijNode;

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
User * createSingleUser( int, int, int, int, int, int, int, int, int );
void destroyUserList( User * );
void printUser( User * );
void printUserList( User *, int );

void createRelationMatrix( Relation *, User, User );
float calculateULab( User, User );
void createFriendship( Relation *, float, float );
void printFriendList( Graph *, User * );
void printRelationBetween( Graph *, User *, User * );
void printRelation( Relation );

Graph * createGraph( User *, int, float );
void destroyGraph( Graph * );
void printGraph( Graph * );


//	Helper Functions

Graph * parseInputFile( const char * );
int isPresent( int *, int, int );
void insertInArray( int *, int, int );
void removeUser( int *, int );
int countUsers( int *, int );
void printMap( DijNode *, int );
int getMin( DijNode *, int, float * );
int allVisited( DijNode *, int );
int getNumNodes( DijNode *, int, float );

//	Query Functions
void getMinLength( Graph *, int );
void getAllNode( Graph *, int, float );
void getFriends( Graph *, int );
void getFriendsOfFriends( Graph *, int );
void getAvgDegreeOfNode( Graph * );
void getAvgDegreeOfSecondNode( Graph * );
