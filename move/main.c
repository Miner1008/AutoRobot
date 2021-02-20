#include<stdio.h>  
#include<conio.h>
#include<string.h>
#include <math.h>

#ifdef _DEBUG
#define INFO_TEXT_PATH		"./Debug/"
#else
#define INFO_TEXT_PATH		"./Release/"
#endif

#define MAX_ROBOT_COUNT 5
#define MAX_BOARD_CELL 20

#define TURN_LEFT_COMMAND	"turnleft"
#define TURN_RIGHT_COMMAND	"turnright"
#define MOVE_COMMAND		"move"
#define SHOW_COMMAND		"show"
#define WHITHIN_COMMAND		"within"
#define TRAVELLED_COMMAND	"travelled"

typedef enum {Fale, True} boolean;
typedef enum {left, top, right, bottom} direction;

struct Robot {
	int id;
	int groupId;
	int posX;
	int posY;

	direction posDirection;
};

struct Board {
	int robotId;
	int robotGroupId;
	
	boolean isPlaced;
};

struct Robot robotNode[MAX_ROBOT_COUNT];
struct Board boardCell[MAX_BOARD_CELL][MAX_BOARD_CELL];

static void movePosition(int robotNo, char* moveCommand, int value)
{
	int oldX, oldY, newX, newY;

	if (strcmp(MOVE_COMMAND, moveCommand) != 0)
		return;

	oldX = robotNode[robotNo].posX;
	oldY = robotNode[robotNo].posY;
	newX = robotNode[robotNo].posX;
	newY = robotNode[robotNo].posY;

	if (robotNode[robotNo].posDirection == left)
		newX -= value;
	else if (robotNode[robotNo].posDirection == right)
		newX += value;
	else if (robotNode[robotNo].posDirection == top)
		newY += value;
	else if (robotNode[robotNo].posDirection == bottom)
		newY -= value;

	if (boardCell[newX][newY].isPlaced == Fale)
	{
		robotNode[robotNo].posX = newX;
		robotNode[robotNo].posY = newY;
	}else{
		if (boardCell[newX][newY].robotGroupId != robotNode[robotNo].groupId)
		{
			robotNode[robotNo].posX = newX;
			robotNode[robotNo].posY = newY;

			//memset(robotNode[boardCell[newX][newY].robotGroupId], 0, sizeof(struct Robot));
		}
	}
	
}

static direction setDirection(int robotNo , char* directionCommand, int value )
{
	int i = 0;
	int oldValue = (int)robotNode[robotNo].posDirection;
	value = value % 4;
	if (strcmp(TURN_RIGHT_COMMAND, directionCommand) == 0)
	{
		oldValue += value;
		oldValue = oldValue % 4;

		robotNode[robotNo].posDirection = (direction)oldValue;
	}
	else if (strcmp(TURN_LEFT_COMMAND, directionCommand) == 0)
	{
		/*use offset to right*/
		value = 4 - value;

		oldValue += value;
		oldValue = oldValue % 4;

		robotNode[robotNo].posDirection = (direction)oldValue;
	}
}

void main()  
{  
	FILE *positionFile = NULL;
	FILE *moveFile = NULL;
	char positionInfo[4096];
	char moveInfo[4096];
	int robotCount = 0;
	int selectedRobotNo = 0;

	boolean readable = True;
	char moveCommand[32];
	int value = 0;

	int index = 0, i = 0, j = 0;

	char positionFileName[1024] = "";
	char moveFileName[1024] ="";
	char positionFilePath[256] = INFO_TEXT_PATH;
	char moveFilePath[256] = INFO_TEXT_PATH;

	int positionValue[4];

	printf("Enter position information file name :");
	scanf("%s", positionFileName);

	printf("Enter move information file name :");
	scanf("%s", moveFileName);

	printf("Robot is moving by these two text %s, %s\n", positionFileName, moveFileName);

	memset(robotNode, 0, sizeof(struct Robot));
	memset(boardCell, 0, sizeof(struct Board) * MAX_BOARD_CELL * MAX_BOARD_CELL);

	strcat(positionFilePath, positionFileName);
	strcat(moveFilePath, moveFileName);
 
	/*load position information*/
	positionFile = fopen(positionFilePath, "r");
    if (positionFile == NULL){
        printf("Could not open file %s", positionFileName);
        return;
    }

	/*init robotNode and boardCell by inputed position information file*/
	for (i=0; i < MAX_ROBOT_COUNT; i++)
	{
		for (j=0; j < 4; j++)
		{
			if (fscanf(positionFile, "%d", &value ) == 1)
				positionValue[i] = value;		
			else
				break;
		}
		
		/*in case failed read file data*/
		if (positionValue[0] < 0)
			break;

		/*set robot information*/
		robotNode[index].id = positionValue[0];
		robotNode[index].groupId = positionValue[1];
		robotNode[index].posX = positionValue[2];
		robotNode[index].posY = positionValue[3];
		robotCount++;

		/*Set board cell information*/
		boardCell[positionValue[2]][positionValue[3]].isPlaced = True;
		boardCell[positionValue[2]][positionValue[3]].robotId = positionValue[0];
		boardCell[positionValue[2]][positionValue[3]].robotGroupId = positionValue[1];

		/*check end point of file*/
		positionValue[0] = -1;
	}

	/*load move information*/
	moveFile = fopen(moveFilePath, "r");
	if (moveFile == NULL){
		printf("Could not open file %s", moveFileName);
		return;
	}

	/*set move information all roboatNode by inputed move information file*/
	readable = True;
	while(readable)
	{
		if (fscanf(moveFile, "%s", moveCommand) != 1)
			break;
		
		if (fscanf(moveFile, "%d", & value) != 1)
			break;

		/*set direction*/
		setDirection(selectedRobotNo, moveCommand, value);
		
		/*move robot*/
		movePosition(selectedRobotNo, moveCommand, value);
	}
	
	printf("robot moved to follow position...\n");
	printf("x = %d \n y = %d ", robotNode[selectedRobotNo].posX, robotNode[selectedRobotNo].posY);

    fclose(positionFile);
	fclose(moveFile);

	getch();
}

struct Robot newRobat(int id, int groupId, int originX, int originY)
{
	struct Robot item;
	
	item.id = id;
	item.groupId = groupId;
	item.posX = originX;
	item.posY = originY;

	return item;
}