#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include<windows.h>

float *ParsedData;
int *EndOfEachTrack;
int *LenOfTracks;
short int NumbersOfTracks;
short int ReadTracksFile(char path[]) {
    FILE *TracksFile;
    TracksFile = fopen(path, "r");
    if (TracksFile != NULL) {
        printf("ReadTracksFile: File Opened Successfully\n");
    } else {
        printf("ReadTracksFile: Can't Find Tracks File\n");
        return -1;
    }

    char ch;
    //short int NumbersOfTracks = -2;
    NumbersOfTracks = -2;
    short int c = 0;
    while (!feof(TracksFile)) {
        ch = fgetc(TracksFile);
        if (ch == ']' | ch == '[') NumbersOfTracks++;
        if (ch == '"') {
            NumbersOfTracks--;
            c++;
        }
    }
    NumbersOfTracks /= 2;
    c /= 2;
    rewind(TracksFile);
    printf("ReadTracksFile: Tracks File Includes %i Tracks\n", NumbersOfTracks);
    /*
    int EndOfEachTrack;
    int i,j,k;
    float*** ParsedData = (float***)malloc(NumbersOfTracks*sizeof(float**));
    int qweprev = 0;
    int qwe = 0;
    for (i = 0; i < NumbersOfTracks; i++) {
        EndOfEachTrack = -2;
        while ((qwe != 1 | qweprev != 2)) {
            qweprev = qwe;
            ch = fgetc(TracksFile);
            if (ch == ']') qwe--;
            if (ch == '[') qwe++;
            if (ch == '\n') EndOfEachTrack++;
        }
        qweprev = qwe;
        ParsedData[i] = (float**) malloc(EndOfEachTrack*sizeof(float*));
        for (j = 0; j < EndOfEachTrack; j++) {
            ParsedData[i][j] = (float*)malloc(3*sizeof(float));
        }
        //printf("DEBUG: %i\n", EndOfEachTrack);
        //ParsedData[i][0][0] = EndOfEachTrack;
    }
    rewind(TracksFile);
    printf("ReadTracksFile: Memory Allocated\n");

    for (i = 0; i < NumbersOfTracks; i++) {
        ParsedData[i][0][0] = i;
    }

    for (i = 0; i < NumbersOfTracks; i++) {
        printf("DEBUG 2: %i\n",  ParsedData[i][0][0]);
    }
    */

    //int *EndOfEachTrack;
    EndOfEachTrack = (int*) malloc(NumbersOfTracks * sizeof(int));

    int qweprev = 0;
    int qwe = 0;
    for (int i = 0; i < NumbersOfTracks; i++) {
        EndOfEachTrack[i] = 0;
        while ((qwe != 1 | qweprev != 2)) {
            qweprev = qwe;
            ch = fgetc(TracksFile);
            if (ch == ']') qwe--;
            if (ch == '[') qwe++;
            if (ch == '"') EndOfEachTrack[i]++;
        }
        EndOfEachTrack[i] /= 2;
        qweprev = qwe;
    }
    rewind(TracksFile);
    
    LenOfTracks = (int*) malloc(NumbersOfTracks * sizeof(int));
    for (int i = 0; i < NumbersOfTracks; i++) {
        LenOfTracks[i] = EndOfEachTrack[i];
    }

    for (int i = 1; i < NumbersOfTracks; i++) {
        EndOfEachTrack[i] += EndOfEachTrack[i - 1];
    }

    //float *ParsedData;
    ParsedData = (float*) malloc(NumbersOfTracks * EndOfEachTrack[NumbersOfTracks] * 3 * sizeof(float));
    printf("ReadTracksFile: Memory Allocated\n");

    /*for (int i = 0; i < NumbersOfTracks; i++) {
        printf("DEBUG 2: %i\n", EndOfEachTrack[i]);
    }*/

    int k = 0;
    int i = 0;
    float n = 0;
    float nd = 0;
    int z = 0;
    int kprev = 0;
    int minus = 1;
    while (!feof(TracksFile)) {
        kprev = k;
        ch = fgetc(TracksFile);
        if (ch == ']') k = 0;
        if (ch == '\n') k = 0;
        if (ch == '"') k = 1;
        if ((k == 1 & ch =='[') | ch == ' ') {
            k = 2;
        }
        if (ch == '.') k = 3;

        if (k == 2 & kprev == 2) {
            if (ch == '-') {
                minus = -1;
            } else {
                n *= 10;
                n += (float)(ch - '0');
                //printf("k == 2 & kprev == 2\n");
                //printf("ch - %c\n", ch);
                //printf("n  - %f\n", n);
                //Sleep(1000);
            }
        }
        if (k == 3 & kprev == 3) {
            //n += (int)(ch - '0') / ((k - 2) * 10.0);
            nd *= 10;
            nd += (float)(ch - '0');
            z++;
        }
        if (kprev >= 2 & k < 2 | ch == ' ') {
            if (z == 0) {
                ParsedData[i] = n * minus;
            } else {
                ParsedData[i] = (n + nd / pow(10,z)) * minus;
            }
            z = 0;
            /*printf("kprev >= 2 & k < 2\n");
            printf("n - %f\n", n);
            printf("nd- %f\n", nd);
            printf("pd- %f\n", ParsedData[i]);*/
            i++;
            n = 0;
            nd = 0;
            minus = 1;
            //Sleep(2000);
        }
        //Sleep(1000);
    }
    printf("ReadTracksFile: File Read To Array\n");

    /*for (int j = 0; j < 27; j++) {
        printf("DEBUG 3: %f\n", ParsedData[j]);
        //Sleep(0);
    }*/

    //printf("%f", ParsedData[0]);

    fclose(TracksFile);
    return(0);
}

/*
0 <= n1 < NumberOfTracks
0 <= n2 < LenOfTrack[n1]
0 <= n3 < 3
*/
float GetParsedData(int n1, int n2, int n3) {
    if (n1 == 0) {
        return ParsedData[n2 * 3 + n3];
    }else {
        return ParsedData[EndOfEachTrack[n1 - 1] * 3 + n2 * 3 + n3];
    }
}







float calculateCircleRadius(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) { //ето написал чатгпт
    float a = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
    float b = sqrt(pow(x3 - x2, 2) + pow(y3 - y2, 2) + pow(z3 - z2, 2));
    float c = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2) + pow(z1 - z3, 2));
  
    float s = (a + b + c) / 2;
    float radius = (a * b * c) / (4 * sqrt(s * (s - a) * (s - b) * (s - c)));

    float crossProduct = ((x2 - x1) * (y3 - y1)) - ((x3 - x1) * (y2 - y1));
  
    return radius * crossProduct / abs(crossProduct);
}


float st_A_center[3];
float st_B_center[3];
int A_eki[2];
int B_eki[2];

void Profile_Think() {

    float min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            float len = sqrt(pow(GetParsedData(i,j,0)-st_A_center[0],2)+pow(GetParsedData(i,j,1)-st_A_center[1],2)+pow(GetParsedData(i,j,2)-st_A_center[2],2));
            if (min >= len) {
                min = len;
                A_eki[0] = i;
                A_eki[1] = j;
            }
        }
    }
    min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            float len = sqrt(pow(GetParsedData(i,j,0)-st_B_center[0],2)+pow(GetParsedData(i,j,1)-st_B_center[1],2)+pow(GetParsedData(i,j,2)-st_B_center[2],2));
            if (min >= len) {
                min = len;
                B_eki[0] = i;
                B_eki[1] = j;
            }
        }
    }
    printf("%i  ",A_eki[0]);
    printf("%i\n",A_eki[1]);
    printf("%i  ",B_eki[0]);
    printf("%i\n",B_eki[1]);
}