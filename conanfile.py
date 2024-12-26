from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


#
class SimpleECSConan(ConanFile):
    name = "SimpleECS"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    default_options = {
        "boost/*:shared": True,
        "boost/*:header_only": False,
        "boost/*:without_log": True,
        "boost/*:without_mpi": True,
        "boost/*:without_url": True,
        "boost/*:without_json": True,
        "boost/*:without_math": True,
        "boost/*:without_test": True,
        "boost/*:without_wave": True,
        "boost/*:without_fiber": True,
        "boost/*:without_graph": True,
        "boost/*:without_regex": True,
        "boost/*:without_timer": True,
        "boost/*:without_atomic": True,
        "boost/*:without_chrono": True,
        "boost/*:without_cobalt": True,
        "boost/*:without_locale": True,
        "boost/*:without_nowide": True,
        "boost/*:without_python": True,
        "boost/*:without_random": True,
        "boost/*:without_system": True,
        "boost/*:without_thread": True,
        "boost/*:without_context": True,
        "boost/*:without_process": True,
        "boost/*:without_charconv": True,
        "boost/*:without_contract": True,
        "boost/*:without_container": False,
        "boost/*:without_coroutine": True,
        "boost/*:without_date_time": True,
        "boost/*:without_exception": True,
        "boost/*:without_iostreams": True,
        "boost/*:without_filesystem": True,
        "boost/*:without_stacktrace": True,
        "boost/*:without_type_erasure": True,
        "boost/*:without_serialization": True,
        "boost/*:without_graph_parallel": True,
        "boost/*:without_program_options": True,
        "boost/*:zlib": False,
        "boost/*:bzip2": False,
        "boost/*:lzma": False,
        "boost/*:zstd": False,
        "boost/*:numa": False,
    }

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_CXX_STANDARD"] = "17"
        tc.generate()

    def requirements(self):
        # Specify all required dependencies
        self.requires("sdl/2.28.3", override=True)
        self.requires("sdl_ttf/2.20.1")
        self.requires("sdl_mixer/2.8.0")
        self.requires("sdl_image/2.6.3")
        self.requires("imgui/1.90.6-docking")
        self.requires("b2/5.2.0")
        self.requires("gtest/1.14.0")
        self.requires("boost/1.86.0")

    def deploy(self):
        self.copy("*.dll", src="bin", dst="bin")

    def layout(self):
        cmake_layout(self)
