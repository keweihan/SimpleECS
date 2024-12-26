FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

USER root

WORKDIR /workspace

# Download system dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    cmake \
    pkg-config \
    gnupg \
    gcc \
    g++ \
    && rm -rf /var/lib/apt/lists/*

COPY conanfile.py /workspace/conanfile.py

RUN mkdir -p /root/.conan2 && \
    pip install conan && \
    python3 --version && \
    cmake --version && \
    conan --version

RUN conan profile detect
RUN conan install . --output-folder=. --build=missing -s build_type=Release -c tools.system.package_manager:mode=install

CMD ["bash"]