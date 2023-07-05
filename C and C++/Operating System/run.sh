#!/bin/bash


##### OS Run Script Group J WS2022 #####
# Execute this file with "bash run.sh"
# You can also use ./run.sh, you might have to chmod +x though
# Consider creating an alias (e.g., "sweb") for this script and adding it to your shell config


# Ansi Colors
ANSI_RED="\u001b[31m"
ANSI_GREEN="\u001b[32m"
ANSI_RESET="\u001b[0m"


# Manually change paths here if so desired
SWEB_PATH="$(pwd)"
BUILD_PATH="/tmp/sweb"


function main() {
    if [ "$1" == "info" ]; then func_info;
  elif [ "$1" == "lol" ]; then func_lol;
  elif [ "$1" == "debug" ]; then func_debug;
  elif [ "$1" == "clean" ]; then func_clean;
  elif [ "$1" == "cclean" ]; then func_cclean;
  elif [ "$1" == "cmake" ]; then func_reset_cmake;
  elif [ "$1" == "git" ]; then func_gitk;
  elif [ "$1" == "help" ] || [ "$1" == "h" ]; then func_help;
  elif [ "$1" == "tc" ]; then func_execute_tc "$@";
  elif [ "$1" == "dis" ]; then func_disassemble "$@";
  elif [ "$1" == "bin" ]; then func_show_bin_headers "$@";
  elif [ "$1" == "32" ] || [ "$1" == "64" ] || [ "$1" == "32_pae" ]; then func_switch_arch "$@";
  elif [ $# -gt 0 ]; then func_unknown_command "$@"; fi
  func_build_sweb && func_execute_sweb
}


function func_build_sweb() {
  print_info "Building SWEB"
  if ! (mkdir -p $BUILD_PATH && cd $BUILD_PATH && cmake "$SWEB_PATH" && make -j); then
    print_error "An unexpected error occurred, terminating"
    exit 1
  fi
}


function func_execute_sweb() {
  print_info "Executing SWEB"
  if ! (mkdir -p $BUILD_PATH && cd $BUILD_PATH && make qemu); then
      print_error "An unexpected error occurred, terminating"
      exit 1
    fi
}


function func_lol(){
		func_clean
		func_build_sweb
		func_execute_sweb
		exit 0

}

function print_info() {
  echo -e "[${ANSI_GREEN}INFO${ANSI_RESET}] $1"
}


function print_error() {
  echo -e "[${ANSI_RED}ERROR${ANSI_RESET}] $1"
  
}


if ! cd "$SWEB_PATH"; then
  print_error "SWEB directory \"$SWEB_PATH\" not found"
  exit 1
fi
print_info "SWEB directory found"


function func_help() {
  echo ""
  print_info "Execution"
  print_info "./run.sh                     ...   Build and execute SWEB in currently selected mode"
  print_info "./run.sh clean               ...   Clear build directory, then build and execute x64"
  print_info "./run.sh 32                  ...   Switch to 32 bit mode"
  print_info "./run.sh 32_pae              ...   Switch to 32_pae bit mode"
  print_info "./run.sh 64                  ...   Switch to 64 bit mode"
  print_info "./run.sh tc p1.sweb <...>    ...   Execute testcase(s) directly or lists testcases"
  echo ""
  print_info "Debugging"
  print_info "./run.sh dis p1.sweb         ...   Disassemble p1.sweb"
  print_info "./run.sh bin p1.sweb         ...   Display binary headers of p1.sweb"
  echo ""
  print_info "Other"
  print_info "./run.sh h                   ...   Display this message"
  print_info "./run.sh cmake               ...   Quickfix: Reset CMake Files"
  print_info "./run.sh info                ...   Show SWEB and build path"
  print_info "./run.sh git                 ...   Show git info"
  echo ""
  exit 0
}


function func_info() {
  print_info "Sweb Path = $SWEB_PATH"
  print_info "Build Path = $BUILD_PATH"
  exit 0
}


function func_debug(){
 	print_info "Sweb Path = $SWEB_PATH"
  print_info "Build Path = $BUILD_PATH"
 	print_info "Initiating Debugging of Sweb"
 	print_info "Clearing build directory "
	rm -rf $BUILD_PATH
  print_info "Preparing and building "
  if ! (mkdir -p $BUILD_PATH && cd $BUILD_PATH && cmake "$SWEB_PATH" && yes | make debug && make -j && make qemugdb); then
    print_error "An unexpected error occurred, terminating"
    exit 1
  fi
 
  exit 0
}


function func_disassemble() {
  if ! objdump -d "$BUILD_PATH/userspace/$2"; then
    print_error "Could not find file $2 to disassemble, possible arguments:"
    ls -1 "$BUILD_PATH/userspace" | grep ".sweb"
    exit 1
  fi
  exit 0
}


function func_show_bin_headers() {
  if ! readelf -a "$BUILD_PATH/userspace/$2"; then
    print_error "Could not find file $2 to read headers from, possible arguments:"
    ls -1 "$BUILD_PATH/userspace" | grep ".sweb"
    exit 1
  fi
  exit 0
}


function func_reset_cmake() {
  TEMP_SWEB_DIR="${SWEB_PATH}/TempSwebDir"
  rm -rf "$TEMP_SWEB_DIR" && mkdir "$TEMP_SWEB_DIR" && cd $TEMP_SWEB_DIR
  if ! git clone https://github.com/IAIK/sweb.git > /dev/null 2>&1; then
    print_error "Could not clone sweb github repository, terminating"
    exit 1
  fi
  cd sweb || exit 1
  for i in $(find . -name "CMake*")
  do
      cp "$i" "${SWEB_PATH}${i:1}"
  done
  rm -rf "$TEMP_SWEB_DIR"
  print_info "Successfully reset CMake files"
  exit 0
}


function func_execute_tc() {
  if [ $# -eq 1 ]; then
    print_error "No testcase(s) provided, possible values:"
    ls -1 $SWEB_PATH/userspace/tests | grep ".c"
    exit 1
  fi
  file=$SWEB_PATH/common/include/kernel/user_progs.h
  rm -rf $file
  touch $file
  echo "char const *user_progs[] = {" >> $file
  for tc in "${@:2}"
  do
    tc=$(echo $tc | sed -e 's/\.c/\.sweb/g')
    echo "\"/usr/$tc\"," >> $file
  done
  echo "0" >> $file
  echo "};" >> $file
}


function func_clean() {
  rm -rf $BUILD_PATH
}

function func_cclean() {
 	print_info "Clearing build directory"
  rm -rf $BUILD_PATH
  exit 0
}


function func_gitk() {
  gitk $SWEB_PATH
  exit 0
}


function func_switch_arch() {
  print_info "Switching target architecture to x86_$1"
  if ! cd "$BUILD_PATH"; then
    print_error "Could not switch to build directory"
    exit 1
  fi
  if ! (yes | make "x86_$1"); then
    print_error "An unexpected error occurred, terminating"
    exit 1
  fi
  print_info "Successfully switched architecture to x$1"
  cd "$SWEB_PATH" && bash run.sh
  exit 0
}


function func_unknown_command() {
  if [ $# -eq 0 ]; then
      return
  fi
  print_error "Unknown command $1"
  exit 1
}




main "$@"
