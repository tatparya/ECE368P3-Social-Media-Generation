#include <stdio.h>
#include <stdlib.h>

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
	User * user = NULL;
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

	//	Open file
	fptr = fopen( filename, "r" );

	//	Get lines
	fscanf( fptr, "%d,%f,%f,%f,%f", &numUsers, &delta1, &delta2, &alpha1, &alpha2 );
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
		user = createUser( id, age, gender, maritalStatus, race,
				birthPlace, language, occupation, income );

		printUser( user );
	}

	graph = createGraph();

	//	Close file
	fclose( fptr );

	return NULL;

}

//	~~~~~~~~~~~~~~~~~~~ Structure Funcitons ~~~~~~~~~~~~~~~~~~~


//	~~~~~~~~~~~~~~~~~~~ USER FUNCTIONS
User * createUser( int id, int age, int gender, int maritalStatus,
	int race, int birthPlace, int language, int occupation, int income )
{
	//	Allocate Memory
	User * user = NULL;
	user = malloc( sizeof( User ) );

	//	Set attributes
	user -> id = id;
	user ->	age = age ;
	user ->	gender = gender;
	user ->	maritalStatus = maritalStatus;
	user ->	race = race;
	user ->	birthPlace = birthPlace;
	user ->	language = language ;
	user ->	occupation = occupation;
	user ->	income = income;

	return user;
}

void printUser( User * user )
{
	printf( "User\n----\n" );
	printf( "User id = %d\n", user -> id );
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

Graph * createGraph()
{
	Graph * graph = NULL;

	//	Allocate memory
	graph = malloc( sizeof( Graph ) );
	graph -> Lmax = 0;
	graph -> numUsers = 0;

	return graph;
}
