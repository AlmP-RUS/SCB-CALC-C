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
struct DMStruct {
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
struct SignalsStruct {
    int signals_len;
    int customNum_len;
    float**** signals;
    int** customNum;
    int FackMethod;
    int CalcType;
    int NumerationStart;
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

struct DMStruct SectionDMData;
struct SignalsStruct SectionSignalsData;
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

    SectionDMData.mode = (int**)malloc(mode_len * 2 * sizeof(int));
    for (int i = 0; i < mode_len; i++) {
        SectionDMData.mode[i] = (int*)malloc(2 * sizeof(int));
    }
    SectionDMData.mode2 = (int**)malloc(mode2_len * 2 * sizeof(int));
    for (int i = 0; i < mode2_len; i++) {
        SectionDMData.mode2[i] = (int*)malloc(2 * sizeof(int));
    }
    SectionDMData.mode_len = mode_len;
    SectionDMData.mode2_len = mode2_len;

    strcpy(SectionDMData.st_A_Name, "");
    strcpy(SectionDMData.st_B_Name, "");

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
                strcat(SectionDMData.st_A_Name, &ch);
            }
            break;
        case 1:
            if (k == 1 & kprev == 1 & ch != '-') {
                //d += (int)(ch - '0');
                strcat(SectionDMData.st_B_Name, &ch);
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
                    SectionDMData.st_A_center[stcenter_i] = (d + dap / pow(10, digitafterdot - 1)) * minus;
                }
                else SectionDMData.st_A_center[stcenter_i] = d;
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
                    SectionDMData.st_B_center[stcenter_i] = (d + dap / pow(10, digitafterdot - 1)) * minus;
                }
                else SectionDMData.st_B_center[stcenter_i] = d;
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
                SectionDMData.interval = d;
                d = 0;
            }
            break;
        case 5:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionDMData.stationWait = d;
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
                    SectionDMData.st_len = d + dap / pow(10, digitafterdot - 1);
                }
                else SectionDMData.st_len = d;
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
                SectionDMData.Coeff = d;
                d = 0;
            }
            break;
        case 8:
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionDMData.SlopeFilter = d;
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
                SectionDMData.mode[mode_i][0] = d;
                SectionDMData.mode[mode_i][1] = dap;
                //printf("%i %i %i\n", mode_i, SectionDMData.mode[0][0], SectionDMData.mode[0][0]);
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
                SectionDMData.mode2[mode2_i][0] = d;
                SectionDMData.mode2[mode2_i][1] = dap;
                //printf("%i %i %i\n", mode_i, SectionDMData.mode[0][0], SectionDMData.mode[0][0]);
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
int signals_len;
int customNum_len;
int ReadSectionSignalsFile(char path[]) {
    //printf("SPAth:%s\n", path);
    FILE* SignalsFile;
    SignalsFile = fopen(path, "r");
    if (SignalsFile == NULL) {
        printf("Can't Find Signals File.\nTry again [0] / Create file with this name [1]: ");
        char ch;
_ReadSectionSignalsFileFileScanagain:
        scanf("%1c", &ch);
        if (ch == '1') {
            SignalsFile = fopen(path, "w");
            fprintf(SignalsFile, "");
        }
        else if (ch != '0') goto _ReadSectionSignalsFileFileScanagain;
        printf("\n");
        return -1;
    }
    //Å´ Reading the file
    int skip_marker = 0;
    int lines_count = 0;
    int mode1or2 = 1;
    signals_len = 0;
    customNum_len = 0;
    int SquareBracketsCount = 0;
    char ch;
    while (!feof(SignalsFile)) {
        ch = fgetc(SignalsFile);
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
                    signals_len++;
                }
                else customNum_len++;
            }
        }
        if (ch == ']') {
            SquareBracketsCount--;
            if (SquareBracketsCount == 0) mode1or2 = 2;
        }
    }
    if (lines_count != 5) {
        printf("File Is Incorrect");
        fclose(SignalsFile);
        return -1;
    }
    rewind(SignalsFile);

    SectionSignalsData.signals = (float****)malloc(signals_len * 17 * sizeof(float));
    for (int i = 0; i < signals_len; i++) {
        SectionSignalsData.signals[i] = (float***)malloc(17 * sizeof(float));
        for (int j = 0; j < 10; j++) {
            if (j != 8) {
                SectionSignalsData.signals[i][j] = (float**)malloc(sizeof(float));
                *SectionSignalsData.signals[i][j] = (float*)malloc(sizeof(float));
                **SectionSignalsData.signals[i][j] = NAN;
                continue;
            }
            SectionSignalsData.signals[i][8] = (float**)malloc(8 * sizeof(float));
            for (int k = 0; k < 4; k++) {
                SectionSignalsData.signals[i][8][k] = (float*)malloc(2 * sizeof(float));
                SectionSignalsData.signals[i][8][k][0] = NAN;
                SectionSignalsData.signals[i][8][k][1] = NAN;
            }
        }
    }
    SectionSignalsData.customNum = (int**)malloc(customNum_len * 2 * sizeof(int));
    for (int i = 0; i < customNum_len; i++) {
        SectionSignalsData.customNum[i] = (int*)malloc(2 * sizeof(int));
    }
    SectionSignalsData.signals_len = signals_len;
    SectionSignalsData.customNum_len = customNum_len;

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
    int commacounter = 0;
    int linecounter = 0;
    SquareBracketsCount = 0;
    int SquareBracketsCountPrev = 0;

    while (!feof(SignalsFile)) {
        ch = fgetc(SignalsFile);
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
        case 0: //FackMethod
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionSignalsData.FackMethod = d;
                d = 0;
            }
            break;
        case 1: //CalcType
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionSignalsData.CalcType = d;
                d = 0;
                digitafterdot = 0;
            }
            break;
        case 2: //Signals
            if (ch == '.') digitafterdot = 1;
            if ((SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot == 0 & ch != '-' & ch != ' ' & ch != ',')) {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if ((SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot > 0 & ch != '-' & ch != ' ' & ch != ',' & ch != '.')) {
                digitafterdot++;
                dap *= 10;
                dap += (int)(ch - '0');
            }
            else if ((ch == ',' & SquareBracketsCount == 2) | (SquareBracketsCount == 1 & SquareBracketsCountPrev == 2)) {
                if (digitafterdot != 0) {
                    **SectionSignalsData.signals[linecounter][commacounter] = (d + dap / pow(10, digitafterdot - 1));
                }
                else **SectionSignalsData.signals[linecounter][commacounter] = d;
                //printf("signals[%i][%i] = %f\n", linecounter, commacounter, **SectionSignalsData.signals[linecounter][commacounter]);
                d = 0;
                dap = 0;
                digitafterdot = 0;
                commacounter++;
            }
            if (SquareBracketsCount == 1 & SquareBracketsCountPrev == 2) {
                commacounter = 0;
                linecounter += 1;
            }
            break;
        case 3: //NumerationStart
            if (k == 1 & kprev == 1 & ch != '-') {
                d *= 10;
                d += (int)(ch - '0');
            }
            else if (k == 0 & kprev == 1) {
                SectionSignalsData.NumerationStart = d;
                d = 0;
                digitafterdot = 0;
            }
            break;
            case 4:
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
                SectionSignalsData.customNum[mode_i][0] = d;
                SectionSignalsData.customNum[mode_i][1] = dap;
                //printf("%i %i %i\n", mode_i, SectionDMData.mode[0][0], SectionDMData.mode[0][0]);
                d = 0;
                dap = 0;
                digitafterdot = 0;
                mode_i++;
            }
            break;
        }
    }

    //ÑTÑqÑÇÑpÑÑÑé Ñ}ÑÖÑÉÑÄÑÇ

    printf("Signals File Loaded To RAM Successfully\n\n");
    fclose(SignalsFile);
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

        if (k == 2 & kprev == 2 & ch != ' ') {
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
long double LenByTracks = 0;
short int TrackProfileIsExist = 0;
int Profile_Think() {
    LenByTracks = 0;
    TrackProfileIsExist = 0;
    //printf("st A center: [%f, %f, %f]\n",SectionDMData.st_A_center[0], SectionDMData.st_A_center[1], SectionDMData.st_A_center[2]);

    double min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            double len = sqrt(pow(GetParsedData(i, j, 0) - SectionDMData.st_A_center[0], 2) + pow(GetParsedData(i, j, 1) - SectionDMData.st_A_center[1], 2) + pow(GetParsedData(i, j, 2) - SectionDMData.st_A_center[2], 2));
            if (min >= len) {
                min = len;
                //printf("A_eki: i = %i, j = %i, min = %f\n",i,j,min);
                A_eki[0] = i;
                A_eki[1] = j;
            }
            //printf("GetParsedData - st A center: i = %i, j = %i: [%f, %f, %f]\n", i, j, GetParsedData(i,j,0) - SectionDMData.st_A_center[0], GetParsedData(i,j,1) - SectionDMData.st_A_center[1], GetParsedData(i,j,2) - SectionDMData.st_A_center[2]);
        }
    }
    min = pow(10, 10);
    for (int i = 0; i < NumbersOfTracks; i++) {
        for (int j = 0; j < LenOfTracks[i]; j++) {
            float len = sqrt(pow(GetParsedData(i, j, 0) - SectionDMData.st_B_center[0], 2) + pow(GetParsedData(i, j, 1) - SectionDMData.st_B_center[1], 2) + pow(GetParsedData(i, j, 2) - SectionDMData.st_B_center[2], 2));
            if (min >= len) {
                min = len;
                //printf("B_eki: i = %i, j = %i, min = %f\n",i,j,min);
                B_eki[0] = i;
                B_eki[1] = j;
            }
        }
    }

    printf("Profile_Think: %s linked to %i track %i node.\nProfile_Think: %s linked to %i track %i node.\n", SectionDMData.st_A_Name, A_eki[0], A_eki[1], SectionDMData.st_B_Name, B_eki[0], B_eki[1]);
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
            //if (TrackProfile[i][0] > 10) printf("Popalsya: [%i] %f Node: [%f %f %f]\n", i, TrackProfile[i][0], GetParsedData(A_eki[0], A_eki[1] + i, 0), GetParsedData(A_eki[0], A_eki[1] + i, 1), GetParsedData(A_eki[0], A_eki[1] + i, 2));
        }
        LenByTracks *= 0.01905;
        //printf("LBT: %Lf\n", LenByTracks);
        //printf("NIS: %i\n", NodesInSection);

        //Å™ ÑBÑÉÑu ÑÇÑpÑqÑÄÑÑÑpÑuÑÑ
        //Å´ ÑEÑãÑu Ñ~Ñu ÑÅÑÇÑÄÑrÑuÑÇÑuÑ~ÑÄ

        int slopeFilter = 0;
        if (slopeFilter == 1) {
            for (int i = 0; i < NodesInSection; i++) {
                long double mp = 0;
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
            long double mp = 0;
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
                if (TrackProfile[i][2] / fabs(TrackProfile[i][2]) != TrackProfile[j][2] / fabs(TrackProfile[j][2])) {
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
    long double len = 0;
    float a1 = 0;
    float a2 = 0;
    if (A_eki[1] > B_eki[1]) x = LenByTracks - x;
    for (int i = 0; i < NodesInSection; i++) {
        len += TrackProfile[i][0];
        if (len - TrackProfile[i][0] / 2 >= x & isfinite(TrackProfile[i][1])) {
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

    if (A_eki[1] > B_eki[1]) {
        if (a1 != 0) a1 *= -1;
        if (a1 != 0) a2 *= -1;
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
    for (int i = 0; i < SectionDMData.mode_len - 1; i++) {
        if (SectionDMData.mode[i][0] > x) break;
        m = SectionDMData.mode[i][1];
        //printf("SectionDMData.mode[%i][1] = %i", i, SectionDMData.mode[i][1]);
    }
    //printf("MMMode - %i", m);
    return m;
}
int mode2(double x) {
    int m = 1.0;
    for (int i = 0; i < SectionDMData.mode2_len - 1; i++) {
        if (SectionDMData.mode2[i][0] > x) break;
        m = SectionDMData.mode2[i][1];
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
    int SectionDMDataModeLenFromBinFile;
    int SectionDMDataMode2LenFromBinFile;
    int SectionDMDataSlopeFilter;
    float SectionDMDataCoeff;
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
    fread(&SectionDMDataModeLenFromBinFile, sizeof(int), 1, BINDM);
    if (SectionDMDataModeLenFromBinFile != SectionDMData.mode_len) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&SectionDMDataMode2LenFromBinFile, sizeof(int), 1, BINDM);
    if (SectionDMDataMode2LenFromBinFile != SectionDMData.mode2_len) { printf("DMcalculation: Mode2 Has Been Changed: ", 2); goto _DMcalculationWriteNewBinSection; }
    for (int i = 0; i < SectionDMData.mode_len - 1; i++) {
        int tempmodedata1, tempmodedata2;
        fread(&tempmodedata1, sizeof(int), 1, BINDM);
        //printf("%i %i\n",tempmodedata1,SectionDMData.mode[i][0]);
        if (tempmodedata1 != SectionDMData.mode[i][0]) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
        fread(&tempmodedata2, sizeof(int), 1, BINDM);
        if (tempmodedata2 != SectionDMData.mode[i][1]) { printf("DMcalculation: Mode Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    }
    for (int i = 0; i < SectionDMData.mode2_len - 1; i++) {
        int tempmode2data1, tempmode2data2;
        fread(&tempmode2data1, sizeof(int), 1, BINDM);
        if (tempmode2data1 != SectionDMData.mode2[i][0]) { printf("DMcalculation: Mode2 Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
        fread(&tempmode2data2, sizeof(int), 1, BINDM);
        if (tempmode2data2 != SectionDMData.mode2[i][1]) { printf("DMcalculation: Mode2 Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    }
    fread(&SectionDMDataSlopeFilter, sizeof(int), 1, BINDM);
    if (SectionDMDataSlopeFilter != SectionDMData.SlopeFilter) { printf("DMcalculation: Slope Filter Variable Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
    fread(&SectionDMDataCoeff, sizeof(float), 1, BINDM);
    if (SectionDMDataCoeff != SectionDMData.Coeff) { printf("DMcalculation: Coeff Has Been Changed: "); goto _DMcalculationWriteNewBinSection; }
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
    fwrite(&SectionDMData.mode_len, sizeof(int), 1, BINDM);
    fwrite(&SectionDMData.mode2_len, sizeof(int), 1, BINDM);
    for (int i = 0; i < SectionDMData.mode_len - 1; i++) {
        fwrite(&SectionDMData.mode[i][0], sizeof(int), 1, BINDM);
        fwrite(&SectionDMData.mode[i][1], sizeof(int), 1, BINDM);
    }
    for (int i = 0; i < SectionDMData.mode2_len - 1; i++) {
        fwrite(&SectionDMData.mode2[i][0], sizeof(int), 1, BINDM);
        fwrite(&SectionDMData.mode2[i][1], sizeof(int), 1, BINDM);
    }
    fwrite(&SectionDMData.SlopeFilter, sizeof(int), 1, BINDM);
    fwrite(&SectionDMData.Coeff, sizeof(float), 1, BINDM);

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
    while (dist <= LenByTracks + 3 & speed >= 0) {
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
        //printf("[%i] dist - %f, speed - %f, time - %f (slope = %f, mode = %i)\n", ttt, dist, speed, time, getProfileByTracks(dist)[0], md);
        ttt += 1;
        if (dist <= dp) {
            break;
        }
        stop1 = ttt;
    }
    printf("LenOfSection: %lf\n", LenByTracks);
    printf("DMcalculation: %f - Running time\n", time);
    dist1 = dist;
    time += SectionDMData.stationWait;
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
    fseek(BINDM, (SectionDMData.mode_len * 2 - 2 + SectionDMData.mode2_len * 2 - 2 + 7) * sizeof(int) + sizeof(float), SEEK_SET);
    fwrite(&Calculated_Length, sizeof(int), 1, BINDM);
    //printf("Calculated_Len(Calc): %i\n", Calculated_Length);
    printf("DMcalculation: New file created successfully.\n");

    fclose(BINDM);
    BINDM = fopen(path, "rb");
    fseek(BINDM, (SectionDMData.mode_len * 2 - 2 + SectionDMData.mode2_len * 2 - 2 + 7) * sizeof(int) + sizeof(float), SEEK_SET);

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
    heigh = ceil((time + SectionDMData.interval) / 10);
    spd_heigh = (1080.0 - 250) / (heigh);
    starty = spd_heigh;
    endx = dist + st_A + SectionDMData.st_len * 2;
    endy = starty + spd_heigh * heigh;
    dist_len = 1920.0/(endx + 50);
    fclose(BINDM);
    return 0;
}
void thinksignalisation() {
    //DrawSignal(100, 12345, 1);
    //printf("VKSy (sprava nalevo):\n");
    int n_old;
    for (int i = 0; i < SectionSignalsData.signals_len; i++) if (**SectionSignalsData.signals[i][1] != 0) n_old = i;
    for (int i = SectionSignalsData.signals_len - 1; i >= 0; i--) {
        for (int j = 0; j < Calculated_Length; j++) {
            if (**SectionSignalsData.signals[i][0] - SectionDMData.st_len / 2 <= Calculated[j][0]) {
                **SectionSignalsData.signals[i][4] = endy - (Calculated[j][2] * spd_heigh / 10);
                **SectionSignalsData.signals[i][5] = endy - (Calculated[j][2] + SectionDMData.interval) * spd_heigh / 10;
                break;
            }
        }
        for (int j = 0; j < Calculated_Length; j++) {
            if (**SectionSignalsData.signals[i][0] + SectionDMData.st_len / 2 <= Calculated[j][0]) {
                **SectionSignalsData.signals[i][6] = endy - (Calculated[j][2] * spd_heigh / 10);
                break;
            }
        }
        //printf("%f\n", **SectionSignalsData.signals[i][6]);
        **SectionSignalsData.signals[i][7] = getProfileByTracks(**SectionSignalsData.signals[i][0])[0];
        //console.log(signals[i])
        //DrawSignal(signals[i][0],'êMçÜ',signals[i][1])

        //VKS sektion-------------------------------------------------
        /*if (**SectionSignalsData.signals[i + 1] != NAN) {
            if (**SectionSignalsData.signals[i + 1][10] != NAN) {
                float vksSpeed;
                for (int j = stop1; j < Calculated_Length; j++) {
                    vksSpeed = Calculated[j][1];
                    if (**SectionSignalsData.signals[i][10] > 0) vksSpeed = **SectionSignalsData.signals[i][10];
                    if (**SectionSignalsData.signals[i][10] < 0) vksSpeed = Calculated[j][1] + **SectionSignalsData.signals[i][10];
                    if (Calculated[j][0] + AccBrake(vksSpeed) >= **SectionSignalsData.signals[i + 1][0] + SectionDMData.st_len / 2) {
                        **SectionSignalsData.signals[i + 1][11] = Calculated[j][0] - SectionDMData.st_len / 2;
                        for (int k = 0; k < Calculated_Length; k++) {
                            if (**SectionSignalsData.signals[i + 1][11] + SectionDMData.st_len / 2 <= Calculated[k][0]) {
                                **SectionSignalsData.signals[i + 1][12] = endy - (Calculated[k][2] * spd_heigh / 10);
                                break;
                            }
                        }
                        printf("VKS % f m.\n", **SectionSignalsData.signals[i + 1][11] - **SectionSignalsData.signals[i][0]);
                        DrawSignal(**SectionSignalsData.signals[i + 1][11], -1, -1);
                        break;
                    }
                }
            }
        }*/
        if (SectionSignalsData.CalcType == 1 | SectionSignalsData.CalcType == 2) {
            if (**SectionSignalsData.signals[i][1] != 0) {
                if (1 | **SectionSignalsData.signals[i][3] == NAN) {
                    **SectionSignalsData.signals[i][3] = Ebrake(**SectionSignalsData.signals[i][2], **SectionSignalsData.signals[i][7], SectionDMData.Coeff);//Math.ceil(Ebrake(signals[i][2],signals[i][7])/12.5)*12.5
                }
                for (int j = 0; j < SectionSignalsData.signals_len; j++) {
                    if (**SectionSignalsData.signals[i][0] + **SectionSignalsData.signals[i][3] <= **SectionSignalsData.signals[j][0]) {
                        SectionSignalsData.signals[i][8][0][0] = **SectionSignalsData.signals[j][0];
                        SectionSignalsData.signals[i][8][0][1] = **SectionSignalsData.signals[j][6];
                        break;
                    }
                }
                SectionSignalsData.signals[i][8][1][0] = SectionSignalsData.signals[n_old][8][0][0];
                SectionSignalsData.signals[i][8][2][0] = SectionSignalsData.signals[n_old][8][1][0];
                SectionSignalsData.signals[i][8][3][0] = SectionSignalsData.signals[n_old][8][2][0];
                SectionSignalsData.signals[i][8][1][1] = SectionSignalsData.signals[n_old][8][0][1];
                SectionSignalsData.signals[i][8][2][1] = SectionSignalsData.signals[n_old][8][1][1];
                SectionSignalsData.signals[i][8][3][1] = SectionSignalsData.signals[n_old][8][2][1];
                if (**SectionSignalsData.signals[i][1] != 1) {  //ÑpÑrÑÑÑÄÑÉÑÑÑÄÑÅÑç
                    float avtostop = 1;
                    SectionSignalsData.signals[i][8][1][1] -= avtostop * spd_heigh / 10;
                }
                n_old = i;
            }
        }
    }
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
    char PathToDMEx[67];
    strcpy(PathToDM, PathToFolder);
    strcat(PathToDM, "/dm");
    _mkdir(PathToDM);
    strcpy(PathToDMEx, PathToDM);
    strcat(PathToDMEx, "/example.txt");
    FILE* ex1, * ex2;
    ex1 = fopen(PathToDMEx, "w");
    fprintf(ex1, "\"St. A\"; #\"st_A_Name\";\n\"St. B\"; #\"st_B_Name\";\n\n\"-557.330933 13539.387695 1617.857422\";  #\"st_A_center\";\n\"-14387.382813 3013.013428 2264.259033\"; #\"st_B_center\";\n\n\"90\";    #\"interval\";\n\"30\";    #\"stationWait\";\n\"57.36\"; #\"st_len\";\n\n\"1\"; #\"Coeff\"; #usually 1 or 1.5\n\"1\"; #\"SlopeFilter\"; #0 or 1\n\nmode = [\n    [125, 0],\n    [200, 2],\n    [225, 0]\n];\n\nmode2 = [\n    [150,0]\n];");
    fclose(ex1);
    char PathToSection[105];
    do {
        do {
            NameOfSection[0] = '/';
            NameOfSection[1] = '\0';
            PathToSection[0] = '\0';
            strcpy(PathToSection, PathToDM);
            printf("Section File Name: ");
            scanf("%50s", NameOfSection + sizeof(char));
            strcat(PathToSection, NameOfSection);
        } while (ReadSectionDMFile(PathToSection) == -1);
        TrackProfileFree();
    } while (Profile_Think() == -1);
    char PathToBin[110];
    strcpy(PathToBin, PathToFolder);
    strcat(PathToBin, "/src");
    strcat(PathToBin, NameOfSection);
    strcat(PathToBin, ".bin");
    DMcalculation(PathToBin);
}
char NameOfSignalisation[52];
void choose_signalisation() {
    char PathToSignals[59];
    char PathToSignalsEx[72];
    strcpy(PathToSignals, PathToFolder);
    strcat(PathToSignals, "/signals");
    _mkdir(PathToSignals);
    //strcat(PathToSignalsEx, "/example.txt");
    //FILE* ex2;
    /*ex2 = fopen(PathToSignalsEx, "w");
    fprintf(ex2, "");
    fclose(ex2);*/
    char PathToSectSignals[105];
    do {
        do {
            NameOfSignalisation[0] = '/';
            NameOfSignalisation[1] = '\0';
            PathToSectSignals[0] = '\0';
            strcpy(PathToSectSignals, PathToSignals);
            printf("Signals File Name: ");
            scanf("%50s", NameOfSignalisation + sizeof(char));
            strcat(PathToSectSignals, NameOfSignalisation);
        } while (ReadSectionSignalsFile(PathToSectSignals) == -1);
    } while (0);
    /*printf("%i\n", SectionSignalsData.FackMethod);
    printf("%i\n", SectionSignalsData.CalcType);
    for (int i = 0; i < signals_len; i++) {
        for (int j = 0; j < 10; j++) {
            if (j != 8) {
                printf("signals[%i][%i] = %f\n", i,j,**SectionSignalsData.signals[i][j]);
                continue;
            }
            for (int k = 0; k < 4; k++) {
                printf("%f %f\n", SectionSignalsData.signals[i][8][k][0], SectionSignalsData.signals[i][8][k][1]);
            }
        }
    }*/
    /*for (int i = 0; i < SectionSignalsData.customNum_len; i++) {
        printf("cs[%i] = {%i, %i}\n", i, SectionSignalsData.customNum[i][0], SectionSignalsData.customNum[i][1]);
    }*/
    for (int i = 0; i < SectionSignalsData.signals_len; i++) {
        if (**SectionSignalsData.signals[i][1] != 0) {
            int flag = 1;
            for (int j = 0; j < SectionSignalsData.customNum_len; j++) {
                if (SectionSignalsData.customNum[j][0] == i) {
                    **SectionSignalsData.signals[i][9] = SectionSignalsData.customNum[j][1];
                    flag = 0;
                    break;
                }
            }
            if (!flag) continue;
            **SectionSignalsData.signals[i][9] = abs(SectionSignalsData.NumerationStart);
            SectionSignalsData.NumerationStart += 2;
        }
    }
    thinksignalisation();
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
        SDL_WINDOW_ALLOW_HIGHDPI
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

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius, int32_t sector)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        if (sector == 1) {
            SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        }
        if (sector == 2) {
            SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        }
        if (sector == 3) {
            SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        }
        if (sector == 4) {
            SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        }

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

float liney;
TTF_Font* font;
SDL_Color font_color = { 0, 0, 0 };
SDL_Surface* surface;
SDL_Texture* texture;
int texW;
int texH;
void DrawSignal(float x, int name, int param) {
    float y = liney;
    x += st_A;
    y -= 3;
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer, x * dist_len, 0, x * dist_len, endy);
    x -= 3;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, x * dist_len, y, x * dist_len + 6, y);
    SDL_RenderDrawLine(renderer, x * dist_len + 3, y, x * dist_len + 3, y + 6);
    SDL_RenderDrawLine(renderer, x * dist_len, y + 6, x * dist_len + 6, y + 6);
    y += 12.5;
    char* n_str[10];
    _itoa(name, &n_str, 10);
    if (param > 0) {
        SDL_RenderDrawLine(renderer, x * dist_len, y, x * dist_len + 3, y);
        SDL_RenderDrawLine(renderer, x * dist_len + 3, y, x * dist_len + 3, y + 6);
        SDL_RenderDrawLine(renderer, x * dist_len + 3, y + 6, x * dist_len, y + 6);
        SDL_RenderDrawLine(renderer, x * dist_len + 3, y + 3, x * dist_len + 8, y + 3);
        DrawCircle(renderer, x * dist_len + 15, y + 3, 6, 1);
        DrawCircle(renderer, x * dist_len + 15, y + 3, 6, 2);
        DrawCircle(renderer, x * dist_len + 15, y + 3, 6, 3);
        DrawCircle(renderer, x * dist_len + 15, y + 3, 6, 4);
        surface = TTF_RenderText_Solid(font, n_str, font_color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = { x * dist_len, y + 15, texW, texH };
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    ///// param == -1
}
void drawsignalisation() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, liney, endx * dist_len, liney);
    //DrawSignal(100, 12345, 1);
    //printf("VKSy (sprava nalevo):\n");
    int n_old;
    for (int i = 0; i < SectionSignalsData.signals_len; i++) if (**SectionSignalsData.signals[i][1] != 0) n_old = i;
    for (int i = SectionSignalsData.signals_len - 1; i >= 0; i--) {
        for (int j = 0; j < Calculated_Length; j++) {
            if (**SectionSignalsData.signals[i][0] - SectionDMData.st_len / 2 <= Calculated[j][0]) {
                **SectionSignalsData.signals[i][4] = endy - (Calculated[j][2] * spd_heigh / 10);
                **SectionSignalsData.signals[i][5] = endy - (Calculated[j][2] + SectionDMData.interval) * spd_heigh / 10;
                break;
            }
        }
        for (int j = 0; j < Calculated_Length; j++) {
            if (**SectionSignalsData.signals[i][0] + SectionDMData.st_len / 2 <= Calculated[j][0]) {
                **SectionSignalsData.signals[i][6] = endy - (Calculated[j][2] * spd_heigh / 10);
                break;
            }
        }
        //printf("%f\n", **SectionSignalsData.signals[i][6]);
        **SectionSignalsData.signals[i][7] = getProfileByTracks(**SectionSignalsData.signals[i][0])[0];
        //console.log(signals[i])
        //DrawSignal(signals[i][0],'êMçÜ',signals[i][1])
        DrawSignal(**SectionSignalsData.signals[i][0], **SectionSignalsData.signals[i][9], **SectionSignalsData.signals[i][1]);

        //VKS sektion-------------------------------------------------
        /*if (**SectionSignalsData.signals[i + 1] != NAN) {
            if (**SectionSignalsData.signals[i + 1][10] != NAN) {
                float vksSpeed;
                for (int j = stop1; j < Calculated_Length; j++) {
                    vksSpeed = Calculated[j][1];
                    if (**SectionSignalsData.signals[i][10] > 0) vksSpeed = **SectionSignalsData.signals[i][10];
                    if (**SectionSignalsData.signals[i][10] < 0) vksSpeed = Calculated[j][1] + **SectionSignalsData.signals[i][10];
                    if (Calculated[j][0] + AccBrake(vksSpeed) >= **SectionSignalsData.signals[i + 1][0] + SectionDMData.st_len / 2) {
                        **SectionSignalsData.signals[i + 1][11] = Calculated[j][0] - SectionDMData.st_len / 2;
                        for (int k = 0; k < Calculated_Length; k++) {
                            if (**SectionSignalsData.signals[i + 1][11] + SectionDMData.st_len / 2 <= Calculated[k][0]) {
                                **SectionSignalsData.signals[i + 1][12] = endy - (Calculated[k][2] * spd_heigh / 10);
                                break;
                            }
                        }
                        printf("VKS % f m.\n", **SectionSignalsData.signals[i + 1][11] - **SectionSignalsData.signals[i][0]);
                        DrawSignal(**SectionSignalsData.signals[i + 1][11], -1, -1);
                        break;
                    }
                }
            }
        }*/
        if (SectionSignalsData.CalcType == 1 | SectionSignalsData.CalcType == 2) {
            // [ÑHÑpÑãÑyÑÑ.ÑTÑâ/ÑKÑÇÑpÑÉÑ~/ÑGÑuÑ|ÑÑ/ÑGÑH]
            if (**SectionSignalsData.signals[i][1] != 0) {
                //ctx.fillText(signIsVisibleDist(Profile(signals[i][0])[1],6),signals[i][0]+st_A,endy+20) //ÑBÑyÑtÑyÑ}ÑÄÑÉÑÑÑé
                if (1 | **SectionSignalsData.signals[i][3] == NAN) {
                    **SectionSignalsData.signals[i][3] = Ebrake(**SectionSignalsData.signals[i][2], **SectionSignalsData.signals[i][7], SectionDMData.Coeff);//Math.ceil(Ebrake(signals[i][2],signals[i][7])/12.5)*12.5
                }
                for (int j = 0; j < SectionSignalsData.signals_len; j++) {
                    if (**SectionSignalsData.signals[i][0] + **SectionSignalsData.signals[i][3] <= **SectionSignalsData.signals[j][0]) {
                        SectionSignalsData.signals[i][8][0][0] = **SectionSignalsData.signals[j][0];
                        SectionSignalsData.signals[i][8][0][1] = **SectionSignalsData.signals[j][6];
                        SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] - 2 - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] - 2 - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] + 2 - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] + 2 - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] - 1 - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] - 1 - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] + 1 - SectionDMData.interval * spd_heigh / 10);
                        SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len - 5, SectionSignalsData.signals[i][8][0][1] + 1 - SectionDMData.interval * spd_heigh / 10, (SectionSignalsData.signals[i][8][0][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10);
                        
                        char n_str[100];
                        char* n[10];
                        _itoa(**SectionSignalsData.signals[i][2], &n, 10);
                        strcpy(n_str, n);
                        strcat(n_str, "km/h");
                        _itoa(**SectionSignalsData.signals[i][3], &n, 10);
                        strcat(n_str, n);
                        strcat(n_str, "min");
                        _itoa(SectionSignalsData.signals[i][8][0][0] - **SectionSignalsData.signals[i][0], &n, 10);
                        strcat(n_str, n);
                        strcat(n_str, "fakt");

                        surface = TTF_RenderText_Solid(font, n_str, font_color);
                        texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                        SDL_Rect dstrect = { (**SectionSignalsData.signals[i][0] + st_A) * dist_len - texW, SectionSignalsData.signals[i][8][0][1] - SectionDMData.interval * spd_heigh / 10 + 5 - texH, texW, texH };
                        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                        SDL_DestroyTexture(texture);
                        SDL_FreeSurface(surface);

                        /*if (signals[j][12] != undefined) {
                            signals[i][8][0] = [signals[j][11], signals[j][12]];
                        }
                        //console.log('hahaha');*/
                        break;
                    }
                }
                SectionSignalsData.signals[i][8][1][0] = SectionSignalsData.signals[n_old][8][0][0];
                SectionSignalsData.signals[i][8][2][0] = SectionSignalsData.signals[n_old][8][1][0];
                SectionSignalsData.signals[i][8][3][0] = SectionSignalsData.signals[n_old][8][2][0];

                SectionSignalsData.signals[i][8][1][1] = SectionSignalsData.signals[n_old][8][0][1];
                SectionSignalsData.signals[i][8][2][1] = SectionSignalsData.signals[n_old][8][1][1];
                SectionSignalsData.signals[i][8][3][1] = SectionSignalsData.signals[n_old][8][2][1];

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                if (**SectionSignalsData.signals[i][1] != 1) {  //ÑpÑrÑÑÑÄÑÉÑÑÑÄÑÅÑç
                    SDL_RenderDrawLine(renderer, (SectionSignalsData.signals[i][8][1][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][1][1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 12.5, SectionSignalsData.signals[i][8][1][1]);
                    float avtostop = 1;
                    SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 12.5, SectionSignalsData.signals[i][8][1][1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 12.5, SectionSignalsData.signals[i][8][1][1] - avtostop * spd_heigh / 10); //ÑrÑÇÑuÑ}Ñë ÑÄÑÑÑ{ÑÇÑçÑÑÑyÑë ÑpÑrÑÑÑÄÑÉÑÑÑÄÑÅÑp
                    SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 20, SectionSignalsData.signals[i][8][1][1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 20, SectionSignalsData.signals[i][8][1][1] - avtostop * spd_heigh / 10);
                    SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 20, SectionSignalsData.signals[i][8][1][1] - avtostop * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][1][1] - avtostop * spd_heigh / 10);
                    SectionSignalsData.signals[i][8][1][1] -= avtostop * spd_heigh / 10;
                }

                if (**SectionSignalsData.signals[i][1] != 1 & i > 0) { //ÑUÑpÑ{ÑÖÑ|ÑéÑÑÑpÑÑÑyÑrÑç
                    float fackTime;
                    TTF_Font* font2;
                    font2 = TTF_OpenFont("bahnschrift.ttf", 13);
                    for (int j = 0; j < Calculated_Length; j++) {
                        float spd = Calculated[j][1];
                        float pst;
                        if (SectionSignalsData.FackMethod == 0) {
                            pst = Ebrake(spd, getProfileByTracks(Calculated[j][0])[0], SectionDMData.Coeff) * 1.15;
                        }
                        if (SectionSignalsData.FackMethod == 1) {
                            if (Calculated[j][0] > **SectionSignalsData.signals[i][0] - 500) pst = AccBrake(spd, Calculated[j][0]);//*1.5
                        }
                        if (**SectionSignalsData.signals[i][0] - SectionDMData.st_len / 2 - pst <= Calculated[j][0]) {
                            
                            fackTime = endy - (Calculated[j][2] + SectionDMData.interval) * spd_heigh / 10;
                            SDL_SetRenderDrawColor(renderer, 66, 135, 245, 255);
                            SDL_RenderDrawLine(renderer, (Calculated[j][0] + SectionDMData.st_len / 2 + st_A) * dist_len, fackTime, (Calculated[j][0] + SectionDMData.st_len / 2 + st_A) * dist_len, fackTime + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (Calculated[j][0] + SectionDMData.st_len / 2 + st_A) * dist_len, fackTime + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 3, fackTime + 10 - 10 + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 3, fackTime + 10 - 10 + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 8 - 10 + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 8 - 10 + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 12 - 10 + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 12 - 10 + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 3, fackTime + 10 - 10 + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 3, fackTime + 10 - 10 + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 9 - 10 + 2 * spd_heigh / 10);
                            SDL_RenderDrawLine(renderer, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 5 - 3, fackTime + 11 - 10 + 2 * spd_heigh / 10, (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 3, fackTime + 10 - 10 + 2 * spd_heigh / 10);

                            
                            SDL_Color blue_font_color = { 66, 135, 245 };

                            char n_str[100];
                            char* n[10];
                            _itoa(ceil(pst), &n, 10);
                            strcpy(n_str, n);
                            strcat(n_str, "m");
                            surface = TTF_RenderText_Solid(font2, n_str, blue_font_color);
                            texture = SDL_CreateTextureFromSurface(renderer, surface);
                            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                            SDL_Rect dstrect = { (**SectionSignalsData.signals[i][0] + st_A) * dist_len - texW - 10, fackTime + 7 - 6 + 2 * spd_heigh / 10 - texH, texW, texH };
                            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                            SDL_DestroyTexture(texture);
                            SDL_FreeSurface(surface);

                            _itoa(ceil(spd), &n, 10);
                            strcpy(n_str, n);
                            strcat(n_str, "km/h");
                            surface = TTF_RenderText_Solid(font2, n_str, blue_font_color);
                            texture = SDL_CreateTextureFromSurface(renderer, surface);
                            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                            SDL_Rect dstrect1 = { (**SectionSignalsData.signals[i][0] + st_A) * dist_len - texW - 10, fackTime + 21 - 3 - 4 + 2 * spd_heigh / 10 - texH, texW, texH };
                            SDL_RenderCopy(renderer, texture, NULL, &dstrect1);
                            SDL_DestroyTexture(texture);
                            SDL_FreeSurface(surface);
                            break;
                        }
                    }
                    char n_str[100];
                    char* n[10];
                    _itoa(floor((SectionSignalsData.signals[i][8][1][1] - fackTime - 2 * spd_heigh / 10) / spd_heigh * 10), &n, 10);
                    strcpy(n_str, n);
                    strcat(n_str, "s.");
                    surface = TTF_RenderText_Solid(font2, n_str, font_color);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                    SDL_Rect dstrect = { (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 5, **SectionSignalsData.signals[i][5], texW, texH };
                    //printf("%f %f\n", (**SectionSignalsData.signals[i][0] + st_A)* dist_len + 7, **SectionSignalsData.signals[i][5] + 15);
                    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }

                /////////////////////////////
                float m[2] = { **SectionSignalsData.signals[i][0] + st_A, **SectionSignalsData.signals[i][4] };
                float z = **SectionSignalsData.signals[i][1];

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
                SDL_RenderDrawLine(renderer, m[0] * dist_len, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][1][1]);
                SDL_RenderDrawLine(renderer, m[0] * dist_len - 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 1, SectionSignalsData.signals[i][8][1][1]);
                SDL_RenderDrawLine(renderer, m[0] * dist_len + 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 1, SectionSignalsData.signals[i][8][1][1]);
                m[0] = **SectionSignalsData.signals[i][0] + st_A;
                m[1] = SectionSignalsData.signals[i][8][1][1];
                if (z == 3 | z == 4) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][2][1]);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len - 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 1, SectionSignalsData.signals[i][8][2][1]);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len + 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 1, SectionSignalsData.signals[i][8][2][1]);
                    m[0] = **SectionSignalsData.signals[i][0] + st_A;
                    m[1] = SectionSignalsData.signals[i][8][2][1];
                }
                if (z == 4) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len, SectionSignalsData.signals[i][8][3][1]);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len - 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 1, SectionSignalsData.signals[i][8][3][1]);
                    SDL_RenderDrawLine(renderer, m[0] * dist_len + 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 1, SectionSignalsData.signals[i][8][3][1]);
                    m[0] = **SectionSignalsData.signals[i][0] + st_A;
                    m[1] = SectionSignalsData.signals[i][8][3][1];
                }
                if (z == 2 | z == 3 | z == 4) {
                    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 0);
                }
                SDL_RenderDrawLine(renderer, m[0] * dist_len, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len, **SectionSignalsData.signals[i][5]);
                SDL_RenderDrawLine(renderer, m[0] * dist_len - 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len - 1, **SectionSignalsData.signals[i][5]);
                SDL_RenderDrawLine(renderer, m[0] * dist_len + 1, m[1], (**SectionSignalsData.signals[i][0] + st_A) * dist_len + 1, **SectionSignalsData.signals[i][5]);
                m[0] = **SectionSignalsData.signals[i][0] + st_A;
                m[1] = **SectionSignalsData.signals[i][5];
                /////////////////////////////

                n_old = i;
            }
        }
    }
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
    font = TTF_OpenFont("bahnschrift.ttf", 15);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
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
    SDL_RenderDrawLine(renderer, (st_A - SectionDMData.st_len / 2) * dist_len, endy + 25, (st_A - SectionDMData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_A - SectionDMData.st_len / 2) * dist_len, endy + 15, (st_A + SectionDMData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_A + SectionDMData.st_len / 2) * dist_len, endy + 15, (st_A + SectionDMData.st_len / 2) * dist_len, endy + 25);

    SDL_RenderDrawLine(renderer, st_B * dist_len, 0, st_B * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B - SectionDMData.st_len / 2) * dist_len, endy + 25, (st_B - SectionDMData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B - SectionDMData.st_len / 2) * dist_len, endy + 15, (st_B + SectionDMData.st_len / 2) * dist_len, endy + 15);
    SDL_RenderDrawLine(renderer, (st_B + SectionDMData.st_len / 2) * dist_len, endy + 15, (st_B + SectionDMData.st_len / 2) * dist_len, endy + 25);

    int sdvig = 0;
    for (int i = 0; i < Calculated_Length - 1; i++) {
        if (i == stop1) sdvig = -SectionDMData.st_len;
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionDMData.st_len / 2 + sdvig) * dist_len, endy - Calculated[i][1] * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionDMData.st_len / 2 + sdvig) * dist_len, endy - Calculated[i + 1][1] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionDMData.st_len / 2) * dist_len, endy - Calculated[i][2] * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionDMData.st_len / 2) * dist_len, endy - Calculated[i + 1][2] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] - SectionDMData.st_len / 2) * dist_len, endy - Calculated[i][2] * spd_heigh / 10, (st_A + Calculated[i + 1][0] - SectionDMData.st_len / 2) * dist_len, endy - Calculated[i + 1][2] * spd_heigh / 10);
        SDL_RenderDrawLine(renderer, (st_A + Calculated[i][0] + SectionDMData.st_len / 2) * dist_len, endy - (Calculated[i][2] + SectionDMData.interval) * spd_heigh / 10, (st_A + Calculated[i + 1][0] + SectionDMData.st_len / 2) * dist_len, endy - (Calculated[i + 1][2] + SectionDMData.interval) * spd_heigh / 10);
    }
    SDL_RenderDrawLine(renderer, startx * dist_len, endy - SectionDMData.interval * spd_heigh / 10, (st_A + SectionDMData.st_len / 2) * dist_len, endy - SectionDMData.interval * spd_heigh / 10);

    const int profile_y[2] = {endy + 125, endy + 162.5};
    SDL_RenderDrawLine(renderer, st_A * dist_len, profile_y[0], st_B * dist_len, profile_y[0]);
    SDL_RenderDrawLine(renderer, st_A * dist_len, profile_y[1], st_B * dist_len, profile_y[1]);
    float p_old = NAN;
    float i_old = 0;
    for (int i = 0; i <= LenByTracks; i++) {
        float p = getProfileByTracks(i)[0];
        if (p != p_old | i == floor(LenByTracks) | i == 0) {
            SDL_RenderDrawLine(renderer, (i + st_A) * dist_len, profile_y[0], (i + st_A) * dist_len, profile_y[1]);
            if (p_old == 0) {
                char* n_str[5];
                _itoa(p_old, &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (i / 2 + i_old / 2 + st_A) * dist_len - texW/2, profile_y[0] + 3, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                _itoa((i - i_old), &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect1 = { (i / 2 + i_old / 2 + st_A) * dist_len - texW / 2, profile_y[1] - 15, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect1);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                SDL_RenderDrawLine(renderer, (i_old + st_A) * dist_len, profile_y[0] / 2 + profile_y[1] / 2, (i + st_A) * dist_len, profile_y[0] / 2 + profile_y[1] / 2);
            }
            if (p_old > 0) {
                char* n_str[5];
                _itoa(p_old, &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (i_old + st_A + 1) * dist_len, profile_y[0] + 1, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                _itoa((i - i_old), &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect1 = { (i + st_A) * dist_len - texW, profile_y[1] - 13, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect1);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                SDL_RenderDrawLine(renderer, (i_old + st_A) * dist_len, profile_y[1], (i + st_A) * dist_len, profile_y[0]);
            }
            if (p_old < 0) {
                char* n_str[5];
                _itoa(-p_old, &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (i + st_A) * dist_len - texW, profile_y[0] + 1, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                _itoa((i - i_old), &n_str, 10);
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect1 = { (i_old + st_A + 1) * dist_len, profile_y[1] - 13, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect1);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                SDL_RenderDrawLine(renderer, (i_old + st_A) * dist_len, profile_y[0], (i + st_A) * dist_len, profile_y[1]);
            }
            i_old = i;
        }
        p_old = p;
    }

    const float plan_y = endy + 200;
    float P_old = NAN;
    int I_old = 0;
    for (int i = 0; i <= LenByTracks; i++) {
        float p = getProfileByTracks(i)[1];
        if (p != P_old | i == floor(LenByTracks)) {
            if (P_old == 0) {
                char* n_str[5];
                _itoa((i - I_old), &n_str, 10);
                //strcat(n_str, "m");
                surface = TTF_RenderText_Solid(font, &n_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (I_old / 2 + i / 2 + st_A) * dist_len - texW / 2, plan_y - texH - 1, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                SDL_RenderDrawLine(renderer, (I_old + st_A) * dist_len, plan_y, (i + st_A) * dist_len, plan_y);
            }
            if (P_old > 0) {
                char* n_str[5];
                char* m_str[5];
                char* nm_str[10];
                _itoa((i - I_old), &m_str, 10);
                _itoa((P_old), &n_str, 10);
                strcpy(nm_str, n_str);
                strcat(nm_str, "/");
                strcat(nm_str, m_str);
                surface = TTF_RenderText_Solid(font, &nm_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (I_old / 2 + i / 2 + st_A) * dist_len - texW / 2, plan_y - texH - 1 + 15, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                DrawCircle(renderer, (I_old + st_A) * dist_len + 15, plan_y, 15, 3);
                DrawCircle(renderer, (i + st_A) * dist_len - 15, plan_y, 15, 4);
                SDL_RenderDrawLine(renderer, (I_old + st_A) * dist_len + 15, plan_y + 15, (i + st_A) * dist_len - 15, plan_y + 15);
            }
            if (P_old < 0) {
                char* n_str[5];
                char* m_str[5];
                char* nm_str[10];
                _itoa((i - I_old), &m_str, 10);
                _itoa((-P_old), &n_str, 10);
                strcpy(nm_str, n_str);
                strcat(nm_str, "/");
                strcat(nm_str, m_str);
                surface = TTF_RenderText_Solid(font, &nm_str, font_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
                SDL_Rect dstrect = { (I_old / 2 + i / 2 + st_A) * dist_len - texW / 2, plan_y +3 - 15, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                DrawCircle(renderer, (I_old + st_A) * dist_len + 15, plan_y, 15, 2);
                DrawCircle(renderer, (i + st_A) * dist_len - 15, plan_y, 15, 1);
                SDL_RenderDrawLine(renderer, (I_old + st_A)* dist_len + 15, plan_y - 15, (i + st_A)* dist_len - 15, plan_y - 15);
            }
            I_old = i;
        }
        P_old = p;
    }
    liney = endy + 62.5;
    drawsignalisation();
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
    choose_signalisation();

    /*for (int i = 0; i < LenByTracks; i++) {
        printf("[%i] = [%f, %f]\n", i, getProfileByTracks(i)[0], getProfileByTracks(i)[1]);
    }*/

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
        printf("\n0 - exit\n1 - reload\n2 - change signalisation\n3 - change section\n4 - change line\n");
        int choise;
        scanf("%1i", &choise);
        switch (choise) {
            case 0:
                running_program = 0;
                break;
            case 1:
                break;
            case 2:
                choose_signalisation();
                break;
            case 3:
                choose_section();
                choose_signalisation();
                break;
            case 4:
                free(ParsedData);
                free(EndOfEachTrack);
                free(LenOfTracks);
                choose_line();
                choose_section();
                choose_signalisation();
                break;
        }
    }
    _getch();
    return 0;
}