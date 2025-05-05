﻿#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <locale>

using boost::asio::ip::tcp;

int main() {
    setlocale(LC_ALL, "");

    try {
        boost::asio::io_context kontekst;

        tcp::acceptor priemnik(kontekst, tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12346));
        std::cout << "Сервер для суммы чисел запущен на 127.0.0.1:12346\n";

        tcp::socket soket(kontekst);
        priemnik.accept(soket);
        std::cout << "Клиент подключён\n";

        // Используем shared_ptr только для буфера, чтобы он не удалился раньше времени
        auto bufer = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(soket, *bufer, '\n',
            [bufer, &soket](const boost::system::error_code& oshibka, std::size_t) {
                if (!oshibka) {
                    std::istream potokVvoda(bufer.get());
                    std::string stroka;
                    std::getline(potokVvoda, stroka);

                    std::stringstream potok(stroka);
                    int chislo, summa = 0;
                    while (potok >> chislo) {
                        summa += chislo;
                    }

                    std::string otvet = std::to_string(summa) + "\n";

                    boost::asio::async_write(soket, boost::asio::buffer(otvet),
                        [&otvet](const boost::system::error_code&, std::size_t) {
                            std::cout << "Ответ отправлен клиенту.\n";
                        });

                }
                else {
                    std::cerr << "Ошибка чтения: " << oshibka.message() << std::endl;
                }
            });

        kontekst.run();

    }
    catch (std::exception& oshibka) {
        std::cerr << "Ошибка сервера: " << oshibka.what() << std::endl;
    }

    return 0;
}
