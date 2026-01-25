# Docker Support for Cub3D

This document provides instructions for running the Cub3D game using Docker on different platforms. Docker allows you to run the game without installing dependencies locally, making it platform-independent.

## Prerequisites

- Docker installed on your system
- Docker Compose (usually comes with Docker Desktop)
- X11 server for GUI display (platform-specific, see below)

## Security Note

⚠️ **For Development/Testing Only**: This Docker setup uses privileged mode and xhost permissions for simplicity. These settings grant elevated privileges and should only be used in trusted development environments. For production deployments, consider:
- Using more restrictive Docker capabilities instead of privileged mode
- Implementing proper X11 authentication mechanisms
- Running in isolated networks

## Platform-Specific Setup

### Linux Users

#### 1. Allow X11 Connections
Before running the game, you need to allow Docker to connect to your X11 server:

```bash
xhost +local:docker
```

#### 2. Build and Run
Using docker-compose (recommended):
```bash
docker-compose up --build
```

Or using Docker directly:
```bash
docker build -t cub3d .
docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  --network host \
  cub3d ./cub3d maps/map.cub
```

#### 3. Cleanup (optional)
When done, you can revoke X11 access:
```bash
xhost -local:docker
```

---

### macOS Users

#### 1. Install XQuartz
XQuartz is an X11 server for macOS:
```bash
brew install --cask xquartz
```

Or download from: https://www.xquartz.org/

#### 2. Configure XQuartz
- Start XQuartz: `open -a XQuartz`
- Open XQuartz Preferences (XQuartz → Preferences)
- Go to "Security" tab
- Enable "Allow connections from network clients"
- Restart XQuartz

#### 3. Set Up X11 Forwarding
In your terminal:
```bash
# Get your IP address
export IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')

# Allow X11 connections from your IP
xhost + $IP
```

#### 4. Build and Run
```bash
docker build -t cub3d .
docker run -it --rm \
  -e DISPLAY=$IP:0 \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  cub3d ./cub3d maps/map.cub
```

**Note:** You may need to restart XQuartz if the display doesn't work on first try.

---

### Windows Users (WSL2)

#### 1. Install X Server
Install an X server for Windows, such as:
- **VcXsrv** (recommended): https://sourceforge.net/projects/vcxsrv/
- **Xming**: http://www.straightrunning.com/XmingNotes/
- **X410** (paid): https://x410.dev/

#### 2. Configure X Server
For VcXsrv:
- Launch XLaunch
- Select "Multiple windows"
- Set Display number to 0
- Start no client
- **Important:** Check "Disable access control"
- Finish and save configuration

#### 3. Set DISPLAY Variable
In your WSL2 terminal:
```bash
# Get Windows host IP
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0

# Or manually set it if above doesn't work
export DISPLAY=<your_windows_ip>:0
```

#### 4. Build and Run
```bash
docker-compose up --build
```

---

## Quick Commands Reference

### Building
```bash
# Build only (don't run)
docker-compose build

# Force rebuild
docker-compose build --no-cache
```

### Running

#### Mandatory Version
```bash
# Using docker-compose
docker-compose run --rm cub3d ./cub3d maps/map.cub

# Different map
docker-compose run --rm cub3d ./cub3d maps/map2.cub
```

#### Bonus Version (with animations, minimap, mouse controls)
```bash
docker-compose run --rm cub3d ./cub3d_bonus maps/map.cub
```

### Cleaning

#### Clean object files
```bash
docker-compose run --rm cub3d make clean
```

#### Clean all (including binaries)
```bash
docker-compose run --rm cub3d make fclean
```

#### Rebuild from scratch
```bash
docker-compose run --rm cub3d make re
```

### Development

#### Interactive shell in container
```bash
docker-compose run --rm cub3d /bin/bash
```

#### Build in container
```bash
docker-compose run --rm cub3d make all
```

#### Stop all containers
```bash
docker-compose down
```

---

## Troubleshooting

### Issue: GUI Window Doesn't Appear

**Linux:**
- Check if X11 forwarding is enabled: `xhost +local:docker`
- Verify DISPLAY variable: `echo $DISPLAY` (should be `:0` or `:1`)
- Check if X server is running: `ps aux | grep X`

**macOS:**
- Ensure XQuartz is running
- Verify network connections are allowed in XQuartz preferences
- Check DISPLAY variable matches your IP: `echo $DISPLAY`
- Try restarting XQuartz

**Windows/WSL2:**
- Ensure X server (VcXsrv) is running
- Check "Disable access control" is enabled
- Verify DISPLAY variable: `echo $DISPLAY`
- Check Windows firewall isn't blocking connections

### Issue: Build Fails

**MiniLibX compilation errors:**
- The Dockerfile automatically builds MiniLibX for Linux
- If build fails, try: `docker-compose build --no-cache`

**Compilation errors:**
- Ensure all source files are present
- Check file permissions: `ls -la`
- Try rebuilding: `docker-compose run --rm cub3d make re`

### Issue: Permission Denied

**X11 socket access:**
```bash
# Linux
sudo chmod 666 /tmp/.X11-unix/*
xhost +local:docker

# macOS
xhost + $(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
```

### Issue: Performance is Slow

Docker adds overhead. For better performance:
- Use native builds when possible
- Docker is mainly for portability and testing
- On Linux, performance should be good
- On macOS/Windows, expect some overhead due to virtualization

### Issue: Can't Find Map File

Make sure to run from the project root:
```bash
cd /path/to/3dGame
docker-compose run --rm cub3d ./cub3d maps/map.cub
```

---

## Technical Details

### What Gets Installed in the Container

- Ubuntu 22.04 base system
- Build tools: gcc, make
- X11 libraries: libx11-dev, libxext-dev, libbsd-dev
- Additional X11: libxrandr-dev, libxinerama-dev, libxcursor-dev
- MiniLibX for Linux (built from source)

### How X11 Forwarding Works

1. Your host X server runs on your machine
2. Docker container shares the X11 socket (`/tmp/.X11-unix`)
3. DISPLAY variable tells apps where to render
4. Container draws to your host's X server

### Makefile Changes

The Makefile now detects the OS and uses appropriate flags:
- **macOS**: `-lmlx -framework OpenGL -framework AppKit`
- **Linux**: `-lmlx -lXext -lX11 -lm -lbsd`

---

## Native Build (Alternative)

If you prefer not to use Docker, you can build natively:

### On macOS
```bash
make all
./cub3d maps/map.cub
```

### On Linux
1. Install MiniLibX:
```bash
git clone https://github.com/42Paris/minilibx-linux.git
cd minilibx-linux
make
sudo cp libmlx.a /usr/local/lib/
sudo cp mlx.h /usr/local/include/
```

2. Build and run:
```bash
make all
./cub3d maps/map.cub
```

---

## Additional Resources

- [Docker Documentation](https://docs.docker.com/)
- [XQuartz (macOS)](https://www.xquartz.org/)
- [VcXsrv (Windows)](https://sourceforge.net/projects/vcxsrv/)
- [MiniLibX Repository](https://github.com/42Paris/minilibx-linux)

---

## Support

If you encounter issues not covered here:
1. Check the main README.md for game-specific help
2. Verify your Docker installation: `docker --version`
3. Check X11 server is running
4. Review Docker logs: `docker-compose logs`

---

## Game Controls

- **W/A/S/D** or **Arrow Keys**: Move
- **Left/Right Arrow** or **Mouse**: Look around (bonus)
- **ESC**: Exit game

Enjoy playing Cub3D! 🎮
