#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <direct.h>

#include "const.c"
#include "profile_via_tracks.c"

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
    int **mode;
    int **mode2;
};

int CreateWorkspace(char path[]) {
    FILE *info;
    char srcpathinfo[64];
    strcpy(srcpathinfo, path);
    strcat(srcpathinfo, "/src");
    mkdir(srcpathinfo);
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
    FILE *SectorFile;
    SectorFile = fopen(path, "r");
    if (SectorFile == NULL) {
        printf("Can't Find Tracks File.\nTry again [0] / Create file with this name [1]: ");
        int ch;
    _ReadSectionDMFileScanagain:
        scanf("%1i",&ch);
        if (ch == 1) {
            SectorFile = fopen(path, "w");
            fprintf(SectorFile,"\"St. A\"; #\"st_A_Name\";\n\"St. B\"; #\"st_B_Name\";\n\n\"0 0 0\";  #\"st_A_center\";\n\"0 0 0\"; #\"st_B_center\";\n\n\"90\";    #\"interval\";\n\"30\";    #\"stationWait\";\n\"57.36\"; #\"st_len\";\n\n\"1\"; #\"Coeff\"; #usually 1 or 1.5\n\"1\"; #\"SlopeFilter\"; #0 or 1\n\nmode = [\n    [150, 0]\n];\n\nmode2 = [\n    [150,0]\n];");
        } else if (ch != 0) goto _ReadSectionDMFileScanagain;
        fclose(SectorFile);
        printf("\n");
        return -1;
    }
    //↓ Reading the file
    int skip_marker = 0;
    int lines_count = 0;
    int mode1or2 = 1;
    mode_len = 1;
    mode2_len = 1;
    int SquareBracketsCount = 0;
    char ch;
    while (!feof(SectorFile)) {
        ch = fgetc(SectorFile);
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
                } else mode2_len++;
            }
        }
        if (ch == ']') {
            SquareBracketsCount--;
            if (SquareBracketsCount == 2) mode1or2 = 2;
        }
    }
    if (lines_count != 11) {
        printf("File Is Incorrect");
        fclose(SectorFile);
        return -1;
    }
    rewind(SectorFile);

    SectionData.mode = (int**) malloc(mode_len * 2 * sizeof(int));
    for (int i = 0; i < mode_len; i++) {
        SectionData.mode[i] = (int*) malloc(2 * sizeof(int));
    }
    SectionData.mode2 = (int**) malloc(mode2_len * 2 * sizeof(int));
    for (int i = 0; i < mode2_len; i++) {
        SectionData.mode2[i] = (int*) malloc(2 * sizeof(int));
    }

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

    while (!feof(SectorFile)) {
        ch = fgetc(SectorFile);
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
                if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0 & ch != ' ') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.' & ch != ' ') {
                    digitafterdot++;
                    dap *= 10;
                    dap += (int)(ch - '0');
                    //printf("%i\n",dap);
                } else if ((k == 0 & kprev == 1) | (ch == ' ' & k == 1)) {
                    if (digitafterdot != 0) {
                        SectionData.st_A_center[stcenter_i] = d + dap / pow(10, digitafterdot - 1);
                        //printf("%f\n", d + dap / pow(10, digitafterdot - 1));
                    } else SectionData.st_A_center[stcenter_i] = d;
                    d = 0;
                    dap = 0;
                    digitafterdot = 0;
                    stcenter_i = (stcenter_i + 1) % 3;
                }
                break;
            case 3:
                if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0 & ch != ' ') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.' & ch != ' ') {
                    digitafterdot++;
                    dap *= 10;
                    dap += (int)(ch - '0');
                } else if ((k == 0 & kprev == 1) | (ch == ' ' & k == 1)) {
                    if (digitafterdot != 0) {
                        SectionData.st_B_center[stcenter_i] = d + dap / pow(10, digitafterdot - 1);
                    } else SectionData.st_B_center[stcenter_i] = d;
                    d = 0;
                    dap = 0;
                    digitafterdot = 0;
                    stcenter_i = (stcenter_i + 1) % 3;
                }
                break;
            case 4:
                if (k == 1 & kprev == 1 & ch != '-') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 0 & kprev == 1) {
                    SectionData.interval = d;
                    d = 0;
                }
                break;
            case 5:
                if (k == 1 & kprev == 1 & ch != '-') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 0 & kprev == 1) {
                    SectionData.stationWait = d;
                    d = 0;
                    digitafterdot = 0;
                }
                break;
            case 6:
                if (k == 1 & kprev == 1 & ch != '-' & digitafterdot == 0) {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 1 & kprev == 1 & digitafterdot > 0 & ch != '.') {
                    digitafterdot++;
                    dap *= 10;
                    dap += (int)(ch - '0');
                } else if (k == 0 & kprev == 1) {
                    if (digitafterdot != 0) {
                        SectionData.st_len = d + dap / pow(10, digitafterdot - 1);
                    } else SectionData.st_len = d;
                    d = 0;
                    dap = 0;
                    digitafterdot = 0;
                }
                break;
            case 7:
                if (k == 1 & kprev == 1 & ch != '-') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 0 & kprev == 1) {
                    SectionData.Coeff = d;
                    d = 0;
                }
                break;
            case 8:
                if (k == 1 & kprev == 1 & ch != '-') {
                    d *= 10;
                    d += (int)(ch - '0');
                } else if (k == 0 & kprev == 1) {
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
                } else if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot > 0 & ch != '.' & ch != ',' & ch != ' ') {
                    digitafterdot++;
                    dap *= 10;
                    dap += (int)(ch - '0');
                    //printf("%c\n", ch);
                } else if ((SquareBracketsCount != 2 & SquareBracketsCountPrev == 2)) {
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
                } else if (SquareBracketsCount == 2 & SquareBracketsCountPrev == 2 & digitafterdot > 0 & ch != '.' & ch != ',' & ch != ' ') {
                    digitafterdot++;
                    dap *= 10;
                    dap += (int)(ch - '0');
                    //printf("%c\n", ch);
                } else if ((SquareBracketsCount != 2 & SquareBracketsCountPrev == 2)) {
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

    //Убрать мусор

    fclose(SectorFile);
    return 0;
}

int main() {
//Interface section
    char PathToFolder[51];
    char TracksFileName[52];
    char PathToTracks[102];
    TracksFileName[0] = '/';
_Start:
    PathToFolder[0] = '\0';
    PathToTracks[1] = '\0';
    /*printf("Work Folder: ");
    scanf("%50s", PathToFolder);
    printf("Tracks File Name: ");
    scanf("%50s", TracksFileName + sizeof(char));

    strcpy(PathToTracks, PathToFolder);
    strcat(PathToTracks, TracksFileName);*/

    strcpy(PathToTracks, "aaa/track_gm_mus_neoorange_e.txt");
    strcpy(PathToFolder, "aaa");

    printf("Full Path To Track File: %s\n\n", PathToTracks);
    if (ReadTracksFile(PathToTracks) == -1) goto _Start;

    CreateWorkspace(PathToFolder);
//Interface section

//Choose section
    char PathToDM[54];
    char PathToSignals[59];
    char PathToDMEx[67];
    char PathToSignalsEx[72];
    strcpy(PathToDM, PathToFolder);
    strcat(PathToDM, "/dm");
    strcpy(PathToSignals, PathToFolder);
    strcat(PathToSignals, "/signals");
    mkdir(PathToDM);
    mkdir(PathToSignals);
    strcpy(PathToDMEx, PathToDM);
    strcpy(PathToSignalsEx, PathToDMEx);
    strcat(PathToDMEx, "/example.txt");
    strcat(PathToSignalsEx, "/example.txt");
    FILE *ex1,*ex2;
    ex1 = fopen(PathToDMEx, "w");
    ex2 = fopen(PathToSignalsEx, "w");
    fprintf(ex1,"\"St. A\"; #\"st_A_Name\";\n\"St. B\"; #\"st_B_Name\";\n\n\"-557.330933 13539.387695 1617.857422\";  #\"st_A_center\";\n\"-14387.382813 3013.013428 2264.259033\"; #\"st_B_center\";\n\n\"90\";    #\"interval\";\n\"30\";    #\"stationWait\";\n\"57.36\"; #\"st_len\";\n\n\"1\"; #\"Coeff\"; #usually 1 or 1.5\n\"1\"; #\"SlopeFilter\"; #0 or 1\n\nmode = [\n    [125, 0],\n    [200, 2],\n    [225, 0]\n];\n\nmode2 = [\n    [150,0]\n];");
    fprintf(ex2,"");
    fclose(ex1);
    fclose(ex2);
    char NameOfSector[52];
    char PathToSector[105];
_Choose:
    NameOfSector[0] = '/';
    NameOfSector[1] = '\0';
    PathToSector[0] = '\0';
    strcpy(PathToSector, PathToDM);
    printf("Sector Name: ");
    scanf("%50s", NameOfSector  + sizeof(char));
    strcat(PathToSector, NameOfSector);
    if (ReadSectionDMFile(PathToSector) == -1) goto _Choose;
//Choose section

    printf("%s\n",SectionData.st_A_Name);
    printf("%s\n",SectionData.st_B_Name);
    printf("%f ",SectionData.st_A_center[0]);
    printf("%f ",SectionData.st_A_center[1]);
    printf("%f\n",SectionData.st_A_center[2]);
    printf("%f ",SectionData.st_B_center[0]);
    printf("%f ",SectionData.st_B_center[1]);
    printf("%f\n",SectionData.st_B_center[2]);
    printf("%i\n",SectionData.interval);
    printf("%i\n",SectionData.stationWait);
    printf("%f\n",SectionData.st_len);
    printf("%f\n",SectionData.Coeff);
    printf("%i\n",SectionData.SlopeFilter);
    printf("%i ",SectionData.mode[0][0]);
    printf("%i\n",SectionData.mode[0][1]);
    printf("%i ",SectionData.mode[1][0]);
    printf("%i\n",SectionData.mode[1][1]);
    printf("%i ",SectionData.mode[2][0]);
    printf("%i\n",SectionData.mode[2][1]);
    printf("%i ",SectionData.mode2[0][0]);
    printf("%i\n\n",SectionData.mode2[0][1]);

    Profile_Think();

    getch();
    return 0;
}