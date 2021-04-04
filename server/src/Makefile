MODEL_DIR=./model/*
SERVICE_DIR=./service/*
DB_DIR=./db/*
SERVER_DIR=server.cpp

main: main.cpp ${SERVER_DIR} ${SERVICE_DIR} ${MODEL_DIR} ${DB_DIR}
	g++ $^ -o $@ -lmuduo_net -lmuduo_base -lmysqlclient -lpthread
