
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

/* ----------------------------------------------------------- */
//#define BUFFER_SIZE 48000
//#define SAMPLE_SIZE 48000
// Collect for 0.3s with 192k
#define BUFFER_SIZE 57600
#define SAMPLE_SIZE 57600
//#define BUFFER_SIZE 960000
//#define SAMPLE_SIZE 960000
/* ----------------------------------------------------------- */
//#define FILTER_FILE "c_20k_23k_100ms_5fd_192kf.txt"  // File name of the chirp
#define MODEL_FILE "Model.txt"  // File name of the model
#define RAW_ENABLE	1	// Keep raw data or not
#define CLEN	0.03
// These parameters should be consistent with the generated chirp file
#define FS	192000
#define START_F	20000
#define END_F	21000
#define EX_BAND 200 //Extra bandwidth when filtering
#define PRC_SIZE 500 // Should be larger than the data size after FFT
#define PRC_WSIZE 51 // Should be larger segmented fft size
#define WLEN	CLEN*FS // Window sizes when doing FFT
#define WNUM	4 //Number of windows for gen training features

int main(int argc, char* argv[])
{
	unsigned int sample;
	int i, j, count=0, local_size=PRC_SIZE, raw_size = BUFFER_SIZE, wsize = WLEN;
	unsigned int buffer_AIN_2[BUFFER_SIZE] ={0};
	time_t rawtime;
	char data_file_name[128];
	char raw_data_name[128];
	FILE* data_file;
	FILE* filter_fd;
	char UART_PATH[30] = "/dev/ttyO5";
	char rec_buf[50] = "";
	int fd;
	int vol = -1;
	int label = -1;
	struct termios old, uart_set;	
	float local_buff[PRC_SIZE] = {0};
	float output_buff[PRC_SIZE] = {0};
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
	//input_pt->data = (float*)&input;
	//input_pt->size = &raw_size;
	//input_pt->allocatedSize = raw_size * sizeof(float);
	//input_pt->numDimensions = 1;
	//input_pt->canFreeData = false;	

	//default volume is(85)
	char vol_buff[3]="vU";
	if(argc>=2){
		vol = atoi(argv[1]);
		if(argc==3){
			label = atoi(argv[2]);
		}
	}
	else{
		printf("Usage:./US_online [volume*] [label]\n");
	}
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

	printf("Starting capture with rate %d ...\n", SAMPLE_SIZE);
	// get time
	time(&rawtime);
	/* Start playback */
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
	//if(read(fd,&rec_buf,sizeof(rec_buf)) > 0){
	//	printf("Set complete.\n");
	//}
	printf("Fire in the hole!\n");
	// Fire the chirp!
	write(fd, "f", 1);
	usleep(1000);
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&old);
	close(fd);
	
	
	/* Start capture */
	//usleep(30000); // wait for chirp to finish 
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
	BBBIO_ADCTSC_work(SAMPLE_SIZE);
	printf("Recording done.\n");

	/* If warmup then we are done*/
	if(argc>=2 && atoi(argv[1])==0){
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
	input_pt->size = &wsize;
	input_pt->allocatedSize = wsize * sizeof(float);
	input_pt->numDimensions = 1;
	input_pt->canFreeData = false;	
	for(i=0;i<WNUM;i++){
		input_pt->data = (float*)&(input[i*wsize]);
		Prep_fft(input_pt, FS, START_F-(EX_BAND/2), END_F+(EX_BAND/2), prc_pt);
		for(j=0;j<PRC_WSIZE;j++){
			output_buff[j+i*PRC_WSIZE] = local_buff[j];
			//printf("output[%d] = %f\n", j+i*PRC_WSIZE, output_buff[j+i*PRC_WSIZE]);
		}
	}
	//Prep_fft(input_pt, FS, START_F-(EX_BAND/2), END_F+(EX_BAND/2), prc_pt);	
	printf("Preprocessing done\n");

	/*LOAD MODEL*/

	// format file name
	strftime(data_file_name, sizeof(data_file_name), "data/%Y-%m-%d_%H:%M:%S-", localtime(&rawtime));
	if(argc==2 && vol!=-1){
		strcat(data_file_name, argv[1]);
	}
	else{
		strcat(data_file_name, "85");
	}
	strcpy(raw_data_name, data_file_name);
	strcat(data_file_name,".dat" );
	strcat(raw_data_name,".raw" );
	
	printf("Saving processed sound data to: %s\n",data_file_name);
	data_file = fopen(data_file_name,"w");	// open file in write mode
	/* add current time value to top of file */
	// fprintf(data_file, "%s\n", data_file_name);
	/* add label*/
	fprintf(data_file, "%d\n", label);
	
	// Write processed data, should be 410 in length for 57600 points
	for(j = 0 ; j < 204 ; j++){
		fprintf( data_file, "%f\n", output_buff[j] );
	}
	fclose(data_file);

	// Write raw data if defined
	if(RAW_ENABLE){
		FILE* temp_fd = fopen(raw_data_name,"w");	// open file in write mode
		for(j = 0 ; j < BUFFER_SIZE ; j++){
			fprintf(temp_fd, "%u\n", buffer_AIN_2[j] );
		}
		fclose(temp_fd);
	}


	iolib_free();
	return 0;
}


