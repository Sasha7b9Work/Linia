set -e    # Stop script when en error occurs

sudo apt-get update
sudo apt-get -y upgrade

sudo apt-get -y install nginx htop openssh-server openssh-client libssh-dev iptables-persistent mc links
sudo apt-get -y install cmake gcc make python3 python3-pip python3-venv build-essential gdb
sudo apt-get -y install libssl-dev libx11-dev libxrandr-dev mesa-common-dev libgl1-mesa-dev libasound2-dev
