#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//	Header File
#include "project3.h"

//	~~~~~~~~~~~~~~~~~~~	Main ~~~~~~~~~~~~~~~~~~~
int main( int argc, char * * argv )
{
	char * inputFile = argv[1];
	char * outputFile = argv[2];

	//	Parsing Input to generate Graph
	parseInputFile( inputFile );

	return 0;
}

//	~~~~~~~~~~~~~~~~~~~ Helper Functions ~~~~~~~~~~~~~~~~~~~

Graph * parseInputFile( const char * filename )
{
	FILE * fptr = NULL;
	int numUsers;
	float delta1;
	float delta2;
	float alpha1;
	float alpha2;
	int i = 0;
	User * userList = NULL;
	Graph * graph = NULL;
	int id;
	int age;
	int gender;
	int maritalStatus;
	int race;
	int birthPlace;
	int language;
	int occupation;
	int income;

	//	Open file and check if successful
	fptr = fopen( filename, "r" );

	if( fptr == NULL )
	{
		printf( "ERROR!\nFile %s could not be opened!\n", filename );
		return 0;
	}

	//	Get lines
	fscanf( fptr, "%d,%f,%f,%f,%f", &numUsers, &delta1, &delta2, &alpha1, &alpha2 );

	//	Allocate user List
	userList = malloc( sizeof(User) * numUsers );

	for( i = 0; i < numUsers; i++ )
	{
		//	Parse inputs
		fscanf( fptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d",
				&id,
				&age,
				&gender,
				&maritalStatus,
				&race,
				&birthPlace,
				&language,
				&occupation,
				&income );

		//	Create user instance
		createUser( &userList[i], id, age, gender, maritalStatus, race,
				birthPlace, language, occupation, income );
	}

	for( i = 0; i < numUsers; i++ )
	{
		printUser( &userList[i] );
	}

	graph = createGraph( userList, numUsers, delta1 );
	printGraph( graph );

	//	Close file
	fclose( fptr );

	destroyGraph( graph );

	return NULL;
}

//	~~~~~~~~~~~~~~~~~~~ Structure Funcitons ~~~~~~~~~~~~~~~~~~~

//	~~~~~~~~~~~~~~~~~~~ USER FUNCTIONS

/*
	Function to create users and return
	an instance
*/
void createUser( User * user, int id, int age, int gender, int maritalStatus,
	int race, int birthPlace, int language, int occupation, int income )
{

	//	Set attributes
	user -> id = id;
	user -> age = age ;
	user -> gender = gender;
	user -> maritalStatus = maritalStatus;
	user -> race = race;
	user -> birthPlace = birthPlace;
	user -> language = language ;
	user -> occupation = occupation;
	user -> income = income;

}

/*
	Function to destroy user and free heap
*/
void destroyUserList( User * userList )
{
	free( userList );
}

/*
	Function to print user details
*/
void printUser( User * user )
{
	printf( "User : %d\n----\n", user -> id );
	printf( "User age = %d\n", user -> age );
	printf( "User gender = %d\n", user -> gender );
	printf( "User maritalStatus = %d\n", user -> maritalStatus );
	printf( "User race = %d\n", user -> race );
	printf( "User birthPlace = %d\n", user -> birthPlace );
	printf( "User language = %d\n", user -> language );
	printf( "User occupation = %d\n", user -> occupation );
	printf( "User income = %d\n\n", user -> income );
}

//	~~~~~~~~~~~~~~~~~~~ GRAPH FUNCTIONS

/*
	Function to create graph and return
	an instance
*/
Graph * createGraph( User * userList, int numUsers, float delta1 )
{
	Graph * graph = NULL;

	//	Allocate memory
	graph = malloc( sizeof( Graph ) );
	graph -> userList = userList;
	graph -> numUsers = numUsers;
	graph -> Lmax = 0;
	graph -> friendshipThreshold = delta1;

	return graph;
}

/*
	Function to destroy graph and free heap
*/
void destroyGraph( Graph * graph )
{
	destroyUserList( graph -> userList );
	free( graph );
}

/*
	Function to print graph details
*/
void printGraph( Graph * graph )
{
	printf( "Graph\n----\n" );
	printf( "Number of Users : %d\n", graph -> numUsers );
	printf( "L max : %0.2f\n", graph -> Lmax );
	printf( "Griendship Threshold : %0.2f\n", graph -> friendshipThreshold);
}
