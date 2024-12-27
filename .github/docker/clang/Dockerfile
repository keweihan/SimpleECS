FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
USER root
WORKDIR /workspace

# Download system dependencies
RUN apt-get update && apt-get install -y \
    wget \
    python3 \
    python3-pip \
    cmake \
    pkg-config \
    lsb-release \
    software-properties-common \
    gnupg \
    libomp-dev \
    libomp5 \
    libclang-dev \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install clang 16
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod u+x llvm.sh && \
    ./llvm.sh 16 && \
    rm llvm.sh

RUN clang-16 --version
RUN ln -s /usr/bin/clang-16 /usr/bin/clang && \
    ln -s /usr/bin/clang++-16 /usr/bin/clang++

# Create symbolic links for OpenMP
RUN ln -s /usr/lib/x86_64-linux-gnu/libomp.so.5 /usr/lib/libomp.so && \
    ln -s /usr/lib/x86_64-linux-gnu/libomp.so.5 /usr/lib/x86_64-linux-gnu/libomp.so

COPY conanfile.py /workspace/conanfile.py
RUN mkdir -p /root/.conan2
RUN pip install conan

# Set compiler and OpenMP flags
ENV CC=clang-16
ENV CXX=clang++-16
ENV CFLAGS="-Wno-strict-prototypes -fopenmp"
ENV CXXFLAGS="-fopenmp"
ENV LDFLAGS="-fopenmp"
ENV LD_LIBRARY_PATH="/usr/lib/llvm-16/lib:${LD_LIBRARY_PATH}"

RUN conan profile detect
RUN conan install . --output-folder=. --build=missing -s build_type=Release -c tools.system.package_manager:mode=install

CMD ["bash"]