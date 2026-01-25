# Docker Support for Cub3D

This document explains how to build and run the Cub3D raycasting game using Docker. Docker support makes the project portable and eliminates the need for manual dependency installation.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Building the Docker Image](#building-the-docker-image)
- [Running the Game](#running-the-game)
- [Platform-Specific Setup](#platform-specific-setup)
- [Development Workflow](#development-workflow)
- [Troubleshooting](#troubleshooting)

## Prerequisites

1. **Docker**: Install Docker from [https://docs.docker.com/get-docker/](https://docs.docker.com/get-docker/)
2. **Docker Compose**: Usually included with Docker Desktop
3. **X11 Display Server**: Required for GUI applications
   - Linux: Usually pre-installed
   - macOS: Install XQuartz
   - Windows: Install VcXsrv or X410

## Quick Start

The fastest way to run the game:

```bash
# Allow X11 connections (required once per session)
xhost +local:docker

# Run the game with docker-compose
docker-compose up cub3d

# Or run the bonus version
docker-compose up cub3d-bonus
```

## Building the Docker Image

Build the Docker image manually:

```bash
docker build -t cub3d .
```

This will:
- Set up Ubuntu 22.04 base image
- Install all required dependencies (gcc, make, X11 libraries)
- Install MiniLibX for Linux from source
- Build the project

## Running the Game

### Using Docker Compose (Recommended)

Docker Compose simplifies the process with pre-configured settings:

```bash
# Run mandatory version
docker-compose up cub3d

# Run bonus version with minimap and animations
docker-compose up cub3d-bonus

# Run with a different map
docker-compose run --rm cub3d ./cub3d maps/map2.cub
```

### Using Docker Directly

For more control:

```bash
# Run mandatory version
docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  --network host \
  cub3d ./cub3d maps/map.cub

# Run bonus version
docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  --network host \
  cub3d ./cub3d_bonus maps/map.cub

# Run with a custom map
docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  --network host \
  cub3d ./cub3d maps/map2.cub
```

## Platform-Specific Setup

### Linux

X11 is usually pre-installed. Before running the game:

```bash
# Allow Docker to connect to X11
xhost +local:docker

# Run the game
docker-compose up cub3d

# When done, restore X11 security (optional)
xhost -local:docker
```

**Troubleshooting Linux X11:**
- If you get "cannot open display" error, ensure `$DISPLAY` is set:
  ```bash
  echo $DISPLAY  # Should output something like ":0" or ":1"
  ```
- Check X11 socket permissions:
  ```bash
  ls -la /tmp/.X11-unix/
  ```

### macOS

macOS requires XQuartz for X11 support:

1. **Install XQuartz:**
   ```bash
   brew install --cask xquartz
   ```
   Or download from [https://www.xquartz.org/](https://www.xquartz.org/)

2. **Configure XQuartz:**
   - Open XQuartz
   - Go to Preferences → Security
   - Enable "Allow connections from network clients"
   - Restart XQuartz

3. **Set up X11 forwarding:**
   ```bash
   # Get your IP address
   IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
   
   # Allow connections from your IP
   xhost + $IP
   
   # Set DISPLAY environment variable
   export DISPLAY=$IP:0
   ```

4. **Run the game:**
   ```bash
   docker-compose up cub3d
   ```

**macOS Alternative - Native Build:**
The Makefile now automatically detects macOS and uses the native frameworks. You can build and run natively without Docker:
```bash
make
./cub3d maps/map.cub
```

### Windows

Windows requires an X server application:

#### Option 1: VcXsrv (Recommended)

1. **Install VcXsrv:**
   - Download from [https://sourceforge.net/projects/vcxsrv/](https://sourceforge.net/projects/vcxsrv/)
   - Install with default settings

2. **Launch XLaunch:**
   - Multiple windows
   - Display number: 0
   - Start no client
   - **Important:** Check "Disable access control"

3. **Set DISPLAY in PowerShell:**
   ```powershell
   $env:DISPLAY="host.docker.internal:0"
   ```

4. **Run the game:**
   ```powershell
   docker-compose up cub3d
   ```

#### Option 2: X410

1. Install X410 from Microsoft Store
2. Launch X410 in "Windowed Apps" mode
3. Set DISPLAY:
   ```powershell
   $env:DISPLAY="host.docker.internal:0"
   ```
4. Run the game

#### Windows with WSL2

If using WSL2:
```bash
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
docker-compose up cub3d
```

## Development Workflow

The docker-compose setup mounts the current directory, allowing live development:

1. **Make code changes** in your editor
2. **Rebuild inside container:**
   ```bash
   docker-compose run --rm cub3d make re
   ```
3. **Run the updated version:**
   ```bash
   docker-compose up cub3d
   ```

### Building Bonus Version

```bash
# Build bonus version inside container
docker-compose run --rm cub3d make bonus

# Run the bonus version
docker-compose up cub3d-bonus
```

### Cleaning Build Artifacts

```bash
# Clean object files
docker-compose run --rm cub3d make clean

# Clean everything
docker-compose run --rm cub3d make fclean

# Rebuild from scratch
docker-compose run --rm cub3d make re
```

## Game Controls

### Mandatory Version
- **W/A/S/D**: Move forward/left/backward/right
- **Arrow Keys**: Rotate camera left/right
- **ESC**: Exit game

### Bonus Version
Additional features:
- **Mouse**: Look around (mouse movement)
- **Minimap**: Top-left corner of screen
- **Animated Sprites**: Wall textures with animations

## Troubleshooting

### Cannot Open Display

**Symptom:** `Error: Can't open display`

**Solution:**
```bash
# Linux/macOS
xhost +local:docker
export DISPLAY=:0

# Windows (PowerShell)
$env:DISPLAY="host.docker.internal:0"
```

### Permission Denied on X11 Socket

**Symptom:** Permission errors accessing `/tmp/.X11-unix`

**Solution (Linux):**
```bash
sudo chmod 777 /tmp/.X11-unix
xhost +local:docker
```

### Docker Build Fails

**Symptom:** Build errors during `docker build`

**Solution:**
```bash
# Clean Docker cache and rebuild
docker system prune -a
docker build --no-cache -t cub3d .
```

### MiniLibX Not Found

**Symptom:** `fatal error: mlx.h: No such file or directory`

**Solution:**
The Dockerfile automatically installs MiniLibX. If you're building natively:

**Linux:**
```bash
git clone https://github.com/42Paris/minilibx-linux.git
cd minilibx-linux
make
sudo cp libmlx.a /usr/local/lib/
sudo cp mlx.h /usr/local/include/
```

**macOS:**
MiniLibX is included with 42 toolchain or install manually.

### Game Window Doesn't Appear

**Symptom:** No errors but window doesn't show

**Solutions:**
1. Verify X server is running (XQuartz, VcXsrv, etc.)
2. Check DISPLAY variable: `echo $DISPLAY`
3. Test X11 with a simple app:
   ```bash
   docker run -it --rm \
     -e DISPLAY=$DISPLAY \
     -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
     --network host \
     ubuntu:22.04 sh -c "apt-get update && apt-get install -y x11-apps && xeyes"
   ```

### Slow Performance

**Symptom:** Game runs slowly or lags

**Solutions:**
- Ensure Docker has enough resources (Docker Desktop → Settings → Resources)
- Use native build on macOS for better performance
- Close other Docker containers
- Check CPU/memory usage: `docker stats`

### Map File Not Found

**Symptom:** `Error: Cannot open map file`

**Solution:**
Make sure the map file path is relative to the project root:
```bash
# Correct
docker-compose run --rm cub3d ./cub3d maps/map.cub

# Incorrect
docker-compose run --rm cub3d ./cub3d /maps/map.cub
```

### Network Host Mode Not Working (Windows/macOS)

**Symptom:** `network_mode: host` not supported

**Solution:**
Edit `docker-compose.yml` and replace `network_mode: host` with port publishing:
```yaml
    ports:
      - "6000:6000"
```

## Advanced Usage

### Custom Map Files

Add your own `.cub` map files to the `maps/` directory:

```bash
# Run with custom map
docker-compose run --rm cub3d ./cub3d maps/your_custom_map.cub
```

Map file format:
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm

F 220,100,0    # Floor color (RGB)
C 225,30,0     # Ceiling color (RGB)

# Map grid (1 = wall, 0 = empty space, N/S/E/W = player start position)
111111
100001
1000N1
111111
```

### Debugging

Run with an interactive shell:
```bash
docker-compose run --rm cub3d bash
# Inside container:
make re
./cub3d maps/map.cub
```

### Check MiniLibX Installation

```bash
docker-compose run --rm cub3d bash
ls -la /usr/local/lib/libmlx*
ls -la /usr/local/include/mlx.h
```

## Additional Resources

- [Cub3D Subject](https://cdn.intra.42.fr/pdf/pdf/960/cub3d.en.pdf)
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- [Docker Documentation](https://docs.docker.com/)
- [XQuartz](https://www.xquartz.org/)
- [VcXsrv](https://sourceforge.net/projects/vcxsrv/)

## License

This project is part of the 42 School curriculum.
