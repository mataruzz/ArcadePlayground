# Use the official Ubuntu image
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    fuse \
    libfuse2 \
    libx11-6 \
    libxext6 \
    libxrender1 \
    libxcb1 \
    libgl1-mesa-glx \
    libglib2.0-0 \
    libegl1 \
    libgl1 \
    libglu1-mesa \
    libnss3 \
    libfontconfig1 \
    libxkbcommon0 \
    libxau6 \
    libxdmcp6 \
    libsm6 
    # && rm -rf /var/lib/apt/lists/*

# Additional programs
RUN apt-get update && apt-get install -y\
    vim


### Configure locale to UTF-8
RUN apt-get update && apt-get install -y \
    locales && \
    rm -rf /var/lib/apt/lists/*

# Generate the desired locale
RUN locale-gen en_US.UTF-8

# Set environment variables
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Setting bash
RUN rm /bin/sh && ln -s /bin/bash /bin/sh

# Copy the AppImage into the container
COPY Arcade_Playground-x86_64.AppImage /home/root/
WORKDIR /home/root

# Make the AppImage executable
RUN chmod +x /home/root/Arcade_Playground-x86_64.AppImage

# Run the AppImage (can be overridden at runtime)
# CMD ["./home/root/Arcade_Playground-x86_64.AppImage --appimage-extract-and-run"]
ENTRYPOINT ["/bin/bash"]
