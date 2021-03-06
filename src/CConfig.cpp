#include <CConfig.h>
#include <CConfigFile.h>

#include <CDir.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CThrow.h>
#include <CFuncs.h>

CConfig::
CConfig(const std::string &name) :
 name_(name)
{
  path_ = CDir::getHome();

  path_ += "/.cconfig";

  CFile file1(path_);

  if (! file1.exists())
    CDir::makeDir(path_);
  else {
    if (! file1.isDirectory())
      CTHROW(path_ + " exists and is not a directory");
  }

  path_ += "/" + name_;

  CFile file2(path_);

  if (! file2.exists())
    CDir::makeDir(path_);
  else {
    if (! file2.isDirectory())
      CTHROW(path_ + " exists and is not a directory");
  }
}

CConfig::
~CConfig()
{
  for (auto &group : groups_)
    delete group;
}

void
CConfig::
save()
{
  for (auto &group : groups_)
    group->save();
}

bool
CConfig::
getValue(const std::string &path, std::string &value) const
{
  return getValue(path, "", value);
}

bool
CConfig::
getValue(const std::string &path, bool *value) const
{
  return getValue(path, "", value);
}

bool
CConfig::
getValue(const std::string &path, int *value) const
{
  return getValue(path, "", value);
}

bool
CConfig::
getValue(const std::string &path, double *value) const
{
  return getValue(path, "", value);
}

bool
CConfig::
getValue(const std::string &path, std::vector<std::string> &values) const
{
  return getValue(path, "", values);
}

bool
CConfig::
getValue(const std::string &path, const std::string &sectionName, std::string &value) const
{
  std::string::size_type pos = path.rfind("/");

  std::string groupPath = "";
  std::string name      = "";

  if (pos != std::string::npos) {
    groupPath = path.substr(0, pos);
    name      = path.substr(pos + 1);
  }
  else
    name = path;

  return getValue(groupPath, sectionName, name, value);
}

bool
CConfig::
getValue(const std::string &path, const std::string &sectionName, bool *value) const
{
  std::string svalue;

  if (! getValue(path, sectionName, svalue))
    return false;

  if (! CStrUtil::toBool(svalue, value))
    return false;

  return true;
}

bool
CConfig::
getValue(const std::string &path, const std::string &sectionName, int *value) const
{
  std::string svalue;

  if (! getValue(path, sectionName, svalue))
    return false;

  if (! CStrUtil::toInteger(svalue, value))
    return false;

  return true;
}

bool
CConfig::
getValue(const std::string &path, const std::string &sectionName, double *value) const
{
  std::string svalue;

  if (! getValue(path, sectionName, svalue))
    return false;

  if (! CStrUtil::toReal(svalue, value))
    return false;

  return true;
}

bool
CConfig::
getValue(const std::string &path, const std::string &sectionName,
         std::vector<std::string> &values) const
{
  std::string svalue;

  if (! getValue(path, sectionName, svalue))
    return false;

  CStrUtil::addWords(svalue, values);

  return true;
}

bool
CConfig::
setValue(const std::string &path, const std::string &value)
{
  return setValue(path, "", value);
}

bool
CConfig::
setValue(const std::string &path, bool value)
{
  return setValue(path, "", value);
}

bool
CConfig::
setValue(const std::string &path, int value)
{
  return setValue(path, "", value);
}

bool
CConfig::
setValue(const std::string &path, double value)
{
  return setValue(path, "", value);
}

bool
CConfig::
setValue(const std::string &path, const std::vector<std::string> &values)
{
  return setValue(path, "", values);
}

bool
CConfig::
setValue(const std::string &path, const std::string &sectionName, const std::string &value)
{
  std::string::size_type pos = path.rfind("/");

  std::string groupPath = "";
  std::string name      = "";

  if (pos != std::string::npos) {
    groupPath = path.substr(0, pos);
    name      = path.substr(pos + 1);
  }
  else
    name = path;

  return setValue(groupPath, sectionName, name, value);
}

bool
CConfig::
setValue(const std::string &path, const std::string &sectionName, bool value)
{
  return setValue(path, sectionName, CStrUtil::toString(value));
}

bool
CConfig::
setValue(const std::string &path, const std::string &sectionName, int value)
{
  return setValue(path, sectionName, CStrUtil::toString(value));
}

bool
CConfig::
setValue(const std::string &path, const std::string &sectionName, double value)
{
  return setValue(path, sectionName, CStrUtil::toString(value));
}

bool
CConfig::
setValue(const std::string &path, const std::string &sectionName,
         const std::vector<std::string> &values)
{
  return setValue(path, sectionName, CStrUtil::toString(values));
}

bool
CConfig::
getValue(const std::string &groupPath, const std::string &sectionName,
         const std::string &name, std::string &value) const
{
  CConfigGroup *group = getGroupFromPath(groupPath);

  return group->getValue(sectionName, name, value);
}

bool
CConfig::
setValue(const std::string &groupPath, const std::string &sectionName,
         const std::string &name, const std::string &value)
{
  CConfigGroup *group = getGroupFromPath(groupPath);

  return group->setValue(sectionName, name, value);
}

bool
CConfig::
getGroups(const std::string &groupPath, std::vector<std::string> &groupNames,
          std::vector<std::string> &groupPaths)
{
  if (groupPath == "") {
    CDir dir(path_);

    std::vector<std::string> files;

    dir.getFilenames(files);

    std::vector<std::string>::iterator pf1 = files.begin();
    std::vector<std::string>::iterator pf2 = files.end  ();

    for ( ; pf1 != pf2; ++pf1) {
      CFile file(path_ + "/" + *pf1);

      if (file.isDirectory()) {
        groupNames.push_back(*pf1);
        groupPaths.push_back(*pf1);
      }
    }
  }
  else {
    CConfigGroup *group = getGroupFromPath(groupPath);

    group->getGroups(groupNames, groupPaths);
  }

  return true;
}

bool
CConfig::
getSections(const std::string &groupPath, std::vector<std::string> &sections)
{
  CConfigGroup *group = getGroupFromPath(groupPath);

  group->getSections(sections);

  return true;
}

bool
CConfig::
getSectionValueNames(const std::string &groupPath, const std::string &sectionName,
                     std::vector<std::string> &names)
{
  CConfigGroup *group = getGroupFromPath(groupPath);

  group->getSectionValueNames(sectionName, names);

  return true;
}

CConfigGroup *
CConfig::
getGroupFromPath(const std::string &groupPath) const
{
  CConfigGroup *group = nullptr;

  std::string groupPath1 = groupPath;

  std::string::size_type pos = groupPath1.find("/");

  while (pos != std::string::npos) {
    std::string lgroupPath = groupPath1.substr(0, pos);
    std::string rgroupPath = groupPath1.substr(pos + 1);

    if (! group)
      group = getGroup(lgroupPath);
    else
      group = group->getGroup(lgroupPath);

    groupPath1 = rgroupPath;

    pos = groupPath1.find("/");
  }

  if (! group)
    group = getGroup(groupPath1);
  else
    group = group->getGroup(groupPath1);

  return group;
}

CConfigGroup *
CConfig::
getGroup(const std::string &groupName) const
{
  for (auto &group : groups_) {
    if (group->isName(groupName))
      return group;
  }

  CConfig *th = const_cast<CConfig *>(this);

  CConfigGroup *group = new CConfigGroup(path_, groupName, groupName);

  th->groups_.push_back(group);

  return group;
}

//-------------

CConfigGroup::
CConfigGroup(const std::string &path, const std::string &name, const std::string &hierName) :
 path_(path), name_(name), hierName_(hierName)
{
  path_ += "/" + name_;

  CFile file(path_);

  if (! file.exists())
    CDir::makeDir(path_);
  else {
    if (! file.isDirectory())
      CTHROW(path_ + " exists and is not a directory");
  }

  file_ = path_ + "/config";

  config_file_ = new CConfigFile();

  config_file_->readFile(file_);
}

CConfigGroup::
~CConfigGroup()
{
  config_file_->writeFile(file_);

  for (auto &group : groups_)
    delete group;

  for (auto &section : sections_)
    delete section;

  delete config_file_;
}

void
CConfigGroup::
save()
{
  config_file_->writeFile(file_);

  for (auto &group : groups_)
    group->save();
}

CConfigGroup *
CConfigGroup::
getGroup(const std::string &groupName) const
{
  for (auto &group : groups_) {
    if (group->isName(groupName))
      return group;
  }

  CConfigGroup *th = const_cast<CConfigGroup *>(this);

  CConfigGroup *group = new CConfigGroup(path_, groupName, hierName_ + "/" + groupName);

  th->groups_.push_back(group);

  return group;
}

CConfigSection *
CConfigGroup::
getSection(const std::string &sectionName) const
{
  for (auto &section : sections_) {
    if (section->isName(sectionName))
      return section;
  }

  CConfigGroup *th = const_cast<CConfigGroup *>(this);

  CConfigSection *section = new CConfigSection(sectionName, config_file_);

  th->sections_.push_back(section);

  return section;
}

bool
CConfigGroup::
getValue(const std::string &sectionName, const std::string &name, std::string &value) const
{
  CConfigSection *section = getSection(sectionName);

  return section->getValue(name, value);
}

bool
CConfigGroup::
setValue(const std::string &sectionName, const std::string &name, const std::string &value)
{
  CConfigSection *section = getSection(sectionName);

  return section->setValue(name, value);
}

bool
CConfigGroup::
getGroups(std::vector<std::string> &groupNames, std::vector<std::string> &groupPaths)
{
  CDir dir(path_);

  std::vector<std::string> files;

  dir.getFilenames(files);

  std::vector<std::string>::iterator pf1 = files.begin();
  std::vector<std::string>::iterator pf2 = files.end  ();

  for ( ; pf1 != pf2; ++pf1) {
    CFile file(path_ + "/" + *pf1);

    if (file.isDirectory()) {
      groupNames.push_back(*pf1);
      groupPaths.push_back(hierName_ + "/" + *pf1);
    }
  }

  return true;
}

bool
CConfigGroup::
getSections(std::vector<std::string> &sections)
{
  return config_file_->getSections(sections);
}

bool
CConfigGroup::
getSectionValueNames(const std::string &sectionName, std::vector<std::string> &names)
{
  CConfigSection *section = getSection(sectionName);

  return section->getValueNames(names);
}

//-------------

CConfigSection::
CConfigSection(const std::string &name, CConfigFile *config_file) :
 name_(name), config_file_(config_file)
{
}

CConfigSection::
~CConfigSection()
{
}

bool
CConfigSection::
getValue(const std::string &name, std::string &value) const
{
  return config_file_->getValue(name_, name, value);
}

bool
CConfigSection::
setValue(const std::string &name, const std::string &value)
{
  return config_file_->setValue(name_, name, value);
}

bool
CConfigSection::
getValueNames(std::vector<std::string> &names) const
{
  return config_file_->getSectionValueNames(name_, names);
}
