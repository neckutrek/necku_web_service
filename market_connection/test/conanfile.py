import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps
from conan.tools.build import cross_building

class MarketConnectionTestsConan(ConanFile):
    name = "market_connection_tests"
    settings = "os", "compiler", "build_type", "arch"
#    generators = "VirtualBuildEnv", "VirtualRunEnv"
#    apply_env = False
#    test_type = "explicit"


    def requirements(self):
        self.requires("market_connection/0.1")
        self.requires("catch2/2.13.9")
        self.requires("openssl/3.0.3")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def test(self):
        if not cross_building(self):
            cmd = os.path.join(self.cpp.build.bindirs[0], "tests")
            self.run(cmd, env="conanrun")

