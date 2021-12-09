#!/bin/bash

echo "Installing PiStatus !"
sudo mkdir -p /usr/PiStaTus
sudo cp ./PiStaTus /usr/PiStaTus/
sudo cp ./pistatusService/pistatus.service /lib/systemd/system/
sudo systemctl enable pistatus.service
sudo systemctl start pistatus.service
echo "Installing finished !"
