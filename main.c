#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <direct.h>
#include <string.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>

int canvas_window_is_running;
struct DMStruckt {
    char st_A_Name[51];
    char st_B_Name[51];
    float st_A_center[3];
    float st_B_center[3];
    int interval;
    int stationWait;
    float st_len;
    float Coeff;
    int SlopeFilter;
    int** mode;
    int** mode2;
    int mode_len;
    int mode2_len;
};

float spd_heigh = 37.5;
float dist_len = 1;

int startx = 50;
float starty;
int heigh;
int endx;
int endy;
int st_A = 200;
int st_B;

///////////
//const.c Å´
short int Ebrake(short int speed, short int slope, short int k) {
    const short int Lenght[17][13] = {
        //60   50   40   30   20   10    0  -10  -20  -30  -40  -50  -60
        {200, 205, 210, 220, 245, 265, 295, 320, 360, 405, 450, 465, 530}, //90
        {180, 185, 190, 200, 220, 235, 265, 280, 315, 355, 395, 420, 475}, //85
        {160, 165, 170, 180, 195, 205, 220, 245, 275, 310, 345, 375, 425}, //80 - 75
        {140, 145, 150, 160, 170, 180, 190, 215, 240, 270, 305, 330, 375}, //75 - 70
        {120, 125, 130, 140, 145, 155, 165, 185, 210, 235, 265, 290, 330}, //70 - 65
        {100, 105, 110, 120, 125, 135, 145, 160, 180, 205, 230, 255, 285}, //65 - 60
        { 92,  93,  95, 100, 110, 115, 125, 135, 155, 175, 195, 220, 245}, //60 - 55
        { 75,  78,  80,  85,  95, 100, 105, 115, 130, 150, 170, 185, 205}, //55 - 50
        { 62,  63,  65,  70,  80,  85,  90,  95, 110, 125, 140, 155, 170}, //50 - 45
        { 52,  53,  55,  60,  65,  70,  75,  80,  90, 100, 115, 130, 145}, //45 - 40
        { 41,  53,  45,  48,  50,  55,  60,  65,  72,  80,  95, 100, 110}, //40 - 35
        { 35,  36,  37,  38,  40,  45,  50,  53,  58,  65,  75,  80,  85}, //35 - 30
        { 27,  28,  29,  30,  31,  35,  40,  42,  45,  50,  55,  60,  65}, //30 - 25
        { 19,  20,  21,  22,  23,  27,  30,  32,  34,  37,  40,  45,  50}, //25 - 20
        { 15,  16,  17,  17,  18,  20,  22,  23,  25,  27,  30,  32,  35}, //20 - 15
        { 10,  11,  13,  13,  14,  14,  15,  16,  17,  18,  20,  22,  23}, //15 - 10
        {  6,   6,  10,  11,  11,  12,  12,  13,  14,  15,  16,  18,  19}, //10
    };
    if (speed < 5) {
        speed = 10;
    }
    if (speed > 90) {
        speed = 90;
        printf("------------------------\nEbrake func: Speed > 90!\n------------------------\n");
    }
    short int m = 18 - ceil(speed / 5.0);
    short int n = 6 - floor(slope / 10.0);
    return Lenght[m][n] * k;
}
short int ARSbrake(short int speed, short int slope, short int k) {
    const short int Lenght[14][13] = {
        //60   50   40   30   20   10    0  -10  -20  -30  -40  -50  -60
        {294, 296, 303, 317, 334, 360, 388, 424, 461, 526, 600, 695, 794}, //90
        {271, 272, 281, 293, 309, 325, 354, 380, 416, 474, 580, 646, 725}, //85
        {248, 250, 260, 270, 284, 289, 320, 335, 370, 422, 561, 599, 659}, //80
        {222, 226, 235, 245, 255, 262, 283, 306, 340, 381, 490, 531, 691}, //75
        {196, 202, 210, 219, 225, 235, 243, 278, 308, 340, 418, 462, 524}, //70
        {176, 181, 188, 195, 200, 208, 219, 241, 266, 292, 341, 395, 447}, //65
        {157, 160, 164, 169, 174, 181, 194, 203, 224, 244, 264, 334, 371}, //60
        {138, 141, 147, 151, 155, 162, 172, 182, 199, 215, 234, 295, 326}, //55
        {121, 122, 129, 132, 136, 142, 149, 159, 173, 187, 206, 257, 282}, //50
        {102, 104, 112, 114, 118, 123, 127, 137, 148, 149, 176, 220, 237}, //45
        { 83,  85,  94,  96,  99, 103, 105, 115, 122, 130, 146, 181, 194}, //40
        { 71,  74,  81,  82,  85,  87,  97, 100, 106, 114, 127, 159, 170}, //35
        { 59,  62,  66,  68,  71,  73,  77,  84,  90,  96, 106, 135, 144}, //30
        //[], //25
        { 56,  58,  59,  60,  61,  62,  70,  74,  76,  81,  85,  99, 115}, //20
        //[], //15
        //[], //10
    };
    if (speed <= 30) {
        if (speed <= 20) {
            speed = 20;
        }
        else {
            speed = 30;
        }
    }
    if (speed > 90) {
        speed = 90;
        printf("--------------------------\nARSbrake func: Speed > 90!\n--------------------------\n");
    }
    short int m = 18 - ceil(speed / 5);
    short int n = 6 - floor(slope / 10);
    return Lenght[m][n] * k;
}
float Fy_Wo(float speed, short int mode) { //(0 - ÑBÑçÑqÑuÑs | 1 - ÑSÑëÑsÑp | 2 - ÑSÑÄÑÇÑ}ÑÄÑwÑuÑ~ÑyÑu)
    const float Force[2][19] = {
        //   0      5     10     15     20     25   30   35   40   45   50   55   60   65   70   75  80  85    90
            {0, 122.5, 122.5, 122.5, 122.5, 122.5,115.1,105.1,  91,  79,  67,  56,  48,40.1,  33,  26,20.1, 14, 10.1}, //ÑTÑÉÑ{ÑÄÑÇÑuÑ~ÑyÑu
            {-2, -2.2,  -2.4,  -2.6,  -2.8,  -3.2, -3.6,   -4,-4.4,-4.8,-5.4,-5.8,-6.6,-7.4,-8.4,-9.6, -11,-13,-14.6}, //ÑBÑçÑqÑuÑs
    };
    if (speed > 90) {
        printf("-----------------------\nFy_Wo func: Speed > 90!\n-----------------------\n");
        speed = 90;
    }
    short int n = ceil(speed / 5.0);
    if (mode == 2) return -Force[0][n];
    return Force[1 - mode][n];
}
//const.c Å™
///////////

int CreateWorkspace(char path[]) {
    FILE* info;
    char srcpathinfo[64];
    strcpy(srcpathinfo, path);
    strcat(srcpathinfo, "/src");
    _mkdir(srcpathinfo);
    strcat(srcpathinfo, "/info.txt");
    info = fopen(srcpathinfo, "w");
    fprintf(info, "Some text");
    fprintf(info, "Another Some text");
    fclose(info);
}

struct DMStruckt SectionData;
int mode_len;
int mode2_len;
int ReadSectionDMFile(char path[]) {
    FILE* SectionFile;
    SectionFile = fopen(path, "r");
    if (SectionFile == NULL) {
        printf("Can't Find Section File.\nTry again [0] / Create file with this name [1]: ");
        char ch;
    _ReadSectionDMFileScanagain:
        scanf("%1c", &ch);
        if (ch == '1') {
            SectionFile = fopen(path, "w");
            fprintf(SectionFile, "\"St. A\"; #\"st_A_Name\";\n\"St. B\"; #\"st_B_Name\";\n\n\"0 0 0\";  #\"st_A_center\";\n\"0 0 0\"; #\"st_B_center\";\n\n\"90\";    #\"interval\";\n\"30\";    #\"stationWait\";\n\"57.36\"; #\"st_len\";\n\n\"1\"; #\"Coeff\"; #usually 1 or 1.5\n\"1\"; #\"SlopeFilter\"; #0 or 1\n\nmode = [\n    [150, 0]\n];\n\nmode2 = [\n    [150,0]\n];");
        }
        else if (ch != '0') goto _ReadSectionDMFileScanagain;
        fclose(SectionFile);
        printf("\n");
        return -1;
    }
    //Å´ Reading the file
    int skip_marker = 0;
    int lines_count = 0;
    int mode1or2 = 1;
    mode_len = 1;
    mode2_len = 1;
    int SquareBracketsCount = 0;
    char ch;
    while (!feof(SectionFile)) {
        ch = fgetc(SectionFile);
        if (ch == '#') skip_marker = 1;
        if (ch == '\n') {
            skip_marker = 0;
            continue;
        }
        if (skip_marker == 1) continue;
        if (ch == ';') lines_count++;
        if (ch == '[') {
            SquareBracketsCount++;
            if (SquareBracketsCount == 2) {
                if (mode1or2 == 1) {
                    mode_len++;
                }
                else mode2_len++;
            }
        }
        if (ch == ']') {
            SquareBracketsCount--;
            if (SquareBracketsCount == 0) mode1or2 = 2;
        }
    }
    if (lines_count != 11) {
        printf("File Is Incorrect");
        fclose(SectionFile);
        return -1;
    }
    rewind(SectionFile);

    SectionData.mode = (int**)malloc(mode_len * 2 * sizeof(int));
    for (int i = 0; i < mode_len; i++) {
        SectionData.mode[i] = (int*)malloc(2 * sizeof(int));
    }
    SectionData.mode2 = (int**)malloc(mode2_len * 2 * sizeof(int));
    for (int i = 0; i < mode2_len; i++) {
        SectionData.mode2[i] = (int*)malloc(2 * sizeof(int));
    }
    SectionData.mode_len = mode_len;
    SectionData.mode2_len = mode2_len;

    strcpy(SectionData.st_A_Name, "");
    strcpy(SectionData.st_B_Name, "");

    int k = 0;
    int d = 0;
    int dap = 0;
    int digitafterdot = 0;
    int stcenter_i = 0;
    int mode_i = 0;
    int mode1_i = 0;
    int mode2_i = 0;
    int kprev = 0;
    int minus = 1;
    lines_count = 0;
    SquareBracketsCount = 0;
    int SquareBracketsCountPrev = 0;

    while (!feof(SectionFile)) {
        ch = fgetc(SectionFile);
        if (ch == '#') skip_marker = 1;
        if (ch == '\n') {
            skip_marker = 0;
            continue;
        }
        if (skip_marker == 1) continue;

        kprev = k;
        SquareBracketsCountPrev = SquareBracketsCount;
        if (ch == ';') lines_count++;
        if (ch == '"') k = (k + 1) % 2;
        if (ch == '.') digitafterdot = 1;
        if (ch == '[') SquareBracketsCount++;
        if (ch == ']') SquareBracketsCount--;
        switch (lines_count) {
        case 0:
            if (k == 1 & kprev == 1 & ch != '-') {
                //d += (int)(ch - '0');
                strcat(SectionData.st_A_Name, &ch);
            }
            break;
        case 1:
            if (k == 1 & kprev == 1 & ch != '-') {
                //d += (int)(ch - '0');
                strcat(SectionData.st_B_Name, &ch);
            }
            digitafterdot = 0;
            break;
        case 2:
            //printf("ch%c k%i digitafterdot%i\n", ch, k, digitafterdot);
            if (ch == '-') minus = -1;
            if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0 & ch != ' ') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.' & ch != ' ') {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
                //printf("%i\n",dap);
            }
            else if ((k == 0 & kprev == 1) | (ch == ' ' & k == 1)) { //ÑAÑpÑs - ÑuÑÉÑ|Ñy 2 Ñy ÑqÑÄÑ|ÑéÑäÑu ÑÅÑÇÑÄÑqÑuÑ|ÑÄÑr - Ñ~Ñu ÑÇÑpÑqÑÄÑÑÑpÑuÑÑ
                if (digitafterdot != 0) {
                    SectionData.st_A_center[stcenter_i] = (d + dap / pow(10, digitafterdot - 1)) * minus;
                }
                else SectionData.st_A_center[stcenter_i] = d;
                d = 0;
                dap = 0;
                digitafterdot = 0;
                //printf("%i - %f\n", stcenter_i, (d + dap / pow(10, digitafterdot - 1)) * minus);
                stcenter_i = (stcenter_i + 1) % 3;
                minus = 1;
            }
            break;
        case 3:
            if (ch == '-') minus = -1;
            if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0 & ch != ' ') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.' & ch != ' ') {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
            }
            else if ((k == 0 & kprev == 1) | (ch == ' ' & k == 1)) {
                if (digitafterdot != 0) {
                    SectionData.st_B_center[stcenter_i] = (d + dap / pow(10, digitafterdot - 1)) * minus;
                }
                else SectionData.st_B_center[stcenter_i] = d;
                d = 0;
                dap = 0;
                digitafterdot = 0;
                stcenter_i = (stcenter_i + 1) % 3;
                minus = 1;
            }
            break;
        case 4:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionData.interval = d;
                d = 0;
            }
            break;
        case 5:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionData.stationWait = d;
                d = 0;
                digitafterdot = 0;
            }
            break;
        case 6:
            if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0) {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.') {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                if (digitafterdot != 0) {
                    SectionData.st_len = d + dap / pow(10, digitafterdot - 1);
                }
                else SectionData.st_len = d;
                d = 0;
                dap = 0;
                digitafterdot = 0;
            }
            break;
        case 7:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionData.Coeff = d;
                d = 0;
            }
            break;
        case 8:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionData.SlopeFilter = d;
                d = 0;
            }
            break;
        case 9:
            if (ch == ',' & SquareBracketsCount == 2) digitafterdot = 1;
            //printf("ch %c SquareBracketsCount %i SquareBracketsCountPrev %i digitafterdot %i\n", ch, SquareBracketsCount, SquareBracketsCountPrev, digitafterdot);
            if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot == 0 & ch != '-' & ch != ' ' & ch != ',') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot > 0 & ch != '.' & ch != ',' & ch != ' ') {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
                //printf("%c\n", ch);
            }
            else if ((SquareBracketsCount != 2 & SquareBracketsCountPrev == 2)) {
                SectionData.mode[mode_i][0] = d;
                SectionData.mode[mode_i][1] = dap;
                //printf("%i %i %i\n", mode_i, SectionData.mode[0][0], SectionData.mode[0][0]);
                d = 0;
                dap = 0;
                digitafterdot = 0;
                mode_i++;
            }
            break;
        case 10:
            if (ch == ',' & SquareBracketsCount == 2) digitafterdot = 1;
            //printf("ch %c SquareBracketsCount %i SquareBracketsCountPrev %i digitafterdot %i\n", ch, SquareBracketsCount, SquareBracketsCountPrev, digitafterdot);
            if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot == 0 & ch != '-' & ch != ' ' & ch != ',') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot > 0 & ch != '.' & ch != ',' & ch != ' ') {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
                //printf("%c\n", ch);
            }
            else if ((SquareBracketsCount != 2 & SquareBracketsCountPrev == 2)) {
                SectionData.mode2[mode2_i][0] = d;
                SectionData.mode2[mode2_i][1] = dap;
                //printf("%i %i %i\n", mode_i, SectionData.mode[0][0], SectionData.mode[0][0]);
                d = 0;
                dap = 0;
                digitafterdot = 0;
                mode2_i++;
            }
            break;
        }
    }

    //ÑTÑqÑÇÑpÑÑÑé Ñ}ÑÖÑÉÑÄÑÇ

    printf("Section File Loaded To RAM Successfully\n\n");
    fclose(SectionFile);
    return 0;
}

////////////////////////
//profile_via_tracks.c Å´
float* ParsedData;
int* EndOfEachTrack;
int* LenOfTracks;
short int NumbersOfTracks;
short int ReadTracksFile(char path[]) {
    FILE* TracksFile;
    TracksFile = fopen(path, "r");
    if (TracksFile != NULL) {
        printf("ReadTracksFile: File Opened Successfully\n");
    }
    else {
        printf("ReadTracksFile: Can't Find Tracks File\n\n");
        return -1;
    }

    char ch;
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


    EndOfEachTrack = (int*)malloc(NumbersOfTracks * sizeof(int));

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

    LenOfTracks = (int*)malloc(NumbersOfTracks * sizeof(int));
    for (int i = 0; i < NumbersOfTracks; i++) {
        LenOfTracks[i] = EndOfEachTrack[i];
    }

    for (int i = 1; i < NumbersOfTracks; i++) {
        EndOfEachTrack[i] += EndOfEachTrack[i - 1];
    }

    ParsedData = (float*)malloc(NumbersOfTracks * EndOfEachTrack[NumbersOfTracks - 1] * 3 * sizeof(float)); //First Critical Fug
    printf("ReadTracksFile: Memory Allocated\n");


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
        if ((k == 1 & ch == '[') | ch == ' ') {
            k = 2;
        }
        if (ch == '.') k = 3;

        if (k == 2 & kprev == 2) {
            if (ch == '-') {
                minus = -1;
            }
            else {
                n *= 10;
                n += (float)(ch - '0');
            }
        }
        if (k == 3 & kprev == 3) {
            nd *= 10;
            nd += (float)(ch - '0');
            z++;
        }
        if (kprev >= 2 & k < 2 | ch == ' ') {
            if (z == 0) {
                ParsedData[i] = n * minus;
            }
            else {
                ParsedData[i] = (n + nd / pow(10, z)) * minus;
            }
            z = 0;
            i++;
            n = 0;
            nd = 0;
            minus = 1;
        }
    }
    printf("ReadTracksFile: File Loaded To RAM\n\n");

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
    }
    else {
        return ParsedData[EndOfEachTrack[n1 - 1] * 3 + n2 * 3 + n3];
    }
}

double calculateCircleRadius(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) { //ÑuÑÑÑÄ Ñ~ÑpÑÅÑyÑÉÑpÑ| ÑâÑpÑÑÑsÑÅÑÑ
    double a = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
    double b = sqrt(pow(x3 - x2, 2) + pow(y3 - y2, 2) + pow(z3 - z2, 2));
    double c = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2) + pow(z1 - z3, 2));

    double s = (a + b + c) / 2;
    double radius = (a * b * c) / (4 * sqrt(s * (s - a) * (s - b) * (s - c)));

    double crossProduct = ((x2 - x1) * (y3 - y1)) - ((x3 - x1) * (y2 - y1));

    return radius * crossProduct / fabs(crossProduct);
}

int A_eki[2];
int B_eki[2];
int NodesInSection;
double** TrackProfile;
long LenByTracks = 0;
short int TrackProfileIsExist = 0;
int Profile_Think() {
    //printf("st A center: [%f, %f, %f]\n",SectionData.st_A_center[0], SectionData.st_A_center[1], SectionData.st_A_center[2]);

    double min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            double len = sqrt(pow(GetParsedData(i, j, 0) - SectionData.st_A_center[0], 2) + pow(GetParsedData(i, j, 1) - SectionData.st_A_center[1], 2) + pow(GetParsedData(i, j, 2) - SectionData.st_A_center[2], 2));
            if (min >= len) {
                min = len;
                //printf("A_eki: i = %i, j = %i, min = %f\n",i,j,min);
                A_eki[0] = i;
                A_eki[1] = j;
            }
            //printf("GetParsedData - st A center: i = %i, j = %i: [%f, %f, %f]\n", i, j, GetParsedData(i,j,0) - SectionData.st_A_center[0], GetParsedData(i,j,1) - SectionData.st_A_center[1], GetParsedData(i,j,2) - SectionData.st_A_center[2]);
        }
    }
    min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            float len = sqrt(pow(GetParsedData(i, j, 0) - SectionData.st_B_center[0], 2) + pow(GetParsedData(i, j, 1) - SectionData.st_B_center[1], 2) + pow(GetParsedData(i, j, 2) - SectionData.st_B_center[2], 2));
            if (min >= len) {
                min = len;
                //printf("B_eki: i = %i, j = %i, min = %f\n",i,j,min);
                B_eki[0] = i;
                B_eki[1] = j;
            }
        }
    }

    printf("Profile_Think: %s linked to %i track %i node.\nProfile_Think: %s linked to %i track %i node.\n", SectionData.st_A_Name, A_eki[0], A_eki[1], SectionData.st_B_Name, B_eki[0], B_eki[1]);
    NodesInSection = abs(B_eki[1] - A_eki[1]);

    if (A_eki[0] == B_eki[0]) {
        TrackProfileIsExist = 1;
        TrackProfile = (double**)malloc((NodesInSection) * sizeof(double*));
        for (int i = 0; i < NodesInSection; i++) {
            TrackProfile[i] = (double*)malloc(3 * sizeof(double));
            TrackProfile[i][0] = sqrt(pow((GetParsedData(A_eki[0], A_eki[1] + i, 0) - GetParsedData(A_eki[0], A_eki[1] + i + 1, 0)), 2) + pow((GetParsedData(A_eki[0], A_eki[1] + i, 1) - GetParsedData(A_eki[0], A_eki[1] + i + 1, 1)), 2) + pow((GetParsedData(A_eki[0], A_eki[1] + i, 2) - GetParsedData(A_eki[0], A_eki[1] + i + 1, 2)), 2));
            TrackProfile[i][1] = (GetParsedData(A_eki[0], A_eki[1] + i + 1, 2) - GetParsedData(A_eki[0], A_eki[1] + i, 2)) / sqrt(pow(GetParsedData(A_eki[0], A_eki[1] + i, 0) - GetParsedData(A_eki[0], A_eki[1] + i + 1, 0), 2) + pow(GetParsedData(A_eki[0], A_eki[1] + i, 1) - GetParsedData(A_eki[0], A_eki[1] + i + 1, 1), 2));
            TrackProfile[i][2] = calculateCircleRadius(GetParsedData(A_eki[0], A_eki[1] + i - 1, 0), GetParsedData(A_eki[0], A_eki[1] + i - 1, 1), 0, GetParsedData(A_eki[0], A_eki[1] + i, 0), GetParsedData(A_eki[0], A_eki[1] + i, 1), 0, GetParsedData(A_eki[0], A_eki[1] + i + 1, 0), GetParsedData(A_eki[0], A_eki[1] + i + 1, 1), 0);

            TrackProfile[i][1] = round(TrackProfile[i][1] * 200) * 5;
            if (TrackProfile[i][1] == -0) TrackProfile[i][1] = 0; //ÑKÑÄÑÉÑÑÑçÑ|Ñé ÑyÑx JS, ÑÑÑÖÑÑ ÑrÑÇÑÄÑtÑu ÑÑÑÄÑwÑu Ñ~ÑÖÑwÑ~ÑÄ
            if (TrackProfile[i][2] == -0) TrackProfile[i][2] = 0;
            if (fabs(TrackProfile[i][2]) > 15000) TrackProfile[i][2] = 0;
            TrackProfile[i][2] *= 0.01905;
            LenByTracks += TrackProfile[i][0];
            TrackProfile[i][0] *= 0.01905;
        }
        LenByTracks *= 0.01905;

        //Å™ ÑBÑÉÑu ÑÇÑpÑqÑÄÑÑÑpÑuÑÑ
        //Å´ ÑEÑãÑu Ñ~Ñu ÑÅÑÇÑÄÑrÑuÑÇÑuÑ~ÑÄ

        int slopeFilter = 0;
        if (slopeFilter == 1) {
            for (int i = 0; i < NodesInSection; i++) {
                double mp = 0;
                int j = i;
                while (TrackProfile[i][1] != 0) {
                    mp += TrackProfile[j][1];
                    j += 1;
                    if (j >= NodesInSection) {
                        j -= 1;
                        break;
                    }
                    if (TrackProfile[i][1] / fabs(TrackProfile[i][1]) != TrackProfile[j][1] / fabs(TrackProfile[j][1])) {
                        j -= 1;
                        break;
                    }
                    if (TrackProfile[j][1] == 0) {
                        j -= 1;
                        break;
                    }
                }
                mp /= j - i + 1;
                while (1) {
                    TrackProfile[i][1] = round(mp);
                    if (i == j) break;
                    i++;
                }
            }
        }

        for (int i = 0; i < NodesInSection; i++) {
            long mp = 0;
            int j = i;
            while (TrackProfile[i][2] != 0) {
                //printf("DEBUG: I = %i, J = %i\n", i, j);
                mp += TrackProfile[j][2];
                j += 1;
                if (j >= NodesInSection) {
                    j -= 1;
                    break;
                }
                if (TrackProfile[j][2] == 0) {
                    j -= 1;
                    break;
                }
                if ((int)TrackProfile[i][2] / fabs(TrackProfile[i][2]) != (int)TrackProfile[j][2] / fabs(TrackProfile[j][2])) {
                    j -= 1;
                    break;
                }
            }
            mp /= j - i + 1;
            while (1) {
                TrackProfile[i][2] = round(mp);
                if (i == j) break;
                i++;
            }
        }

        free(ParsedData);
        free(EndOfEachTrack);
        free(LenOfTracks);

        st_B = st_A + LenByTracks;
        printf("Profile_Think: Received Date Is Processed\n\n");
        return 0;
    }
    else {
        printf("Profile_Think: st.A & st.B are not at the same track!\n\n");
        return -1;
    }
}

void TrackProfileFree() {
    if (A_eki[0] == B_eki[0] & TrackProfileIsExist == 1) {
        for (int i = 0; i < NodesInSection; i++) {
            free(TrackProfile[i]);
        }
        free(TrackProfile);
    }
}

float* getProfileByTracks(float x) {
    long len = 0;
    float a1 = 0;
    float a2 = 0;
    if (A_eki[1] > B_eki[1]) x = LenByTracks - x;
    for (int i = 0; i < NodesInSection; i++) {
        len += TrackProfile[i][0];
        if (len - TrackProfile[i][0] / 2 >= x & TrackProfile[i][1] != NAN) {
            a1 = TrackProfile[i][1];
            break;
        }
    };
    len = 0;
    for (int i = 0; i < NodesInSection; i++) {
        len += TrackProfile[i][0];
        if (len >= x) {
            if (isfinite(TrackProfile[i][2])) a2 = TrackProfile[i][2];
            break;
        }
    }

    static float arr[2];
    arr[0] = a1;
    arr[1] = a2;
    return arr;
}
//profile_via_tracks.c Å™
////////////////////////

//////////
//Calc.c Å´
int mode(double x) {
    int m = 1;
    for (int i = 0; i < SectionData.mode_len - 1; i++) {
        if (SectionData.mode[i][0] > x) break;
        m = SectionData.mode[i][1];
        //printf("SectionData.mode[%i][1] = %i", i, SectionData.mode[i][1]);
    }
    //printf("MMMode - %i", m);
    return m;
}
int mode2(double x) {
    int m = 1.0;
    for (int i = 0; i < SectionData.mode2_len - 1; i++) {
        if (SectionData.mode2[i][0] > x) break;
        m = SectionData.mode2[i][1];
    }
    return m;
}

float dt(int mode, float slope, float r, float Ss, float Ds) {
    float As;
    float Fy_i;

    As = Ss + Ds / 2;
    Fy_i = Fy_Wo(As, mode) - slope;
    //printf("As - %f, mode - %i, Fy_Wo - %f\n", As, mode, Fy_Wo(As, mode));
    //if (r != 0) Fy_i; //-= 750 / r;
    //return (105/3.6*Ds)/(Fy_i);
    //printf("(31 * %f) / (Fy_Wo(%f, %i) - %f) = %f (Fy_Wo(%f, %i) = %f)\n",Ds,As,mode,slope,(31*Ds)/(Fy_i),As,mode,Fy_Wo(As,mode));
    return (31 * Ds) / (Fy_i);
}

float AccBrake(float sp, float x) { //ÑDÑ|Ñë ÑÅÑÄÑyÑÉÑ{Ñp ÑÑÑÄÑâÑ{Ñy Ñ~ÑpÑâÑpÑ|Ñp ÑÑÑÄÑÇÑ}ÑÄÑwÑuÑ~ÑyÑë
    float L = 0;
    for (float i = sp; i > 0; i -= 0.1) {
        float timeDelta = dt(1, -getProfileByTracks(x + L)[0], getProfileByTracks(x + L)[1], i, 0.1);
        if (timeDelta <= 0) {
            i += 0.1 * 2;
            //timeDelta *= 1;
        }
        L += (i - 0.1 / 2) * fabs(timeDelta / 3.6);
    };
    return L;
}

double** Calculated;
int Calculated_Length;
double time;
double dist;
int stop1;
int DMcalculation(char path[]) {
    FILE* BINDM;
    int SectionDataModeLenFromBinFile;
    int SectionDataMode2LenFromBinFile;
    int SectionDataSlopeFilter;
    float SectionDataCoeff;
    int aeki0, aeki1, beki0, beki1;
    float DeltaSpeed;
    double speed;
    float timeDelta;
    float distDelta;
    int ttt;
    double dp;
    int md;
    BOOL stt;
    double dist1;
    BINDM = fopen(path, "rb");
    if (BINDM == NULL) { printf("DMcalculation: Cannot Find The Binary File: "); goto _DMcalculationWriteNewBinSection; }
    //ÑÅÑÇÑÄÑrÑuÑÇÑ{Ñp ÑÜÑpÑzÑ|Ñp Ñ~Ñp ÑÉÑÄÑÄÑÑÑrÑuÑÑÑÉÑÑÑrÑyÑu ÑyÑÉÑáÑÄÑtÑ~ÑyÑ{ÑpÑ}
    fread(&SectionDataModeLenFromBinFile, sizeof(int), 1, BINDM);
    if (SectionDataModeLenFromBinFile != SectionData.mode_len) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&SectionDataMode2LenFromBinFile, sizeof(int), 1, BINDM);
    if (SectionDataMode2LenFromBinFile != SectionData.mode2_len) { printf("DMcalculation: Mode2 Has Been Changed: ", 2); goto _DMcalculationWriteNewBinSection; }
    for (int i = 0; i < SectionData.mode_len - 1; i++) {
        int tempmodedata1, tempmodedata2;
        fread(&tempmodedata1, sizeof(int), 1, BINDM);
        //printf("%i %i\n",tempmodedata1,SectionData.mode[i][0]);
        if (tempmodedata1 != SectionData.mode[i][0]) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
        fread(&tempmodedata2, sizeof(int), 1, BINDM);
        if (tempmodedata2 != SectionData.mode[i][1]) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    }
    for (int i = 0; i < SectionData.mode2_len - 1; i++) {
        int tempmode2data1, tempmode2data2;
        fread(&tempmode2data1, sizeof(int), 1, BINDM);
        if (tempmode2data1 != SectionData.mode2[i][0]) { printf("DMcalculation: Mode2 Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
        fread(&tempmode2data2, sizeof(int), 1, BINDM);
        if (tempmode2data2 != SectionData.mode2[i][1]) { printf("DMcalculation: Mode2 Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    }
    fread(&SectionDataSlopeFilter, sizeof(int), 1, BINDM);
    if (SectionDataSlopeFilter != SectionData.SlopeFilter) { printf("DMcalculation: Slope Filter Variable Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&SectionDataCoeff, sizeof(float), 1, BINDM);
    if (SectionDataCoeff != SectionData.Coeff) { printf("DMcalculation: Coeff Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&aeki0, sizeof(int), 1, BINDM);
    if (aeki0 != A_eki[0]) { printf("DMcalculation: Station A Coords Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&aeki1, sizeof(int), 1, BINDM);
    if (aeki1 != A_eki[1]) { printf("DMcalculation: Station A Coords Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&beki0, sizeof(int), 1, BINDM);
    if (beki0 != B_eki[0]) { printf("DMcalculation: Station B Coords Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&beki1, sizeof(int), 1, BINDM);
    if (beki1 != B_eki[1]) { printf("DMcalculation: Station B Coords Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    //printf("%i %i %i %i\n", aeki0, aeki1, beki0, beki1);
    printf("DMcalculation: New file not created.\n");
    goto _DMcalculationBinToArray;

_DMcalculationWriteNewBinSection:
    if (BINDM != NULL) fclose(BINDM);
    printf("Creating new file...\n");
    BINDM = fopen(path, "wb");
    fwrite(&SectionData.mode_len, sizeof(int), 1, BINDM);
    fwrite(&SectionData.mode2_len, sizeof(int), 1, BINDM);
    for (int i = 0; i < SectionData.mode_len - 1; i++) {
        fwrite(&SectionData.mode[i][0], sizeof(int), 1, BINDM);
        fwrite(&SectionData.mode[i][1], sizeof(int), 1, BINDM);
    }
    for (int i = 0; i < SectionData.mode2_len - 1; i++) {
        fwrite(&SectionData.mode2[i][0], sizeof(int), 1, BINDM);
        fwrite(&SectionData.mode2[i][1], sizeof(int), 1, BINDM);
    }
    fwrite(&SectionData.SlopeFilter, sizeof(int), 1, BINDM);
    fwrite(&SectionData.Coeff, sizeof(float), 1, BINDM);

    fwrite(&A_eki[0], sizeof(int), 1, BINDM);
    fwrite(&A_eki[1], sizeof(int), 1, BINDM);
    fwrite(&B_eki[0], sizeof(int), 1, BINDM);
    fwrite(&B_eki[1], sizeof(int), 1, BINDM);
    fseek(BINDM, sizeof(int), SEEK_CUR);

_DMcalculationCalc:
    DeltaSpeed = 0.01;
    speed = 0;
    time = 0;
    timeDelta = 0;
    dist = 0;
    distDelta = 0;
    ttt = 0;
    dp = 0;
    md;
    stt = FALSE;
    while (dist <= LenByTracks & speed >= 0) {
        if (!stt & dist >= LenByTracks - 1000) if (dist >= LenByTracks - AccBrake(speed, dist)) { stt = TRUE; printf("DMcalculation: %f - Brake Point\n", dist); };
        if (!stt) {
            md = mode(dist);
        }
        else md = 2;
        timeDelta = dt(md, getProfileByTracks(dist)[0], getProfileByTracks(dist)[1], speed, DeltaSpeed);
        if (timeDelta <= 0) {
            DeltaSpeed *= -1;
            timeDelta = dt(md, getProfileByTracks(dist)[0], getProfileByTracks(dist)[1], speed, DeltaSpeed);
        }
        time += timeDelta;
        speed += DeltaSpeed;
        distDelta = (speed + DeltaSpeed / 2) * timeDelta / 3.6;
        dp = dist;
        dist += distDelta;
        fwrite(&dist, sizeof(double), 1, BINDM);
        fwrite(&speed, sizeof(double), 1, BINDM);
        fwrite(&time, sizeof(double), 1, BINDM);
        ttt += 1;
        if (dist <= dp) {
            break;
        }
        stop1 = ttt;
    }
    printf("DMcalculation: %f - Running time\n", time);
    dist1 = dist;
    time += SectionData.stationWait;
    while (dist <= LenByTracks + 300 & speed >= 0) {
        timeDelta = dt(mode2(dist - dist1), getProfileByTracks(dist)[0], getProfileByTracks(dist)[1], speed, DeltaSpeed);
        if (timeDelta <= 0) {
            DeltaSpeed *= -1;
        }
        timeDelta = dt(mode2(dist - dist1), getProfileByTracks(dist)[0], getProfileByTracks(dist)[1], speed, DeltaSpeed);
        time += timeDelta;
        speed += DeltaSpeed;
        distDelta = (speed + DeltaSpeed / 2) * timeDelta / 3.6;
        dp = dist;
        dist += distDelta;
        //Calculated[ttt] = [dist,speed,time];
        fwrite(&dist, sizeof(double), 1, BINDM);
        fwrite(&speed, sizeof(double), 1, BINDM);
        fwrite(&time, sizeof(double), 1, BINDM);
        //printf("[%i] dist - %f, speed - %f, time - %f\n", ttt, dist, speed, time);
        ttt += 1;
        if (dist <= dp) {
            break;
        }
    }
    fwrite(&time, sizeof(double), 1, BINDM);
    fwrite(&dist, sizeof(double), 1, BINDM);
    fwrite(&stop1, sizeof(int), 1, BINDM);
    Calculated_Length = ttt;
    fseek(BINDM, (SectionData.mode_len * 2 - 2 + SectionData.mode2_len * 2 - 2 + 7) * sizeof(int) + sizeof(float), SEEK_SET);
    fwrite(&Calculated_Length, sizeof(int), 1, BINDM);
    //printf("Calculated_Len(Calc): %i\n", Calculated_Length);
    printf("DMcalculation: New file created successfully.\n");

    fclose(BINDM);
    BINDM = fopen(path, "rb");
    fseek(BINDM, (SectionData.mode_len * 2 - 2 + SectionData.mode2_len * 2 - 2 + 7) * sizeof(int) + sizeof(float), SEEK_SET);

_DMcalculationBinToArray:
    fread(&Calculated_Length, sizeof(int), 1, BINDM);////////////////////////////////////////////
    //printf("Calculated_Len(Bin): %i\n", Calculated_Length);
    Calculated = (double**)malloc(Calculated_Length * 3 * sizeof(double));
    for (int i = 0; i < Calculated_Length; i++) {
        Calculated[i] = (double*)malloc(3 * sizeof(double));
        fread(&Calculated[i][0], sizeof(double), 1, BINDM);
        fread(&Calculated[i][1], sizeof(double), 1, BINDM);
        fread(&Calculated[i][2], sizeof(double), 1, BINDM);
        //printf("Calculated[i] = [%f, %f, %f]\n",Calculated[i][0],Calculated[i][1],Calculated[i][2]);
        //Sleep(100);
    }
    fread(&time, sizeof(double), 1, BINDM);
    fread(&dist, sizeof(double), 1, BINDM);
    fread(&stop1, sizeof(int), 1, BINDM);
    printf("DMcalculation: DM Data Loaded to RAM successfully.\n\n");
    heigh = ceil((time + SectionData.interval) / 10);
    spd_heigh = 1080.0 / (heigh + 5);
    starty = spd_heigh;
    endx = dist + st_A + SectionData.st_len * 2;
    endy = starty + spd_heigh * heigh;
    dist_len = 1920.0/(endx + 50);
    fclose(BINDM);
    return 0;
}
//Calc.c Å™
//////////

///////////////////
//Previous main() Å´
char PathToFolder[51];
void choose_line() {
    char TracksFileName[52];
    char PathToTracks[102];
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

        //strcpy(PathToTracks, "aaa/track_gm_mus_neoorange_e.txt");
        //strcpy(PathToFolder, "aaa");

        printf("Full Path To Track File: %s\n\n", PathToTracks);
    } while (ReadTracksFile(PathToTracks) == -1);

    CreateWorkspace(PathToFolder);
}
char NameOfSection[52];
void choose_section() {
    char PathToDM[54];
    char PathToSignals[59];
    char PathToDMEx[67];
    char PathToSignalsEx[72];
    strcpy(PathToDM, PathToFolder);
    strcat(PathToDM, "/dm");
    strcpy(PathToSignals, PathToFolder);
    strcat(PathToSignals, "/signals");
    _mkdir(PathToDM);
    _mkdir(PathToSignals);
    strcpy(PathToDMEx, PathToDM);
    strcpy(PathToSignalsEx, PathToDMEx);
    strcat(PathToDMEx, "/example.txt");
    strcat(PathToSignalsEx, "/example.txt");
    FILE* ex1, * ex2;
    ex1 = fopen(PathToDMEx, "w");
    ex2 = fopen(PathToSignalsEx, "w");
    fprintf(ex1, "\"St. A\"; #\"st_A_Name\";\n\"St. B\"; #\"st_B_Name\";\n\n\"-557.330933 13539.387695 1617.857422\";  #\"st_A_center\";\n\"-14387.382813 3013.013428 2264.259033\"; #\"st_B_center\";\n\n\"90\";    #\"interval\";\n\"30\";    #\"stationWait\";\n\"57.36\"; #\"st_len\";\n\n\"1\"; #\"Coeff\"; #usually 1 or 1.5\n\"1\"; #\"SlopeFilter\"; #0 or 1\n\nmode = [\n    [125, 0],\n    [200, 2],\n    [225, 0]\n];\n\nmode2 = [\n    [150,0]\n];");
    fprintf(ex2, "");
    fclose(ex1);
    fclose(ex2);
    char PathToSection[105];
    do {
        NameOfSection[0] = '/';
        NameOfSection[1] = '\0';
        PathToSection[0] = '\0';
        strcpy(PathToSection, PathToDM);
        printf("Section File Name: ");
        scanf("%50s", NameOfSection + sizeof(char));
        strcat(PathToSection, NameOfSection);
        TrackProfileFree();
    } while (ReadSectionDMFile(PathToSection) == -1 | Profile_Think() == -1);
    char PathToBin[110];
    strcpy(PathToBin, PathToFolder);
    strcat(PathToBin, "/src");
    strcat(PathToBin, NameOfSection);
    strcat(PathToBin, ".bin");
    DMcalculation(PathToBin);
}
//Previous main() Å™
///////////////////

/////////////
//drawing.c Å´
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int initialize_window(void) {
    TTF_Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return 0;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.");
        return 0;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer");
        return 0;
    }
    return 1;
}

void setup() {

}
void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        canvas_window_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            canvas_window_is_running = FALSE;
        break;
    }
}
void update() {

}
void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
    TTF_Font* font;
    font = TTF_OpenFont("bahnschrift.ttf", 15);
    SDL_Color font_color = {0, 0, 0};
    SDL_Surface* surface;
    SDL_Texture* texture;
    int texW;
    int texH;
    /*for (let i = 0; i < heigh; i++) {
        ctx.fillText((heigh * 10 - i * 10).toString(), startx - 12.5, starty + i * spd_heigh + 12 / 2);
        ctx.moveTo(startx, starty + i * spd_heigh);
        ctx.lineTo(endx, starty + i * spd_heigh);
    };*/
    for (int i = 0; i < heigh; i++) {
        int n = heigh * 10 - i * 10;
        char* n_str[5];
        _itoa(n, &n_str, 10);
        surface = TTF_RenderText_Solid(font, &n_str, font_color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        //SDL_Rect dstrect = { 12.5 * dist_len, starty + i * spd_heigh - 12 / 2, texW, texH };
        SDL_Rect dstrect = { startx * dist_len - texW - 5, starty + i * spd_heigh - 12 / 2, texW, texH };
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        SDL_RenderDrawLine(renderer, startx * dist_len, starty + i * spd_heigh, endx * dist_len, starty + i * spd_heigh);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, startx * dist_len, 0, startx * dist_len, endy);
    SDL_RenderDrawLine(renderer, 0, endy, endx * dist_len, endy);
    SDL_RenderDrawLine(renderer, endx * dist_len, 0, endx * dist_len, endy);

    SDL_RenderDrawLine(renderer, st_A * dist_len, 0, st_A * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_A - SectionData.st_len / 2) * dist_len, endy + 25, (st_A - SectionData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_A - SectionData.st_len / 2) * dist_len, endy + 15, (st_A + SectionData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_A + SectionData.st_len / 2) * dist_len, endy + 15, (st_A + SectionData.st_len / 2) * dist_len, endy + 25);

    SDL_RenderDrawLine(renderer, st_B * dist_len, 0, st_B * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B - SectionData.st_len / 2) * dist_len, endy + 25, (st_B - SectionData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B - SectionData.st_len / 2) * dist_len, endy + 15, (st_B + SectionData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B + SectionData.st_len / 2) * dist_len, endy + 15, (st_B + SectionData.st_len / 2) * dist_len, endy + 25);

    int sdvig = 0;
    for (int i = 0; i < Calculated_Length - 1; i++) {
        if (i == stop1) sdvig = -SectionData.st_len;
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionData.st_len / 2 + sdvig) * dist_len, endy - Calculated[i][1] * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionData.st_len / 2 + sdvig) * dist_len, endy - Calculated[i + 1][1] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionData.st_len / 2) * dist_len, endy - Calculated[i][2] * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionData.st_len / 2) * dist_len, endy - Calculated[i + 1][2] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] - SectionData.st_len / 2) * dist_len, endy - Calculated[i][2] * spd_heigh / 10, (st_A + Calculated[i + 1][0] - SectionData.st_len / 2) * dist_len, endy - Calculated[i + 1][2] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionData.st_len / 2) * dist_len, endy - (Calculated[i][2] + SectionData.interval) * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionData.st_len / 2) * dist_len, endy - (Calculated[i + 1][2] + SectionData.interval) * spd_heigh / 10);
    }
    SDL_RenderDrawLine(renderer, startx * dist_len, endy - SectionData.interval * spd_heigh / 10, (st_A + SectionData.st_len / 2) * dist_len, endy - SectionData.interval * spd_heigh / 10);

    SDL_RenderPresent(renderer);
}
void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
}
//drawing.c Å™
/////////////

int main() {
    choose_line();
    choose_section();
    //choose_signalisation();


    int running_program = 1;
    while (running_program) {
        canvas_window_is_running = initialize_window();
        setup();
        while (canvas_window_is_running) {
            process_input();
            update();
            render();
        }
        destroy_window();
        printf("0 - exit\n1 - reload\n2 - change signalisation\n3 - change section\n4 - change line\n");
        int choise;
        scanf("%1i", &choise);
        switch (choise) {
            case 0:
                running_program = 0;
                break;
            case 1:
                break;
            case 2:
                //choose_signalisation();
                break;
            case 3:
                choose_section();
                //choose_signalisation();
                break;
            case 4:
                choose_line();
                choose_section();
                //choose_signalisation();
                break;
        }
    }
    _getch();
    return 0;
}