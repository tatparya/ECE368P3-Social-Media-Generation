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

	float Lmax;
	float friendShipThreshold;
	float numUsers;

}	Graph;

//	~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~

//	Structure Funcitons

User * createUser( int id, int age, int gender, int maritalStatus,
	int race, int birthPlace, int language, int occupation, int income );
void printUser( User * user );

//	Helper Functions
Graph * parseInputFile( const char * filename );
