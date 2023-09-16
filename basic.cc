#include "gtkmm/enums.h"
#include "gtkmm/grid.h"
#include "gtkmm/label.h"
#include "gtkmm/range.h"
#include "gtkmm/scale.h"
#include "gtkmm/widget.h"
#include "pangomm/attributes.h"
#include "pangomm/attrlist.h"
#include "sigc++/functors/mem_fun.h"
#include <gtkmm.h>
#include <iostream>
#include <ostream>
#include <string>

class MyGrid : public Gtk::Grid
{
public:
  MyGrid();

protected:
  bool update_color(Gtk::ScrollType t, double d, uint what_color);
  void copy_color();
  static std::string char_to_hexstr(unsigned char c);
  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;

  Gtk::Label label;
  Gtk::Scale scale_r;
  Gtk::Scale scale_g;
  Gtk::Scale scale_b;
  Gtk::Button copy_button;

  Gtk::Label label_red;
  Gtk::Label label_green;
  Gtk::Label label_blue;

};

std::string MyGrid::char_to_hexstr(unsigned char c) {
  char buffer[23];
  sprintf(buffer, "%X", c);
  return std::string(buffer);

}

void MyGrid::copy_color(){
  Gtk::Widget::get_clipboard()->set_text("0x" + char_to_hexstr(r) + char_to_hexstr(g) + char_to_hexstr(b));
}

bool MyGrid::update_color(Gtk::ScrollType t, double d, uint what_color){
  if(what_color == 0) {
    r = d;
  } else if (what_color == 1){
    g = d;
  } else if (what_color == 2){
    b = d;
  }
  Pango::AttrList attrll;
  Pango::Attribute color = Pango::Attribute::create_attr_foreground(r << 8, g << 8, b << 8); 
  attrll.insert(color);
  label.set_attributes(attrll);
  return false;
}

MyGrid::MyGrid(): label("This is how the Color looks!"), copy_button("Copy HexVal"), label_red("Red"), label_blue("Blue"), label_green("Green"){
  scale_r.set_range(0, 255);
  scale_g.set_range(0, 255);
  scale_b.set_range(0, 255);

  scale_r.signal_change_value().connect(sigc::bind(sigc::mem_fun(*this, &MyGrid::update_color), 0), false);
  scale_g.signal_change_value().connect(sigc::bind(sigc::mem_fun(*this, &MyGrid::update_color), 1), false);
  scale_b.signal_change_value().connect(sigc::bind(sigc::mem_fun(*this, &MyGrid::update_color), 2), false);

  copy_button.signal_clicked().connect(sigc::mem_fun(*this, &MyGrid::copy_color));

  set_column_homogeneous(true);
  set_row_homogeneous(true);


  Pango::AttrList attrll;
  Pango::Attribute color = Pango::Attribute::create_attr_foreground(r << 8, g << 8, b << 8); 
  attrll.insert(color);
  label.set_attributes(attrll);


  attach(label_red, 0, 1);
  attach(label_green, 1, 1);
  attach(label_blue, 2, 1);

  attach(label, 0, 0);
  attach(copy_button, 1, 0);
  attach(scale_r, 0, 2);
  attach(scale_g, 1, 2);
  attach(scale_b, 2, 2);
}



class MainWindow : public Gtk::Window
{
public:
  MainWindow();
protected:
  MyGrid grid;
};

MainWindow::MainWindow()
{
  set_title("Basic Color Slider App");
  set_name("Color Slider App");
  set_default_size(200, 200);
  set_child(grid);
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.backspace-phill.color-slider");

  return app->make_window_and_run<MainWindow>(argc, argv);
}
