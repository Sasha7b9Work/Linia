# Сканирует адреса в диапазоне 192.168.1.1-254 на возможность подключения по ssh

# Получаем список активных IP-адресов
active_ips=$(fping -ag 192.168.1.0/24 2>/dev/null)

# Перебираем только активные IP
for IP in $active_ips; do
    echo -n "Проверка $IP... "
    timeout 2 bash -c "echo >/dev/tcp/$IP/22" 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "✅ Порт 22 открыт на $IP"
    else
        echo "❌ Порт 22 закрыт на $IP"
    fi
done
