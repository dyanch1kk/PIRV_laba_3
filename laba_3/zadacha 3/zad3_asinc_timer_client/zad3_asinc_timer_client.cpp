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

        // ✅ Правильное подключение по адресу 127.0.0.1 и порту 12347
        tcp::resolver resolver(kontekst);
        auto konec = resolver.resolve("127.0.0.1", "12347");
        boost::asio::connect(soket, konec);

        std::cout << "Подключено к серверу.\n";

        std::string komand = "ping\n";
        boost::asio::write(soket, boost::asio::buffer(komand));
        std::cout << "Отправлено: " << komand;

        boost::asio::streambuf bufer;
        boost::asio::read_until(soket, bufer, '\n');

        std::istream potokVvoda(&bufer);
        std::string otvet;
        std::getline(potokVvoda, otvet);

        std::cout << "Получен ответ: " << otvet << std::endl;
    }
    catch (std::exception& oshibka) {
        std::cerr << "Ошибка клиента: " << oshibka.what() << std::endl;
    }

    return 0;
}
