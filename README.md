# ofxCanonEOS

Openframeworks addon for the Canon EOS SDK - an alternative for ofxCanon and ofxEdsdk.
OSX Only for now.

## usage (OSX)

1. [Register as canon developer & download canon sdk](https://www.didp.canon-europa.com/)
 
2. EDSDK.framework in canon sdk download doesn't seem to be working. [Download the latest EOS Utility for you camera](http://usa.canon.com/cusa/support/consumer/eos_slr_camera_systems/eos_digital_slr_cameras/). You might not have a previous EOS Utility install or a CD. You can force the installer to install EOS utility:
	1. Copy the updater app from the dmg to your desktop
	2. Right click and choose Show Package Contents…
	3. Delete the file Contents/Resources/Info.datx
	4. After that run the updater app again. You should be able to install the EOS Utility
 
3. Replace the EDSDK.framework from the SDK download by the one from EOS Utility:
	1. Show Package Contents from Application/Canon Utilities/EOS Utility/EOS Utility.app
	2. Copy Contents/Frameworks/EDSDK.framework over to YOUR_CANON_SDK_LOCATION/EDSDK/Framework (remove the previous one)

4. Copy the EDSDK folder into this directory: OF > addons > ofxCanonEOS > libs

5. Open the OF Project Generator application and build a new app, including the ofxCanonEOS addon.

6. Add DDP.framework and EDSDK.framework to your xcode project.

7. Go to Build Phases > Copy Files and add the DDP.framework and EDSDK.framework files there as well.

## Copyright and license

Copyright © 2013 Wouter Verweirder

Author: Wouter Verweirder

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.