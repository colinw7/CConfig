#include <CConfig.h>
#include <iostream>

int
main(int argc, char **argv)
{
  CConfig config("test");

  std::string toolbar_color;
  std::string toolbar_clock_color;

  config.getValue("toolbar"      , "color", toolbar_color);
  config.getValue("toolbar/clock", "color", toolbar_clock_color);

  std::cout << toolbar_color       << "\n";
  std::cout << toolbar_clock_color << "\n";

  config.setValue("toolbar"      , "color", "red"  );
  config.setValue("toolbar/clock", "color", "green");

  config.getValue("toolbar"      , "color", toolbar_color);
  config.getValue("toolbar/clock", "color", toolbar_clock_color);

  std::cout << toolbar_color       << "\n";
  std::cout << toolbar_clock_color << "\n";
}
