# Перезагружаем правила udev
sudo udevadm control --reload-rules

# Перезагружаем systemd, чтобы он «увидел» новый сервис
sudo systemctl daemon-reload
