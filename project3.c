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

	//	Parsing Input, Generate Graph and Output
	parseInputFile( inputFile, outputFile );

	return 0;
}

//	~~~~~~~~~~~~~~~~~~~ Helper Functions ~~~~~~~~~~~~~~~~~~~

Graph * parseInputFile( const char * inputFile, const char * outputFile )
{
	FILE * fptr = NULL;
	int numUsers;
	float delta1;
	float delta2;
	float alpha1;
	float alpha2;
	int i = 0;
	Graph * graph1 = NULL;
	Graph * graph2 = NULL;
	User * userList = NULL;
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
	fptr = fopen( inputFile, "r" );

	if( fptr == NULL )
	{
		printf( "ERROR!\nFile %s could not be opened!\n", inputFile );
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

	//printUserList( userList, numUsers );

	graph1 = createGraph( userList, numUsers, delta1 );
	graph2 = createGraph( userList, numUsers, delta2 );

	destroyUserList( userList );
	destroyGraph( graph1 );
	destroyGraph( graph2 );


	//	Close file
	fclose( fptr );

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

void printUserList( User * userList, int numUsers )
{
	int i;
	for( i = 0; i < numUsers; i++ )
	{
		printUser( &userList[i] );
	}
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

//	~~~~~~~~~~~~~~~~~~~ RELATION FUNCTIONS

/*
	Function to create relationship matrix
*/
void createRelationMatrix( Relation * relation,
							User user1, User user2 )
{
	relation -> Uab = calculateULab( user1, user2 );
	relation -> Lab = 0;
	relation -> isFriend = 0;
}

float calculateULab( User user1 ,User user2 )
{
	float Uab;

	int age;
	int gender;
	int maritalStatus;
	int race;
	int birthPlace;
	int language;
	int occupation;
	int income;

	age = abs( user1.age - user2.age );
	gender = abs( user1.gender - user2.gender );
	maritalStatus = abs( user1.maritalStatus - user2.maritalStatus );
	race = abs( user1.race - user2.race );
	birthPlace = abs( user1.birthPlace - user2.birthPlace );
	language = abs( user1.language - user2.language );
	occupation = abs( user1.occupation - user2.occupation );
	income = abs( user1.income - user2.income );

	Uab = sqrt( age * age + gender * gender + maritalStatus * maritalStatus +
				race * race + birthPlace * birthPlace + language * language +
				occupation * occupation + income * income );

	Uab *= 100;
	Uab = trunc( Uab );
	Uab = Uab / 100;

	return Uab;
}

void createFriendship( Relation * relation, float delta, float Lmax )
{
	float Lab = 1 - ( relation -> Uab / Lmax );

	Lab *= 100;
	Lab = trunc( Lab );
	Lab = Lab / 100;

	relation -> Lab = Lab;
	if( Lab > delta )
	{
		relation -> isFriend = 1;
	}
}


//	~~~~~~~~~~~~~~~~~~~ GRAPH FUNCTIONS

/*
	Function to create graph and return
	an instance
*/
Graph * createGraph( User * userList, int numUsers, float delta1 )
{
	Graph * graph = NULL;
	int i, j;

	//	Allocate memory
	graph = malloc( sizeof( Graph ) );
	graph -> relationMatrix = malloc( sizeof( Relation * ) * numUsers );
	for( i = 0; i < numUsers; i++ )
	{
		graph -> relationMatrix[i] = malloc( sizeof( Relation ) * numUsers );
	}

	graph -> userList = userList;
	graph -> numUsers = numUsers;
	graph -> Lmax = 0;
	graph -> friendshipThreshold = delta1;

	//	Create relation matrix
	for( i = 0; i < numUsers; i++ )
	{
		for( j = 0; j < numUsers; j++ )
		{
			createRelationMatrix( &( graph -> relationMatrix[i][j] ),
					graph -> userList[i], graph -> userList[j] );

			if( graph -> relationMatrix[i][j].Uab > graph -> Lmax )
			{
				graph -> Lmax = graph -> relationMatrix[i][j].Uab;
			}
		}
	}

	//	Create friendships based on
	for( i = 0; i < numUsers; i++ )
	{
		for( j = 0; j < numUsers; j++ )
		{
			createFriendship( &(graph -> relationMatrix[i][j] ),
			 	graph -> friendshipThreshold, graph -> Lmax );
		}
	}

	return graph;
}

/*
	Function to destroy graph and free heap
*/
void destroyGraph( Graph * graph )
{
	if( graph == NULL )
		return;

	int i;
	for( i = 0; i < graph -> numUsers; i++ )
	{
		free( graph -> relationMatrix[i] );
	}
	free( graph -> relationMatrix );
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
	printf( "Friendship Threshold : %0.2f\n", graph -> friendshipThreshold);
}
