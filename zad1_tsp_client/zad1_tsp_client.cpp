﻿#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <locale>

using boost::asio::ip::tcp;

int main() {
    setlocale(LC_ALL, "");

    try {
        boost::asio::io_context kontekst;
        tcp::socket soket(kontekst);
        tcp::endpoint server(boost::asio::ip::make_address("127.0.0.1"), 12345);
        soket.connect(server);

        std::cout << "Введите сообщение для сервера: ";
        std::string soobshenie;
        std::getline(std::cin, soobshenie);
        soobshenie += '\n';

        boost::asio::write(soket, boost::asio::buffer(soobshenie));

        boost::asio::streambuf bufer;
        boost::asio::read_until(soket, bufer, '\n');

        std::istream potokVvoda(&bufer);
        std::string otvet;
        std::getline(potokVvoda, otvet);
        std::cout << "Ответ от сервера: " << otvet << std::endl;

    }
    catch (std::exception& oshibka) {
        std::cerr << "Ошибка клиента: " << oshibka.what() << std::endl;
    }

    return 0;
}
