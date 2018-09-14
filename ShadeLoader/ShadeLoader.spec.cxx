#include "gtest/gtest.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ShadeLoader.hxx"

namespace {

// The fixture for testing class Foo.
class ShadeLoaderTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ShadeLoaderTest() {
     // You can do set-up work for each test here.
  }

  ~ShadeLoaderTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(ShadeLoaderTest, LoadsFragment) {
	std::ifstream fragment{"Shaders/fragment.glsl", std::ios::in};
	if (not fragment.is_open()) FAIL();
	TK::ShadeLoader shader{};
	shader.template add_shader<TK::FragmentShader>(fragment);
	fragment.close();
}

TEST_F(ShadeLoaderTest, LoadsVertex) {
	std::ifstream vertex{"Shaders/vertex.glsl", std::ios::in};
	if (not vertex.is_open()) FAIL();
	TK::ShadeLoader shader{};
	shader.template add_shader<TK::VertexShader>(vertex);
	vertex.close();
}

TEST_F(ShadeLoaderTest, LinkShaders) {
	TK::ShadeLoader shader{};

	// Vertex Shader
	std::ifstream vertex{"Shaders/vertex.glsl", std::ios::in};
	if (not vertex.is_open()) FAIL();
	shader.template add_shader<TK::VertexShader>(vertex);
	vertex.close();
	// Fragment Shader
	std::ifstream fragment{"Shaders/fragment.glsl", std::ios::in};
	if (not fragment.is_open()) FAIL();
	shader.template add_shader<TK::FragmentShader>(fragment);
	fragment.close();

	shader.compile();
}

}  // namespace

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutCreateWindow("");
  glewInit();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}