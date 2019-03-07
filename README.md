# ofxMotive

This addon allows for easy access to Optitrack Motive's API for accessing 2D and 3D optical motion capture data

## Dependencies

1. Motive API
2. Camera SDK

Since Motive only works in Windows, this addon has only been developed on Windows 10 with Visual Studios 2017.

## How to use this addon with your project

1. Add this addon to your addons file
2. Generate your OF project with the Project Generator
3. In Visual Studio, click on your project in the Solution Explorer, the click the wrench icon at the top to edit its Properties. Set *Configuration* to *All Configurations* and *Platform* to *All Platforms*
4. Under *Configuration Properties > C/C++ > General > Additional Include Directories*, add
   - `$(NPTRACKINGTOOLS_INC)` (this resolves into `C:\Program Files\OptiTrack\Motive\inc`)
   - `$(NP_FIRST_PARTY)\SharedLibraries\RigidBodySolver\Include` (this resolves into `\SharedLibraries\RigidBodySolver\Include`)
5. Under *Configuration Properties > Linker > General > Additional Include Directories*, add 
   - `$(NPTRACKINGTOOLS_LIB)` (this should resolve into `C:\Program Files\OptiTrack\Motive\lib`)
   - `C:\Program Files\OptiTrack\Motive`
   - `C:\Program Files\OptiTrack\Motive\plugins\platforms`
6. Under *Configuration Properties > Linker > Input*, add 
   - `NPTrackingToolsx64.lib` for a 64 bit architecture
7. Copy all files from the folder within this addon titled *Motive2.1 Required Libraries* into your project's executable directory (*bin*). The files that will be copied include those in the below image. It is crucial for these files to be accessible at the same file structure level as the executable itself. For more information, [see this page](https://v21.wiki.optitrack.com/index.php?title=Motive_API:_Quick_Start_Guide#Library_Files).

![](https://v21.wiki.optitrack.com/images/6/6a/MotiveAPI_RequiredLIB.png)


## Additional Resources

`README_AUTHOR.md` contains instructions and explanations for you.
`README_DEPLOY.md` is filled with a template for an informative README file you might want to use with your addon.

Before announcing your addon to the world, you should remove this file (`README.md`) and the author instructions, and rename `README_DEPLOY.md` to `README.md`.
Also, if you have special instructions which people should see when submitting a pull request or open an issue in your addon repository, add a `CONTRIBUTING.md` file at the root of your repository. For more details, see https://github.com/blog/1184-contributing-guidelines

This template reflects the help text found at http://ofxaddons.com/howto, and will be updated from time to time by the OF developers.

Thanks for listening, and happy coding!
