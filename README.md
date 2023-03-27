В качестве сервера выступал официальный docker server clickhouse: https://hub.docker.com/r/yandex/clickhouse-server

Чтобы запустить сервер необходимо выполнить: docker run -d --network=host --name some-clickhouse-server --ulimit nofile=262144:262144 clickhouse/clickhouse-server

На сервер можно попасть: http://localhost:8123/play?user=default

Необходимо выполнить команды из файла: comands.sql

Проверка cppcheck не дала значимых результатов, пытался с разными настройками. Результат проверок в файле check1.txt Ссылка на проект: https://cppcheck.sourceforge.io/