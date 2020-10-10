#include <stdio.h>
#include <string.h>

struct SinhVien;

struct DiemMH {
	float toan;
	float van;
	float anh;
};

struct SinhVien {
	char ten[20];
	struct DiemMH diem;
};

void ghiFile(struct SinhVien* ds, int slsv) {

	char fName[26];
	printf("Nhap ten file: ");
	gets(fName);
	FILE* fOut = fopen(fName, "a");
	int i;
	for(i = 0; i < slsv; i++) {
		struct SinhVien sv = ds[i];
		fprintf(fOut, "%s (%f+%f+%f)/3 = %f\n", sv.ten, sv.diem.toan, sv.diem.van, sv.diem.anh, ((sv.diem.toan+sv.diem.van+sv.diem.anh)/3));
	}
	fclose(fOut);
}

void docFile(struct SinhVien *ds, int* slsv) {
	FILE* fOut = fopen("input.txt", "r");
	int i = 0;
	if(fOut) {
		for(;;) {
			struct SinhVien sv;
			fscanf(fOut, "%s %f %f %f\n", &sv.ten, &sv.diem.toan, &sv.diem.van, &sv.diem.anh);		
			ds[i++] = sv;
			if(feof(fOut)) { // thoat chuong trinh
				break;
			}
		}
	}	
	fclose(fOut);
	*slsv = i;
}

int main() {
	struct SinhVien dssv[100];
	int slsv = 0;
	docFile(dssv, &slsv);
	ghiFile(dssv, slsv);
	return 0;
}

