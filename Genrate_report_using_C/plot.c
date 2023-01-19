#include <hpdf.h>

int main(int argc, char **argv)
{
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_REAL x, y;
    int i;

    /* data points */
    HPDF_REAL data[] = {3, 4, 3, 8, 5, 6, 3, 5, 6, 7, 7, 8, 9, 8, 5, 12};

    /* create a new PDF document */
    pdf = HPDF_New(NULL, NULL);

    /* add a new page object */
    page = HPDF_AddPage(pdf);

    /* set the width and height of the page */
    HPDF_Page_SetWidth(page, 500);
    HPDF_Page_SetHeight(page, 700);

    int graph_height = 300;
    int graph_width = 200;
    /* set the line width */
    HPDF_Page_SetLineWidth(page, 0.5);

    /* set the starting point */
    x = 30;
    y = 100;

    /* move the current point to the starting point */
    HPDF_Page_MoveTo(page, x, y);

    /* draw the line */
    for (i = 0; i < sizeof(data) / sizeof(HPDF_REAL); i++) {
        x += 10;
        y = data[i]*20 + 100;
        HPDF_Page_LineTo(page, x, y);
    }

    /* stroke the line */
    HPDF_Page_Stroke(page);

    /* draw x-axis */
    HPDF_Page_MoveTo(page, 30, 100);
    HPDF_Page_LineTo(page, graph_width - 30, 100);
    HPDF_Page_Stroke(page);

    /* draw y-axis */
    HPDF_Page_MoveTo(page, 30, 100);
    //
    HPDF_Page_LineTo(page, 30, graph_height + 100);
    HPDF_Page_Stroke(page);

    /* add labels for the axes */
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 5);
    HPDF_Page_TextOut(page, graph_width - 30, 100 - 5, "X-axis");
    HPDF_Page_TextOut(page, 30 - 15, graph_height + 100 + 5, "Y-axis");
    HPDF_Page_EndText(page);

    /* save the document to a file */
    HPDF_SaveToFile(pdf, "graph.pdf");

    /* clean up */
    HPDF_Free(pdf);

    return 0;
}
