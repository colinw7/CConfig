#include <CConfigFile.h>
#include <CConfig.h>

#include <CFile.h>
#include <CStrUtil.h>

#include <iostream>
#include <algorithm>

struct CDeletePointer {
  template<class T>
  void operator()(const T *p) const {
    delete p;
  }
};

CConfigFile::
CConfigFile()
{
}

CConfigFile::
~CConfigFile()
{
  for_each(sections_.begin(), sections_.end(), CDeletePointer());
}

bool
CConfigFile::
readFile(const std::string &fileName)
{
  CFile file(fileName);

  if (! file.exists())
    return false;

  CConfigFileSection *ini_section = NULL;

  std::string line;

  while (file.readLine(line)) {
    line = CStrUtil::stripSpaces(line);

    int len = line.size();

    if (len <= 0)
      continue;

    if (line[0] == '[' && (len > 1 && line[len - 1] == ']')) {
      std::string sectionName = CStrUtil::stripSpaces(line.substr(1, len - 2));

      ini_section = addSection(sectionName);
    }
    else {
      std::string::size_type pos = line.find("=");

      if (pos != std::string::npos) {
        std::string name  = CStrUtil::stripSpaces(line.substr(0, pos));
        std::string value = CStrUtil::stripSpaces(line.substr(pos + 1));

        std::string::size_type pos1 = value.find("$(");

        if (pos1 != std::string::npos) {
          std::string lhs = value.substr(0, pos1);

          std::string envname = value.substr(pos1 + 2);

          std::string::size_type pos2 = envname.find(")");

          if (pos2 != std::string::npos) {
            std::string rhs = envname.substr(pos2 + 1);

            envname = envname.substr(0, pos2);

            std::string envval = getenv(envname.c_str());

            value = lhs + envval + rhs;
          }
        }

        if (ini_section == NULL)
          ini_section = addSection("");

        ini_section->addNameValue(name, value);
      }
      else
        std::cerr << "Invalid line " << line << std::endl;
    }
  }

  return true;
}

bool
CConfigFile::
writeFile(const std::string &fileName)
{
  CFile file(fileName);

  CConfigFileSectionList::iterator p1 = sections_.begin();
  CConfigFileSectionList::iterator p2 = sections_.end  ();

  for ( ; p1 != p2; ++p1) {
    (*p1)->writeFile(&file);
  }

  return true;
}

bool
CConfigFile::
getValue(const std::string &sectionName, const std::string &name, std::string &value)
{
  CConfigFileSection *ini_section = addSection(sectionName);

  if (ini_section == NULL)
    return false;

  CConfigFileNameVal *ini_name_value = ini_section->getNameValue(name);

  if (ini_name_value == NULL) {
    ini_section->addNameValue(name, value);

    return false;
  }

  value = ini_name_value->getValue();

  return true;
}

bool
CConfigFile::
getValue(const std::string &sectionName, const std::string &name, int &value)
{
  std::string str = CStrUtil::toString(value);

  if (! getValue(sectionName, name, str))
    return false;

  if (! CStrUtil::isInteger(str))
    return false;

  value = CStrUtil::toInteger(str);

  return true;
}

bool
CConfigFile::
getValue(const std::string &sectionName, const std::string &name, double &value)
{
  std::string str = CStrUtil::toString(value);

  if (! getValue(sectionName, name, str))
    return false;

  if (! CStrUtil::isReal(str))
    return false;

  value = CStrUtil::toReal(str);

  return true;
}

bool
CConfigFile::
setValue(const std::string &sectionName, const std::string &name, const std::string &value)
{
  CConfigFileSection *ini_section = addSection(sectionName);

  ini_section->addNameValue(name, value);

  return true;
}

bool
CConfigFile::
setValue(const std::string &sectionName, const std::string &name, int value)
{
  return setValue(sectionName, name, CStrUtil::toString(value));
}

bool
CConfigFile::
setValue(const std::string &sectionName, const std::string &name, double value)
{
  return setValue(sectionName, name, CStrUtil::toString(value));
}

bool
CConfigFile::
getSections(std::vector<std::string> &sections)
{
  CConfigFileSectionList::iterator p1 = sections_.begin();
  CConfigFileSectionList::iterator p2 = sections_.end  ();

  for ( ; p1 != p2; ++p1)
    sections.push_back((*p1)->getName());

  return true;
}

CConfigFileSection *
CConfigFile::
addSection(const std::string &name)
{
  CConfigFileSection *ini_section = getSection(name);

  if (ini_section == NULL) {
    ini_section = new CConfigFileSection(name);

    sections_.push_back(ini_section);
  }

  return ini_section;
}

CConfigFileSection *
CConfigFile::
getSection(const std::string &name)
{
  CConfigFileSectionList::iterator p1 = sections_.begin();
  CConfigFileSectionList::iterator p2 = sections_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->isName(name))
      return *p1;

  return NULL;
}

bool
CConfigFile::
getSectionValueNames(const std::string &sectionName, std::vector<std::string> &names)
{
  CConfigFileSection *section = getSection(sectionName);

  if (section == NULL)
    return false;

  section->getValueNames(names);

  return true;
}

//--------------------

CConfigFileSection::
CConfigFileSection(const std::string &name) : name_(name)
{
}

CConfigFileSection::
~CConfigFileSection()
{
  for_each(name_values_.begin(), name_values_.end(), CDeletePointer());
}

CConfigFileNameVal *
CConfigFileSection::
addNameValue(const std::string &name, const std::string &value)
{
  CConfigFileNameVal *ini_name_value = getNameValue(name);

  if (ini_name_value == NULL) {
    ini_name_value = new CConfigFileNameVal(name, value);

    name_values_.push_back(ini_name_value);
  }
  else
    ini_name_value->setValue(value);

  return ini_name_value;
}

CConfigFileNameVal *
CConfigFileSection::
getNameValue(const std::string &name)
{
  CConfigFileNameValList::iterator p1 = name_values_.begin();
  CConfigFileNameValList::iterator p2 = name_values_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->isName(name))
      return *p1;

  return NULL;
}

bool
CConfigFileSection::
writeFile(CFile *file)
{
  CConfigFileNameValList::iterator p1 = name_values_.begin();
  CConfigFileNameValList::iterator p2 = name_values_.end  ();

  if (p1 != p2) {
    if (name_ != "") {
      std::string line = "[" + name_ + "]" + "\n";

      file->write(line);
    }
  }

  for ( ; p1 != p2; ++p1)
    (*p1)->writeFile(file);

  return true;
}

void
CConfigFileSection::
getValueNames(std::vector<std::string> &names)
{
  CConfigFileNameValList::iterator p1 = name_values_.begin();
  CConfigFileNameValList::iterator p2 = name_values_.end  ();

  for ( ; p1 != p2; ++p1)
    names.push_back((*p1)->getName());
}

//-------------

CConfigFileNameVal::
CConfigFileNameVal(const std::string &name, const std::string &value) :
 name_(name), value_(value)
{
}

bool
CConfigFileNameVal::
writeFile(CFile *file)
{
  std::string line = name_ + "=" + value_ + "\n";

  file->write(line);

  return true;
}
