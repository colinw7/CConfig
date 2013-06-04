#ifndef CConfigFile_H
#define CConfigFile_H

#include <string>
#include <vector>
#include <list>

class CFile;
class CConfigFileSection;
class CConfigFileNameVal;

class CConfigFile {
  typedef std::list<CConfigFileSection *> CConfigFileSectionList;

 private:
  CConfigFileSectionList sections_;

 public:
  CConfigFile();
 ~CConfigFile();

  bool readFile (const std::string &fileName);
  bool writeFile(const std::string &fileName);

  bool getValue(const std::string &section, const std::string &name, std::string &value);
  bool getValue(const std::string &section, const std::string &name, int &value);
  bool getValue(const std::string &section, const std::string &name, double &value);

  bool setValue(const std::string &section, const std::string &name, const std::string &value);
  bool setValue(const std::string &section, const std::string &name, int value);
  bool setValue(const std::string &section, const std::string &name, double value);

  bool getSections(std::vector<std::string> &sections);
  bool getSectionValueNames(const std::string &section, std::vector<std::string> &names);

 private:
  CConfigFileSection *addSection(const std::string &name);
  CConfigFileSection *getSection(const std::string &name);
};

class CConfigFileSection {
  typedef std::list<CConfigFileNameVal *> CConfigFileNameValList;

 private:
  std::string            name_;
  CConfigFileNameValList name_values_;

 public:
  CConfigFileSection(const std::string &name);
 ~CConfigFileSection();

  CConfigFileNameVal *addNameValue(const std::string &name, const std::string &value);
  CConfigFileNameVal *getNameValue(const std::string &name);

  bool writeFile(CFile *file);

  void getValueNames(std::vector<std::string> &name);

  std::string getName() { return name_; }

  bool isName(const std::string &name) { return name_ == name; }
};

class CConfigFileNameVal {
 private:
  std::string name_;
  std::string value_;

 public:
  CConfigFileNameVal(const std::string &name, const std::string &value);

  bool writeFile(CFile *file);

  std::string getName() const { return name_; }
  std::string getValue() const { return value_; }

  void setValue(const std::string &value) { value_ = value; }

  bool isName(const std::string &name) { return name_ == name; }
};

#endif
