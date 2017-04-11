docker run --name mysql2 -e MYSQL_ROOT_PASSWORD=temp -d mysql/mysql-server
docker exec -it mysql2 mysql -uroot -p