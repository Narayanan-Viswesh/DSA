#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define ZONES 12
#define INF 99999

typedef struct {
    int id;
    char type[20];
    int location;
    char timestamp[30];
} Emergency;

const char* zoneNames[ZONES] = {
    "Downtown", "Uptown", "Industrial Area", "Suburb A",
    "Suburb B", "Tech Park", "University Area", "Old Town",
    "New City", "Harbor", "Mall Area", "Residential Zone"
};

int city[ZONES][ZONES] = {
    {0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {2, 0, 3, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, 3, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, 1, 0, 4, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, 4, 0, 2, 3, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, 2, 0, INF, 5, INF, INF, INF, INF},
    {INF, INF, INF, INF, 3, INF, 0, 4, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, 5, 4, 0, 2, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, 2, 0, 3, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, 3, 0, 1, 2},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 1, 0, 4},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 2, 4, 0}
};

int police, fire, ambulance;

void assignRandomLocations() {
    srand(time(NULL));
    police = rand() % ZONES;
    fire = rand() % ZONES;
    ambulance = rand() % ZONES;
}

int getZoneIndex(const char* name) {
    for (int i = 0; i < ZONES; i++) {
        if (strcasecmp(zoneNames[i], name) == 0) return i;
    }
    return -1;
}

int dijkstra(int src, int dest, int path[]) {
    int dist[ZONES], visited[ZONES], prev[ZONES];
    for (int i = 0; i < ZONES; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < ZONES - 1; count++) {
        int min = INF, minIndex = -1;
        for (int v = 0; v < ZONES; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                minIndex = v;
            }
        }

        if (minIndex == -1) break;

        visited[minIndex] = 1;
        for (int v = 0; v < ZONES; v++) {
            if (!visited[v] && city[minIndex][v] != INF && dist[minIndex] + city[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + city[minIndex][v];
                prev[v] = minIndex;
            }
        }
    }

    int pathIndex = 0;
    int current = dest;
    int temp[ZONES];
    while (current != -1) {
        temp[pathIndex++] = current;
        current = prev[current];
    }
    for (int i = 0; i < pathIndex; i++) {
        path[i] = temp[pathIndex - i - 1];
    }
    path[pathIndex] = -1;

    return dist[dest];
}

void getCurrentTime(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", t);
}

void saveToDatabase(Emergency e) {
    FILE *fp = fopen("emergency_db.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "ID: %d | Type: %s | Location: %s (Zone %d) | Timestamp: %s\n",
                e.id, e.type, zoneNames[e.location], e.location, e.timestamp);
        fclose(fp);
    }
}

void deployEmergency(Emergency e) {
    int source;
    int travelTime;
    int path[ZONES + 1];

    if (strcmp(e.type, "Fire") == 0) {
        source = fire;
    } else if (strcmp(e.type, "Medical") == 0) {
        source = ambulance;
    } else if (strcmp(e.type, "Crime") == 0) {
        source = police;
    } else {
        printf("{\"status\": \"error\", \"message\": \"Invalid emergency type\"}\n");
        return;
    }

    travelTime = dijkstra(source, e.location, path);

    if (travelTime == INF) {
        printf("{\"status\": \"error\", \"message\": \"No available path to destination\"}\n");
    } else {
        if (e.id == -1) {
            // Called from Flask
            printf("{\n");
            printf("  \"status\": \"success\",\n");
            printf("  \"type\": \"%s\",\n", e.type);
            printf("  \"source_zone\": \"%s\",\n", zoneNames[source]);
            printf("  \"destination_zone\": \"%s\",\n", zoneNames[e.location]);
            printf("  \"travel_time\": %d,\n", travelTime);
            printf("  \"timestamp\": \"%s\",\n", e.timestamp);
            printf("  \"path\": [");
            for (int i = 0; path[i] != -1; i++) {
                printf("\"%s\"", zoneNames[path[i]]);
                if (path[i + 1] != -1) printf(", ");
            }
            printf("]\n}\n");
        } else {
            // CLI version
            printf("\n🚨 %s vehicle deployed from %s to %s in %d minutes!\n",
                   e.type, zoneNames[source], zoneNames[e.location], travelTime);
            printf("PATH: ");
            for (int i = 0; path[i] != -1; i++) {
                printf("%s ", zoneNames[path[i]]);
            }
            printf("\n");
        }

        // Update vehicle location after dispatch
        if (strcmp(e.type, "Fire") == 0) {
            fire = e.location;
        } else if (strcmp(e.type, "Medical") == 0) {
            ambulance = e.location;
        } else if (strcmp(e.type, "Crime") == 0) {
            police = e.location;
        }

        saveToDatabase(e);
    }
}

int main(int argc, char *argv[]) {
    assignRandomLocations();

    // You can persist request ID using a file if needed
    int requestId = 1;

    if (argc == 3) {
        Emergency e;
        e.id = -1; // Flask/API mode
        strcpy(e.type, argv[1]);

        int zoneIndex = getZoneIndex(argv[2]);
        if (zoneIndex == -1) {
            printf("{ \"status\": \"error\", \"message\": \"Invalid zone name\" }\n");
            return 1;
        }

        e.location = zoneIndex;
        getCurrentTime(e.timestamp);
        deployEmergency(e);
        return 0;
    }

    printf("\n🚨 Emergency Vehicle Deployment System 🚨\n");
    printf("\nVehicle Locations:\n");
    printf("🚔 Police: %s\n", zoneNames[police]);
    printf("🚒 Firetruck: %s\n", zoneNames[fire]);
    printf("🚑 Ambulance: %s\n", zoneNames[ambulance]);

    char cont = 'y';
    while (cont == 'y' || cont == 'Y') {
        Emergency e;
        e.id = requestId++;

        char zoneInput[50];
        printf("\nEnter Emergency Type (Fire/Medical/Crime): ");
        scanf("%s", e.type);
        printf("Enter Emergency Location (Zone Name): ");
        scanf(" %49[^\n]", zoneInput); // Safe input

        int zoneIndex = getZoneIndex(zoneInput);
        if (zoneIndex == -1) {
            printf("❌ Invalid zone name. Try again.\n");
            continue;
        }

        e.location = zoneIndex;
        getCurrentTime(e.timestamp);
        deployEmergency(e);

        printf("\nLog another request? (y/n): ");
        scanf(" %c", &cont);
    }

    printf("\n✅ Requests saved to emergency_db.txt\n");
    return 0;
}
