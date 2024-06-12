#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <direct.h>

#include "const.c"
#include "profile_via_tracks.c"

int CreateWorkspace(char path[]) {
    FILE *info;
    char srcpathinfo[63];
    strcpy(srcpathinfo, path);
    strcat(srcpathinfo, "/src");
    mkdir(srcpathinfo);
    strcat(srcpathinfo, "/info.txt");
    info = fopen(srcpathinfo, "w");
    fprintf(info, "Some text");
    fprintf(info, "Another Some text");
    fclose(info);
}

void Interface() {
    char PathToFolder[50];
    char TracksFileName[51];
    char PathToTracks[101];
    TracksFileName[0] = '/';
    do {
        PathToFolder[0] = '\0';
        PathToTracks[1] = '\0';
        printf("Work Folder: ");
        scanf("%50s", PathToFolder);
        printf("Tracks File Name: ");
        scanf("%50s", TracksFileName + sizeof(char));

        strcpy(PathToTracks, PathToFolder);
        strcat(PathToTracks, TracksFileName);

        printf("Full Path To Track File: %s\n\n", PathToTracks);
    } while (ReadTracksFile(PathToTracks) == -1);
    CreateWorkspace(PathToFolder);
}

int slopeFilter;

int main() {
    //Interface();
    slopeFilter = 1;
    ReadTracksFile("E:/VSCode/Schitalochka C/track_gm_mus_neoorange_e.txt");

    //for (int i = 0; i < NumbersOfTracks; i++) printf("%i\n",LenOfTracks[i]);

    st_A_center[0] = -557.330933;
    st_A_center[1] = 13539.387695;
    st_A_center[2] = 1617.857422;
    st_B_center[0] = -14387.382813;
    st_B_center[1] = 3013.013428;
    st_B_center[2] = 2264.259033;
    Profile_Think();


    /*for (int i = 0; i < LenByTracks; i++) {
        float *arr = getProfileByTracks(i);
        printf("i=%i: %f, %f\n", i, arr[0], arr[1]);
        free(arr);
    }*/
    for (int i = 0; i < B_eki[1] - A_eki[1]; i++) {
        printf("i=%i: %f, %f\n", i, TrackProfile[i][1], TrackProfile[i][2]);
    }

    getch();
    return 0;
}