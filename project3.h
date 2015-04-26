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
	User user1;
	User user2;

}	Relation;

typedef struct GraphStruct {

	User * userList;
	float Lmax;
	float friendshipThreshold;
	int numUsers;

}	Graph;

//	~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~

//	Structure Funcitons

void createUser(  User * user, int id, int age, int gender, int maritalStatus,
	int race, int birthPlace, int language, int occupation, int income );
void destroyUserList( User * userList );
void printUser( User * user );

Graph * createGraph( User * userList, int numUsers, float delta1 );
void destroyGraph( Graph * graph );
void printGraph( Graph * graph );


//	Helper Functions
Graph * parseInputFile( const char * filename );
