#!/bin/bash

cat <<"EOF"

  .-')    .-') _                _  .-')                .-') _          _   .-')       ('-.
 ( OO ). (  OO) )              ( \( -O )              (  OO) )        ( '.( OO )_   _(  OO)
(_)---\_)/     '._  .-'),-----. ,------.   ,--.   ,--./     '._ ,-.-') ,--.   ,--.)(,------.
/    _ | |'--...__)( OO'  .-.  '|   /`. '   \  `.'  / |'--...__)|  |OO)|   `.'   |  |  .---'
\  :` `. '--.  .--'/   |  | |  ||  /  | | .-')     /  '--.  .--'|  |  \|         |  |  |
 '..`''.)   |  |   \_) |  |\|  ||  |_.' |(OO  \   /      |  |   |  |(_/|  |'.'|  | (|  '--.
.-._)   \   |  |     \ |  | |  ||  .  '.' |   /  /\_     |  |  ,|  |_.'|  |   |  |  |  .--'
\       /   |  |      `'  '-'  '|  |\  \  `-./  /.__)    |  | (_|  |   |  |   |  |  |  `---.
 `-----'    `--'        `-----' `--' '--'   `--'         `--'   `--'   `--'   `--'  `------'

EOF

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

echo "
#####################
#   Setting up...   #
#####################
"
if [[ -n "${1}" ]]; then
  buildType="${1}"
else
  buildType="${RELEASE_BUILD_TYPE}"
fi

if [[ ${buildType} != "${DEBUG_BUILD_TYPE}" && ${buildType} != "${RELEASE_BUILD_TYPE}" ]]; then
  echo "Invalid build type [${buildType}], expected [${DEBUG_BUILD_TYPE},${RELEASE_BUILD_TYPE}]"
  exit 1
fi

echo "-- Using build type [${buildType}]"

buildFolder="cmake-build-$(echo ${buildType} | awk '{print tolower($0)}')"
echo "-- Using build folder [${buildFolder}]"

echo "-- Preparing build folder [$(pwd)/${buildFolder}]"
rm -rf "${buildFolder}"
mkdir "${buildFolder}"

echo "
###########################
#   Generating files...   #
###########################
"
cmake -S . -B "${buildFolder}" -DCMAKE_BUILD_TYPE="${buildType}"

echo "
###########################
#   Building project...   #
###########################
"
cmake --build "${buildFolder}" --config "${buildType}"

echo "
##############################
#   Installing binaries...   #
##############################
"
cmake --install "${buildFolder}"

echo "
############
#   DONE   #
############
"