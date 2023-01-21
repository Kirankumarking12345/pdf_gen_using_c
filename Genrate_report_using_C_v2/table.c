#include <hpdf.h>
#include <stdio.h>
int main() {
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Font font;

    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Error creating PDF\n");
        return 1;
    }

    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    
    
    /************************ Table data *****************************/
    char *data[3][3] = {
        {"Name", "Age", "Address"},
        {"John Smith", "35", "123 Main St."},
        {"Jane Doe", "28", "456 Park Ave."}
    };

    /* Table dimensions */
    float row_height = 20;
    float col_width = 100;

    /* Table position */
    float x = 50;
    float y = 550;

    /* Draw table */
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            HPDF_Page_BeginText(page);
            HPDF_Page_TextOut(page, x + (j * col_width), y - (i * row_height), data[i][j]);
            HPDF_Page_EndText(page);
        }
    }

    /* Draw table border */
    HPDF_Page_Rectangle(page, x, y - (3 * row_height), col_width * 3, row_height * 3);
    HPDF_Page_Stroke(page);

    /* Save and close PDF */
    HPDF_SaveToFile(pdf, "table.pdf");
    HPDF_Free(pdf);

    return 0;
}
