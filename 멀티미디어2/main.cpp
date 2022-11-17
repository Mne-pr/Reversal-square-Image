#include <stdio.h>
#include <windows.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) 

int main() {
	FILE* file;		        // file pointer
	BITMAPFILEHEADER hf;	// 파일헤더 (bmp file header)
	BITMAPINFOHEADER hInfo; // 비트맵 정보헤더 (bitmap information header)
	int rwsize;	// 라인 당 바이트수 (bytes per a line)
	BYTE* lpImg;		    // 입력 데이터 포인터 (pointer for input image data)
	BYTE* lpOutImg;	        // 출력 데이터 포인터 (pointer for output image data)
	int x, y;

	RGBQUAD* LookUpTable;// 룩업테이블 포인터
	int sizeOfLookupTable;
	int whatType;        // 파일유형 구분
	int boxX = 0, boxY = 0, boxWidth = 0, boxHeight = 0;// 색칠할 사각형 크기
	int graycolor = 0; // Gray
	int rColor = 0, gColor = 0, bColor = 0; //TrueColor
	char filename[100] = "output";
	char numbertemp[10];

	// 입력 영상 파일 오픈
	file = fopen("input.bmp", "rb");
	if (file == NULL) {
		printf("There is no file!!!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // 파일 헤더 읽음
	if (hf.bfType != 0x4D42) // BMP 포맷 ('BM') 인지를 확인
		return -1;

	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // 비트맵 정보 헤더 읽음
	printf("Image Size: (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);

	// 편집 가능한 피일인지 확인
	if (hInfo.biBitCount == 8 && hInfo.biClrUsed == 0) {
		printf("Image Type Is : GrayScale\n");
		whatType = 1;
		sizeOfLookupTable = hf.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		// 8bit 그레이스케일인 경우 룩업테이블 크기 계산
	}
	else if (hInfo.biBitCount == 24) {
		printf("Image Type Is : TrueColor\n");
		whatType = 2;// TrueColor
	}
	else {
		printf("Bad File format!!\n");
		return -1;
	}

	// 박스 출력에 대한 각종 입력 받음
	printf("x (0 ~ %d) > ",hInfo.biWidth); scanf("%d", &boxX);
	printf("y (0 ~ %d) > ",hInfo.biHeight); scanf("%d", &boxY);
	printf("width (0 ~ %d) > ",hInfo.biWidth - boxX); scanf("%d", &boxWidth);
	printf("height (0 ~ %d) > ",hInfo.biHeight - boxY); scanf("%d", &boxHeight);
	if (whatType == 1) {
		printf("gray (0 ~ 255) > "); scanf("%d", &graycolor);
	}
	else if (whatType == 2) {
		printf("Red (0 ~ 255) > "); scanf("%d", &rColor);
		printf("Green (0 ~ 255) > "); scanf("%d", &bColor);
		printf("Blue (0 ~ 255) > "); scanf("%d", &gColor);
	}

	switch (whatType) {
	case 1:
		strcat(filename, "_Grayscale");
		// color table 읽기 위해 메모리 할당 후 읽어들임
		LookUpTable = (RGBQUAD*)malloc(sizeOfLookupTable);
		fread(LookUpTable, sizeof(RGBQUAD), sizeOfLookupTable/sizeof(RGBQUAD), file);

		// 입출력 데이터를 위한 라인 당 바이트 수 계산
		rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);  // 입력 영상

		// 입력 영상 데이터를 위한 메모리 할당
		lpImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// 영상 데이터를 입력 영상으로부터 읽음
		fread(lpImg, sizeof(char), rwsize * hInfo.biHeight, file);
		fclose(file);


		// 출력 영상 데이터를 위한 메모리 할당
		lpOutImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// 영상 데이터 반전해서 옮김
		for (y = 0; y < hInfo.biHeight; y++) {
			for (x = 0; x < hInfo.biWidth; x++) {
				if ((x >= boxX) && (x <= (boxX + boxWidth)) && (y >= (hInfo.biHeight - boxHeight - boxY)) && (y <= (hInfo.biHeight - boxY))) 
				{
					lpOutImg[y * rwsize + x] = graycolor;//주어진 네모칸 정보에 따라 지정한 밝기로 채움
				}
				else {
					lpOutImg[y * rwsize + x] = 255 - lpImg[y * rwsize + x];//반전해서 채움
				}
			}
		}
		break;

	case 2:
		strcat(filename, "_Truecolor");
		//이제 트루컬러를 복사해야 함. 아직 구현 자체를 못했음
		// 사각형은 제치고 일단 원래대로 반전만 해보자
		
		// 입출력 데이터를 위한 라인 당 바이트 수 계산
		rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);  // 입력 영상

		// 입력 영상 데이터를 위한 메모리 할당
		lpImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// 영상 데이터를 입력 영상으로부터 읽음
		fread(lpImg, sizeof(char), rwsize * hInfo.biHeight, file);
		fclose(file);


		// 출력 영상 데이터를 위한 메모리 할당
		lpOutImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		for (y = 0; y < hInfo.biHeight; y++) {
			for (x = 0; x < hInfo.biWidth; x++) {
				if ((x >= boxX) && (x <= (boxX + boxWidth)) && (y >= (hInfo.biHeight - boxHeight - boxY)) && (y <= (hInfo.biHeight - boxY))) {
					lpOutImg[y * rwsize + 3 * x + 2] = rColor; // R
					lpOutImg[y * rwsize + 3 * x + 1] = gColor; // G 
					lpOutImg[y * rwsize + 3 * x + 0] = bColor; // B
					//printf("%d %d %d\n", lpOutImg[y * rwsize + 3 * x + 2], lpOutImg[y * rwsize + 3 * x + 1], lpOutImg[y * rwsize + 3 * x + 0]);
				}
				else {
					lpOutImg[y * rwsize + 3 * x + 2] = 255 - lpImg[y * rwsize + 3 * x + 2];  /* R */
					lpOutImg[y * rwsize + 3 * x + 1] = 255 - lpImg[y * rwsize + 3 * x + 1];  /* G */
					lpOutImg[y * rwsize + 3 * x + 0] = 255 - lpImg[y * rwsize + 3 * x + 0];  /* B */
				}
			}
		}
		
		hf.bfOffBits = 54; // There is no palette
		break;
	default:
		break;
	}

	strcat(filename, "_"); sprintf(numbertemp,"%d",boxX); strcat(filename, numbertemp);
	strcat(filename, "-"); sprintf(numbertemp, "%d", boxY); strcat(filename, numbertemp);
	strcat(filename, "-"); sprintf(numbertemp, "%d", boxWidth); strcat(filename, numbertemp);
	strcat(filename, "-"); sprintf(numbertemp,"%d",boxHeight);strcat(filename, numbertemp);
	strcat(filename, ".bmp");
	file = fopen(filename, "wb");

	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), file);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), file);
	if(whatType == 1){ 
		fwrite(LookUpTable,sizeof(char),sizeOfLookupTable, file);
		free(LookUpTable);
		//룩업테이블 메모리 해제
	}
	fwrite(lpOutImg, sizeof(char), rwsize * hInfo.biHeight, file);
	

	fclose(file);

	// 메모리 해제
	free(lpOutImg);
	free(lpImg);
	printf("wait...\n");
	system(filename);
	printf("done\n");
	return 0;
}