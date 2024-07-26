# Test inside docker
Make the Docker containers connect to the X server running on your local machine
```
xhost +local:docker
```
Build the image and run the docker
```
docker compose build && docker compose run arcade_playground
```

Once inside the docker:
```
./Arcade_Playground-x86_64.AppImage --appimage-extract-and-run
```

You might notice differences in font, icons and style. Still trying to fix it, having a simple robust stable, independent from external dependencies.
