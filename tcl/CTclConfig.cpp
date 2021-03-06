#include <std_c++.h>
#include <CTclApp.h>
#include <CTclAppCommand.h>
#include <CConfig.h>
#include <CTclConfig.h>

#include "CTclConfig.k"

#define CTclConfigAppCommand(N) \
class N : public CTclAppCommand { \
 private: \
  CTclConfig *config_; \
\
 public: \
  N(CTclConfig *config) : \
    CTclAppCommand(config, #N), config_(config) { \
  } \
\
 protected: \
  bool proc(int argc, const char **argv); \
};

CTclConfigAppCommand(CTclConfigSetApplication)
CTclConfigAppCommand(CTclConfigSetNameValue)

int
main(int argc, char **argv)
{
  CTclConfig app(argc, argv);
}

CTclConfig::
CTclConfig(int argc, char **argv) :
 CTclApp(argc, argv), config_(NULL)
{
  init();
}

CTclConfig::
~CTclConfig()
{
  delete config_;
}

void
CTclConfig::
addCommands()
{
  new CTclConfigSetApplication(this);
  new CTclConfigSetNameValue(this);
}

string
CTclConfig::
getTclStr()
{
  return CTclConfig_TK;
}

void
CTclConfig::
setApplication(const string &name)
{
  if (config_ != NULL)
    delete config_;

  config_ = new CConfig(name);

  eval("CTclConfigClearItems");

  addGroups("", "");
}

void
CTclConfig::
setNameValue(const string &name, const string &value)
{
  config_->setValue(name, "", value);

  config_->save();
}

void
CTclConfig::
addGroups(const string &path, const string &groupName)
{
  vector<string> groupNames, groupPaths;

  if (! config_->getGroups(path, groupNames, groupPaths))
    return;

  addSections(path, groupName);

  vector<string>::iterator pgroupNames1 = groupNames.begin();
  vector<string>::iterator pgroupNames2 = groupNames.end  ();
  vector<string>::iterator pgroupPaths1 = groupPaths.begin();
  vector<string>::iterator pgroupPaths2 = groupPaths.end  ();

  for ( ; pgroupNames1 != pgroupNames2 && pgroupPaths1 != pgroupPaths2;
        ++pgroupNames1, ++pgroupPaths1) {
    eval("CTclConfigAddGroup {%s} {%s}",
         (*pgroupPaths1).c_str(), (*pgroupNames1).c_str());

    addGroups(*pgroupPaths1, *pgroupNames1);

    eval("CTclConfigCloseGroup {%s}", (*pgroupPaths1).c_str());
  }
}

void
CTclConfig::
addSections(const string &path, const string &groupName)
{
  vector<string> sections;

  if (! config_->getSections(path, sections))
    return;

  string path1;

  vector<string>::iterator psection1 = sections.begin();
  vector<string>::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1) {
    if (*psection1 != "") {
      string section1 = "[" + *psection1 + "]";

      if (path != "")
        path1 = path + "/" + section1;
      else
        path1 = section1;

      eval("CTclConfigAddSection {%s} {%s} {%s}",
           path1.c_str(), groupName.c_str(), section1.c_str());

      eval("CTclConfigCloseGroup {%s}", path1.c_str());
    }

    addSectionNameValues(path, groupName, *psection1);
  }
}

void
CTclConfig::
addSectionNameValues(const string &path, const string &groupName,
                     const string &sectionName)
{
  vector<string> names;

  if (! config_->getSectionValueNames(path, sectionName, names))
    return;

  string path1, path2, value;

  vector<string>::iterator pname1 = names.begin();
  vector<string>::iterator pname2 = names.end  ();

  for ( ; pname1 != pname2; ++pname1) {
    config_->getValue(path, sectionName, *pname1, value);

    if (sectionName != "") {
      string section1 = "[" + sectionName + "]";

      if (path != "")
        path1 = path + "/" + section1;
      else
        path1 = section1;

      path1 += "/";
    }
    else {
      if (path != "")
        path1 = path + "/";
      else
        path1 = "";
    }

    path1 += (*pname1);

    path2 = path1;

    path1 += "_Value";

    eval("CTclConfigAddSectionNameValue {%s} {%s} {%s} {%s} {%s} {%s}",
         path1.c_str(), groupName.c_str(), sectionName.c_str(),
         (*pname1).c_str(), value.c_str(), path2.c_str());
  }
}

bool
CTclConfigSetApplication::
proc(int, const char **argv)
{
  config_->setApplication((char *) argv[1]);

  return true;
}

bool
CTclConfigSetNameValue::
proc(int, const char **argv)
{
  config_->setNameValue((char *) argv[1], (char *) argv[2]);

  return true;
}
