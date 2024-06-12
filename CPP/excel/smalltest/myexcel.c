/* ************************************************************************
> File Name:     myexcel.c
> Author:        niu0217
> Created Time:  Wed 12 Jun 2024 10:18:02 PM CST
> Description:   
 ************************************************************************/

#include <xlsxwriter.h>

int main()
{
  lxw_workbook *workbook = workbook_new("myexcel.xlsx");
  lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
  int row = 0;
  int col = 0;

  worksheet_write_string(worksheet, row, col, "Hello me!", NULL);

  return workbook_close(workbook);
}