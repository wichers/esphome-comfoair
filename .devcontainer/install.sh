sudo apt update & sudo apt install -y pip
sudo pip3 install setuptools
git clone https://github.com/esphome/esphome.git
cd esphome 
pip3 install -r ./requirements_optional.txt
sudo python3 setup.py install