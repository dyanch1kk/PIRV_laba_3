﻿#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>

using boost::asio::ip::tcp;

int main() {
    setlocale(LC_ALL, "");

    try {
        boost::asio::io_context kontekst;

        tcp::acceptor priemnik(kontekst, tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345));
        std::cout << "Сервер запущен на 127.0.0.1:12345\nОжидание подключения...\n";

        for (;;) {
            tcp::socket soket(kontekst);
            priemnik.accept(soket);
            std::cout << "Клиент подключён: " << soket.remote_endpoint() << std::endl;

            boost::asio::streambuf bufer;
            boost::asio::read_until(soket, bufer, '\n');

            std::istream potokVvoda(&bufer);
            std::string soobshenie;
            std::getline(potokVvoda, soobshenie);

            std::cout << "Получено сообщение: " << soobshenie << std::endl;

            std::transform(soobshenie.begin(), soobshenie.end(), soobshenie.begin(), ::toupper);
            std::string otvet = soobshenie + "\n";

            boost::asio::write(soket, boost::asio::buffer(otvet));
            std::cout << "Ответ отправлен: " << otvet;
        }

    }
    catch (std::exception& oshibka) {
        std::cerr << "Ошибка сервера: " << oshibka.what() << std::endl;
    }

    return 0;
}
