#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


void init();
void menu();
long read_number();
int upload();
int create_copy();
int show_properties();


#pragma pack(push, 1)

typedef struct {
    unsigned short file_type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} File_Header;


typedef struct {
    unsigned int header_size;
    int width;
    int height;
    unsigned short num_planes;
    unsigned short bits_per_pixel;
    unsigned int compression;
    unsigned int image_size; 
    int x_ppm;
    int y_ppm;
    unsigned int colors_used;
    unsigned int important_colors;
} Info_Header;


typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;

#pragma pack(pop)

File_Header *p1;
Info_Header *p2;
FILE *p3;

char file[30];
char copy[30];

int main() {
	int option;
    init();
	while(1) {
		menu();
        option = read_number();
        if (option == 1) upload();
        else if (option == 2) create_copy();
        else if (option == 3) show_properties();
        else break;
	}
    return EXIT_SUCCESS;
}


int upload() {
    unsigned char buffer [0x1000];
    memset(buffer, 0, 0x1000);
    int n = read(0, buffer, 0x1000);
    if (n == 0)
        return EXIT_FAILURE;
    
    p3 = fopen(file, "w+");
    if (p3 == NULL)
        return EXIT_FAILURE;
    fwrite(buffer, n, 1, p3);
    fclose(p3);

    return EXIT_SUCCESS;
}


int create_copy() {
    unsigned char two_pixels;
    unsigned char index;
    unsigned char nybble;
    unsigned char *ptr;
    unsigned char color_table[256][4];
    Pixel pixel;
    int row_size;
    int array_size;
    unsigned char *pixels;

    
    p3 = fopen(file, "r");
    if (p3 == NULL)
        return EXIT_FAILURE;

    p1 = malloc(sizeof(File_Header));
    if (p1 == NULL)
        return EXIT_FAILURE;
    fread(p1, sizeof(File_Header), 1, p3);

    p2 = malloc(sizeof(Info_Header));
    if (p2 == NULL)
        return EXIT_FAILURE;
    fread(p2, sizeof(Info_Header), 1, p3);

    if (p1->file_type != 0x4d42 || !(p2->bits_per_pixel == 0x18 || p2->bits_per_pixel == 0x4))
        return EXIT_FAILURE;
    
    if (p2->compression != 0 || p2->num_planes != 1 || p2->width < 1 || p2->height < 1)
        return EXIT_FAILURE;

    if (p2->colors_used == 0 && p2->bits_per_pixel == 4)
        p2->colors_used = 1 << 4;

    fread(color_table, p2->colors_used, 4, p3);
        
    row_size = ((p2->width * 24 + 32 - 1 ) / 32) * 4;
    array_size = row_size * p2->height;
   
    pixels = (unsigned char *) malloc(array_size);
    if (pixels == NULL)
        return EXIT_FAILURE;
    memset(pixels, 0, array_size);
    
    nybble = 1;
    ptr = pixels;
    
    for (int y = 0; y < p2->height; y++) {
        if (p2->bits_per_pixel == 0x4) {
            for (int x = 0; x < p2->width/2; x++) {
                two_pixels = (unsigned char) getc(p3);
                for (int z = 0; z < 2; z++, ptr += 3) {
                    if (nybble) {
                        index = (two_pixels >> 4) & 0x0f;
                        nybble = 0;
                    } else {
                        index = two_pixels & 0x0f;
                        nybble = 1;
                    }
                    pixel.blue = color_table[index][0];
                    pixel.green = color_table[index][1];
                    pixel.red = color_table[index][2];
                    memcpy(ptr, &pixel, sizeof(pixel));
                }
            }
        } 
        else if (p2->bits_per_pixel == 0x18) {
            for (int x = 0; x < p2->width; x++, ptr += 3) {
                pixel.blue = (unsigned char) getc(p3);
                pixel.green = (unsigned char) getc(p3);
                pixel.red= (unsigned char) getc(p3);
                memcpy(ptr, &pixel, sizeof(pixel));
            }
            for (int z = p2->width * 3; z & 3; z++, ptr++)
                ptr[0] = (unsigned char) getc(p3);
        }
    }

    fclose(p3);

    p3 = fopen(copy, "w+");
    if (p3 == NULL)
        return EXIT_FAILURE;
    
    if (p2->bits_per_pixel == 0x4) {
        p1->size = sizeof(File_Header) + sizeof(Info_Header) + array_size;
        p1->offset = sizeof(File_Header) + sizeof(Info_Header);
        p2->image_size = array_size;
        p2->bits_per_pixel = 0x18;
        p2->colors_used = 0;
        p2->important_colors = 0;
    }

    fwrite(p1, sizeof(File_Header), 1, p3);
    fwrite(p2, sizeof(Info_Header), 1, p3);
    fwrite(pixels, (size_t)array_size, 1, p3);
    fclose(p3);

    free(p1);
    free(p2);
    free(pixels);
    p1 = 0;
    p2 = 0;
    pixels = 0;

    printf("\n\tThe copy is ready for distribution!");
    return EXIT_SUCCESS;
}


int show_properties() {
    p3 = fopen(copy, "r");
    if (p3 == NULL)
        return EXIT_FAILURE;

    p1 = malloc(sizeof(File_Header));
    if (p1 == NULL)
        return EXIT_FAILURE;
    fread(p1, sizeof(File_Header), 1, p3);

    p2 = malloc(sizeof(Info_Header));
    if (p2 == NULL)
        return EXIT_FAILURE;
    fread(p2, sizeof(Info_Header), 1, p3);
    
    puts("\n");
    printf("\t[0x%x, ", p1->file_type);
    printf("0x%x, ", p1->size);
    printf("0x%x, ", p1->reserved1);
    printf("0x%x, ", p1->reserved2);
    printf("0x%x, ", p1->offset);
    write(1, &stdin, 8);
    printf(", ");
    printf("0x%x, ", p2->header_size);
    printf("%d, ", p2->width);
    printf("%d, ", p2->height);
    printf("0x%x, ", p2->num_planes);
    printf("0x%x, ", p2->bits_per_pixel);
    printf("0x%x, ", p2->compression);
    printf("0x%x, ", p2->image_size);
    printf("0x%x, ", p2->x_ppm);
    printf("0x%x, ", p2->y_ppm);
    printf("0x%x, ", p2->colors_used);
    printf("0x%x]", p2->important_colors);
    puts("\n");
    
    free(p1);
    free(p2);
    p1 = 0;
    p2 = 0;

    return EXIT_SUCCESS;
}


void menu(){
    puts("\n\n ==========----- ThE Galery -----==========");
	puts("\t1. Upload");
	puts("\t2. Create Copy");
	puts("\t3. Show Properties");
	puts("\t4. Exit");
	printf("\t > ");
}


long read_number() {
    char buffer[3];
    fgets(buffer, sizeof(buffer), stdin);
    return strtol(buffer, NULL, 10);
}


void init() {
    long long uid;
    srand(time(NULL));
	setvbuf(stdin,0,_IONBF,0);
	setvbuf(stdout,0,_IONBF,0);
	setvbuf(stderr,0,_IONBF,0);
    uid = ((unsigned long)rand() << 32) | rand();
    snprintf(file, sizeof(file), "%lld", uid); 
    snprintf(copy, sizeof(copy), "%lld", uid); 
    strcat(file, "_img");
    strcat(copy, "_copy");
    puts("\n\t\t\tWelcome to ThE Galery!");
    puts("\tThe world's number one art reproductions site.");
}
