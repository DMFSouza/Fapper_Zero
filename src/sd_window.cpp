#include "sd_window.h"
#include "SD_MMC.h"
#include "global_flags.h"
#include "ui.h"
#include "global_flags.h"
#include "pin_config.h"
#include <vector>


static void draw_sd_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    /*If the cells are drawn...*/
    if(dsc->part == LV_PART_ITEMS) {
        uint32_t row = dsc->id /  lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Make the texts in the first cell center aligned*/
        if(row == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*In the first column align the texts to the right*/
        else if(col == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
        }

        /*MAke every 2nd row grayish*/
        if((row != 0 && row % 2) == 0) {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
    }
}
void lv_sd_window(lv_obj_t *parent)
{
  String pswdContent = "";
 // lv_obj_t *container = lv_obj_create(parent);
 // lv_obj_set_size(container, 290, 150);
 // lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 15);

  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, 1);
  SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
  if (!SD_MMC.begin("/sdcard", true))
  {
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE)
  {
    return;
  }

  if (cardType == CARD_MMC)
  {
  }
  else if (cardType == CARD_SD)
  {
  }
  else if (cardType == CARD_SDHC)
  {
  }

  File pswd = SD_MMC.open("/captive_portals/logs/logs.txt");
  if (!pswd)
  {
    return;
  }
  else if (pswd.available())
  {
    pswdContent = pswd.readString();
    pswd.close();
  }

  lv_obj_t *table = lv_table_create(parent);
  lv_table_set_col_cnt(table, 2); // Defina o número de colunas

  // Adicione cabeçalhos à tabela
  lv_table_set_cell_value(table, 0, 0, "Username");
  lv_table_set_cell_value(table, 0, 1, "Password");

  // Divida as informações em linhas
  int rowIndex = 0; // Comece com a primeira linha
  int start = 0;
  int end;

  for (int i = 0; i < pswdContent.length(); i++)
  {
    char currentChar = pswdContent[i];

    if (currentChar == '\n' || i == pswdContent.length() - 1)
    {
      end = i;
      if (i == pswdContent.length() - 1)
      {
        end = i + 1; // Inclua o último caractere
      }

      String line = pswdContent.substring(start, end);

      // Ignorar linhas em branco
      if (!line.isEmpty())
      {
        int columnIndex = rowIndex % 2;
        int rowIndexInTable = 1 + rowIndex / 2; // Comece com a segunda linha
        lv_table_set_cell_value(table, rowIndexInTable, columnIndex, line.c_str()); // Adicione à tabela
        rowIndex++;
      }

      start = i + 1; // Pule o caractere de nova linha
    }
  }

  /*Set a smaller height to the table. It'll make it scrollable*/
  lv_obj_set_height(table, 150);
  lv_obj_set_width(table, 290);
  lv_obj_center(table);

  /*Add an event callback to apply some custom drawing*/
  lv_obj_add_event_cb(table, draw_sd_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
}


void runSDWindow(lv_obj_t *parent)
{
  lv_sd_window(parent);
}

app_t sd_window = {
  .setup_func_cb = runSDWindow,
  .exit_func_cb = nullptr,
  .user_data = nullptr
};
