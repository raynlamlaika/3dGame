#!/bin/bash

# Cub3D Docker Helper Script
# Automatically detects OS and sets up X11 forwarding

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored messages
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect operating system
OS=$(uname -s)
print_info "Detected OS: $OS"

# Default values
MAP_FILE="maps/map.cub"
USE_BONUS=false
DOCKER_CMD="docker-compose"

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --bonus)
            USE_BONUS=true
            shift
            ;;
        --map)
            MAP_FILE="$2"
            shift 2
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --bonus         Use bonus version (with animations, minimap, mouse)"
            echo "  --map <file>    Specify map file (default: maps/map.cub)"
            echo "  --help, -h      Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                          # Run mandatory version with default map"
            echo "  $0 --bonus                  # Run bonus version"
            echo "  $0 --map maps/map2.cub      # Run with different map"
            echo "  $0 --bonus --map maps/map2.cub  # Bonus with custom map"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Determine which binary to run
if [ "$USE_BONUS" = true ]; then
    BINARY="./cub3d_bonus"
    print_info "Using bonus version"
else
    BINARY="./cub3d"
    print_info "Using mandatory version"
fi

# Platform-specific setup
case "$OS" in
    Linux)
        print_info "Setting up X11 for Linux..."
        
        # Check if DISPLAY is set
        if [ -z "$DISPLAY" ]; then
            print_warning "DISPLAY not set, using :0"
            export DISPLAY=:0
        fi
        
        # Enable X11 forwarding
        print_info "Enabling X11 forwarding..."
        xhost +local:docker > /dev/null 2>&1 || print_warning "Failed to run xhost (might need to install xhost)"
        
        # Run with docker-compose
        print_info "Running: $BINARY $MAP_FILE"
        docker-compose run --rm cub3d $BINARY $MAP_FILE
        
        # Cleanup (optional)
        # xhost -local:docker
        ;;
        
    Darwin)
        print_info "Setting up X11 for macOS..."
        
        # Check if XQuartz is installed
        if ! command -v xquartz &> /dev/null && ! [ -d "/Applications/XQuartz.app" ]; then
            print_error "XQuartz not found!"
            echo "Please install XQuartz:"
            echo "  brew install --cask xquartz"
            echo "Or download from: https://www.xquartz.org/"
            exit 1
        fi
        
        # Check if XQuartz is running
        if ! pgrep -x "Xquartz" > /dev/null && ! pgrep -x "X11" > /dev/null; then
            print_warning "XQuartz not running. Starting XQuartz..."
            open -a XQuartz
            sleep 3
        fi
        
        # Get IP address
        IP=$(ifconfig en0 2>/dev/null | grep inet | awk '$1=="inet" {print $2}')
        if [ -z "$IP" ]; then
            # Try other interfaces
            IP=$(ifconfig en1 2>/dev/null | grep inet | awk '$1=="inet" {print $2}')
        fi
        
        if [ -z "$IP" ]; then
            print_error "Could not determine IP address"
            echo "Please manually set DISPLAY and run:"
            echo "  export DISPLAY=<your_ip>:0"
            echo "  docker run -it --rm -e DISPLAY=\$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix:rw cub3d $BINARY $MAP_FILE"
            exit 1
        fi
        
        print_info "Using IP: $IP"
        export DISPLAY=$IP:0
        
        # Allow X11 connections
        print_info "Allowing X11 connections..."
        xhost + $IP > /dev/null 2>&1
        
        # Build if needed
        print_info "Building Docker image..."
        docker build -t cub3d . -q || docker build -t cub3d .
        
        # Run with docker
        print_info "Running: $BINARY $MAP_FILE"
        docker run -it --rm \
            -e DISPLAY=$DISPLAY \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            cub3d $BINARY $MAP_FILE
        ;;
        
    MINGW*|MSYS*|CYGWIN*)
        print_info "Detected Windows environment"
        print_warning "Please use WSL2 for better compatibility"
        
        # Check if running in WSL
        if grep -qi microsoft /proc/version 2>/dev/null; then
            print_info "Running in WSL2"
            
            # Get Windows host IP
            WIN_IP=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
            export DISPLAY=$WIN_IP:0
            
            print_info "Using DISPLAY=$DISPLAY"
            print_warning "Make sure X server (VcXsrv/Xming) is running on Windows!"
            
            # Run with docker-compose
            print_info "Running: $BINARY $MAP_FILE"
            docker-compose run --rm cub3d $BINARY $MAP_FILE
        else
            print_error "Please run this from WSL2"
            exit 1
        fi
        ;;
        
    *)
        print_error "Unsupported operating system: $OS"
        echo "Please refer to README-DOCKER.md for manual setup"
        exit 1
        ;;
esac

print_info "Done!"
