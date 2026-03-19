# Use Ubuntu 22.04 as base image
FROM ubuntu:22.04

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies and X11 libraries
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libx11-dev \
    libxext-dev \
    libbsd-dev \
    xorg \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Clone and build MiniLibX for Linux
RUN GIT_SSL_NO_VERIFY=1 git clone https://github.com/42Paris/minilibx-linux.git /tmp/minilibx-linux && \
    cd /tmp/minilibx-linux && \
    make && \
    cp libmlx.a /usr/local/lib/ && \
    cp mlx.h /usr/local/include/ && \
    rm -rf /tmp/minilibx-linux

# Set working directory
WORKDIR /app

# Copy project files
COPY . /app

# Build the game
RUN make bonus

# Set DISPLAY environment variable (can be overridden at runtime)
ENV DISPLAY=:0

# Default command
CMD ["./cub3d_bonus", "maps/map.cub"]
