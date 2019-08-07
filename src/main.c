/*
 Copyright ? 2019-2019, Zhang Hongda, a64091937. All Rights Reserved.
 Description:A BP Neurial Network Based on C Project
 Author:Zhang Hongda
 Creation time:2019-7-28
 */
//һ�������� C ���Ե� BP ��������룬����ֱ���� VC++�����С�
//˫���㣬��������ѵ��Ȩֵ��ѵ����֮��Ȩֵ���ڼ�����
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "file_manager.h"
#include "bp.h"
#include "def.h"


int	main(int argc, char const *argv[])
{
    FILE *fpe, *fpf, *fpg, *fpa;
    double e1, e2, sum = 0;
    int i, j, k = 0, kk = 0; //k�����������ĸ�����
    In_Data_File_Struct BP_In_File = {25000, 35000, 60000, 40, 39, 41, "60000.txt"};
    FP64* fpTrainData = NULL;
    FP64* fpTestData = NULL;
    FP64* fpInTrainData = NULL;
    FP64* fpOutTrainData = NULL;
    
    FP64* File_Size1=NULL;


    printf("%s\n", BP_In_File.scDataFile);
    printf("%d", sizeof(long));

    printf("\nint64u%d", sizeof(INT64U));
    printf("\nfl32_%d", sizeof(FP32));
    printf("\n Data_In_%0.2f", ((float)1 / 100));

    File_Size1 = readData_func(BP_In_File,fpTrainData,fpTestData,fpInTrainData,fpOutTrainData);
    for(j = 0; j < 40; j++) {
        printf("%lf ", File_Size1[j]);   //����ȡ���������ȴ洢��dd[i][j]������
    }
    
    readData();	//��ȡ��������

    initBPNework();	//��ʼ����Ԫ����

    trainNetwork();	//ѵ����Ԫ����

    if((fpe = fopen("MaxOut.txt", "w")) == NULL) {	//MaxOut�Ǹ������λ��������ļ�
        printf("can not write the MaxOut file\n");
        exit(0);
    }
    if((fpf = fopen("testout.txt", "w")) == NULL) {	//testout�������������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
        printf("can not write the testout file\n");
        exit(0);
    }
    if((fpg = fopen("special.txt", "w")) == NULL) {	//special���������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
        printf("can not write the special file\n");
        exit(0);
    }
    if((fpa = fopen("analysis.txt", "a")) == NULL) {	//special���������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
        printf("can not write the analysis file\n");
        exit(0);
    }
    for(i = 0; i < testData; i++) {
        //e1��ʵ�����
        result(test_dd[i][0], test_dd[i][1], test_dd[i][2], test_dd[i][3], test_dd[i][4], test_dd[i][5], test_dd[i][6], test_dd[i][7], test_dd[i][8], test_dd[i][9],
               test_dd[i][10], test_dd[i][11], test_dd[i][12], test_dd[i][13], test_dd[i][14], test_dd[i][15], test_dd[i][16], test_dd[i][17], test_dd[i][18], test_dd[i][19],
               test_dd[i][20], test_dd[i][21], test_dd[i][22], test_dd[i][23], test_dd[i][24], test_dd[i][25], test_dd[i][26], test_dd[i][27], test_dd[i][28], test_dd[i][29],
               test_dd[i][30], test_dd[i][31], test_dd[i][32], test_dd[i][33], test_dd[i][34], test_dd[i][35], test_dd[i][36], test_dd[i][37], test_dd[i][38]);
        fprintf(fpf, "%d ", i); //��һ�����ݵ����



        Max[i] = resultOut[0]; //�ҳ����ĸ����Ƕ���
        MaxNum[i] = 110;
        for(j = 0; j < Out; j++) {
            fprintf(fpf, "%lf ", resultOut[j]); //��ӡ����һ���ÿһ������
            if(Max[i] < resultOut[j]) {
                Max[i] = resultOut[j];
                MaxNum[i] = j;
            }

        }

        fprintf(fpf, "\n");
        //kk=MaxNum[i];
        if(MaxNum[i] != test_dd[i][39]) {	//��ÿһ������ÿһ������ĸ���д��special�ļ���

            for(j = 0; j < Out; j++) {
                fprintf(fpg, "%lf ", resultOut[j]); //��ӡ����һ���ÿһ������
            }
            //printf("   %lf  \n",MaxNum[i]);
            fprintf(fpg, "   %d  ", MaxNum[i]);
            fprintf(fpg, "   %lf  \n", test_dd[i][39]);
        }


    }
    for(i = 0; i < testData; i++) {
        fprintf(fpe, "%d %lf\n", MaxNum[i], Max[i]);
        if(MaxNum[i] != test_dd[i][39]) {	//��������������ÿһ�����������ʴ�ӡ����Ļ�ϣ�ͬʱ׷�ӵ�special�ļ���ĩβ
            k++;      //k�����������ݵĸ���

            //fprintf(fpg,"ʵ�������%d--%lf Ӧ�������%0.0lf\n",MaxNum[i],Max[i],test_dd[i][39]);
        }

    }

    printf("%lf", ((double)k) / testData); //����Ļ�ϴ�ӡ����ʣ���������������������

    fprintf(fpa, "%lf %lf %6d %6d %6d %lf %lf\n", A, B, TrainC, Neuron, Neuron1, dd_e, ((double)k) / testData);

    writeNeuron_2();		//����Ԫ��Ȩֵд������ļ���
    fclose(fpe);
    fclose(fpf);
    fclose(fpg);
    fclose(fpa);
    system("pause");	//��ͣ���ڲ鿴���

    return 0;

}