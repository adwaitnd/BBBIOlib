#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "BBBiolib.h"
#include "rt_nonfinite.h"
#include "Prep_fft_emxutil.h"
#include "fft.h"
#include "Prep_fft.h"
#include <math.h>

/* ----------------------------------------------------------- */
// Occ: 0.3s with 192k
#define BUFFER_SIZE 57600
#define SAMPLE_SIZE 57600
// Presence: 50ms with 192k
#define TONE_SAMPLE_SIZE 9600
#define PATH_TO_MODEL "/home/debian/Model.txt"  // File name of the model
#define PATH_TO_TEMP_MODEL "/home/debian/New_Model.txt"  // File name of the temp model
#define RAW_ENABLE	0	// Keep raw data or not
#define CLEN	0.03
// These parameters should be consistent with the generated chirp file
#define FS	192000
#define START_F	20000
#define END_F	21000
#define EX_BAND 200 //Extra bandwidth when filtering
#define PRC_SIZE 500 // Should be larger than the data size after FFT
#define PRC_WSIZE 51 // Should be equals to the fft size of segmented data -> (BAND+EXTRA_BAND):192k/2(Hz) = X*2:8192(next power of WLEN) 
#define WLEN	CLEN*FS // Window sizes when doing FFT
#define WNUM	4 //Number of windows for gen training features
#define FSIZE	WNUM*PRC_WSIZE //ttl feature size
#define MAX_OCC	20
#define TONE_DELAY 192 //0.001*FS
#define TONE_PRC_SIZE 17 //calculated like PRC_WSIZE 
#define DELAY	2

//For debugging
#define DEBUG	0

float Occ_est(char* path, int flen, float* data, float weight){
	int i, j;
	/*Load the model from path*/
	FILE* fd;
	char* line = NULL;
	size_t len=0;
	ssize_t read;
	float* res_buff = malloc(flen*sizeof(float));
	float* pca_buff = NULL;	
	float ftemp=0;
	float res=0;
	int count=0;	
	int temp = 0;
	int npca = 0;
	char* d;
	/*
	The model should have the following format: 
	[v_norm, v_mean, num_pca, v_pca, v_0, base, unit_d]
	Estmation = (abs((data/v_norm - v_mean)*v_pca*weight - v_0) - base)/unit_d
	*/
	printf("Reading model at: %s\n", path);
	fd = fopen(path, "r");
	if(fd==NULL){
		printf("Model not found\n");
		return -1;
	}	
	//TODO:for debugging	
	if(DEBUG){	
		for(i=0;i<flen;i++){
			printf("%f ", data[i]);
		}
		printf("End of Start\n");
	}
	while( count<2 && (read = getline(&line, &len, fd))!= -3 ){
		count++;
		// parse each line
		temp = 0;
		d  = strtok(line, " ");
		while(d!=NULL){
			//TODO: add too many feature check
			ftemp = atof(d);
			if(count==1){
				res_buff[temp] = data[temp]/ftemp;
			}
			else if(count==2){
				res_buff[temp] = res_buff[temp]-ftemp;
			}
			temp++;
			d = strtok(NULL, " ");
		}
		if (temp<flen){
			printf("Error: Too few features in line %d\n", count);
			return -1;
		}
		if(DEBUG){	
			//TODO:for debugging		
			for(i=0;i<flen;i++){
				printf("%f ", res_buff[i]);
			}
			printf("End of %d\n", count);
		}
	}
	if(count<2){
		printf("Error: model incomplete\n");
		return -1;
	}
	// Read v_pca
	if((read = getline(&line, &len, fd)) == -1){
		printf("No num_pca\n");
		return -1;
	}
	npca = atoi(line);
	if(npca<0 || npca>20){
		printf("Error: num_pca incorrect\n");
		return -1;
	}
	pca_buff = malloc(npca*sizeof(float));
	for(i=0;i<npca;i++){
		pca_buff[i]=0;
		read = getline(&line, &len, fd);
		if(read==-1){
			printf("V_pca incomplete\n");
			return -1;
		}
		temp=0;
		d  = strtok(line, " ");
		while(d!=NULL && temp<flen){
			ftemp = atof(d);
			//printf("[%f %f]", ftemp, res_buff[temp]);
			pca_buff[i]+= res_buff[temp]*ftemp;
			temp++;
			d = strtok(NULL, " ");
		}
		if (temp<flen){
			printf("Error: Too few features in VPCA\n");
			return -1;
		}
	}
	//TODO:multiply pca_buff by weight based on presence detection
	if(DEBUG){	
		for(i=0;i<npca;i++){
			printf("%f ", pca_buff[i]);
		}
		printf("End of PCA\n");
	}
	//Get v_0, base, unit_d 	
	if((read = getline(&line, &len, fd)) == -1){
		printf("No v_0\n");
		return -1;
	}
	// parse each line
	temp = 0;
	d  = strtok(line, " ");
	while(d!=NULL){
		//TODO: Check if too many features
		ftemp = atof(d);
		pca_buff[temp] = pca_buff[temp] - ftemp;
		if(pca_buff[temp]<0){
			pca_buff[temp] = -1*pca_buff[temp];
		}
		res += pca_buff[temp];
		temp++;
		d = strtok(NULL, " ");
	}
	if (temp<npca){
		printf("Error: Too few PCA features, get %d\n", temp);
		return -1;
	}
	if(DEBUG){	
		for(i=0;i<npca;i++){
			printf("%f ", pca_buff[i]);
		}
		printf("End of PCA distance\n");
		printf("RES=%f\n", res);
	}
	if((read = getline(&line, &len, fd)) == -1){
		printf("No base\n");
		return -1;
	}
	ftemp = atof(line); // base
	res -= ftemp;
	if(DEBUG){
		printf("RES=%f after base\n", res);
	}
	if((read = getline(&line, &len, fd)) == -1){
		printf("No unit_d\n");
		return -1;
	}
	ftemp = atof(line); // unit_d
	res = res/ftemp;
	if(DEBUG){
		printf("RES=%f after unit_d\n", res);
	}

	fclose(fd);
	free(line);
	free(res_buff);
	if(pca_buff!=NULL){
		free(pca_buff);
	}
	return res;
}

int BBB_init(unsigned int* buffer_AIN_2){
	/* BBBIOlib init*/
	iolib_init();
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);
	BBBIO_GPIO_set_dir(BBBIO_GPIO1 , 0, BBBIO_GPIO_PIN_17);

	/* using ADC_CALC toolkit to decide the ADC module argument . Example Sample rate : 10000 sample/s
	 *
	 *	#./ADC_CALC -f 10000 -t 5
	 *
	 *	Suggest Solution :
	 *		Clock Divider : 160 ,   Open Dly : 0 ,  Sample Average : 1 ,    Sample Dly : 1
	 *
	 */
//	const int clk_div = 34 ;

	// ADC sampling settings for 48k
	//const int clk_div = 25;
	//const int open_dly = 5;
	//const int sample_dly = 1;
	// For 192k
	const int clk_div = 5;
	const int open_dly = 10;
	const int sample_dly = 1;
	/*ADC work mode : Timer interrupt mode
	 *	Note : This mode handle SIGALRM using signale() function in BBBIO_ADCTSC_work();
	 */
	BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);	// use timer interrupts

	/*ADC work mode : Busy polling mode  */
	/* BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, clk_div);*/


	// microphone is connected to AIN2
	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN2, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly, \
				BBBIO_ADC_STEP_AVG_1, buffer_AIN_2, BUFFER_SIZE);
	

	//BBBIO_ADCTSC_module_ctrl(BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, 1);

	//	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN1, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_1, buffer_AIN_1, 100);
	return 0;
}

int BBB_free(){
	iolib_free();
	return 0;
}

float cal_std(float* input, float size){
	float res = 0;
	int i, n;
    	float average, variance, std_deviation, sum = 0, sum1 = 0;
 
    /*  Compute the sum of all elements */
    	for (i = 0; i < size; i++){
		sum = sum + input[i];
    	}
    	average = sum / size;
    /*  Compute  variance  and standard deviation  */
    	for (i = 0; i < size; i++){
		sum1 = sum1 + pow((input[i] - average), 2);
    	}
    	variance = sum1 / size;
    	std_deviation = sqrt(variance);
    	//printf("Average of all elements = %.2f\n", average);
    	//printf("variance of all elements = %.2f\n", variance);
    	//printf("Standard deviation = %.2f\n", std_deviation);
	return std_deviation;
}

int Presence_detect(float input[5][TONE_PRC_SIZE], float dop_thres, float freq_thres, float energy_thres){
	int i=0, j=0, res=-1;
	//float temp_max[TONE_PRC_SIZE]={0};
	float temp_max = 0;
	float temp_data[5] = {0};
	float sum=0;
	float dop=0, freq=0, energy=0;
	time_t rawtime;
	char data_file_name[128];
	FILE* file_fd;
	//char raw_data_name[128];
	// get time
	time(&rawtime);
	strftime(data_file_name, sizeof(data_file_name), "data/%Y-%m-%d_%H:%M:%S-presence.dat", localtime(&rawtime));
	printf("In detect:\n");
	/*
	for(i=0;i<5;i++){
		for(j=0;j<TONE_PRC_SIZE;j++){
			printf("[%f] ", input[i][j]);
		}
		printf("\n");
	}
	for(i=0;i<5;i++){
		for(j=0;j<TONE_PRC_SIZE;j++){
			if(input[i][j]>temp_max){
				temp_max= input[i][j];
			}
		}
	}
	*/
	//Normalize
	for(i=0;i<5;i++){
		for(j=0;j<TONE_PRC_SIZE;j++){
			temp_data[i]+=input[i][j];
		}
	}
	for(i=0;i<5;i++){
		if(temp_data[i]>temp_max){
			temp_max = temp_data[i];
		}
	}
	for(i=0;i<5;i++){
		temp_data[i] = temp_data[i]/temp_max;
	}
	
	for(i=0;i<5;i++){
		for(j=0;j<TONE_PRC_SIZE;j++){
			input[i][j] = input[i][j]/temp_max;
			//printf("[%f] ", input[i][j]);
		}
		//printf("\n");
	}
	
	//Cal var of energy 
	energy = pow(cal_std(temp_data, 5),2);
	printf("Get mean energy var %f\n", energy);
	//Cal Doppler
	//Cal var of freq 
	for(i=0;i<TONE_PRC_SIZE;i++){
		for(j=0;j<5;j++){
			temp_data[j] = input[j][i];
		}	
		sum += cal_std(temp_data, 5);	
	}
	freq = sum / (float)TONE_PRC_SIZE;	
	printf("Get mean std %f\n", freq);
	
	// Output decision
	res = (dop>dop_thres) || (freq>freq_thres) || (energy>energy_thres);



	// Write to file 	
	file_fd = fopen(data_file_name,"w");	// open file in write mode
	for(i=0;i<5;i++){
		for(j=0;j<TONE_PRC_SIZE;j++){
			fprintf(file_fd, "%f ", input[i][j]);
		}	
		fprintf(file_fd, "\n");
	}
	fprintf(file_fd, "%f\n", dop);
	fprintf(file_fd, "%f\n", freq);
	fprintf(file_fd, "%f\n", energy);
	fprintf(file_fd, "%d\n", res);
	fclose(file_fd);
	//


	return res;
}
int Presence_record(int vol, unsigned int buffer_AIN_2[BUFFER_SIZE], float* output_buff){
	
	time_t rawtime;
	//char data_file_name[128];
	//char raw_data_name[128];
	int fd, i=0, j=0, local_size=TONE_PRC_SIZE, wsize = TONE_SAMPLE_SIZE-TONE_DELAY;
	struct termios old, uart_set;	
	char vol_buff[3]="vU"; // default v=85
	char UART_PATH[30] = "/dev/ttyO5";
	float local_buff[TONE_PRC_SIZE] = {0};
	float input[TONE_SAMPLE_SIZE-TONE_DELAY] = {0};
	FILE* file_fd;

	struct emxArray_real32_T prc_data;	
	struct emxArray_real32_T* prc_pt = &prc_data;
	prc_pt->data = (float*)&local_buff;
	prc_pt->size = &local_size;
	prc_pt->allocatedSize = local_size * sizeof(float);
	prc_pt->numDimensions = 1;
	prc_pt->canFreeData = false;	
	struct emxArray_real32_T input_data;	
	struct emxArray_real32_T* input_pt = &input_data;
	input_pt->data = (float*)&input;
	input_pt->size = &wsize;
	input_pt->allocatedSize = wsize * sizeof(float);
	input_pt->numDimensions = 1;
	input_pt->canFreeData = false;	

	/* Start playback */
	printf("Starting capture with rate %d ...\n", SAMPLE_SIZE);
	// get time
	time(&rawtime);
	//Open uart
	fd = open(UART_PATH, O_RDWR | O_NOCTTY);
	if(fd < 0){
		printf("Dev port failed to open\n");
		return 1;
	}
	//save current attributes
	tcgetattr(fd,&old);
	bzero(&uart_set,sizeof(uart_set)); 

	uart_set.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	uart_set.c_iflag = IGNPAR | ICRNL;
	uart_set.c_oflag = 0;
	uart_set.c_lflag = 0;
	uart_set.c_cc[VTIME] = 0;
	uart_set.c_cc[VMIN]  = 1;

	//clean the line and set the attributes

	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&uart_set);
	if(vol!=-1){
		sprintf(vol_buff, "v%c", vol);
	}
	write(fd, vol_buff, 2);
	usleep(500000);
	printf("Fire in the hole!\n");
	// Fire the chirp!
	write(fd, "t", 1);
	usleep(1000);
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&old);
	close(fd);
	
	
	/* Start capture */
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
	BBBIO_ADCTSC_work(TONE_SAMPLE_SIZE+WLEN);
	printf("Recording done.\n");

	/* Preprocessing */
	// copy and convert to float
	for(i=0;i<TONE_SAMPLE_SIZE-TONE_DELAY;i++){
		input[i] = (float)buffer_AIN_2[i+TONE_DELAY+5760]; // skip crosstalk
	} 
	Prep_fft(input_pt, FS, START_F-(EX_BAND/2), START_F+(EX_BAND/2), prc_pt);
	for(i=0;i<TONE_PRC_SIZE;i++){
		output_buff[i] = local_buff[i];
		//printf("[%f] ", local_buff[i]);
	}
	/*	
	file_fd = fopen("raw_data.txt","w");	// open file in write mode
	for(j = 0 ; j < TONE_SAMPLE_SIZE-TONE_DELAY ; j++){
			fprintf(file_fd, "%f\n", input[j] );
	}
	fclose(file_fd);
	
	file_fd = fopen("prc_data.txt","w");	// open file in write mode
	for(j = 0 ; j < TONE_PRC_SIZE ; j++){
			fprintf(file_fd, "%f\n", output_buff[j] );
	}
	fclose(file_fd);
	*/	
	
	return 0;
}


int Playback_record(int flag, int label, int vol, unsigned int buffer_AIN_2[BUFFER_SIZE], float* output_buff){

	time_t rawtime;
	int i=0, j=0, local_size=PRC_SIZE, wsize = WLEN;
	int fd;
	struct termios old, uart_set;	
	char vol_buff[3]="vU"; // default v=85
	char data_file_name[128];
	char raw_data_name[128];
	char UART_PATH[30] = "/dev/ttyO5";
	char temp_buff[10]="";
	float local_buff[PRC_SIZE] = {0};
	FILE* data_file;
	FILE* temp_fd;
	float input[BUFFER_SIZE] = {0};
	struct emxArray_real32_T prc_data;	
	struct emxArray_real32_T* prc_pt = &prc_data;
	prc_pt->data = (float*)&local_buff;
	prc_pt->size = &local_size;
	prc_pt->allocatedSize = local_size * sizeof(float);
	prc_pt->numDimensions = 1;
	prc_pt->canFreeData = false;	
	struct emxArray_real32_T input_data;	
	struct emxArray_real32_T* input_pt = &input_data;
	input_pt->size = &wsize;
	input_pt->allocatedSize = wsize * sizeof(float);
	input_pt->numDimensions = 1;
	input_pt->canFreeData = false;	

	/* Start playback */
	printf("Starting capture with rate %d ...\n", SAMPLE_SIZE);
	// get time
	time(&rawtime);
	//Open uart
	fd = open(UART_PATH, O_RDWR | O_NOCTTY);
	if(fd < 0){
		printf("Dev port failed to open\n");
		return 1;
	}
	//save current attributes
	tcgetattr(fd,&old);
	bzero(&uart_set,sizeof(uart_set)); 

	uart_set.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	uart_set.c_iflag = IGNPAR | ICRNL;
	uart_set.c_oflag = 0;
	uart_set.c_lflag = 0;
	uart_set.c_cc[VTIME] = 0;
	uart_set.c_cc[VMIN]  = 1;

	//clean the line and set the attributes

	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&uart_set);
	if(vol!=-1){
		sprintf(vol_buff, "v%c", vol);
	}
	write(fd, vol_buff, 2);
	usleep(500000);
	printf("Fire in the hole!\n");
	// Fire the chirp!
	write(fd, "f", 1);
	usleep(1000);
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&old);
	close(fd);
	
	
	/* Start capture */
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
	BBBIO_ADCTSC_work(SAMPLE_SIZE);
	printf("Recording done.\n");

	/* If warmup then we are done*/
	if(flag==1){
		return 0;
	}	 

	/* Preprocessing */
	// copy and convert to float
	for(i=0;i<BUFFER_SIZE;i++){
		input[i] = (float)buffer_AIN_2[i];
	} 
	//for(i=0;i<BUFFER_SIZE;i++){
	//	printf("Get %f\n", input[i]);
	//}
	/* FFT the segmented raw data based on window size*/
	for(i=0;i<WNUM;i++){ 
		input_pt->data = (float*)&(input[(i+1)*wsize]); //ignore the first window since its crosstalk
		//input_pt->data = (float*)&(buffer_AIN_2[i*wsize]);
		Prep_fft(input_pt, FS, START_F-(EX_BAND/2), END_F+(EX_BAND/2), prc_pt);
		for(j=0;j<PRC_WSIZE;j++){
			//output_buff[j+i*PRC_WSIZE] = local_buff[j];
			output_buff[j+i*PRC_WSIZE] = prc_pt->data[j];
			//printf("output[%d] = %f\n", j+i*PRC_WSIZE, output_buff[j+i*PRC_WSIZE]);
		}
	}
	//Prep_fft(input_pt, FS, START_F-(EX_BAND/2), END_F+(EX_BAND/2), prc_pt);	
	printf("Preprocessing done\n");
	/* Save data to files*/
	// format file name
	strftime(data_file_name, sizeof(data_file_name), "data/%Y-%m-%d_%H:%M:%S-", localtime(&rawtime));
	sprintf(temp_buff, "%d", vol);
	strcat(data_file_name, temp_buff);
	strcpy(raw_data_name, data_file_name);
	strcat(data_file_name,".dat" );
	strcat(raw_data_name,".raw" );
	printf("Saving processed sound data to: %s\n",data_file_name);
	data_file = fopen(data_file_name,"w");	// open file in write mode
	/* add current time value to top of file */
	// fprintf(data_file, "%s\n", data_file_name);
	/* add label*/
	fprintf(data_file, "%d\n", label);
	
	// Write processed data, 4 windows * 51 points (5760 points FFT)
	//410 points for 57600 points FFT
	for(j = 0 ; j < FSIZE ; j++){
		fprintf( data_file, "%f\n", output_buff[j] );
	}
	fclose(data_file);
	// Write raw data if defined
	if(RAW_ENABLE){
		temp_fd = fopen(raw_data_name,"w");	// open file in write mode
		for(j = 0 ; j < BUFFER_SIZE ; j++){
			fprintf(temp_fd, "%u\n", buffer_AIN_2[j] );
		}
		fclose(temp_fd);
	}

	return 0;
}

int Recal(char* src_path, char* tar_path, int flen, float data[5][PRC_SIZE]){
	// The model should have the following format: 
	// [v_norm, v_mean, num_pca, v_pca, v_0, base, unit_d]
	// The function updates only the v_0, base, and unit_d
	int i, j, temp, offset;
	FILE* fd1;
	FILE* fd2;
	char* line = NULL;
	char* d;
	size_t len=0;
	ssize_t read;
	int count = 0;
	int npca = 0;
	char str[200];
	float base;
	float new_base = 0;
	float unit_d;
	float ftemp=0;
	float* new_v0=NULL;
	float** pca_buff=NULL;	
	float** prc_data;
	prc_data = malloc(5*sizeof(float*));
	for(i=0;i<5;i++){
		prc_data[i] = malloc(flen * sizeof(float));
		for(j=0;j<flen;j++){
			prc_data[i][j] = 0;
		}
	}

	fd1 = fopen(src_path, "r");
	fd2 = fopen(tar_path, "w+");
	if(fd1==NULL || fd2==NULL){
		printf("Model open failed.\n");
		return -1;
	}	

	while( count<3 && (read = getline(&line, &len, fd1))!= -3 ){
		count++;
		// copy each line
		fwrite(line, sizeof(char), strlen(line), fd2);
		// extract elements
		temp = 0;
		d  = strtok(line, " ");
		while(d!=NULL){
			ftemp = atof(d);
			for(i=0;i<5;i++){
				if(count==1){
					prc_data[i][temp] = data[i][temp]/ftemp;
				}
				else if(count==2){
					prc_data[i][temp] = prc_data[i][temp]-ftemp;
				}
			}
			temp++;
			d = strtok(NULL, " ");
		}
	}
	if(count<3){
		printf("Error: model incomplete\n");
		return -1;
	}
	count=0;
	npca = atoi(line);
	if(npca<0 || npca>20){
		printf("Error: num_pca incorrect\n");
		return -1;
	}
	new_v0 = malloc(npca*sizeof(float));
	for(j=0;j<npca;j++){
		new_v0[j] = 0;
	}
	pca_buff = malloc(5*sizeof(float*));
	for(i=0;i<5;i++){
		pca_buff[i] = malloc(npca*sizeof(float));
		for(j=0;j<npca;j++){
			pca_buff[i][j] = 0;
		}
	}
	for(i=0;i<npca;i++){
		read = getline(&line, &len, fd1);
		if(read==-1){
			printf("V_pca incomplete\n");
			return -1;
		}
		fwrite(line, sizeof(char), strlen(line), fd2);
		// project with the average location 
		temp=0;
		d  = strtok(line, " ");
		while(d!=NULL && temp<flen){
			ftemp = atof(d);
			//printf("[%f %f]", ftemp, res_buff[temp]);
			for(j=0;j<5;j++){
				pca_buff[j][i] += prc_data[j][temp]*ftemp;
			}
			temp++;
			d = strtok(NULL, " ");
		}
	}
	
	while( count<3 && (read = getline(&line, &len, fd1))!= -3 ){
		count++;
		// copy each line
		if (count==1){
			printf("get v_0:%s\n", line);
		}
		else if (count==2){
			base = atof(line);
			printf("get base %f\n", base);
		}
		else if (count==3){
			unit_d = atof(line);
			printf("get unit_d %f\n", unit_d);
		}
	}
	if(count<3){
		printf("Error: model incomplete\n");
		return -1;
	}
	
	// calculate the new v_0 and variance from prcessed data in pca_buff[5][npca]
	if(DEBUG){
		for(i=0;i<5;i++){
			for(j=0;j<npca;j++){
				printf("[%f]", pca_buff[i][j]);
			}
			printf("\n");
		}
	}
	for(i=0;i<5;i++){
		for(j=0;j<npca;j++){
			new_v0[j]+=pca_buff[i][j]/5;
		}
	}
	// write new v0
	temp = 0;
	offset = 0;
	for(i=0;i<npca;i++){
		temp = sprintf(str+offset, "%f ", new_v0[i]);
		if(temp<0){
			printf("Buffering error\n");
			return -1;
		}
		offset+=temp;
	}
	sprintf(str+offset, "\n");
	fwrite(str, sizeof(char), strlen(str), fd2);
	// calculate base distance
	for(i=0;i<5;i++){
		for(j=0;j<npca;j++){
			ftemp = (new_v0[j] - pca_buff[i][j])/5;
			new_base += (ftemp>=0) ? ftemp:-ftemp;
		}
	}
	sprintf(str, "%f\n", new_base);
	fwrite(str, sizeof(char), strlen(str), fd2);
	// adjust unit_d based on the ratio of bases		
	unit_d = unit_d * new_base/base;	
	sprintf(str, "%f\n", unit_d);
	fwrite(str, sizeof(char), strlen(str), fd2);


	printf("Update complete.\n");
	fclose(fd1);
	fclose(fd2);
	for(i=0;i<5;i++){
		free(prc_data[i]);
	}
	free(prc_data);
	if(pca_buff!=NULL){
		for(i=0;i<5;i++){
			free(pca_buff[i]);
		}
		free(pca_buff);
	}
	if(new_v0!=NULL){
		free(new_v0);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	//int local_size=PRC_SIZE, wsize = WLEN;
	unsigned int buffer_AIN_2[BUFFER_SIZE]={0};
	char model_path[30] = PATH_TO_MODEL;
	char temp_model_path[30] = PATH_TO_TEMP_MODEL;
	int vol = 85;
	int label = -1;
	int i=0, j=0;
	float res_pre = 0;
	float res_old = 0;
	float res_new = 0;
	int flag=0;
	float output_buff[PRC_SIZE] = {0};
	float recal_buff[5][PRC_SIZE] = {0};
	float tone_output[5][TONE_PRC_SIZE] = {0};
	if(argc>=2){
		vol = atoi(argv[1]);
		if(vol==0){
			flag=1; //for warmup
		}
		if(argc==3){
			label = atoi(argv[2]);
		}
	}
	else{
		printf("Usage:./US_online [volume*] [label]\n");
	}

	BBB_init(buffer_AIN_2);

	/* Start playback */
	//Playback_record(flag, label, vol, buffer_AIN_2, output_buff);
	
	for(i=0;i<5;i++){
		Presence_record(100, buffer_AIN_2, tone_output[i]);
		sleep(1);
	}
	res_pre = Presence_detect(tone_output, 10, 0.008, 0.008);
	printf("*** [1]Presence decision:%f\n", res_pre);
	
	/*Load model if existed and then estimate occupancy*/
	//Playback_record(flag, label, vol, buffer_AIN_2, output_buff);
	Playback_record(flag, label, vol, buffer_AIN_2, output_buff);
	for(i=0;i<5;i++){
		Playback_record(flag, label, vol, buffer_AIN_2, output_buff);
		sleep(1);
		res_old += Occ_est(model_path, FSIZE, output_buff, 0.5);
		res_new += Occ_est(temp_model_path, FSIZE, output_buff, 0.5);
	}
	printf("*** [OLD]Occupancy: %f\n", res_old/5);
	printf("*** [NEW]Occupancy: %f\n", res_new/5);
		

	//if empty, test again
	if (res_pre==0){
		for(i=0;i<5;i++){
			Presence_record(100, buffer_AIN_2, tone_output[i]);
			sleep(1);
		}
		res_pre = Presence_detect(tone_output, 10, 0.008, 0.008);
		printf("*** [2]Presence decision:%f\n", res_pre);
		// recalibrate if both are detected as empty 
		if (res_pre==0){
			// Recalibration 
			printf("*** Start recalibration ...\n");
			// collect some new data
			for(i=0;i<5;i++){
				Playback_record(flag, label, vol, buffer_AIN_2, recal_buff[i]);
				sleep(1);
			}
			Recal(model_path, temp_model_path, FSIZE, recal_buff);
		}
	}


	/*clean up*/
	BBB_free();
	return 0;
}


