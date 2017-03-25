#include <std_c++.h>
#include <CConfig/CConfig.h>

int
main(int argc, char **argv)
{
  CConfig config("test");

  string toolbar_color;
  string toolbar_clock_color;

  config.getValue("toolbar"      , "color", toolbar_color);
  config.getValue("toolbar/clock", "color", toolbar_clock_color);

  cout << toolbar_color       << endl;
  cout << toolbar_clock_color << endl;

  config.setValue("toolbar"      , "color", "red"  );
  config.setValue("toolbar/clock", "color", "green");

  config.getValue("toolbar"      , "color", toolbar_color);
  config.getValue("toolbar/clock", "color", toolbar_clock_color);

  cout << toolbar_color       << endl;
  cout << toolbar_clock_color << endl;
}
