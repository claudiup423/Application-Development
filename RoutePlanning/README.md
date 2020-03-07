# Route Planning Project

This repo contains the implementation of a A* route planner running on OpenStreetMap in C++.

The user is asked for 2 coordinates with x and y between 0 and 100. This program can calculate and draw the shortest path from start point to end point. 

## Running the project

This project was tested on Ubuntu 18.04.

### Install dependencies
  - Install Visual Studio
  - Install Cmake: 
  ```
  sudo apt purge cmake (If you have Ros installed this will ruine it, you will have to install it again)
  wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
  sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
  sudo apt-get update
  sudo apt-get install cmake
  ```
  This instructions can also be found at:
  `https://apt.kitware.com/`

  - Follow the vcpkg instructin (https://github.com/Microsoft/vcpkg)
  - Install io2d and dependencies by following this link:
  `https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md`
  Comment out the last 2 sections in PO267_Reflmpl/CMakeLists.txt (Samples and tests)
  ```
  cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
  cmake --build .
  sudo make install
  ```
  ## Run the alghorithm
  ```
  cd ~/Route_Planning_Project/build
  ./OSM_A_star_search
  ```
  ## Test Maps
