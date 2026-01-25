# Use Ubuntu as the base image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    git \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libbsd-dev \
    xorg \
    && rm -rf /var/lib/apt/lists/*

# Install MiniLibX for Linux
WORKDIR /tmp
RUN git clone https://github.com/42Paris/minilibx-linux.git && \
    cd minilibx-linux && \
    make && \
    cp libmlx.a /usr/local/lib/ && \
    cp mlx.h /usr/local/include/ && \
    cp libmlx_Linux.a /usr/local/lib/ 2>/dev/null || true && \
    cd .. && \
    rm -rf minilibx-linux

# Set working directory for the project
WORKDIR /app

# Copy project files
COPY . .

# Build the project
RUN make

# Set display environment variable
ENV DISPLAY=:0

# Default command - run the mandatory version with a default map
CMD ["./cub3d", "maps/map.cub"]
