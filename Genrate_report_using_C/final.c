#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"

#define graph
#define table

const char *page_title = "Ayati Devices";
HPDF_Doc pdf;
char fname[256];
HPDF_Page page;
HPDF_Font def_font;
HPDF_REAL tw, st;
HPDF_REAL height;
HPDF_REAL width;

jmp_buf env;

#ifdef HPDF_DLL
void __stdcall
#else
void
#endif
    error_handler(HPDF_STATUS error_no,
                  HPDF_STATUS detail_no,
                  void *user_data)
{
    printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
           (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

void Page_headings(const char *study_type)
{
    /* Print the title of the page (with positioning center). */
    def_font = HPDF_GetFont(pdf, "Courier-Bold", NULL);
    HPDF_Page_SetFontAndSize(page, def_font, 30);

    tw = HPDF_Page_TextWidth(page, page_title);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, page_title);
    HPDF_Page_EndText(page);

    /* output subtitle. */
    HPDF_Page_BeginText(page);
    HPDF_Font font1 = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font1, 10);
    HPDF_Page_TextOut(page, 50, height - 90, "Report no :");
    HPDF_Page_TextOut(page, 450, height - 90, "Report no :");
    HPDF_Page_TextOut(page, 50, height - 110, "Patient ID :");
    HPDF_Page_TextOut(page, 250, height - 110, "Name :");
    HPDF_Page_TextOut(page, 450, height - 110, "Age :");

    const char *study = study_type;
    def_font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
    HPDF_Page_SetFontAndSize(page, def_font, 20);
    st = HPDF_Page_TextWidth(page, study);
    HPDF_Page_TextOut(page, (width - st) / 2, height - 150, study);

    HPDF_Page_SetFontAndSize(page, font1, 12);
    HPDF_Page_TextOut(page, 50, height - 190, "BRIEF EXAMINATION NOTES:");

    HPDF_Page_EndText(page);

    HPDF_Page_SetLineWidth(page, 1);
    HPDF_Page_Rectangle(page, 50, height - 300, 500, 100);
    HPDF_Page_Stroke(page);

    HPDF_Page_BeginText(page);
    HPDF_Font fontimghed = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
    HPDF_Page_SetFontAndSize(page, fontimghed, 12);
    HPDF_Page_TextOut(page, width - 180, height - 350, "PERFUSION IMAGE");
    HPDF_Page_TextOut(page, width - 170, height - 580, "NORMAL IMAGE");
    HPDF_Page_EndText(page);
}

int main(int argc, char **argv)
{
    

    HPDF_Image image1, image2;

    // strcpy(fname, argv[0]);
    strcat(fname, "Report.pdf");

    pdf = HPDF_New(error_handler, NULL);
    if (!pdf)
    {
        printf("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env))
    {
        HPDF_Free(pdf);
        return 1;
    }

    /* Add a new page object. */
    page = HPDF_AddPage(pdf);

    height = HPDF_Page_GetHeight(page);
    width = HPDF_Page_GetWidth(page);

    /***********************    Graph   ********************/

    HPDF_REAL x_graph, y_graph;
    int i;

#ifdef graph
    /* data points */
    HPDF_REAL points[] = {3, 4, 3, 8, 5, 6, 3, 5, 6, 7, 7, 8, 9, 8, 5, 12, 10, 8, 7, 10,
                          8, 6, 4, 5, 6};

    int graph_height = 300;
    int graph_width = 350;
    int mov_up = 150;
    int mov_rig = 50;
    /* set the line width */
    HPDF_Page_SetLineWidth(page, 0.5);

    /* set the starting point */
    x_graph = mov_rig;
    y_graph = mov_up;

    /* move the current point to the starting point */
    HPDF_Page_MoveTo(page, x_graph, y_graph);

    /* draw the line */
    for (i = 0; i < sizeof(points) / sizeof(HPDF_REAL); i++)
    {
        x_graph += 10;
        y_graph = points[i] * 20 + mov_up;
        HPDF_Page_LineTo(page, x_graph, y_graph);
    }

    /* stroke the line */
    HPDF_Page_Stroke(page);

    /* draw x-axis */
    HPDF_Page_MoveTo(page, mov_rig, mov_up);
    HPDF_Page_LineTo(page, graph_width - mov_rig, mov_up);
    HPDF_Page_Stroke(page);

    /* draw y-axis */
    HPDF_Page_MoveTo(page, mov_rig, mov_up);
    HPDF_Page_LineTo(page, mov_rig, graph_height + mov_up);
    HPDF_Page_Stroke(page);

    /* add labels for the axes */
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", NULL), 6);
    HPDF_Page_TextOut(page, graph_width - mov_rig - 15, mov_up - 6, "X-axis");
    HPDF_Page_TextOut(page, mov_rig, graph_height + mov_up + 6, "Y-axis");
    HPDF_Page_EndText(page);

#endif

    Page_headings("Perfusion Study");

#ifdef table
    /*********************  table   **************/

    /* Table data */
    char *data[3][7] = {
        {"Rol No", "Mean ", "Std dev", "%", "Median", "Min", "Max"},
        {"1", "35", "10", "Ref", "117", "110", "130"},
        {"2", "28", "11", "75%", "117", "110", "130"}};

    /* Table dimensions */
    float row_height = 20;
    float col_width = 40;

    /* Table position */
    float x = 50;
    float y = 100;

    HPDF_Page_SetLineWidth(page, 1);
    /* Draw table */
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 7; j++)
        {

            if (i == 0)
            {
                HPDF_Font fontt = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
                HPDF_Page_SetFontAndSize(page, fontt, 10);
                HPDF_Page_Rectangle(page, x + (j * col_width) - 8, y - (i * row_height) - 2, col_width, row_height);
                HPDF_Page_Stroke(page);

                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, x + (j * col_width)-4, y - (i * row_height), data[i][j]);
                HPDF_Page_EndText(page);
                fontt = HPDF_GetFont(pdf, "Helvetica", NULL);
                HPDF_Page_SetFontAndSize(page, fontt, 10);
            }
            else
            {
                HPDF_Page_Rectangle(page, x + (j * col_width) - 8, y - (i * row_height) - 2, col_width, row_height);
                HPDF_Page_Stroke(page);

                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, x + (j * col_width), y - (i * row_height), data[i][j]);
                HPDF_Page_EndText(page);
            }
        }
    }

    /* Draw table border */
    // HPDF_Page_Rectangle(page, x, y - (3 * row_height), col_width * 3, row_height * 3);
    // HPDF_Page_Stroke(page);

    /***********************    Table END       *******************/
#endif

    image1 = HPDF_LoadJpegImageFromFile(pdf, "lsci.jpg");
    image2 = HPDF_LoadJpegImageFromFile(pdf, "normal.jpg");

    for (i = 0; i < 2; i++)
    {
        switch (i)
        {
        case 0:
            HPDF_Page_DrawImage(page, image2, 370, 50, HPDF_Image_GetWidth(image2) / 10, HPDF_Image_GetHeight(image2) / 6);
            HPDF_Page_DrawImage(page, image1, 370, 285, HPDF_Image_GetWidth(image1) / 10, HPDF_Image_GetHeight(image1) / 6);
            break;
        case 1:
            page = HPDF_AddPage(pdf);
            Page_headings("PORH Study");
            HPDF_Page_DrawImage(page, image2, 370, 50, HPDF_Image_GetWidth(image2) / 10, HPDF_Image_GetHeight(image2) / 6);
            HPDF_Page_DrawImage(page, image1, 370, 285, HPDF_Image_GetWidth(image1) / 10, HPDF_Image_GetHeight(image1) / 6);
            break;
        
        }
    }

    HPDF_SaveToFile(pdf, fname);
    /* clean up */
    HPDF_Free(pdf);

    return 0;
}