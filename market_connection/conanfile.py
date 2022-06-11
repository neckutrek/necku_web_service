from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class MarketConnectionConan(ConanFile):
    name = "market_connection"
    version = "0.1"

    # Optional metadata
    license = "GPL-3"
    author = "Marcus Johansson <>"
    url = "https://github.org/neckutrek/necku_web_service"
    description = "A C++/Java trading application just for fun"
    topics = ("c++","java","trading","boost","websockets")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the ecipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("openssl/3.0.3")
        self.requires("openjdk/16.0.1")
        self.requires("boost/1.79.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["market_connection"]

