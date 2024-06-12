#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "const.c"
#include "profile_via_tracks.c"


int slopeFilter;

int main() {
    slopeFilter = 1;
    ReadTracksFile("E:/VSCode/Schitalochka C/track_gm_mus_neoorange_e.txt");
    //float a,b,c,d,e,f;

    for (int i = 0; i < NumbersOfTracks; i++) printf("%i\n",LenOfTracks[i]);

    //while (1) {
        /*scanf("%f",&st_A_center[0]);
        scanf("%f",&st_A_center[1]);
        scanf("%f",&st_A_center[2]);
        scanf("%f",&st_B_center[0]);
        scanf("%f",&st_B_center[1]);
        scanf("%f",&st_B_center[2]);*/
        st_A_center[0] = -557.330933;
        st_A_center[1] = 13539.387695;
        st_A_center[2] = 1617.857422;
        st_B_center[0] = -14387.382813;
        st_B_center[1] = 3013.013428;
        st_B_center[2] = 2264.259033;
        Profile_Think();
        
        printf("%f ", TrackProfile[0][0]);
        printf("%f ", TrackProfile[0][1]);
        printf("%f\n", TrackProfile[0][2]);

        printf("%f ", TrackProfile[3][0]);
        printf("%f ", TrackProfile[3][1]);
        printf("%f\n", TrackProfile[3][2]);

        printf("%f ", TrackProfile[55][0]);
        printf("%f ", TrackProfile[55][1]);
        printf("%f\n", TrackProfile[55][2]);
        
        //float *arr1 = getProfileByTracks(0);
        //printf("%f ", arr1[1]);
        //printf("%f\n", arr1[1]);
        /*printf("%f ", getProfileByTracks(1037)[0]);
        printf("%f\n", getProfileByTracks(1037)[1]);
        printf("%f ", getProfileByTracks(1234)[0]);
        printf("%f\n", getProfileByTracks(1234)[1]);*/
        /*printf("%f ", getProfileByTracks(0));
        printf("%f ", getProfileByTracks(1037));
        printf("%f\n", getProfileByTracks(1234));*/
    //}
    getch();
    return 0;
}