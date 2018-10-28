#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_LENGTH 40



typedef struct game
{
	char name1[NAME_LENGTH]; // name of team 1
	char name2[NAME_LENGTH];// name of team 2
	int goals1;   // goals scored  by team 1
	int goals2;  // goals scored by team 2
} GAME;

typedef struct game_node
{
	GAME agame;
	struct game_node *next;
} GAME_NODE;


typedef struct team
{
	char name[NAME_LENGTH];
	int games_played;
	int wins;
	int draws;
	int losses;
	GAME_NODE *games;
} TEAM;

GAME* ReadGames(int *pngames);
GAME ReadGAME();
TEAM* FillTable(int *pnum_teams, GAME all_games[], int num_games);
TEAM* PrepareTableInit(int* tsize, GAME allgames[], int num_games);
TEAM updateTEAM(GAME *name, TEAM* table, int gp, int w, int d, int l);
TEAM * BuildListForward(GAME g);
void PrintTable(TEAM team[], int sz);
TEAM* PrepareTable(int* tsize, GAME allgames[], int num_games);
TEAM* PrepareTableCont(int *sizeOfTable, GAME allgames[], TEAM* table, int num_games);
TEAM updateTEAMCont(TEAM* table, int gp, int w, int d, int l);
void PrintRecList(GAME_NODE* head);
void FreeAllGames(GAME *all_games, int num_games);
void FreeAllTeams(TEAM *table, int num_teams);
void main()
{
	GAME* all_games = NULL;
	TEAM* table = NULL;
	int num_games = 0, num_teams = 0;

	all_games = ReadGames(&num_games);
	table = FillTable(&num_teams, all_games, num_games);
	PrintTable(table, num_teams);
	FreeAllGames(all_games, num_games);
	FreeAllTeams(table, num_teams);
}

GAME* ReadGames(int *pngames) {

	GAME* game = NULL; // NULL is important because we are going using REALLOC
	GAME s;
	char choice;

	*pngames = 0; // set num game to 0
	puts("Do you want to add a game? (y/n)");
	scanf(" %c", &choice);
	while (choice != 'n')
	{
		s = ReadGAME();
		// 1. allocate memory for another student
		game = (GAME*)realloc(game, (*pngames + 1) * sizeof(GAME));
		// 2. copy new game to its place
		game[*pngames] = s;
		// 3. update *pngames
		(*pngames)++;
		// 4. read new game
		puts("Do you want to add a game? (y/n)");
		scanf(" %c", &choice);
	}

	return game;
}
GAME ReadGAME() {

	GAME s;
	printf("name of team 1: ");
	getchar();
	gets(s.name1);
	printf("name of team 2: ");
	scanf("%s", &s.name2);
	printf("goals scored  by team 1: ");
	scanf("%d", &s.goals1);
	printf("goals scored by team 2: ");
	scanf("%d", &s.goals2);

	return s;

}
TEAM* FillTable(int *pnum_teams, GAME all_games[], int num_games) {

	TEAM* table = NULL;
	*pnum_teams = 0;

	table = PrepareTable(pnum_teams, all_games, num_games);

	return table;
}
TEAM* PrepareTable(int* pnum_teams, GAME allgames[], int num_games) {

	TEAM* table = NULL;
	*pnum_teams = 0;
	table = PrepareTableInit(pnum_teams, allgames, num_games);
	table = PrepareTableCont(pnum_teams, allgames, table, num_games);
}
TEAM* PrepareTableInit(int* tsize, GAME allgames[], int num_games) {

	int j = 0, i = 0, l1 = 0, w1 = 0, d = 0, gp = 1, l2 = 1, w2 = 1;
	TEAM t;
	TEAM* table = NULL;

	if ((&allgames[j])->goals1>(&allgames[j])->goals2) {
		w1 = 1;
		w2 = 0;
	}
	else if ((&allgames[j])->goals1 == (&allgames[j])->goals2) {
		d = 1;
		w2 = 0;
		w1 = 0;
		l1 = 0;
		l2 = 0;
	}
	else
	{
		l1 = 1;
		l2 = 0;
	}

	t = updateTEAM((&allgames[i])->name1, table[*tsize].name, gp, w1, d, l1);
	t.games = BuildListForward(allgames[i]);
	table = (TEAM*)realloc(table, (*tsize + 1) * sizeof(TEAM));
	table[*tsize] = t;
	(*tsize)++;

	t = updateTEAM((&allgames[i])->name2, table[*tsize].name, gp, w2, d, l2);
	t.games = BuildListForward(allgames[i]);
	table = (TEAM*)realloc(table, (*tsize + 1) * sizeof(TEAM));
	table[*tsize] = t;
	(*tsize)++;

	return table;

}
TEAM * BuildListForward(GAME g)
{
	GAME_NODE* head = NULL;
	GAME_NODE* tail = NULL;
	GAME t;
	{
		head = (GAME*)malloc(sizeof(GAME));
		head->agame = g;
		head->next = NULL;
		tail = head;

		return head; // returns the address of the first node
	}
}
TEAM* PrepareTableCont(int* tsize, GAME allgames[], TEAM* table, int num_games) {
	int i, j = 0, l1 = 0, w1 = 0, d = 0, gp = 1, l2 = 1, w2 = 1;
	TEAM t;
	int sizeOfTable = 2;

	int check = 0;
	int check1 = 0;
	for (i = 1; i < num_games; i++)
	{
		int insertName1 = 1;
		int insertName2 = 1;
		for (j = 0; j < sizeOfTable; j++)
		{
			if ((&allgames[j])->goals1>(&allgames[j])->goals2) {
				w1 = 1;
				w2 = 0;
			}
			else if ((&allgames[j])->goals1 == (&allgames[j])->goals2) {
				d = 1;
			}
			else
			{
				l1 = 1;
				l2 = 0;
			}

			if (strcmp(allgames[i].name1, table[j].name) == 0) {

				updateTEAMCont((&table[j]), gp, w1, d, l1);
				if (check == 0) {

					GAME_NODE* head = (&table[j])->games;
					GAME_NODE* tail = head;
					tail->next = (GAME*)malloc(sizeof(GAME));
					tail = tail->next; // move tail to the last element
					tail->agame = allgames[i];
					tail->next = NULL;
					check1 = 1;

				}
				else {
					GAME_NODE* head = (&table[j])->games;
					GAME_NODE* tail = (&table[j])->games->next;
					tail->next = (GAME*)malloc(sizeof(GAME));
					tail = tail->next; // move tail to the last element
					tail->agame = allgames[i];
					tail->next = NULL;
					table->games = head;
					insertName1 = 0;
				}
				insertName1 = 0;

			}
			if (strcmp(allgames[i].name2, table[j].name) == 0) {

				updateTEAMCont((&table[j]), gp, w2, d, l2);
				if (check1 == 0) {

					GAME_NODE* head = (&table[j])->games;
					GAME_NODE* tail = head;
					tail->next = (GAME*)malloc(sizeof(GAME));
					tail = tail->next; // move tail to the last element
					tail->agame = allgames[i];
					tail->next = NULL;
					check1 = 1;

				}
				else {
					GAME_NODE* head = (&table[j])->games;
					GAME_NODE* tail = (&table[j])->games->next;
					tail->next = (GAME*)malloc(sizeof(GAME));
					tail = tail->next; // move tail to the last element
					tail->agame = allgames[i];
					tail->next = NULL;
					table->games = head;
					insertName1 = 0;
				}
				insertName2 = 0;

			}

		}
		if (insertName1 == 1)
		{
			t = updateTEAM((&allgames[i])->name1, table[*tsize].name, gp, w1, d, l1);
			t.games = BuildListForward(allgames[i]);
			table = (TEAM*)realloc(table, (*tsize + 1) * sizeof(TEAM));
			table[*tsize] = t;
			(*tsize)++;
			sizeOfTable++;
		}
		if (insertName2 == 1) {
			t = updateTEAM((&allgames[i])->name2, table[*tsize].name, gp, w2, d, l2);
			t.games = BuildListForward(allgames[i]);
			table = (TEAM*)realloc(table, (*tsize + 1) * sizeof(TEAM));
			table[*tsize] = t;
			(*tsize)++;
			sizeOfTable++;
		}
	}

	return table;
}
TEAM updateTEAMCont(TEAM* table, int gp, int w, int d, int l) {
	GAME_NODE* tail = NULL;
	table->games_played += gp;
	table->wins += w;
	table->draws += d;
	table->losses += l;

}
TEAM updateTEAM(GAME *namoe, TEAM* table, int gp, int w, int d, int l) {
	TEAM t;
	table = namoe;
	strcpy(&t.name, table);
	t.games_played = gp;
	t.wins = w;
	t.draws = d;
	t.losses = l;
	return t;
}
void PrintTable(TEAM team[], int sz)
{
	int i;

	for (i = 0; i < sz; i++)
	{
		printf("Name: \t%s\tgames_played:\t%d\twins:\t%d\tdraws\t%d\tlosses\t%d\nThe games this team played :\n", team[i].name, team[i].games_played, team[i].wins, team[i].draws, team[i].losses);
		PrintRecList(team[i].games);
	}
}
void PrintRecList(GAME_NODE* head)
{
	while (head != NULL)
	{
		printf("\nName team1: \n%s\nName team2 :\n%s\ngoals score team1 :\n%d\ngoals score team2 :\n%d\t\t->", head->agame.name1, head->agame.name2, head->agame.goals1, head->agame.goals2);
		head = head->next;
	}
	printf("NULL\n");
}
void FreeAllGames(GAME *all_games, int num_games) {
	free(all_games);
}
void FreeAllTeams(TEAM *table, int num_teams) {

	GAME_NODE* tmp = NULL;
	GAME_NODE* head = &table[0].games->agame;
	while (head!=NULL) {
		
		tmp = head->next;
		free(head->next);
		head = tmp;
	}
	free(table);
}