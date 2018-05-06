proc CTclConfigCreateWidgets { parent } {
  set frame $parent.frame

  frame $frame

  #---

  set app_frame $frame.app_frame

  frame $app_frame

  label $app_frame.label -text "Application" -width 11 -anchor w
  entry $app_frame.entry -textvariable ctcl_config_app_name

  pack $app_frame.label $app_frame.entry -side left

  pack $app_frame -side top -anchor w

  bind $app_frame.entry <Return> CTclConfigSetApplicationCmd

  #---

  set name_frame $frame.name_frame

  frame $name_frame

  label $name_frame.label -text "Name" -width 11 -anchor w
  entry $name_frame.entry -textvariable ctcl_config_edit_name

  pack $name_frame.label $name_frame.entry -side left

  pack $name_frame -side top -anchor w

  #---

  set value_frame $frame.value_frame

  frame $value_frame

  label $value_frame.label -text "Value" -width 11 -anchor w
  entry $value_frame.entry -textvariable ctcl_config_edit_value

  pack $value_frame.label $value_frame.entry -side left

  pack $value_frame -side top -anchor w

  bind $value_frame.entry <Return> CTclConfigSetNameValueCmd

  #---

  CTclConfigAddTree $frame

  #---

  pack $frame -side top -anchor w -fill both -expand true
}

#-------

proc CTclConfigSetApplicationCmd { } {
  global ctcl_config_app_name

  CTclConfigSetApplication $ctcl_config_app_name
}

proc CTclConfigSetNameValueCmd { } {
  global ctcl_config_edit_name
  global ctcl_config_edit_value

  CTclConfigSetNameValue $ctcl_config_edit_name $ctcl_config_edit_value

  global ctcl_config_list ctcl_config_edit_item

  $ctcl_config_list item configure $ctcl_config_edit_item 1 \
     -text $ctcl_config_edit_value
}

proc CTclConfigAddTree { parent } {
  tixTree $parent.tree -width 275 -height 325 \
          -options { columns 2 header true }

  global ctcl_config_tree ctcl_config_list

  set ctcl_config_tree $parent.tree
  set ctcl_config_list [$parent.tree subwidget hlist]
  
  $ctcl_config_list configure -separator        /
  $ctcl_config_list configure -selectmode       single
  $ctcl_config_list configure -indicator        true
  $ctcl_config_list configure -drawbranch       true
  $ctcl_config_list configure -itemtype         imagetext
  $ctcl_config_list configure -selectbackground white
  $ctcl_config_list configure -selectforeground black
  
  $ctcl_config_list configure -browsecmd CTclConfigBrowseProc
  $ctcl_config_list configure -indicatorcmd CTclConfigIndicatorProc

  $ctcl_config_list header create 0 -text "Name"
  $ctcl_config_list header create 1 -text "Value"
  
  pack $parent.tree -side top -fill both -expand true
}

proc CTclConfigBrowseProc { item } {
  global ctcl_config_list

  if {$item == ""} return

  set name [$ctcl_config_list info data $item]

  if {[string first "_Group_:"   $name] != -1} return
  if {[string first "_Section_:" $name] != -1} return
  
  global ctcl_config_edit_name ctcl_config_edit_value ctcl_config_edit_item

  set ctcl_config_edit_name $name

  set value [$ctcl_config_list item cget $item 1 -text]

  set ctcl_config_edit_value $value

  set ctcl_config_edit_item $item
}

proc CTclConfigIndicatorProc { item } {
  set type [tixEvent type]

  if { $type == "<Activate>" || $type == "<Return>" } {
    CTclConfigToggleGroup $item
  }
}

proc CTclConfigClearItems { } {
  global ctcl_config_list

  $ctcl_config_list delete all
}

proc CTclConfigAddGroup { path group } {
  global ctcl_config_list

  $ctcl_config_list add $path -text $group -data "_Group_:$group"

  CTclConfigOpenGroup $path
}

proc CTclConfigAddSection { path group section } {
  global ctcl_config_list

  $ctcl_config_list add $path -text $section -data "_Section_:$group:$section"

  CTclConfigOpenGroup $path
}

proc CTclConfigAddSectionNameValue { path group section name value key } {
  global ctcl_config_list

  $ctcl_config_list add $path -text $name -data "$key"

  $ctcl_config_list item create $path 1 -text $value
}

proc CTclConfigToggleGroup { path } {
  global ctcl_config_tree

  if {[$ctcl_config_tree getmode $path] == "close"} {
    CTclConfigCloseGroup $path
  } \
  else {
    CTclConfigOpenGroup $path
  }

  CTclConfigBrowseProc ""
}

proc CTclConfigOpenGroup { path } {
  global ctcl_config_tree 

  $ctcl_config_tree open $path

  $ctcl_config_tree setmode $path close
}

proc CTclConfigCloseGroup { path } {
  global ctcl_config_tree

  $ctcl_config_tree close $path

  $ctcl_config_tree setmode $path open
}

proc CTclConfigSetGroupColor { path color } {
  global ctcl_config_list

  set style [tixDisplayStyle imagetext -fg $color]

  $ctcl_config_list entryconfigure $path -style $style
}

CTclConfigCreateWidgets ""
