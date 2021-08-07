#include "flow_text.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

extern "C" {
#include <ncurses.h>
}


struct Item {
  char bullet;
  std::string text;
};

typedef std::vector<Item> Items;


struct FormattedItem {
  char bullet;
  std::vector<std::string> text;
};

typedef std::vector<FormattedItem> FormattedItems;

int main(int argc, char* argv[]) {
  const int ncolumns = 30;

  Items items;
  for (int i = 0; i < 10; ++i) {
    items.push_back({'+', "Dit is een item"});
    items.push_back({'*', "Dit is nog een item"});
    items.push_back({'+', "Dit is nog een item maar dan een met een hele lange tekst die waarschijnlijk wel over meerdere regels zal gaan."});
  }

  // Initialise and start ncurses
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  int nrow,ncol;
  getmaxyx(stdscr,nrow,ncol);
  int selected = 0;


  FormattedItems fitems;
  for (auto p = items.begin(); p != items.end(); ++p) {
    fitems.push_back({p->bullet, flow_text(p->text, ncol-2)});
  }

  while (true) {
    clear();
    int row = 0;
    int item = 0;
    for (auto p = fitems.begin(); p != fitems.end(); ++p, ++item) {
      std::string bullet{"  "};
      bullet[0] = p->bullet;
      if (item == selected) attron(A_REVERSE);
      mvprintw(row, 0, bullet.c_str());
      for (auto q = p->text.begin(); q != p->text.end(); ++q) {
        mvprintw(row, 2, q->c_str());
        row++;
      }
      if (item == selected) attroff(A_REVERSE);
    }
    refresh();

    int ch = getch();
    if (ch == KEY_F(1)) break;
    if (ch == KEY_RESIZE) {
      //TODO: check if ncol has changed and only then reflow the text
      getmaxyx(stdscr,nrow,ncol);
      fitems.clear();
      for (auto p = items.begin(); p != items.end(); ++p) {
        fitems.push_back({p->bullet, flow_text(p->text, ncol-2)});
      }
    }
    if (ch == KEY_DOWN) {
      if (selected < fitems.size()-1) ++selected;
    }
    if (ch == KEY_UP) {
      if (selected > 0) --selected;
    }
  }
  endwin();
  return 0;
}

