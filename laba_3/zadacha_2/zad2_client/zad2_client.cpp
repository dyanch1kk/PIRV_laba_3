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
        tcp::endpoint server(boost::asio::ip::make_address("127.0.0.1"), 12346);
        soket.connect(server);

        std::cout << "Введите числа через пробел (например: 5 12 7): ";
        std::string chisla;
        std::getline(std::cin, chisla);
        chisla += '\n';

        boost::asio::write(soket, boost::asio::buffer(chisla));

        boost::asio::streambuf bufer;
        boost::asio::read_until(soket, bufer, '\n');

        std::istream potokVvoda(&bufer);
        std::string otvet;
        std::getline(potokVvoda, otvet);
        std::cout << "Сумма: " << otvet << std::endl;

    }
    catch (std::exception& oshibka) {
        std::cerr << "Ошибка клиента: " << oshibka.what() << std::endl;
    }

    return 0;
}
