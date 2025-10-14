# wxMathPlot

I have updated the mathplot component for wxWidgets 3.2.2.
I have made some improvements like :
- corrected some bugs (and probably add others lol )
- work on Linux
- rewrote some methods
- added a config window (that you can have access with the right click) - makes it easy to enable/disable plots, change colors, etc.
- add "view as bar" for XY functions
- add horizontal and vertical line
- add second Y axis management (only 2 Y axis supported)
- add log axis
- view fullscreen if plot is in a single frame
- add in the demo the samples of the original project
- lot of things ...<br>
I keep the philosophy and the structure so it was easy to update existing project (I hope !).

Please note configuration (build) symbols you may want to  define:
- **ENABLE_MP_NAMESPACE** place all MathPlot components in the MathPlot namespace (default places everything in global namespace)
- **ENABLE_MP_CONFIG** enables Lionel's advanced configuration dialog (default is old behavior)

Doxygen on codedocs (missing diagrams due to codedocs problems): <a href="https://codedocs.xyz/GitHubLionel/wxMathPlot/" target="_blank">https://codedocs.xyz/GitHubLionel/wxMathPlot/</a>

Example showing new advanced configuration dialog:
![Demo](Demo.png "Demo")  

Enjoy<br>
Lionel
