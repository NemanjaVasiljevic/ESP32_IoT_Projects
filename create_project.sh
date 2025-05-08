#!/bin/bash

echo "Enter the project name:"

# Promp the user for the project name and wait for input
read PROJECT_NAME

# Check if the project name is empty
if [ -z "$PROJECT_NAME" ]; then
    echo "Project name cannot be empty."
    exit 1
fi

mkdir $PROJECT_NAME
cd $PROJECT_NAME
touch ${PROJECT_NAME}.cpp
cat <<EOL > ${PROJECT_NAME}.cpp
#include <iostream>

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
EOL
touch ${PROJECT_NAME}.h
mkdir ${PROJECT_NAME}_esp32
mkdir -p build

# Create a CMakeLists.txt file
cat <<EOL > CMakeLists.txt
# if confused look https://www.youtube.com/watch?v=A735Y4kMIPM
cmake_minimum_required(VERSION 3.10)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp) 
EOL

# Create build and run script
cat <<EOL > build_and_run.sh
#!/bin/bash
# Build the project
cd build
cmake ..
make
# Run the project
./${PROJECT_NAME}
EOL
chmod +x build_and_run.sh

# Create build script
cat <<EOL > build.sh
#!/bin/bash
# Build the project
cd build
cmake ..
make
EOL
chmod +x build.sh

# Create run current version script
cat <<EOL > run_current_version.sh
#!/bin/bash
# Run the project
cd build
./${PROJECT_NAME}
EOL
chmod +x run_current_version.sh

echo "Project $PROJECT_NAME created successfully."
echo "To build the project, run ./build.sh"
echo "To run the project, run ./run_current_version.sh"
echo "To build and run the project, run ./build_and_run.sh"