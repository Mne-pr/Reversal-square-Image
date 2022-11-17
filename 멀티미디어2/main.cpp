#include <stdio.h>
#include <windows.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) 

int main() {
	FILE* file;		        // file pointer
	BITMAPFILEHEADER hf;	// ������� (bmp file header)
	BITMAPINFOHEADER hInfo; // ��Ʈ�� ������� (bitmap information header)
	int rwsize;	// ���� �� ����Ʈ�� (bytes per a line)
	BYTE* lpImg;		    // �Է� ������ ������ (pointer for input image data)
	BYTE* lpOutImg;	        // ��� ������ ������ (pointer for output image data)
	int x, y;

	RGBQUAD* LookUpTable;// ������̺� ������
	int sizeOfLookupTable;
	int whatType;        // �������� ����
	int boxX = 0, boxY = 0, boxWidth = 0, boxHeight = 0;// ��ĥ�� �簢�� ũ��
	int graycolor = 0; // Gray
	int rColor = 0, gColor = 0, bColor = 0; //TrueColor
	char filename[100] = "output";
	char numbertemp[10];

	// �Է� ���� ���� ����
	file = fopen("input.bmp", "rb");
	if (file == NULL) {
		printf("There is no file!!!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // ���� ��� ����
	if (hf.bfType != 0x4D42) // BMP ���� ('BM') ������ Ȯ��
		return -1;

	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // ��Ʈ�� ���� ��� ����
	printf("Image Size: (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);

	// ���� ������ �������� Ȯ��
	if (hInfo.biBitCount == 8 && hInfo.biClrUsed == 0) {
		printf("Image Type Is : GrayScale\n");
		whatType = 1;
		sizeOfLookupTable = hf.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		// 8bit �׷��̽������� ��� ������̺� ũ�� ���
	}
	else if (hInfo.biBitCount == 24) {
		printf("Image Type Is : TrueColor\n");
		whatType = 2;// TrueColor
	}
	else {
		printf("Bad File format!!\n");
		return -1;
	}

	// �ڽ� ��¿� ���� ���� �Է� ����
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
		// color table �б� ���� �޸� �Ҵ� �� �о����
		LookUpTable = (RGBQUAD*)malloc(sizeOfLookupTable);
		fread(LookUpTable, sizeof(RGBQUAD), sizeOfLookupTable/sizeof(RGBQUAD), file);

		// ����� �����͸� ���� ���� �� ����Ʈ �� ���
		rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);  // �Է� ����

		// �Է� ���� �����͸� ���� �޸� �Ҵ�
		lpImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// ���� �����͸� �Է� �������κ��� ����
		fread(lpImg, sizeof(char), rwsize * hInfo.biHeight, file);
		fclose(file);


		// ��� ���� �����͸� ���� �޸� �Ҵ�
		lpOutImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// ���� ������ �����ؼ� �ű�
		for (y = 0; y < hInfo.biHeight; y++) {
			for (x = 0; x < hInfo.biWidth; x++) {
				if ((x >= boxX) && (x <= (boxX + boxWidth)) && (y >= (hInfo.biHeight - boxHeight - boxY)) && (y <= (hInfo.biHeight - boxY))) 
				{
					lpOutImg[y * rwsize + x] = graycolor;//�־��� �׸�ĭ ������ ���� ������ ���� ä��
				}
				else {
					lpOutImg[y * rwsize + x] = 255 - lpImg[y * rwsize + x];//�����ؼ� ä��
				}
			}
		}
		break;

	case 2:
		strcat(filename, "_Truecolor");
		//���� Ʈ���÷��� �����ؾ� ��. ���� ���� ��ü�� ������
		// �簢���� ��ġ�� �ϴ� ������� ������ �غ���
		
		// ����� �����͸� ���� ���� �� ����Ʈ �� ���
		rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);  // �Է� ����

		// �Է� ���� �����͸� ���� �޸� �Ҵ�
		lpImg = (BYTE*)malloc(rwsize * hInfo.biHeight);

		// ���� �����͸� �Է� �������κ��� ����
		fread(lpImg, sizeof(char), rwsize * hInfo.biHeight, file);
		fclose(file);


		// ��� ���� �����͸� ���� �޸� �Ҵ�
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
		//������̺� �޸� ����
	}
	fwrite(lpOutImg, sizeof(char), rwsize * hInfo.biHeight, file);
	

	fclose(file);

	// �޸� ����
	free(lpOutImg);
	free(lpImg);
	printf("wait...\n");
	system(filename);
	printf("done\n");
	return 0;
}