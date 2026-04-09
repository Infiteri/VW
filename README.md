# vw - vanea wallpaper

vw aims to be a free, open source wallpaper engine built by one dev, it also aims to serve as a big portfolio project 

# Building the project
after downloading the source code, cmd into the project root and run

```cmd
make
```

this should output 
```
VW build: 
build outputs same executable: 
                                win - Builds the windows executable
                                dev - Builds the dev executable
                                run - Run the executable
                                kill - Kill the running executable
                                vendor - Builds vendor files
                                assets - Copies the assets needed by VW
```

select any option you like:
- win - builds for the windows platform
- dev - builds for development

(note that every exe builds in the exact same Bin/VW.exe)

other build options include:
- assets - this copies the engine assets
- run - runs the built executable
- vendor - builds all the vendor files, copies DLLs and sets up Bin for the .exe to be ran

