# Config dialog for wxMathPlot

This is a version of MathPlotConfigDialog created with wxFormBuilder instead of wxSmith.<br>

Its only advantage is that wxFormBuilder correctly uses the wxStaticBoxSizer construct with wxStaticBox.<br>

No problems under Windows, but under Linux, numerous warnings appear in the console.<br>

See <a href="https://docs.wxwidgets.org/3.2/classwx_static_box_sizer.html" target="_blank">wxStaticBoxSizer Class Reference</a> for more details.

You can import a wxSmith model to wxFormBuilder with the XRC import on menu file. But you need to do some operations.
- rename the MathPlotConfigdialog.wxs to MathPlotConfigdialog.xrc
- open the file with a text editor:
  - delete all string: name=""
  - replace the word <i>variable</i> by <i>name</i>
  - replace at the beginning of the file the two first lines:
  
  	\<?xml version="1.0" encoding="utf-8" ?\>
	\<wxsmith\> <br>
  by <br>
	\<?xml version="1.0" encoding="UTF-8" standalone="yes"?\>
	\<resource xmlns="http://www.wxwidgets.org/wxxrc" version="2.5.3.0"\>
      
  - replace in the last line \</wxsmith\> by \</resource\>
  - save the file then import it in wxFormBuilder

- ignore wxFormBuilder complain
- name the project MathPlotConfigBuilder and save it
- close the project

You now need to do some corrections because the import is not perfect
- In an editor, open the wxFormBuilder project
- replace all strings <i>expanded="true"</i> by <i>expanded="false"</i> (that is the general option we want)
- change the wrap property from 0 to -1
- change the auth_needed property from empty to 0
- save and re-open the project
- you now need to make some corrections manually:
  - check all wxStaticBoxSizer and re-validate the orientation
  - add all events on button, choice needed (see in the code the handler section)
  - verify all is ok ...
