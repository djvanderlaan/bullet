#include "flow_text.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>


struct Item {
  char bullet;
  std::string text;
};

typedef std::vector<Item> Items;


struct FormattedItem {
  char bullet;
  std::vector<std::string> text;
  unsigned int starting_line;
};

typedef std::vector<FormattedItem> FormattedItems;

void reflow_items(FormattedItems& fitems, const Items& items, int ncol) {
  fitems.clear();
  unsigned int line = 0;
  for (auto p = items.begin(); p != items.end(); ++p) {
    auto t = flow_text(p->text, ncol-2);
    fitems.push_back({p->bullet, t, line});
    line += t.size();
  }
}

int main(int argc, char* argv[]) {
  const int ncolumns = 30;

  Items items;
  items.push_back({'+', "1 Met een flinke hoeveelheid test bla die bla die bla"});
  items.push_back({'*', "2"});
  items.push_back({'+', "3"});
  items.push_back({'+', "4 Bla die bla die bla die bla"});
  items.push_back({'+', "5"});
  items.push_back({'+', "6"});
  items.push_back({'+', "7"});
  items.push_back({'+', "8"});
  items.push_back({'+', "9 Ook met een flinke hoeveelheid tekst"});
  items.push_back({'+', "10"});
  items.push_back({'+', "11"});
  items.push_back({'+', "12"});


  int nrow = 10;
  int ncol = 10;
  unsigned int current_line = 3;
  unsigned int current_item = 4;
  std::cout << "current item = " << current_item << "\n";

  FormattedItems fitems;
  reflow_items(fitems, items, ncol);

  // Check is current item is completely on screen; and if not if it is before
  // or after
  int on_screen = 0; // 0 = yes; <0 before; >0 after
  if (fitems[current_item].starting_line < current_line) {
    on_screen = -(current_line - fitems[current_item].starting_line);
  } else if ((fitems[current_item].starting_line + fitems[current_item].text.size()) >
    (current_line + nrow)) {
    on_screen = (fitems[current_item].starting_line + fitems[current_item].text.size()) -
    (current_line + nrow) - 1; //TODO why the -1???
  }
  std::cout << "on screen = " << on_screen << "\n";
  // Scroll up or down to make sure that the current item is on screen
  current_line += on_screen;

  unsigned int first_item = 0;
  unsigned int last_item = 0;
  for (auto p = fitems.begin(); p != fitems.end(); ++p) {
    if ((p->starting_line + p->text.size()) <= current_line) ++first_item;
    if (p->starting_line >= (current_line + nrow - 1L)) {
      break;
    } else {
      ++last_item;
    }
  }
  std::cout << first_item << ", " << last_item << "\n";



  unsigned int nlines = 0;
  for (unsigned int i = first_item; i <= last_item; ++i) {
    const FormattedItem& item = fitems[i];
    int line = item.starting_line - current_line;
    for (auto p = item.text.begin(); p != item.text.end(); ++p, ++line) {
      if (line >= 0 && nlines < nrow) {
        std::cout << nlines << " ";
        std::cout << "+ " << *p << "\n";
        ++nlines;
      }
    }
  }

  return 0;
}

