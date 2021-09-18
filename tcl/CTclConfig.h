class CTclConfig : public CTclApp {
 public:
  CTclConfig(int argc, char **argv);
 ~CTclConfig();

  void setApplication(const std::string &name);
  void setNameValue(const std::string &name, const std::string &value);

  void addGroups(const std::string &path, const std::string &groupName);
  void addSections(const std::string &path, const std::string &groupName);
  void addSectionNameValues(const std::string &path, const std::string &groupName,
                            const std::string &sectionName);

 protected:
  void addCommands();

  std::string getTclStr();

 private:
  CConfig *config_ { nullptr };
};
