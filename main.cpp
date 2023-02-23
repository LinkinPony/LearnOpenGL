#include <iostream>

#include "Source/Editor/editor.h"


int main() {
  auto editor = std::make_shared<Editor>(800, 600);
  editor->run();
  return 0;
}