#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//	Header File
#include "project3.h"

//	~~~~~~~~~~~~~~~~~~~	Main ~~~~~~~~~~~~~~~~~~~
int main( int argc, char * * argv )
{
	char * inputFile = argv[1];

	int i = 0;
	for( i = 0; i < argc; i++ )
	{
		printf( "%s\n", argv[i] );
	}

	//	Parsing Input, Generate Graph and Output
	parseInputFile( inputFile );

	return 0;
}

//	~~~~~~~~~~~~~~~~~~~ Helper Functions ~~~~~~~~~~~~~~~~~~~

Graph * parseInputFile( const char * inputFile )
{
	FILE * fptr = NULL;
	int numUsers;
	float delta1;
	float delta2;
	float alpha;
	int queryId;
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
	fscanf( fptr, "%d,%f,%f,%d,%f", &numUsers, &delta1, &delta2, &queryId, &alpha );

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

	/*
	int inputid;
	printf( "Enter user id for friends\n" );
	scanf( "%d", &inputid );
	while( inputid != 0 )
	{
		printFriendList( graph1, &(graph1 -> userList[ inputid - 1 ]) );
		printf( "Enter user id for friends\n" );
		scanf( "%d", &inputid );
	}

	int j;
	for( i = 0; i < numUsers; i ++ )
	{
		for( j = 0; j < numUsers; j++ )
		{
			printRelationBetween( graph1,
			&(graph1 -> userList[i]), &(graph1 -> userList[j]) );
		}
	}
	*/

	//  ~~~~~~~~~~~~~~~~~~~	QUERYS ~~~~~~~~~~~~~~~~~~~

	//	Dense Graph
	printf(  "Dense Graph : \n-------------\n" );
	printf(  "\n** Query1 **\n" );
	getMinLength( graph1, queryId );
	printf(  "\n** Query2 **\n" );
	getAllNode( graph1, queryId, alpha );
	printf(  "\n** Query3 **\n" );
	getFriends( graph1, queryId );
	printf(  "\n** Query4 **\n" );
	getFriendsOfFriends( graph1, queryId );
	printf(  "\n** Query5 **\n" );
	getAvgDegreeOfNode( graph1 );
	printf(  "\n** Query6 **\n" );
	getAvgDegreeOfSecondNode( graph1 );

	//	Sparse Graph
	printf(  "\nSparse Graph : \n-------------\n" );
	printf(  "\n** Query1 **\n" );
	getMinLength( graph2, queryId );
	printf(  "\n** Query2 **\n" );
	getAllNode( graph2, queryId, alpha );
	printf(  "\n** Query3 **\n" );
	getFriends( graph2, queryId );
	printf(  "\n** Query4 **\n" );
	getFriendsOfFriends( graph2, queryId );
	printf(  "\n** Query5 **\n" );
	getAvgDegreeOfNode( graph2 );
	printf(  "\n** Query6 **\n" );
	getAvgDegreeOfSecondNode( graph2 );

	//  ~~~~~~~~~~~~~~~~~~~	CLEAN UP  ~~~~~~~~~~~~~~~~~~~

	destroyUserList( userList );
	destroyGraph( graph1 );
	destroyGraph( graph2 );

	//	Close file
	fclose( fptr );
	return NULL;
}

//	~~~~~~~~~~~~~~~~~~~ QUERY Funcitons ~~~~~~~~~~~~~~~~~~~


/*
QUERY 1:	Function to get number of friends and print
all friends
*/
void getMinLength( Graph * graph, int queryId )
{
	int i, j;
	int numMin;
	float LabMin = 1;
	float Lab;
	int * idArray;
	idArray = malloc( sizeof( int ) * graph -> numUsers );
	for( i = 0; i < graph -> numUsers; i++ )
	{
		if( graph -> relationMatrix[ queryId-1 ][i].isFriend == 1 )
		{
			Lab = graph -> relationMatrix[ queryId-1 ][i].Lab;
			if( Lab < LabMin )
			{
				LabMin = Lab;
				idArray[0] = i + 1;
				numMin = 1;
				j = 1;
			}
			else if( Lab == LabMin )
			{
				idArray[ numMin ] = i + 1;
				numMin ++;
				j++;
			}
		}
	}

	printf(  "Minimum Path Length : %0.2f", LabMin );
	printf(  "\nIDs: " );
	for( i = 0; i < numMin; i++ )
	{
		printf(  "%d", idArray[i] );
		j--;
		if( j )
		{
			printf(  ", " );
		}
		else
		{
			printf(  "\n" );
		}
	}
	free( idArray );
}

/*
	QUERY 2:	Get all shortes paths to nodes
*/
void getAllNode( Graph * graph, int queryId, float alpha )
{
	int i, j;
	float min, dist;
	int minInd;
	DijNode * map;

	//	Allocate dij map
	map = malloc( sizeof( DijNode ) * graph -> numUsers );

	//	Populaing the map
	for( i = 0; i < graph -> numUsers; i++ )
	{
		if( graph -> relationMatrix[ queryId - 1 ][i].isFriend == 1 )
			map[i].distance = graph -> relationMatrix[ queryId - 1 ][i].Lab;
		else
			map[i].distance = 9999;

		if( i == queryId-1 )
		{
			map[i].visited = 1;
			map[i].distance = 0;
		}

		else
			map[i].visited = 0;
	}

	//printMap( map, graph -> numUsers );

	for( i = 0; i < graph -> numUsers; i++ )
	{
		//	Get minimum, minInd gives new path
		minInd = getMin( map, graph -> numUsers, &min );
		map[minInd].visited = 1;
		for( j = 0; j < graph -> numUsers; j++ )
		{
			if( map[i].visited == 0 && graph -> relationMatrix[ minInd ][j].isFriend == 1 )
			{
				dist = graph -> relationMatrix[minInd][j].Lab;
				if( dist + min < map[j].distance )
				{
					map[j].distance = dist + min;
				}
			}
		}
		//printMap( map, graph -> numUsers );
	}

	printf(  "Number of nodes within threshold: %d\n",
			getNumNodes( map, graph -> numUsers, alpha ) );
}

int getNumNodes( DijNode * map, int numUsers, float alpha )
{
	int i = 0;
	int count = 0;
	for( i = 0; i < numUsers; i++ )
	{
		if( map[i].distance < alpha )
		{
			count++;
		}
	}

	return count - 1;
}

int allVisited( DijNode * map, int numUsers )
{
	int i = 0;
	for( i = 0; i < numUsers; i++ )
	{
		if( map[i].visited == 0 )
			return 0;
	}

	return 1;
}

void printMap( DijNode * node, int numUsers )
{
	int i;
	for( i = 0; i < numUsers; i++ )
	{
		printf( "%d %0.2f %d\n", i+1, node[i].distance, node[i].visited );
	}
	printf( "\n" );
}

int getMin( DijNode * map, int numUsers, float * num )
{
	int i;
	float min = 9999;
	int minInd = 0;
	for( i = 1; i < numUsers; i++ )
	{
		if( map[i].visited == 0 && map[i].distance < min )
		{
			min = map[i].distance;
			minInd = i;
		}
	}

	* num = min;

	return minInd;
}

/*
	QUERY 3:	Function to get number of friends and print
				all friends
*/
void getFriends( Graph * graph,  int queryId )
{
	int i, j;
	int numFriends = 0;
	int * idArray;
	idArray = malloc( sizeof( int ) * graph -> numUsers );
	for( i = 0; i < graph -> numUsers; i++ )
	{
		if( graph -> relationMatrix[ queryId-1 ][i].isFriend == 1 )
		{
			idArray[ numFriends ] = i + 1;
			numFriends++;
		}
	}

	printf(  "Num friends : %d\n", numFriends );
	j = numFriends;

	printf( "Ids :\t");

	for( i = 0; i < numFriends; i++ )
	{
		printf(  "%d", idArray[i] );
		j--;
		if( j )
		{
			printf(  ", " );
		}
		else
		{
			printf(  "\n" );
		}
	}

	free( idArray );
}

/*
	QUERY 4:	Function to get number of friends and print
				all friends
*/
void getFriendsOfFriends( Graph * graph,  int queryId )
{
	int i, j;
	int numFriends = 0;
	int numFriends2 = 0;
	int * idArray;
	int * idArray2;
	idArray = malloc( sizeof( int ) * graph -> numUsers );
	idArray2 = malloc( sizeof( int ) * graph -> numUsers );
	for( i = 0; i < graph -> numUsers; i++ )
	{
		if( graph -> relationMatrix[ queryId-1 ][i].isFriend == 1 )
		{
			idArray[ numFriends ] = i + 1;
			idArray2[ numFriends2 ] = i + 1;
			numFriends++;
			numFriends2++;
		}
	}

	int index;
	for( i = 0; i < numFriends; i++ )	//	Iterate through 1st Friends
	{
		for( j = 0; j < graph -> numUsers; j++ )	//	Iterate through all users
		{
			if( graph -> relationMatrix[ idArray[i] - 1 ][ j ].isFriend == 1 && j != queryId - 1 )
			{
				index = isPresent( idArray2, numFriends2, j + 1 );
				if( index < 0 )
				{
					insertInArray( idArray2, numFriends2, j + 1 );
					numFriends2++;
				}
			}
		}
	}

	printf(  "Num friends two hops away : %d\n", numFriends2 );
	j = numFriends2;

	printf( "Ids :\t");

	for( i = 0; i < numFriends2; i++ )
	{
		printf(  "%d", idArray2[i] );
		j--;
		if( j )
		{
			printf(  ", " );
		}
		else
		{
			printf(  "\n" );
		}
	}

	free( idArray );
	free( idArray2 );
}

int isPresent( int * array, int numElements, int element )
{
	int index;
	int beg = 0;
	int end = numElements - 1;
	int mid = ( beg + end ) / 2;
	int found = 0;

	while( beg <= end && !found )
	{
		mid = ( beg + end ) / 2;
		if( array[ mid ] == element )
		{
			found++;
			index = mid;
		}
		else if( array[ mid ] > element )
		{
			end = mid - 1;
		}
		else
		{
			beg = mid + 1;
		}
	}
	if( found )
	{
		return index;
	}
	else
	{
		return -1;
	}
}

void insertInArray( int * array, int numElements, int element )
{
	int end = numElements - 1;
	while( element < array[ end ] )
	{
		array[ end + 1 ] = array[ end ];
		end--;
	}
	array[ end + 1 ] = element;
}


/*
	QUERY 5:	Function to get average degree of nodes
				in graph
*/
void getAvgDegreeOfNode( Graph * graph )
{
	int i, j;
	int numFriends = 0;
	float avg;

	for( j = 0; j < graph -> numUsers; j++ )
	{
		for( i = 0; i < graph -> numUsers; i++ )
		{
			if( graph -> relationMatrix[j][i].isFriend == 1 )
			{
				numFriends++;
			}
		}
	}

	avg = (float) numFriends / graph -> numUsers;
	avg *= 100;
	avg = trunc( avg );
	avg = avg / 100;
	printf(  "Average Degree of Nodes : %0.2f\n", avg );
}



/*
	QUERY 6:	Function to get average degree of second-level Friends
				in graph
*/
void getAvgDegreeOfSecondNode( Graph * graph )
{

	int k;
	int i, j;
	int numFriends = 0;
	int numFriends2 = 0;
	int * idArray;
	int * idArray2;
	int numSecondLevelFriends = 0;
	float avg;

	for( k = 0; k < graph -> numUsers; k++ )
	{
		numFriends = 0;
		numFriends2 = 0;
		idArray = malloc( sizeof( int ) * graph -> numUsers );
		idArray2 = malloc( sizeof( int ) * graph -> numUsers );
		for( i = 0; i < graph -> numUsers; i++ )
		{
			if( graph -> relationMatrix[ k ][i].isFriend == 1 )
			{
				idArray[ numFriends ] = i + 1;
				idArray2[ numFriends2 ] = i + 1;
				numFriends++;
			}
		}

		int index;
		for( i = 0; i < numFriends; i++ )	//	Iterate through 1st Friends
		{
			for( j = 0; j < graph -> numUsers; j++ )	//	Iterate through all users
			{
				if( graph -> relationMatrix[ idArray[i] - 1 ][ j ].isFriend == 1 && j != k )
				{
					index = isPresent( idArray2, numFriends2, j + 1 );
					if( index < 0 )
					{
						insertInArray( idArray2, numFriends2, j + 1 );
						numFriends2++;
					}
				}
			}
		}
		numSecondLevelFriends += numFriends2;

		free( idArray );
		free( idArray2 );
	}

	avg = (float) numSecondLevelFriends / graph -> numUsers;
	avg *= 100;
	avg = trunc( avg );
	avg = avg / 100;
	printf(  "Average Degree of Nodes : %0.2f\n", avg );
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

User * createSingleUser( int id, int age, int gender, int maritalStatus,
int race, int birthPlace, int language, int occupation, int income )
{

	User * user;
	user = malloc( sizeof( User ) );

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

	return user;
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

	age = 			abs( user1.age - 			user2.age 			);
	gender = 		abs( user1.gender - 		user2.gender 		);
	maritalStatus = abs( user1.maritalStatus - 	user2.maritalStatus );
	race = 			abs( user1.race - 			user2.race 			);
	birthPlace = 	abs( user1.birthPlace - 	user2.birthPlace 	);
	language = 		abs( user1.language - 		user2.language 		);
	occupation = 	abs( user1.occupation - 	user2.occupation 	);
	income = 		abs( user1.income - 		user2.income 		);

	Uab = sqrt( age * age + gender * gender + maritalStatus * maritalStatus +
				race * race + birthPlace * birthPlace + language * language +
				occupation * occupation + income * income );

	Uab *= 100;
	//printf( "Userid = %d %d\nBefore : %f\n", user1.id, user2.id, Uab );
	Uab = trunc( Uab );
	//printf( "Trun = %f\n", Uab );
	Uab = Uab / 100;
	//printf( "Final = %f\n", Uab );
	//printf( "Final = %0.2f\n", Uab );

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

void printFriendList( Graph * graph, User * user )
{
	int i;
	printf( "FriendShip Status\n-----------------\nUser %d is friends with the following users:\n",
			user -> id );
	for( i = 0; i < graph -> numUsers; i++ )
	{
		if( graph -> relationMatrix[ user -> id - 1 ][i].isFriend == 1 )
		{
			printf( "%d ", i + 1 );
		}
	}
	printf( "\n" );
}

void printRelationBetween( Graph * graph, User * user1, User * user2 )
{
	Relation relation;
	relation = graph -> relationMatrix[ user1 -> id - 1 ][ user2 -> id - 1 ];

	printf( "Relation Status : User %d to User %d\n", user1 -> id, user2 -> id );
	printRelation( relation );
}

void printRelation( Relation relation )
{

	printf( "Uab : %0.2f\n", relation.Uab );
	printf( "Lab : %0.2f\n",  relation.Lab );
	printf( "Is friend : %d\n\n", relation.isFriend );
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
			if ( i != j )
			{
				createFriendship( &(graph -> relationMatrix[i][j] ),
					graph -> friendshipThreshold, graph -> Lmax );
			}
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
