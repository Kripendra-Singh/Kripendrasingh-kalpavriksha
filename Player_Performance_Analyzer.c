/*
 * @Author: Kripendra Singh
 * @Description: This program demonstrates a Smart Player Performance Analyzer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Players_data.h"

#define TEAM_COUNT 10
#define MAX_PLAYER 50
#define NAME_LENGTH 51

typedef enum PlayerRole {
    BATSMAN = 1,
    BOWLER = 2,
    ALL_ROUNDER = 3
} PlayerRole;

typedef struct ODIPlayer {
    int playerId;
    char name[NAME_LENGTH];
    char teamName[NAME_LENGTH];
    PlayerRole role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct ODIPlayer *next;
} ODIPlayer;

typedef struct Team {
    int teamId;
    char name[NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;
    ODIPlayer *head;
} Team;



int READ_INTEGER(const char *message) {
    int number;
    char ch;

    while (1) {
        printf("%s", message);

        if (scanf("%d%c", &number, &ch) == 2 && ch == '\n') {
            return number;
        }

        while ((ch = getchar()) != '\n' && ch != EOF);

        printf("Invalid input. Enter a valid integer.\n");
    }
}

float READ_FLOAT(const char *message) {
    float number;
    char ch;

    while (1) {
        printf("%s", message);

        if (scanf("%f%c", &number, &ch) == 2 &&
            ch == '\n' &&
            isfinite(number)) {
            return number;
        }

        while ((ch = getchar()) != '\n' && ch != EOF);

        printf("Invalid input. Enter a valid number.\n");
    }
}

PlayerRole READ_ROLE() {
    int role;
    char ch;

    while (1) {
        printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");

        if (scanf("%d%c", &role, &ch) == 2 &&
            ch == '\n' &&
            role >= BATSMAN &&
            role <= ALL_ROUNDER) {
            return (PlayerRole)role;
        }

        while ((ch = getchar()) != '\n' && ch != EOF);

        printf("Invalid role. Enter only 1, 2, or 3.\n");
    }
}



float calculatePerformanceIndex(ODIPlayer *player) {
    if (player->role == BATSMAN) {
        return (player->battingAverage * player->strikeRate) / 100.0;
    }

    if (player->role == BOWLER) {
        return (player->wickets * 2) + (100 - player->economyRate);
    }

    if (player->role == ALL_ROUNDER) {
        return ((player->battingAverage * player->strikeRate) / 100.0)
               + (player->wickets * 2);
    }

    return 0;
}

PlayerRole getRole(const char *role) {
    if (strcmp(role, "Batsman") == 0)
        return BATSMAN;

    if (strcmp(role, "Bowler") == 0)
        return BOWLER;

    if (strcmp(role, "All-rounder") == 0)
        return ALL_ROUNDER;

    return 0;
}



void initializeTeams(Team teamList[]) {
    for (int i = 0; i < TEAM_COUNT; i++) {
        teamList[i].teamId = i + 1;
        strcpy(teamList[i].name, teams[i]);
        teamList[i].totalPlayers = 0;
        teamList[i].averageBattingStrikeRate = 0;
        teamList[i].head = NULL;
    }
}

int findTeamById(Team teamList[], int teamId) {
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (teamList[i].teamId == teamId)
            return i;
    }

    return -1;
}

int findTeamByName(Team teamList[], const char *name) {
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (strcmp(teamList[i].name, name) == 0)
            return i;
    }

    return -1;
}



void insertPlayer(Team *team, ODIPlayer *player) {
    player->next = team->head;
    team->head = player;
    team->totalPlayers++;
}

void initializePlayers(Team teamList[]) {
    for (int i = 0; i < playerCount; i++) {
        int teamIndex = findTeamByName(teamList, players[i].team);

        if (teamIndex == -1)
            continue;

        ODIPlayer *newPlayer = malloc(sizeof(ODIPlayer));

        if (newPlayer == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }

        newPlayer->playerId = players[i].id;
        strcpy(newPlayer->name, players[i].name);
        strcpy(newPlayer->teamName, players[i].team);
        newPlayer->role = getRole(players[i].role);
        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;

        newPlayer->performanceIndex =
            calculatePerformanceIndex(newPlayer);

        insertPlayer(&teamList[teamIndex], newPlayer);
    }
}

void calculateTeamAverage(Team *team) {
    float sum = 0;
    int count = 0;

    ODIPlayer *temp = team->head;

    while (temp != NULL) {
        if (temp->role == BATSMAN || temp->role == ALL_ROUNDER) {
            sum += temp->strikeRate;
            count++;
        }

        temp = temp->next;
    }

    if (count > 0)
        team->averageBattingStrikeRate = sum / count;
    else
        team->averageBattingStrikeRate = 0;
}

void displayPlayer(ODIPlayer *player) {
    printf("%-6d %-20s %-12s %-6d %-8.2f %-8.2f %-6d %-8.2f %-8.2f\n",
           player->playerId,
           player->name,
           player->teamName,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}



void displayTeamPlayers(Team teamList[], int teamId) {
    int index = findTeamById(teamList, teamId);

    if (index == -1) {
        printf("Team not found.\n");
        return;
    }

    ODIPlayer *temp = teamList[index].head;

    printf("\nPlayers of Team: %s\n", teamList[index].name);

    printf("ID     Name                 Team         Runs   Avg      SR       Wkts  ER       PI\n");

    while (temp != NULL) {
        displayPlayer(temp);
        temp = temp->next;
    }

    printf("\nTotal Players: %d\n",
           teamList[index].totalPlayers);

    printf("Average Batting Strike Rate: %.2f\n",
           teamList[index].averageBattingStrikeRate);
}



void addPlayer(Team teamList[], int teamId) {
    int index = findTeamById(teamList, teamId);

    if (index == -1) {
        printf("Team not found.\n");
        return;
    }

    if (teamList[index].totalPlayers >= MAX_PLAYER) {
        printf("Maximum players reached.\n");
        return;
    }

    ODIPlayer *newPlayer = malloc(sizeof(ODIPlayer));

    if (newPlayer == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newPlayer->playerId = READ_INTEGER("\nEnter Player ID: ");

    if (newPlayer->playerId <= 0) {
        printf("Invalid Player ID.\n");
        free(newPlayer);
        return;
    }

    for (int i = 0; i < TEAM_COUNT; i++) {
        ODIPlayer *temp = teamList[i].head;

        while (temp != NULL) {
            if (temp->playerId == newPlayer->playerId) {
                printf("Player ID already exists.\n");
                free(newPlayer);
                return;
            }

            temp = temp->next;
        }
    }

    printf("Enter Name: ");

    if (fgets(newPlayer->name, NAME_LENGTH, stdin) == NULL) {
        free(newPlayer);
        return;
    }

    newPlayer->name[strcspn(newPlayer->name, "\n")] = '\0';

    if (strlen(newPlayer->name) == 0) {
        printf("Invalid name.\n");
        free(newPlayer);
        return;
    }

    newPlayer->role = READ_ROLE();

    newPlayer->totalRuns = READ_INTEGER("Enter Total Runs: ");

    if (newPlayer->totalRuns < 0) {
        printf("Invalid runs.\n");
        free(newPlayer);
        return;
    }

    newPlayer->battingAverage = READ_FLOAT("Enter Batting Average: ");

    if (newPlayer->battingAverage < 0) {
        printf("Invalid average.\n");
        free(newPlayer);
        return;
    }

    newPlayer->strikeRate = READ_FLOAT("Enter Strike Rate: ");

    if (newPlayer->strikeRate < 0) {
        printf("Invalid strike rate.\n");
        free(newPlayer);
        return;
    }

    newPlayer->wickets = READ_INTEGER("Enter Wickets: ");

    if (newPlayer->wickets < 0) {
        printf("Invalid wickets.\n");
        free(newPlayer);
        return;
    }

    newPlayer->economyRate = READ_FLOAT("Enter Economy Rate: ");

    if (newPlayer->economyRate < 0) {
        printf("Invalid economy rate.\n");
        free(newPlayer);
        return;
    }

    strcpy(newPlayer->teamName, teamList[index].name);

    newPlayer->performanceIndex =
        calculatePerformanceIndex(newPlayer);

    insertPlayer(&teamList[index], newPlayer);

    calculateTeamAverage(&teamList[index]);

    printf("\nPlayer added successfully!\n");
}


void displayTopKPlayers(Team teamList[], int teamId, PlayerRole role, int k) {
    int index = findTeamById(teamList, teamId);

    if (index == -1) {
        printf("Team not found.\n");
        return;
    }

    if (role < BATSMAN || role > ALL_ROUNDER || k <= 0) {
        printf("Invalid role or K.\n");
        return;
    }

    int count = 0;

    ODIPlayer *temp = teamList[index].head;

    while (temp != NULL) {
        if (temp->role == role)
            count++;

        temp = temp->next;
    }

    if (count == 0) {
        printf("No players found for this role.\n");
        return;
    }

    ODIPlayer **playerArray = malloc(count * sizeof(ODIPlayer *));

    if (playerArray == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    temp = teamList[index].head;

    int j = 0;

    while (temp != NULL) {
        if (temp->role == role) {
            playerArray[j] = temp;
            j++;
        }

        temp = temp->next;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (playerArray[j]->performanceIndex <
                playerArray[j + 1]->performanceIndex) {

                ODIPlayer *swap = playerArray[j];
                playerArray[j] = playerArray[j + 1];
                playerArray[j + 1] = swap;
            }
        }
    }

    printf("\nTop %d Players of %s\n",
           k, teamList[index].name);

    printf("ID     Name                 Team         Runs   Avg      SR       Wkts  ER       PI\n");

    if (k > count)
        k = count;

    for (int i = 0; i < k; i++) {
        displayPlayer(playerArray[i]);
    }

    free(playerArray);
}



void displayPlayersByRole(Team teamList[], PlayerRole role) {
    if (role < BATSMAN || role > ALL_ROUNDER) {
        printf("Invalid role.\n");
        return;
    }

    int count = 0;

    for (int i = 0; i < TEAM_COUNT; i++) {
        ODIPlayer *temp = teamList[i].head;

        while (temp != NULL) {
            if (temp->role == role)
                count++;

            temp = temp->next;
        }
    }

    if (count == 0) {
        printf("No players found.\n");
        return;
    }

    ODIPlayer **playerArray = malloc(count * sizeof(ODIPlayer *));

    if (playerArray == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int index = 0;

    for (int i = 0; i < TEAM_COUNT; i++) {
        ODIPlayer *temp = teamList[i].head;

        while (temp != NULL) {
            if (temp->role == role) {
                playerArray[index] = temp;
                index++;
            }

            temp = temp->next;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (playerArray[j]->performanceIndex <
                playerArray[j + 1]->performanceIndex) {

                ODIPlayer *swap = playerArray[j];
                playerArray[j] = playerArray[j + 1];
                playerArray[j + 1] = swap;
            }
        }
    }

    printf("\nPlayers Sorted by Performance Index\n");

    printf("ID     Name                 Team         Runs   Avg      SR       Wkts  ER       PI\n");

    for (int i = 0; i < count; i++) {
        displayPlayer(playerArray[i]);
    }

    free(playerArray);
}



void displayTeamsByASR(Team teamList[]) {
    Team *teamArray[TEAM_COUNT];

    for (int i = 0; i < TEAM_COUNT; i++) {
        teamArray[i] = &teamList[i];
    }

    for (int i = 0; i < TEAM_COUNT - 1; i++) {
        for (int j = 0; j < TEAM_COUNT - i - 1; j++) {
            if (teamArray[j]->averageBattingStrikeRate <
                teamArray[j + 1]->averageBattingStrikeRate) {

                Team *swap = teamArray[j];
                teamArray[j] = teamArray[j + 1];
                teamArray[j + 1] = swap;
            }
        }
    }

    printf("\nTeams Sorted by Average Batting Strike Rate\n");

    printf("ID    Team Name              Avg SR      Players\n");

    for (int i = 0; i < TEAM_COUNT; i++) {
        printf("%-5d %-22s %-12.2f %d\n",
               teamArray[i]->teamId,
               teamArray[i]->name,
               teamArray[i]->averageBattingStrikeRate,
               teamArray[i]->totalPlayers);
    }
}


int menu() {
    printf("\n========================================\n");
    printf("ICC ODI Player Performance Analyzer\n");
    printf("========================================\n");

    printf("1. Add Player\n");
    printf("2. Display Players of a Team\n");
    printf("3. Display Teams by Average Strike Rate\n");
    printf("4. Display Top K Players of a Team by Role\n");
    printf("5. Display All Players of a Role\n");
    printf("6. Exit\n");

    return READ_INTEGER("Enter Choice: ");
}



int main() {
    Team teamList[TEAM_COUNT] = {0};

    initializeTeams(teamList);
    initializePlayers(teamList);

    for (int i = 0; i < TEAM_COUNT; i++) {
        calculateTeamAverage(&teamList[i]);
    }

    while (1) {
        int choice = menu();

        if (choice == 6) {
            printf("Exiting Program.\n");
            break;
        }

        switch (choice) {
            case 1: {
                int teamId = READ_INTEGER("Enter Team ID: ");

                addPlayer(teamList, teamId);
                break;
            }

            case 2: {
                int teamId = READ_INTEGER("Enter Team ID: ");

                displayTeamPlayers(teamList, teamId);
                break;
            }

            case 3:
                displayTeamsByASR(teamList);
                break;

            case 4: {
                int teamId = READ_INTEGER("Enter Team ID: ");
                PlayerRole role = READ_ROLE();
                int k = READ_INTEGER("Enter K: ");

                displayTopKPlayers(teamList, teamId, role, k);
                break;
            }

            case 5: {
                PlayerRole role = READ_ROLE();

                displayPlayersByRole(teamList, role);
                break;
            }

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}