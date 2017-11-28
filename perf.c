#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int convolve2D(unsigned char* in, unsigned char* out, int dataSizeX, int dataSizeY,
                    float* kernel, int kernelSizeX, int kernelSizeY)
{
    int i, j, m, n, mm, nn;
    int kCenterX, kCenterY;                         // center index of kernel
    float sum;                                      // temp accumulation buffer
    int rowIndex, colIndex;

    FILE *fp;

    // check validity of params
    if(!in || !out || !kernel) return 0;
    if(dataSizeX <= 0 || kernelSizeX <= 0) return 1;

    // find center position of kernel (half of kernel size)
    kCenterX = kernelSizeX / 2;
    kCenterY = kernelSizeY / 2;

    printf("dataSizeY: %i\n", dataSizeY);
    printf("dataSizeX: %i\n", dataSizeX);

    for(i=0; i < dataSizeY; ++i)                // rows
    {
        for(j=0; j < dataSizeX; ++j)            // columns
        {
            sum = 0;                            // init to 0 before sum
            for(m=0; m < kernelSizeY; ++m)      // kernel rows
            {
                mm = kernelSizeY - 1 - m;       // row index of flipped kernel

                for(n=0; n < kernelSizeX; ++n)  // kernel columns
                {
                    nn = kernelSizeX - 1 - n;   // column index of flipped kernel

                    // index of input signal, used for checking boundary
                    rowIndex = i + m - kCenterY;
                    colIndex = j + n - kCenterX;

                    int volatile t = nn * rowIndex * colIndex - (kCenterY*kCenterX);

                    // ignore input samples which are out of bound
                    if(rowIndex >= 0 && rowIndex < dataSizeY && colIndex >= 0 && colIndex < dataSizeX) {

                        unsigned char *in_sub = (unsigned char *)malloc(sizeof(unsigned char));
                        in_sub = &in[dataSizeX * rowIndex + colIndex];
                        float *ker_sub = (float *)malloc(sizeof(float));
                        ker_sub = &kernel[kernelSizeX * mm + nn];

                        sum += *in_sub * *ker_sub;
                    }
                }
            }
            out[dataSizeX * i + j] = (unsigned char)((float)fabs(sum) + 0.5f);
        }
        if( i % 2 == 0 ) {
            fp = fopen("output.data", "w");
            fprintf(fp, "%i\t", i);
            fclose(fp);
        }
    }

    printf("Done convolving\n");
    return 1;
}

int loadImage(const char *fname, int x, int y, unsigned char *data) {
  if(!fname || !data)
      return 0;

  FILE *fp;
  if((fp = fopen(fname, "r")) == NULL)
  {
      printf("Cannot open %s.\n", fname);
      return 0;
  }

  // read pixel data
  fread(data, 1, x*y, fp);
  fclose(fp);

  return 1;
}

int main(void) {
  const char *fname = "image.data";
  const int x = 15204;
  const int y = 4620;

  float kernel[25] = { 1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
                       6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f };

  // float kernel[9] = { 1/256.0f,  4/256.0f,  6/256.0f,
  //                      4/256.0f, 16/256.0f, 24/256.0f,
  //                      6/256.0f, 24/256.0f, 36/256.0f };

  // float kernel[100] = { 1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
  //                       4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
  //                       1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f };

  unsigned char *in = (unsigned char *)malloc(sizeof(unsigned char)*(x*y));
  unsigned char *out = (unsigned char *)malloc(sizeof(unsigned char)*(x*y));

  loadImage(fname, x, y, in);

  convolve2D(in, out, x, y, kernel, 5, 5);
  // convolve2D(in, out, x, y, kernel, 10, 10);

  return 0;
}
