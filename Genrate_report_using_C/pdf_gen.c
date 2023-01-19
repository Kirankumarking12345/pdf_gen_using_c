#include <hpdf.h>
#include <stdio.h>

int main(int argc, char** argv) 
{
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Image image,image2;

    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Error: Unable to create PDF object\n");
        return 1;
    }

    page = HPDF_AddPage(pdf);
    image = HPDF_LoadJpegImageFromFile(pdf, "image.jpg");
    image2 = HPDF_LoadPngImageFromFile(pdf,"image.png");
    HPDF_Page_DrawImage(page, image, 10, 10, HPDF_Image_GetWidth(image), HPDF_Image_GetHeight(image));
    //HPDF_Page_DrawImage(page, image2, 0, 0, HPDF_Image_GetWidth(image2), HPDF_Image_GetHeight(image2));

    HPDF_SaveToFile(pdf, "output.pdf");

    /* Clean up */
    HPDF_Free(pdf);

    return 0;
}
