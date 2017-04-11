docker rm some-nginx
docker run --name some-nginx -d -p 8080:80 base-nginx
start http://localhost:8080/index.html