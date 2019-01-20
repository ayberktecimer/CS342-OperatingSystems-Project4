#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>  
FILE *fp;
//XFILE *fp_out;
int values[1000];
int sorted_values[1000];
char input_file_name[250];
int headpos;
int num_of_req;
int highest_cylinder_pos;
//FCFS 
int num_of_moves_FCFS;
int FCFS_visited[1000];
//SSTF
int num_of_moves_SSTF;
int SSTF_visited[1000];

//SCAN
int num_of_moves_SCAN;

//CSCAN
int num_of_moves_CSCAN;

//LOOK
int num_of_moves_LOOK;

//C-LOOK
int num_of_moves_CLOOK;

int sorting_method( const void* a, const void* b )
{
	if( *(int*)a == *(int*)b ) return 0;
	return *(int*)a < *(int*)b ? -1 : 1;
}

int copy_array(int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		sorted_values[i] = arr[i];
	}
	return 1;
}
void read_from_rand_num(){
	
	srand((unsigned int)time(NULL));

	memset(values, 0, sizeof values);
	memset(sorted_values, 0, sizeof sorted_values);
	for (int i = 0; i < num_of_req; i++)
	{
		values[i] = rand() % 5000;
		//Xprintf("random values %d : %d\n", i, values[i]);
	}
	copy_array(values,num_of_req);
	qsort( sorted_values, num_of_req, sizeof(int), sorting_method );
	
}
int read_from_input_file(){
	memset(values, 0, sizeof values);
	memset(sorted_values, 0, sizeof sorted_values);

	fp = fopen(input_file_name, "r"); // read mode

	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	while( !feof(fp) ) {
		int current_index = -1;
		int current_value= -1;
		fscanf( fp , "%d %d\n" , &current_index , &current_value);
		values[cnt] = current_value;
		//Xprintf("current_index: %d values %d : %d\n", current_index, cnt, values[cnt]);
		cnt++;
	}

	// numara degis
	copy_array(values,num_of_req);
	qsort( sorted_values, num_of_req, sizeof(int), sorting_method );

	return 1;
}

void applyFCFS(){
	//Xprintf("\n**********BEGINNING OF FCFS**********\n");
	memset(FCFS_visited, 0, sizeof FCFS_visited);
	num_of_moves_FCFS = 0;
	int curr_place = headpos;
	// numara degis
	for (int i = 0; i < num_of_req; i++)
	{
		for(int j = i+1; j< num_of_req; j++){
			if(values[j]== values[i]){
				FCFS_visited[j] = 1;
			}
		}
		//printf("visited i : %d\n",FCFS_visited[i] );
		if(FCFS_visited[i] == 0){
			//Xprintf("curr : %d values i: %d\n",curr_place,values[i] );
			num_of_moves_FCFS += abs(curr_place - values[i]);
			//printf("sum: %d \n", num_of_moves_FCFS);
			curr_place = values[i];
		}

	}
	//Xprintf("num of moves FCFS: %d\n", num_of_moves_FCFS);
	printf("FCFS: %d\n", num_of_moves_FCFS);
	//Xprintf("**********END OF FCFS**********\n");

	//Xfprintf(fp_out, "FCFS: %d\n", num_of_moves_FCFS);
	
}

void applySSTF(){
	//Xprintf("\n**********BEGINNING OF SSTF**********\n");
	num_of_moves_SSTF = 0;
	int curr_place = headpos;
	int req_index = -1;
	// numara degis
	for( int i = 0 ; i < num_of_req ; i++ )
		SSTF_visited[i] = 0;
	// numara degis
	for (int  i = 0; i < num_of_req; i++)
	{
		int min_dist = 5000;
		int min_value = 5000;
		for (int j = 0; j < num_of_req; j++)
		{
			if(min_dist >= abs(curr_place - sorted_values[j]) && SSTF_visited[j] == 0){
				min_dist = abs(curr_place - sorted_values[j]);
				min_value = sorted_values[j];
				req_index = j;
			}		
		}
		SSTF_visited[req_index] = 1;
		num_of_moves_SSTF += min_dist;
		curr_place = min_value;
		//Xprintf("min_value: %d min_dist: %d\n", min_value, min_dist);
	}
	//Xprintf("num_of_moves_SSTF : %d\n", num_of_moves_SSTF);
	printf("SSTF: %d\n", num_of_moves_SSTF);
	//Xprintf("**********END OF SSTF**********\n");

	//Xfprintf(fp_out, "SSTF: %d\n", num_of_moves_SSTF);

}


int find_current_index(){

	int value = -1;
	if(headpos > sorted_values[num_of_req-1]){
		return num_of_req-1;
	}else if(headpos == sorted_values[num_of_req-1]){
		int j = num_of_req-1;
		int current_index = num_of_req-1;
		while(j >= 1 && sorted_values[j]== sorted_values[j-1]){
			j--;
			current_index--;
		}
		return current_index;
	}else if(headpos < sorted_values[0]){
		return 0;
	}else{
		for(int i = 0; i < num_of_req-1; i++){
			//printf("sorted i+1 : %d\n",sorted_values[i+1] );
			if(headpos == sorted_values[i])
				value = sorted_values[i];
			if(headpos > sorted_values[i] && headpos < sorted_values[i+1]){

				value = sorted_values[i+1];
			}
		}
		//Xprintf("value: %d\n",value );
		for(int i = 0 ; i < num_of_req;i++){
			if(value == sorted_values[i]){
				//Xprintf("index : %d\n",i );
				return i;
			}
		}
	}

	//printf("hata var!!\n");
	return -1;
}

void applySCAN(){
	//Xprintf("\n**********BEGINNING OF SCAN**********\n");
	num_of_moves_SCAN = 0;

	int current_index = find_current_index();
	//Xprintf("current index: %d\n", current_index);
	int current_value = headpos;
	if(headpos > sorted_values[num_of_req-1]){
		num_of_moves_SCAN += highest_cylinder_pos - headpos;
		num_of_moves_SCAN += highest_cylinder_pos - sorted_values[0];
	}else if(headpos <= sorted_values[0]){
		num_of_moves_SCAN += sorted_values[num_of_req-1] - headpos;
	}else{
			// saga gidis buraya -1 ekledin
		int cnt = 0;
		for (int i = current_index; i < num_of_req; i++)
		{
			cnt++;
			//Xprintf("bu kadar artti sag: %d degerler: %d %d\n",abs(current_value - sorted_values[i]),current_value,sorted_values[i]);
			num_of_moves_SCAN += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}
		//Xprintf("count: %d\n",cnt );
	//printf("bu kadar artti 199 : %d\n",abs(highest_cylinder_pos - current_value));

	//eger sola dönüs yoksa en sona kadar gitme 
		if(current_index > 0){
			//Xprintf("en saga gidis %d %d\n", highest_cylinder_pos, current_value);

			num_of_moves_SCAN += abs(highest_cylinder_pos - current_value);
		}

		current_value = highest_cylinder_pos;

		for(int i = current_index-1; i >= 0; i--){
			//Xprintf("bu kadar artti sol: %d degerler: %d %d\n",abs(current_value - sorted_values[i]), current_value, sorted_values[i]);
			num_of_moves_SCAN += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}
	}

	//Xprintf("num_of_moves_SCAN: %d\n", num_of_moves_SCAN);
	printf("SCAN: %d\n", num_of_moves_SCAN);
	//printf("current_index: %d\n", current_index);
	//Xprintf("**********END OF SCAN**********\n");
	//Xfprintf(fp_out, "SCAN: %d\n", num_of_moves_SCAN);

}

/*
1 98
2 183
3 37
4 122
5 14
6 124
7 65
8 67
*/
void applyCSCAN(){
	//Xprintf("\n**********BEGINNING OF CSCAN**********\n");
	num_of_moves_CSCAN = 0;

	int current_index = find_current_index();
	int current_value = headpos;
	//Xprintf("current index: %d\n", current_index);
	// saga gidis
	if(headpos > sorted_values[num_of_req-1]){
		num_of_moves_CSCAN += highest_cylinder_pos- headpos;
		num_of_moves_CSCAN += highest_cylinder_pos;
		num_of_moves_CSCAN += sorted_values[num_of_req-1];
	}else if (headpos <= sorted_values[0])
	{
		num_of_moves_CSCAN = sorted_values[num_of_req-1] - headpos;
	}else{
		for (int i = current_index; i < num_of_req; i++)
		{
			//Xprintf("bu kadar artti sag: %d - %d %d\n",current_value,sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_CSCAN += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}

	// en saga gidis 199 degis test amaclı
		if(current_index > 0){
			
			//Xprintf("en saga gidis: %d - %d %d\n",current_value,highest_cylinder_pos, abs(highest_cylinder_pos-current_value) );
			num_of_moves_CSCAN += abs(highest_cylinder_pos - current_value);
		//en sagdan en sola gidis
			//Xprintf("en sagdan en sola gidis: %d - %d %d\n",highest_cylinder_pos,0, abs(highest_cylinder_pos) );
			num_of_moves_CSCAN += abs(highest_cylinder_pos-0);
		}

		current_value = 0;

		for (int i = 0; i < current_index; i++)
		{
			//Xprintf("bu kadar artti sol: %d - %d %d\n",current_value, sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_CSCAN += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}
	}

	//printf("num_of_moves_CSCAN: %d\n", num_of_moves_CSCAN);
	printf("C-SCAN: %d\n", num_of_moves_CSCAN);
	//printf("current_index: %d\n", current_index);
	//Xprintf("**********END OF CSCAN**********\n");
	//Xfprintf(fp_out, "C-SCAN: %d\n", num_of_moves_CSCAN);
}

void applyLOOK(){
	//Xprintf("\n**********BEGINNING OF LOOK**********\n");
	num_of_moves_LOOK = 0;

	int current_index = find_current_index();
	int current_value = headpos;
	if(headpos >= sorted_values[num_of_req-1]){
		num_of_moves_LOOK += headpos -sorted_values[0];
	}else if(headpos <= sorted_values[0]){
		num_of_moves_LOOK += sorted_values[num_of_req-1] - headpos;
	}else{
		//Xprintf("current index: %d\n", current_index);
	//saga gidis
		for (int i = current_index; i < num_of_req; i++)
		{
			//Xprintf("bu kadar artti sag: %d - %d %d\n",current_value,sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_LOOK += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}

	//sağdan sola gidip ilk elemanı bulma 199 degis
		for (int i = current_index-1; i >=0; i--)
		{
			//Xprintf("bu kadar artti sol: %d - %d %d\n",current_value,sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_LOOK += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}
	}
	//Xprintf("num_of_moves_LOOK: %d\n", num_of_moves_LOOK);
	printf("LOOK: %d\n", num_of_moves_LOOK);
	//printf("current_index: %d\n", current_index);
	//Xprintf("**********END OF LOOK**********\n");
	//Xfprintf(fp_out, "LOOK: %d\n", num_of_moves_LOOK);

}
// 0 2 3 5 8 11
void applyCLOOK(){
	//Xprintf("\n**********BEGINNING OF C-LOOK**********\n");
	num_of_moves_CLOOK = 0;

	int current_index= find_current_index();
	//Xprintf("current index: %d\n", current_index);
	int current_value= headpos;
	//saga gidis
	if(headpos > sorted_values[num_of_req-1]){
		num_of_moves_CLOOK += headpos - sorted_values[0];
		num_of_moves_CLOOK += sorted_values[num_of_req-1] - sorted_values[0];
	}else if(headpos <= sorted_values[0]){
		num_of_moves_CLOOK+= sorted_values[num_of_req-1] - headpos;
	}else{
		for (int i = current_index; i < num_of_req; i++)
		{
			//Xprintf("bu kadar artti sag: %d - %d %d\n",current_value,sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_CLOOK += abs(current_value - sorted_values[i]);
			current_value = sorted_values[i];
		}

	//sagdan ilk degere dönüs
		if(current_index > 0){
			
			//Xprintf("bu kadar artti sagdan sola: %d - %d %d\n",current_value,sorted_values[0], abs(current_value - sorted_values[0]));
			num_of_moves_CLOOK += abs(current_value- sorted_values[0]);
			current_value = sorted_values[0];
		}

	//en soldan saga doğru arama
		for (int i = 1; i < current_index; i++)
		{
			//Xprintf("bu kadar artti soldan saga: %d - %d %d\n",current_value,sorted_values[i], abs(current_value - sorted_values[i]));
			num_of_moves_CLOOK += abs(current_value- sorted_values[i]);
			current_value = sorted_values[i];
		}
	}

	//Xprintf("num_of_moves_CLOOK: %d\n", num_of_moves_CLOOK);
	printf("C-LOOK: %d\n", num_of_moves_CLOOK);
	//printf("current_index: %d\n", current_index);
	//Xprintf("**********END OF CLOOK**********\n");
	//Xfprintf(fp_out, "C-LOOK: %d\n", num_of_moves_CLOOK);
}
void statistics(){
	int num_of_tries = 100;
	double num_of_moves_FCFS_mean=0;
	double num_of_moves_SSTF_mean=0;
	double num_of_moves_SCAN_mean=0;
	double num_of_moves_CSCAN_mean=0;
	double num_of_moves_LOOK_mean=0;
	double num_of_moves_CLOOK_mean=0;

	double FCFS_dev=0;
	double SSTF_dev=0;
	double SCAN_dev=0;
	double CSCAN_dev=0;
	double LOOK_dev=0;
	double CLOOK_dev=0;
	double FCFS_S_arr[num_of_tries];
	memset(FCFS_S_arr, 0, sizeof FCFS_S_arr);


	int SSTF_S_arr[num_of_tries];
	memset(SSTF_S_arr, 0, sizeof SSTF_S_arr);

	int SCAN_S_arr[num_of_tries];
	memset(SCAN_S_arr, 0, sizeof SCAN_S_arr);


	int CSCAN_S_arr[num_of_tries];
	memset(CSCAN_S_arr, 0, sizeof CSCAN_S_arr);

	int LOOK_S_arr[num_of_tries];
	memset(LOOK_S_arr, 0, sizeof LOOK_S_arr);


	int CLOOK_S_arr[num_of_tries];
	memset(CLOOK_S_arr, 0, sizeof CLOOK_S_arr);

	for (int i = 0; i < num_of_tries; i++)
	{
		srand((unsigned int)time(NULL));
		headpos = rand() % 5000;
		printf("headpos: %d\n", headpos);
		read_from_rand_num();
		sleep(1);
		applyFCFS();
		applySSTF();
		applySCAN();
		applyCSCAN();
		applyLOOK();
		applyCLOOK();

		FCFS_S_arr[i] = num_of_moves_FCFS;
		SSTF_S_arr[i] = num_of_moves_SSTF;
		SCAN_S_arr[i] = num_of_moves_SCAN;
		CSCAN_S_arr[i] = num_of_moves_CSCAN;
		LOOK_S_arr[i] = num_of_moves_LOOK;
		CLOOK_S_arr[i] = num_of_moves_CLOOK;

		num_of_moves_FCFS_mean += num_of_moves_FCFS;
		num_of_moves_SSTF_mean += num_of_moves_SSTF;
		num_of_moves_SCAN_mean += num_of_moves_SCAN;
		num_of_moves_CSCAN_mean += num_of_moves_CSCAN;
		num_of_moves_LOOK_mean += num_of_moves_LOOK;
		num_of_moves_CLOOK_mean += num_of_moves_CLOOK;
	}

	num_of_moves_FCFS_mean = num_of_moves_FCFS_mean/num_of_tries;
	num_of_moves_SCAN_mean = num_of_moves_SCAN_mean/num_of_tries;
	num_of_moves_SSTF_mean = num_of_moves_SSTF_mean/num_of_tries;
	num_of_moves_CSCAN_mean = num_of_moves_CSCAN_mean/num_of_tries;
	num_of_moves_LOOK_mean=num_of_moves_LOOK_mean/num_of_tries;
	num_of_moves_CLOOK_mean = num_of_moves_CLOOK_mean/num_of_tries;

	for (int i = 0; i < num_of_tries; i++)
	{
		FCFS_dev += pow(FCFS_S_arr[i] - num_of_moves_FCFS_mean,2);
		SSTF_dev += pow(SSTF_S_arr[i] - num_of_moves_SSTF_mean,2);
		SCAN_dev += pow(SCAN_S_arr[i] - num_of_moves_SCAN,2);
		CSCAN_dev += pow(CSCAN_S_arr[i]- num_of_moves_CSCAN_mean,2);
		LOOK_dev += pow(LOOK_S_arr[i]- num_of_moves_LOOK_mean,2);
		CLOOK_dev += pow(CLOOK_S_arr[i] - num_of_moves_CLOOK_mean,2);

	}
	FCFS_dev = FCFS_dev / num_of_tries;
	SSTF_dev = SSTF_dev / num_of_tries;
	SCAN_dev = SCAN_dev / num_of_tries;
	CSCAN_dev = CSCAN_dev / num_of_tries;
	LOOK_dev = LOOK_dev / num_of_tries;
	CLOOK_dev = CLOOK_dev / num_of_tries;

	FCFS_dev= sqrt(FCFS_dev);
	SSTF_dev = sqrt(SSTF_dev);
	SCAN_dev = sqrt(SCAN_dev);
	CSCAN_dev = sqrt(CSCAN_dev);
	LOOK_dev = sqrt(LOOK_dev);
	CLOOK_dev = sqrt(CLOOK_dev);

	printf("FCFS Mean: %f\n", num_of_moves_FCFS_mean);
	printf("SSTF Mean: %f\n", num_of_moves_SSTF_mean);
	printf("SCAN Mean: %f\n", num_of_moves_SCAN_mean);
	printf("C-SCAN Mean: %f\n", num_of_moves_CSCAN_mean);
	printf("LOOK Mean: %f\n", num_of_moves_LOOK_mean);
	printf("C-LOOK Mean: %f\n", num_of_moves_CLOOK_mean);

	printf("FCFS Standard Deviation: %f\n",FCFS_dev );
	printf("SSTF Standard Deviation: %f\n", SSTF_dev);
	printf("SCAN Standard Deviation: %f\n", SCAN_dev);
	printf("C-SCAN Standard Deviation: %f\n", CSCAN_dev);
	printf("LOOK Standard Deviation: %f\n",LOOK_dev );
	printf("C-LOOK Standard Deviation: %f\n", CLOOK_dev);
}
int main(int argc, char *argv[])
{
	//updatele bu 2 degeri DIKKAT
	num_of_req = 1000;
	highest_cylinder_pos =4999;

	if(argc == 1){
		perror("No headpos is provided\n");
		exit(EXIT_FAILURE);
	}else if(argc ==2){
		headpos = atof(argv[1]);
		read_from_rand_num();
	}else if(argc ==3){
		headpos = atof(argv[1]);
		strcpy( input_file_name , argv[2] );
		if(headpos >= 5000 || headpos < 0){
			perror("Headpos is not in given interval 0-4999\n");
			exit(EXIT_FAILURE);
		}
		read_from_input_file();
	}
	//Xprintf("argc: %d\n", argc);
	//Xprintf("headpos: %d\n", headpos);
	for (int i = 0; i < argc; i++)
	{
		//Xprintf("argv values %d : %s\n", i,argv[i]);
	}

	// numara degis
	for (int i = 0; i < num_of_req; i++)
	{
		//Xprintf("sorted values %d : %d\n", i,sorted_values[i]);
	}

	//Xfp_out = fopen("output", "w");

	applyFCFS();
	applySSTF();
	applySCAN();
	applyCSCAN();
	applyLOOK();
	applyCLOOK();
	//statistics();
	return 0;
}