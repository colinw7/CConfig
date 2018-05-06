class CTclConfig : public CTclApp {
 private:
  CConfig *config_;

 public:
  CTclConfig(int argc, char **argv);
 ~CTclConfig();

  void setApplication(const string &name);
  void setNameValue(const string &name, const string &value);

  void addGroups(const string &path, const string &groupName);
  void addSections(const string &path, const string &groupName);
  void addSectionNameValues(const string &path, const string &groupName,
                            const string &sectionName);

 protected:
  void addCommands();

  string getTclStr();
};
