#include <allegro.h>

//char motif_name[5][4] = { "", "SUNY", "CSTL", "RCKY", "SNWY"};

int main(int argc, char *argv[])
{

PACKFILE *file;
int l, k, j, i;
int maps[20][15][2][101];
int motif[100];

   allegro_init();

 file = pack_fopen(argv[1], "rp");

 for (l = 0; l < 100; l++)
 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 maps[i][j][k][l] = pack_igetw(file);

 for (l = 0; l < 100; l++)
    motif[l] = pack_igetw(file);

 pack_fclose(file);

 file = pack_fopen(argv[2], "wp");

 pack_iputw('B', file);
 pack_iputw('L', file);
 pack_iputw('n', file);

 for (l = 0; l < 100; l++)
 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 pack_iputw(maps[i][j][k][l], file);

 for (l = 0; l < 100; l++)
 {
// pack_fwrite(motif_name[motif[l]], sizeof(motif_name[motif[l]]), file);
   switch(motif[l])
   {
      case 1:
         pack_fwrite("SUNY", 4, file);
         break;
      case 2:
         pack_fwrite("CSTL", 4, file);
         break;
      case 3:
         pack_fwrite("RCKY", 4, file);
         break;
      case 4:
         pack_fwrite("SNWY", 4, file);
         break;
      default:
         pack_fwrite("    ", 4, file);
         break;
   }
 }

 pack_fclose(file);
 return(0);
}