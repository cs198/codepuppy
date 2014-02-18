#!/usr/bin/env bash

echo "First, this script will check if you have all the required code-checking tools installed."
echo

INSTALL_ERR="After installing, try committing again."
check_success () {
  if [ $? -ne 0 ]; then
      echo "Program failed to install. Please follow the instructions and try committing again."
      exit 1
  fi
}

command -v rubocop >/dev/null 2>&1 || {
  echo "Error: rubocop not installed. It is necessary for checking your Ruby style."
  echo
  echo "Please install rubocop by running: "
  echo "gem install rubocop"
  echo
  while true; do
    read -p "Do you wish to install rubocop now? [Yn] " yn
    yn=${yn:-y}
    case $yn in
      [Yy]* ) gem install rubocop; check_success; break;;
      [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
      * ) echo "Please answer yes or no.";;
    esac
    unset yn
  done
}

command -v jshint >/dev/null 2>&1 || {
  echo "Error: jshint not installed. It is necessary for checking your Javascript style."
  echo
  command -v npm >/dev/null 2>&1 || {
  echo "You do not have node and Node Package Manager (npm) installed. jshint depends on node, and is easily installed through npm. To install node, please run these commands:"
    echo

    if [ "$(uname)" == "Darwin" ]
    then
      echo "brew install node"
      while true; do
        read -p "Do you wish to install node now? [Yn] " yn
        yn=${yn:-y}
        case $yn in
            [Yy]* ) brew install node; check_success; break;;
            [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
            * ) echo "Please answer yes or no.";;
        esac
        unset yn
      done
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
    then
      echo For Debian/Ubuntu:
      echo "sudo apt-get install node"
      echo "sudo apt-get install npm"
      while true; do
        read -p "Do you wish to install node now? [Yn] " yn
        yn=${yn:-y}
        case $yn in
            [Yy]* ) sudo apt-get install node; check_success; sudo apt-get install npm; check_success; break;;
            [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
            * ) echo "Please answer yes or no.";;
        esac
        unset yn
      done
    elif [ -n "$COMSPEC" -a -x "$COMSPEC" ]
    then
      # Windows
      echo "Go to this link:"
      echo http://nodejs.org/download/
      exit 1
    fi

    echo
  }

  command -v jshint >/dev/null 2>&1 || {
    echo "Please install jshint by running: "
    echo "sudo npm install -g jshint"

    while true; do
      read -p "Do you wish to install jshint now? [Yn] " yn
      yn=${yn:-y}
      case $yn in
          [Yy]* ) sudo npm install -g jshint; check_success; break;;
          [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
          * ) echo "Please answer yes or no.";;
      esac
      unset yn
    done
  }
}

# in case they installed jshint before, but uninstalled node for some reason...
command -v node>/dev/null 2>&1 || {
  echo "It looks like you have already installed jshint"
  echo "However, you do not have node installed. jshint depends on node. To install node, please run these commands:"
  echo

  if [ "$(uname)" == "Darwin" ]
  then
    echo "brew install node"
    while true; do
      read -p "Do you wish to install node now? [Yn] " yn
      yn=${yn:-y}
      case $yn in
          [Yy]* ) brew install node; check_success; break;;
          [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
          * ) echo "Please answer yes or no.";;
      esac
      unset yn
    done
  elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
  then
    echo For Debian/Ubuntu:
    echo "sudo apt-get install node"
    while true; do
      read -p "Do you wish to install node now? [Yn] " yn
      yn=${yn:-y}
      case $yn in
          [Yy]* ) sudo apt-get install node; check_success; break;;
          [Nn]* ) echo "$INSTALL_ERR"; exit 1;;
          * ) echo "Please answer yes or no.";;
      esac
      unset yn
    done
  elif [ -n "$COMSPEC" -a -x "$COMSPEC" ]
  then
    # Windows
    echo "Go to this link:"
    echo http://nodejs.org/download/
    exit 1
  fi
}

echo "All checkers needed are installed."
echo "Now, the pre-commit script will be installed into your git repo."
echo

cp pre-commit ../.git/hooks/pre-commit

echo "Success! Happy coding!"
