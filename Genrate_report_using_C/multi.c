#include <hpdf.h>

int main()
{
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Image image1, image2, image3;
    int i;

    pdf = HPDF_New(NULL, NULL);
    

    image1 = HPDF_LoadJpegImageFromFile(pdf, "image.jpg");
    image2 = HPDF_LoadPngImageFromFile(pdf, "image.png");
    //image3 = HPDF_LoadJpegImageFromFile(pdf, "image.jpg");

    for (i = 0; i < 3; i++) {
        page = HPDF_AddPage(pdf);
        switch (i) {
            case 0:
                HPDF_Page_DrawImage(page, image1, 30, 150, HPDF_Image_GetWidth(image1)/2, HPDF_Image_GetHeight(image1)/2);
                HPDF_Page_DrawImage(page, image2, 280, 10, HPDF_Image_GetWidth(image2)/4, HPDF_Image_GetHeight(image2)/3);
                HPDF_Page_DrawImage(page, image2, 280, 300, HPDF_Image_GetWidth(image2)/4, HPDF_Image_GetHeight(image2)/3);
                break;
            case 1:

                HPDF_Page_DrawImage(page, image2, 0, 0, HPDF_Image_GetWidth(image2)/2, HPDF_Image_GetHeight(image2)/2);
                break;
            case 2:
                //HPDF_Page_DrawImage(page, image3, 250, 50, HPDF_Image_GetWidth(image3), HPDF_Image_GetHeight(image3));
                break;
        }
    }

    HPDF_SaveToFile(pdf, "images.pdf");
    HPDF_Free(pdf);

    return 0;
}
