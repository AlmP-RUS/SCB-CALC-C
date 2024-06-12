#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "const.c"
#include "profile_via_tracks.c"


int main() {
    ReadTracksFile("E:/VSCode/Schitalochka C/track_gm_mus_neoorange_e.txt");
    //float a,b,c,d,e,f;

    for (int i = 0; i < NumbersOfTracks; i++) printf("%i\n",LenOfTracks[i]);

    while (1) {
        scanf("%f",&st_A_center[0]);
        scanf("%f",&st_A_center[1]);
        scanf("%f",&st_A_center[2]);
        scanf("%f",&st_B_center[0]);
        scanf("%f",&st_B_center[1]);
        scanf("%f",&st_B_center[2]);
        Profile_Think();
    }
    getch();
    return 0;
}