#include<stdio.h>
#include<stdlib.h>

#define INPUT_FILEIN "./lena_gray_512X512.bmp"
#define OUTPUT_FILEOUT "./lena_output.bmp"

void printArr_u8(unsigned char arr[],int width,int height);

void printArr_u8(unsigned char arr[],int width,int height){
	for(int i = 0;i<height;i++){
		for(int j = 0;j<width;j++){
			printf("%d ",arr[width*i+j]);
		}
		printf("\n");
	}
	printf("\n\n\n");
}

int main(){
	//read an bmp image
		FILE *fp;

		fp = fopen(INPUT_FILEIN,"rb");
		if(fp == NULL){
			printf("Cannot read file given!!");
		}
		unsigned char header[54];
		fread(header,sizeof(unsigned char),54,fp);

		unsigned char colorTable[1024]; // to store the colorTable, if it exists.

	 	int width = *(int *)&header[18];
	 	int height = *(int *)&header[22];
	 	short bitDepth = *(short *)&header[28];
		if(bitDepth <= 8)
			fread(colorTable, sizeof(unsigned char), 1024, fp);

		printf("width: %d\n",width);
		printf("height: %d\n",height);
		printf("bitDepth: %hi\n\n",bitDepth);

		int image_dimension = width*height;
		unsigned char *input_arr = (unsigned char *)malloc(image_dimension*sizeof(unsigned char));

		fread(input_arr,sizeof(unsigned char),image_dimension,fp);

//		printArr_u8(input_arr,width,height);

//		short x1 = 0;
//		short y1 = 0;
		short x2 = 3;
		short y2 = 3;
		int new_width = (width*3);
		int new_height = (height*3)-2;
		int new_dimension = new_width * new_height;
		unsigned char *result_arr = (unsigned char *)malloc(new_dimension*sizeof(unsigned char));
		printf("\nNew Width = %d\nNew Height = %d\nNumber of elements = %d\n",new_width,new_height,new_width*new_height);
		unsigned char A,B,C,D;
		int y_offset = 3;
		int x_offset = y_offset;//for assigning calculated pixel values to result_arr;

		int count = 0; // to count number of loops
		for(int i = 0;i<(height-1);i++){
			A = input_arr[width*i];
			D = input_arr[width*(i+1)];

			for(int j = 0;j<(width-1);j ++){
				//accessing 4 elements
				B = input_arr[width*i+(j+1)];
				C = input_arr[width*i+width+(j+1)];

				unsigned char result = 0;
						for(int k = 0;k<=y_offset;k++){
							for(int l = 0;l<=x_offset; l++){
								count++;
//								result = (A*(x2-l)*(y2-k)+
//										  B*(l-x1)*(y2-k)+
//								          C*(l-x1)*(k-y1)+
//										  D*(x2-l)*(k-y1))/((y2-y1)*(x2-x1));
								result = (A*(x2-l)*(y2-k)+
										 (B*l)*(y2-k)+
										 (C*l*k)+
										 D*(x2-l)*k)/(y2*x2);
								result_arr[(y_offset*i*new_width)+ //verticle tile traversal
										   (y_offset*j)+ //horizontal tile traversal
										   (k*new_width+l)] = result; // intra tile traversal
							}
						}
				A = B;
				D = C;
			}
		}

		printArr_u8(result_arr,new_width,new_height);

		FILE *fo;
		fo = fopen(OUTPUT_FILEOUT,"wb");

		fwrite(header,sizeof(unsigned char),54,fo);

		fseek( fo, 18, SEEK_SET );
		fwrite(&new_width,sizeof(unsigned char),4,fo);

		fseek(fo,22,SEEK_SET);
		fwrite(&new_height,sizeof(unsigned char),4,fo);

		fseek(fo,54,SEEK_SET);
		fwrite(colorTable,sizeof(unsigned char),1024,fo);
		fwrite(result_arr,sizeof(unsigned char),new_dimension,fo);

		fclose(fp);
		fclose(fo);

}
