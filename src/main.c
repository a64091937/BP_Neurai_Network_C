//һ�������� C ���Ե� BP ��������룬����ֱ���� VC++�����С�
//˫���㣬��������ѵ��Ȩֵ��ѵ����֮��Ȩֵ���ڼ�����
#include <stdio.h> 
#include <time.h> 
#include <math.h> 
#include <stdlib.h>

#define Data	    25000	//����������
#define testData	35000	//��������                                
#define allData	    60000  //1���ݿ�

#define In 39		// ����
#define Out 41		//�������ȷ�������һλΪ1�����Ե����Ϊ����
#define Neuron	 80	//��Ԫ #����0
#define Neuron1	 80	//��Ԫ #����1
#define TrainC	20	//ѵ������
#define A	0.01	//����ֵ
#define B	0.01	//ѧϰ����

double d_all[allData][40];	//��dd[Data][40]�ֱ�õ���������
double d_allgai[allData];
double d_paixu[allData];

double max_data=0;

double d_in[Data][In],d_out[Data][Out];	//��dd[Data][40]�ֱ�õ���������
double dd[Data][40],dd_out[Data][Out];	//�������ļ��е�����������ȡ�������浽���������
double test_dd[testData][40];	//�������ļ��е�����������ȡ�������浽���������

double Max[testData];//ÿһ��������ݵ����ֵ�Ƕ���
int    MaxNum[testData];//����������һλ�Ǽ�

//������̣�d_in->w->o->u->o1->v->d_out
//                    #����0                       #����1
double w[Neuron][In],o[Neuron],u[Neuron1][Neuron],o1[Neuron1],v[Out][Neuron1];	//��Ԫ��Ȩֵ

double Maxin[In],Minin[In],Maxout[Out],Minout[Out];//���ֵ��Сֵ���������������һ��

double OutputData[Out];	//��Ԫ�����

double resultOut[Out];	//���ԵĽ��

double dw[Neuron][In],du[Neuron1][Neuron],dv[Out][Neuron1];	//��Ԫ��Ȩֵ��������

double e;//ÿһ��ѵ����ƽ�����

double dd_e;

//��ȡ����
void readData(){

	FILE *fp1,*fp2,*fp3,*fp4; 
	int i,j,k,cx,outi;
	double temp;

	if((fp1=fopen("60000.txt","r"))==NULL){	//��ȡinout.txt�������Ծ͵������󾯸�
		printf("can not read the  inout file\n"); 
		exit(0);
	}

	for(i=0;i<allData;i++){
		for(j=0; j<40; j++){
			fscanf(fp1,"%lf",&d_all[i][j]);    //����ȡ���������ȴ洢��dd[i][j]������
		}
	}	
	fclose(fp1);

//��ȡ�����������ˣ�����Ҫ�����ȡ��
	srand((unsigned)time(NULL)); 

	for (i = 0; i < allData; ++i){
		d_allgai[i]=(rand()*2)/2+100; //
		d_paixu[i]=d_allgai[i];
	}

    for (j = 0; j < allData - 1; j++){
        for (i = 0; i < allData - 1 - j; i++)
        {
            if(d_paixu[i] > d_paixu[i+1])
            {
                temp = d_paixu[i];
                d_paixu[i] = d_paixu[i+1];
                d_paixu[i+1] = temp;
            }
        }

	}

	max_data=d_paixu[Data];


	cx=0;
    for (i = 0; i < Data; i++){
		
        for (j = cx; j < allData; j++)
        {
            if(d_allgai[j] <= max_data)
            {
				for (outi = 0; outi < 40; outi++){
					dd[i][outi]=d_all[j][outi];
				
				}
				cx=j+1;
				d_allgai[j] = 1;
				break;
            }
        }

	}
	for(i=0;i<Data;i++){
		for(j=0; j<In; j++)
			d_in[i][j]  = dd[i][j];	         //��dd[i][j]�е����벿���ȴ洢��d_in[i][j] ��
		
		k = dd[i][39];
		
		for(j=0; j<Out; j++){
			d_out[i][j] = 0;         //��dd[i][j]�е���������ȴ洢��d_out[i][j]��
			dd_out[i][j] = 0; 
		}
		d_out[i][k] = 1; 
		dd_out[i][k] = 1; 

	}

	cx=0;
    for (i = 0; i < testData; i++){
		
        for (j = cx; j < allData; j++)
        {
            if(d_allgai[j] != 1)
            {
				for (outi = 0; outi < 40; outi++){
					test_dd[i][outi]=d_all[j][outi];
				
				}
				cx=j+1;
				break;
            }
        }

	}


}


//��ʼ�������� 
void initBPNework(){
	FILE *fp1,*fp2,*fp3; 
	int i,j;

	//�����������ֵ����Сֵ 
	for(i=0; i<In; i++){

		Minin[i]=Maxin[i]=d_in[0][i];

		for(j=0; j<Data; j++){	
			Maxin[i]=Maxin[i]>d_in[j][i]?Maxin[i]:d_in[j][i]; 
			Minin[i]=Minin[i]<d_in[j][i]?Minin[i]:d_in[j][i];

		}

	}

	//������ݵ������Сֵ 
	for(i=0; i<Out; i++){

		Minout[i]=Maxout[i]=d_out[0][i]; 
		for(j=0; j<Data; j++){

			Maxout[i]=Maxout[i]>d_out[j][i]?Maxout[i]:d_out[j][i]; 
			Minout[i]=Minout[i]<d_out[j][i]?Minout[i]:d_out[j][i];
		
		}

	}

	//�������ݹ�һ������ ����0~1֮��
	for (i = 0; i < In; i++)

		for(j = 0; j < Data; j++) 
			d_in[j][i]=(d_in[j][i]-Minin[i]+1)/(Maxin[i]-Minin[i]+1); //��һ���������ͳһת���ɷ�������ʽ��0<x<1������ͳһ����

/*	//������ݹ�һ������ 
	for (i = 0; i < Out; i++)

		for(j = 0; j < Data; j++) 
			d_out[j][i]=(d_out[j][i]-Minout[i]+1)/(Maxout[i]-Minout[i]+1);

*/
	srand((unsigned)time(NULL)); 
	//��ȡ����Ȩֵw���ļ���û�����������Ȩֵw
	if((fp1=fopen("neuronw.txt","r"))==NULL){
	
		//����0����Ȩֵ���
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < In; ++j){
				w[i][j]=(rand()*2.0/RAND_MAX-1)/2; //��Ԫ��Ȩֵ,һ��Ϊ���������ֵ��0��1֮��
				dw[i][j]=0;					       //��Ԫ��Ȩֵ��������
			}
	}
	else{
		for (i = 0; i < Neuron; ++i){ 
			for (j = 0; j < In; ++j){
				fscanf(fp1,"%lf",&w[i][j]);  //��neuronw.txt�ж�ȡ����0������Ȩֵ
				dw[i][j]=0;					//��Ԫ��Ȩֵ��������
			}	
		}
		fclose(fp1);
	}

	//��ȡ����Ȩֵu���ļ���û�����������Ȩֵu
	if((fp2=fopen("neuronu.txt","r"))==NULL){
	
		//����1����Ȩֵ���  
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Neuron1; ++j){
				u[j][i]=(rand()*2.0/RAND_MAX-1)/2; 	//��Ԫ��Ȩֵ,һ��Ϊ���������ֵ��0��1֮��
				du[j][i]=0;					        //��Ԫ��Ȩֵ��������
			}		
	}
	else{
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Neuron1; ++j){
				fscanf(fp2,"%lf",&u[j][i]);  //��neuronu.txt�ж�ȡ����1������Ȩֵ
				du[j][i]=0;					//��Ԫ��Ȩֵ��������
			}
		fclose(fp2);
	}

	//��ȡ����Ȩֵu���ļ���û�����������Ȩֵu	
	if((fp3=fopen("neuronv.txt","r"))==NULL){
	
     	//����1���Ȩֵ���  
		for (i = 0; i < Neuron1; ++i)
			for (j = 0; j < Out; ++j){
				v[j][i]=(rand()*2.0/RAND_MAX-1)/2; 	//��Ԫ��Ȩֵ,һ��Ϊ���������ֵ��0��1֮��
				dv[j][i]=0;					//��Ԫ��Ȩֵ��������
			}		
	}
	else{
		for (i = 0; i < Neuron1; ++i)
			for (j = 0; j < Out; ++j){
				fscanf(fp3,"%lf ",&v[j][i]);   //��neuronv.txt�ж�ȡ����1�����Ȩֵ
				dv[j][i]=0;					//��Ԫ��Ȩֵ��������
			}
		fclose(fp3);
	}
}


//comput0�����򴫵�
void computO(int var){	//var Ϊĳһ����������

	int i,j; 
	double sum,y=0,ei[Out];

	for (i = 0; i < Neuron; ++i){ //Neuron Ϊ#����0-��Ԫ����

		sum=0;

		for (j = 0; j < In; ++j) 
			sum+=w[i][j]*d_in[var][j];

		o[i]=1/(1+exp(-1*sum));//sigmoid����----����Ԫ����ļ����
								
	}

	for (i = 0; i < Neuron1; ++i){ //Neuron1 Ϊ#����1-��Ԫ����

		sum=0;

		for (j = 0; j < Neuron; ++j) 
			sum+=u[i][j]*o[j];

		o1[i]=1/(1+exp(-1*sum));	
								
	}

	for (i = 0; i < Out; ++i){

		sum=0;

		for (j = 0; j < Neuron1; ++j) 
			sum+=v[i][j]*o1[j]; 

		ei[i]=exp(sum);	//���ֵ��Ϊȫ�ֱ�������Out��
		y += ei[i];
	}
	for (i = 0; i < Out; ++i){

		OutputData[i] = ei[i]/y;	//���ֵ��Ϊȫ�ֱ�������Out��

	}
}


//���򴫲��㷨����
void backUpdate(int var){   //var Ϊĳһ����������

	int i,j,k,m;
    double t[Neuron1],t1[Neuron];
    for (k = 0; k < Neuron1; ++k)   
    {
        t[k]=0;
        for (m = 0; m < Out; ++m){	
            t[k]+=(OutputData[m]-d_out[var][m])*v[m][k];

            dv[m][k]=A*dv[m][k]+B*(OutputData[m]-d_out[var][m])*o1[k];
            v[m][k]-=dv[m][k];
        }

        for (j = 0; j < Neuron; ++j){	
            du[k][j]=A*du[k][j]+B*t[k]*o1[k]*(1-o1[k])*o[j];
            u[k][j]-=du[k][j];
        }
		
    }

    for (j = 0; j < Neuron; ++j)   
    {
        t1[j]=0;
        for (k = 0; k < Neuron1; ++k){	
				t1[j]+=t[k]*o1[k]*(1-o1[k])*u[k][j];	
        }

        for (i = 0; i < In; ++i){	
            dw[j][i]=A*dw[j][i]+B*t1[j]*o[j]*(1-o[j])*d_in[var][i];
            w[j][i]-=dw[j][i];
        }	
    }
}
//���ڲ���׼ȷ�ʵĺ������������򴫵�
void result(double var1,double var2,double var3,double var4,double var5,double var6,double var7,double var8,
			  double var9,double var10,
			  double var11,double var12,double var13,double var14,double var15,double var16,double var17,double var18,
			  double var19,double var20,
			  double var21,double var22,double var23,double var24,double var25,double var26,double var27,double var28,
			  double var29,double var30,
			  double var31,double var32,double var33,double var34,double var35,double var36,double var37,
			  double var38,double var39){	//������������������仯���˴���Ҫ�޸ĺ�����������*������׼ȷ��*��ʱ��Ҫ�õ��ĺ���

	int i,j; 
	double sum,res=0;

	var1=(var1-Minin[0]+1)/(Maxin[0]-Minin[0]+1); //�������ݹ�һ�� 
	var2=(var2-Minin[1]+1)/(Maxin[1]-Minin[1]+1); //�������ݹ�һ��
	var3=(var3-Minin[2]+1)/(Maxin[2]-Minin[2]+1); //�������ݹ�һ�� 
	var4=(var4-Minin[3]+1)/(Maxin[3]-Minin[3]+1); //�������ݹ�һ��
	var5=(var5-Minin[4]+1)/(Maxin[4]-Minin[4]+1); //�������ݹ�һ�� 
	var6=(var6-Minin[5]+1)/(Maxin[5]-Minin[5]+1); //�������ݹ�һ��
	var7=(var7-Minin[6]+1)/(Maxin[6]-Minin[6]+1); //�������ݹ�һ�� 
	var8=(var8-Minin[7]+1)/(Maxin[7]-Minin[7]+1); //�������ݹ�һ��
	var9=(var9-Minin[8]+1)/(Maxin[8]-Minin[8]+1); //�������ݹ�һ�� 
	var10=(var10-Minin[9]+1)/(Maxin[9]-Minin[9]+1); //�������ݹ�һ��

	var11=(var11-Minin[10]+1)/(Maxin[10]-Minin[10]+1); //�������ݹ�һ�� 
	var12=(var12-Minin[11]+1)/(Maxin[11]-Minin[11]+1); //�������ݹ�һ��
	var13=(var13-Minin[12]+1)/(Maxin[12]-Minin[12]+1); //�������ݹ�һ�� 
	var14=(var14-Minin[13]+1)/(Maxin[13]-Minin[13]+1); //�������ݹ�һ��
	var15=(var15-Minin[14]+1)/(Maxin[14]-Minin[14]+1); //�������ݹ�һ�� 
	var16=(var16-Minin[15]+1)/(Maxin[15]-Minin[15]+1); //�������ݹ�һ��
	var17=(var17-Minin[16]+1)/(Maxin[16]-Minin[16]+1); //�������ݹ�һ�� 
	var18=(var18-Minin[17]+1)/(Maxin[17]-Minin[17]+1); //�������ݹ�һ��
	var19=(var19-Minin[18]+1)/(Maxin[18]-Minin[18]+1); //�������ݹ�һ�� 
	var20=(var20-Minin[19]+1)/(Maxin[19]-Minin[19]+1); //�������ݹ�һ��

	var21=(var21-Minin[20]+1)/(Maxin[20]-Minin[20]+1); //�������ݹ�һ�� 
	var22=(var22-Minin[21]+1)/(Maxin[21]-Minin[21]+1); //�������ݹ�һ��
	var23=(var23-Minin[22]+1)/(Maxin[22]-Minin[22]+1); //�������ݹ�һ�� 
	var24=(var24-Minin[23]+1)/(Maxin[23]-Minin[23]+1); //�������ݹ�һ��
	var25=(var25-Minin[24]+1)/(Maxin[24]-Minin[24]+1); //�������ݹ�һ�� 
	var26=(var26-Minin[25]+1)/(Maxin[25]-Minin[25]+1); //�������ݹ�һ��
	var27=(var27-Minin[26]+1)/(Maxin[26]-Minin[26]+1); //�������ݹ�һ�� 
	var28=(var28-Minin[27]+1)/(Maxin[27]-Minin[27]+1); //�������ݹ�һ��
	var29=(var29-Minin[28]+1)/(Maxin[28]-Minin[28]+1); //�������ݹ�һ�� 
	var30=(var30-Minin[29]+1)/(Maxin[29]-Minin[29]+1); //�������ݹ�һ��

	var31=(var31-Minin[30]+1)/(Maxin[30]-Minin[30]+1); //�������ݹ�һ�� 
	var32=(var32-Minin[31]+1)/(Maxin[31]-Minin[31]+1); //�������ݹ�һ��
	var33=(var33-Minin[32]+1)/(Maxin[32]-Minin[32]+1); //�������ݹ�һ�� 
	var34=(var34-Minin[33]+1)/(Maxin[33]-Minin[33]+1); //�������ݹ�һ��
	var35=(var35-Minin[34]+1)/(Maxin[34]-Minin[34]+1); //�������ݹ�һ�� 
	var36=(var36-Minin[35]+1)/(Maxin[35]-Minin[35]+1); //�������ݹ�һ��
	var37=(var37-Minin[36]+1)/(Maxin[36]-Minin[36]+1); //�������ݹ�һ�� 
	var38=(var38-Minin[37]+1)/(Maxin[37]-Minin[37]+1); //�������ݹ�һ��
	var39=(var39-Minin[38]+1)/(Maxin[38]-Minin[38]+1); //�������ݹ�һ�� 




	for (i = 0; i < Neuron; ++i){

		sum=0;

		sum=w[i][0]*var1+w[i][1]*var2+w[i][2]*var3+w[i][3]*var4+w[i][4]*var5+w[i][5]*var6+w[i][6]*var7+w[i][7]*var8+w[i][8]*var9+w[i][9]*var10
			+w[i][10]*var11+w[i][11]*var12+w[i][12]*var13+w[i][13]*var14+w[i][14]*var15+w[i][15]*var16+w[i][16]*var17+w[i][17]*var18+w[i][18]*var19+w[i][19]*var20
			+w[i][20]*var21+w[i][21]*var22+w[i][22]*var23+w[i][23]*var24+w[i][24]*var25+w[i][25]*var26+w[i][26]*var27+w[i][27]*var28+w[i][28]*var29+w[i][29]*var30
			+w[i][30]*var31+w[i][31]*var32+w[i][32]*var33+w[i][33]*var34+w[i][34]*var35+w[i][35]*var36+w[i][36]*var37+w[i][37]*var38+w[i][38]*var39; //��������仯���������� 
		o[i]=1/(1+exp(-1*sum));

	}
	for (i = 0; i < Neuron1; ++i){ 

		sum=0;

		for (j = 0; j < Neuron; ++j) 
			sum+=u[i][j]*o[j];

		o1[i]=1/(1+exp(-1*sum));	
								
	}
	for (i = 0; i < Out; ++i){ 
		sum=0;
		for (j = 0; j < Neuron1; ++j)
			sum+=v[i][j]*o1[j];
		resultOut[i]=(sum*(Maxout[i]-Minout[i]+1)+Minout[i]-1);

	}

	//������softmax��������������ʻ�
	for (i = 0; i < Out; ++i){ 
		
		resultOut[i]=exp(resultOut[i]);//softmax������Pi=ei/(����ei֮��)
		res+=resultOut[i];

	}
	for (i = 0; i < Out; ++i){ 		
		resultOut[i]=resultOut[i] /res;
	}

}

//����Ԫ��Ȩֵд������ļ���
void writeNeuron(){  

	FILE *fp1,*fp2,*fp3; 
	int i,j;

	if((fp1=fopen("neuronw.txt","w"))==NULL){

		printf("can not open the neuron file\n"); 
		exit(0);

	}

	for (i = 0; i < Neuron; ++i){ 
		for (j = 0; j < In; ++j){

			fprintf(fp1,"%lf ",w[i][j]);  //������0������Ȩֵд��neuronw.txt��

		}
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	if((fp2=fopen("neuronu.txt","w"))==NULL){

		printf("can not open the neuron file\n"); 
		exit(0);

	}


	for (i = 0; i < Neuron; ++i){

		for (j = 0; j < Neuron1; ++j){

			fprintf(fp2,"%lf ",u[j][i]);  //������1������Ȩֵд��neuronu.txt��

		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);

	if((fp3=fopen("neuronv.txt","w"))==NULL){

		printf("can not open the neuron file\n"); 
		exit(0);

	}
	for (i = 0; i < Neuron1; ++i){

		for (j = 0; j < Out; ++j){

			fprintf(fp3,"%lf ",v[j][i]);  //������1�����Ȩֵд��neuronv.txt��

		}
		fprintf(fp3,"\n");
	}
	fclose(fp3);

}

//ѵ��������ĺ���
void trainNetwork(){

	int i,cc=0,yzc=0,j;
	double error=0;
	FILE *fp1; 
	if((fp1=fopen("zhunquelv_out.txt","a"))==NULL){	//��ȡinout.txt�������Ծ͵������󾯸�
		printf("can not read the  fanhua_out file\n"); 
		exit(0);
	}
	do{

		e=0;
		yzc=0;
		for (i = 0; i < Data; ++i){			//ÿ����һ����Data�Σ��͵õ�һ��ƫ��ٷֱ�֮��

			computO(i);						//���򴫲���ȡ���ֵ,������� output[out]��������
			error =0;
			for(j = 0;j<Out;++j)
				error+=fabs( (OutputData[j]-d_out[i][j]) );  //�������������ƽ�����
			e+=error;	//ƫ��İٷֱ�֮��

			backUpdate(i);					//���򴫲�������


		}


		for (i = 0; i < Data; ++i){			//ÿ����һ����Data�Σ��͵õ�һ��ƫ��ٷֱ�֮��

			result(dd[i][0], dd[i][1], dd[i][2], dd[i][3], dd[i][4], dd[i][5], dd[i][6], dd[i][7], dd[i][8], dd[i][9],
				   dd[i][10],dd[i][11],dd[i][12],dd[i][13],dd[i][14],dd[i][15],dd[i][16],dd[i][17],dd[i][18],dd[i][19],
			       dd[i][20],dd[i][21],dd[i][22],dd[i][23],dd[i][24],dd[i][25],dd[i][26],dd[i][27],dd[i][28],dd[i][29],
			       dd[i][30],dd[i][31],dd[i][32],dd[i][33],dd[i][34],dd[i][35],dd[i][36],dd[i][37],dd[i][38]);
			Max[i]=resultOut[0];//�ҳ����ĸ����Ƕ���
			MaxNum[i]=0;
			for(j=0;j<Out;j++){
		
				if(Max[i]<resultOut[j]){
					Max[i]=resultOut[j];
					MaxNum[i]=j;
				}
			}


			if(MaxNum[i] != dd[i][39]){	//��ÿһ������ÿһ������ĸ���д��special�ļ���
				yzc++;
			} 


		}


		dd_e = 	((double)yzc/(double)Data) ;
		fprintf(fp1,"%d	%lf %lf\n",cc,e/Data,((double)yzc/(double)Data) );
		printf("%d	%lf %lf\n",cc,e/Data,((double)yzc/(double)Data) );		//cc�ǵ�ǰѵ��������e/Data��ƽ��ƫ��ٷֱ�

		cc++;
		
	}while(cc<TrainC && e/Data>0.0005);//��ѵ����������TrainC����ƽ��ƫ��ٷֱ�С��0.3%���˳�ѭ��������ѵ����
	fclose(fp1);
}



int	main(int argc, char const *argv[]){

	FILE *fpe,*fpf,*fpg,*fpa;
	double e1,e2,sum=0;
	int i,j,k=0,kk=0;//k�����������ĸ�����


	readData();	//��ȡ��������

	initBPNework();	//��ʼ����Ԫ����

	trainNetwork();	//ѵ����Ԫ����

	if((fpe=fopen("MaxOut.txt","w"))==NULL){	//MaxOut�Ǹ������λ��������ļ�
		printf("can not write the MaxOut file\n"); 
		exit(0);
	}
	if((fpf=fopen("testout.txt","w"))==NULL){	//testout�������������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
		printf("can not write the testout file\n"); 
		exit(0);
	}
	if((fpg=fopen("special.txt","w"))==NULL){	//special���������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
		printf("can not write the special file\n"); 
		exit(0);
	}
	if((fpa=fopen("analysis.txt","a"))==NULL){	//special���������������ݵ��ļ�����һ��������һ�����ݵ���ţ�
		printf("can not write the analysis file\n"); 
		exit(0);
	}	
	for(i=0;i<testData;i++){
		//e1��ʵ�����
		result(test_dd[i][0],test_dd[i][1],test_dd[i][2],test_dd[i][3],test_dd[i][4],test_dd[i][5],test_dd[i][6],test_dd[i][7],test_dd[i][8],test_dd[i][9],
			   test_dd[i][10],test_dd[i][11],test_dd[i][12],test_dd[i][13],test_dd[i][14],test_dd[i][15],test_dd[i][16],test_dd[i][17],test_dd[i][18],test_dd[i][19],
			   test_dd[i][20],test_dd[i][21],test_dd[i][22],test_dd[i][23],test_dd[i][24],test_dd[i][25],test_dd[i][26],test_dd[i][27],test_dd[i][28],test_dd[i][29],
			   test_dd[i][30],test_dd[i][31],test_dd[i][32],test_dd[i][33],test_dd[i][34],test_dd[i][35],test_dd[i][36],test_dd[i][37],test_dd[i][38]);
		fprintf(fpf,"%d ",i);//��һ�����ݵ����
		


		Max[i]=resultOut[0];//�ҳ����ĸ����Ƕ���
		MaxNum[i]=110;
		for(j=0;j<Out;j++){
			fprintf(fpf,"%lf ",resultOut[j]);//��ӡ����һ���ÿһ������
			if(Max[i]<resultOut[j]){
				Max[i]=resultOut[j];
				MaxNum[i]=j;
			}
	
		}

		fprintf(fpf,"\n");
		//kk=MaxNum[i];
		if(MaxNum[i] != test_dd[i][39]){	//��ÿһ������ÿһ������ĸ���д��special�ļ���
		
			for(j=0;j<Out;j++){
				fprintf(fpg,"%lf ",resultOut[j]);//��ӡ����һ���ÿһ������
			}
			//printf("   %lf  \n",MaxNum[i]);
			fprintf(fpg,"   %d  ",MaxNum[i]);
			fprintf(fpg,"   %lf  \n",test_dd[i][39]);
		} 
	

    }
	for(i=0;i<testData;i++){
		fprintf(fpe,"%d %lf\n",MaxNum[i],Max[i]);
		if(MaxNum[i]!=test_dd[i][39]){	//��������������ÿһ�����������ʴ�ӡ����Ļ�ϣ�ͬʱ׷�ӵ�special�ļ���ĩβ
			k++;      //k�����������ݵĸ���

		                        	//fprintf(fpg,"ʵ�������%d--%lf Ӧ�������%0.0lf\n",MaxNum[i],Max[i],test_dd[i][39]);
		}

	}

	printf("%lf",((double)k)/testData);//����Ļ�ϴ�ӡ����ʣ���������������������
	
	fprintf(fpa,"%lf %lf %6d %6d %6d %lf %lf\n",A,B,TrainC,Neuron,Neuron1,dd_e,((double)k)/testData);

	writeNeuron();		//����Ԫ��Ȩֵд������ļ���
	fclose(fpe);
	fclose(fpf);
	fclose(fpg);
	fclose(fpa);
	system("pause");	//��ͣ���ڲ鿴���

	return 0;

}