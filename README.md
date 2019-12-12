# ofxMotive

## Introduction

This addon allows for easy access to Optitrack Motive's API for accessing 2D and 3D optical motion capture data

### Systems

Since Motive only works in Windows, this addon has only been developed on Windows 10 with Visual Studios 2017.

### Dependencies

1. [Motive API](https://optitrack.com/products/motive/tracker/)
2. [Camera SDK](https://optitrack.com/products/camera-sdk/)

#### Installation

1. Ensure Motive has been downloaded, installed and has a valid license. By default, its program files should be installed to `C:\Program Files\OptiTrack\Motive`
2. Download the Camera SDK to a convenient location.

## How to use this addon with your project

1. Add this addon to your addons file
2. Generate your OF project with the Project Generator
3. Include the appropriate headers and libraries in your addon. There are two ways to do this:
   1. Property Sheets: The Easy Way
      1. Open the *Property Manager* under *View > Other Windows > Property Manager*
      2. For every configuration (e.g. 'Debug | x64') under your project (e.g. 'example-with -RUI'), right click on the configuration and choose *Add Existing Property Sheet*
      3. Navigate to the ofxMotive addon directory and select *ofxMotive.props* 
   2. Manually Editing Properties: The Hard Way
      1. In Visual Studio, click on your project in the Solution Explorer, the click the wrench icon at the top to edit its Properties. Set *Configuration* to *All Configurations* and *Platform* to *All Platforms*
      2. Under *Configuration Properties > C/C++ > General > Additional Include Directories*, add
         - `$(NPTRACKINGTOOLS_INC)` (this resolves into `C:\Program Files\OptiTrack\Motive\inc`)
         - `$(NP_FIRST_PARTY)\SharedLibraries\RigidBodySolver\Include` (this resolves into `\SharedLibraries\RigidBodySolver\Include`)
      3. Under *Configuration Properties > Linker > General > Additional Include Directories*, add 
         - `$(NPTRACKINGTOOLS_LIB)` (this should resolve into `C:\Program Files\OptiTrack\Motive\lib`)
         - `C:\Program Files\OptiTrack\Motive`
         - `C:\Program Files\OptiTrack\Motive\plugins\platforms`
      4. Under *Configuration Properties > Linker > Input*, add 
         - `NPTrackingToolsx64.lib` for a 64 bit architecture
4. Copy all files from the folder within this addon titled *Motive2.1 Required Libraries* into your project's executable directory (*bin*). The files that will be copied include those in the below image. It is crucial for these files to be accessible at the same file structure level as the executable itself. For more information, [see this page](https://v21.wiki.optitrack.com/index.php?title=Motive_API:_Quick_Start_Guide#Library_Files).

![](https://v21.wiki.optitrack.com/images/6/6a/MotiveAPI_RequiredLIB.png)

## Troubleshooting

##### Poco Include Errors

Sometimes, Project Generator includes the Poco addon's libraries, instead of OpenFramework's libraries. There are two ways to fix these errors:

1. Property Sheets: The Easy Way
   1. Open the *Property Manager* under *View > Other Windows > Property Manager*
   2. For every configuration (e.g. 'Debug | x64') under your project (e.g. 'example-with -RUI'), right click on the configuration and choose *Add Existing Property Sheet*
   3. Navigate to the ofxMotive addon directory and select *ofxPoco.props* 
2. Manually Editing Properties: The Hard Way
   1. In Project Properties > Configuration Properties > C/C++  > Additional Include Directories, include:
      - `$(OF_ROOT)\libs\poco\include`
   2. In Project Properties > Configuration Properties > Linker > Additional Include Directories, include:
      1. `$(OF_ROOT)\libs\poco\lib\vs`
      2. `$(OF_ROOT)\libs\poco\lib\vs\x64\Debug`
      3. `$(OF_ROOT)\libs\poco\lib\vs\x64\Release`
      4. `$(OF_ROOT)\libs\poco\lib\vs\Win32\Debug`
      5. `$(OF_ROOT)\libs\poco\lib\vs\Win32\Release`

## Reference

Useful reference materials include the wikis for both API's:

- Motive API [wiki](https://v20.wiki.optitrack.com/index.php?title=Motive_API)
- Camera SDK [wiki](https://v20.wiki.optitrack.com/index.php?title=Camera_SDK)



