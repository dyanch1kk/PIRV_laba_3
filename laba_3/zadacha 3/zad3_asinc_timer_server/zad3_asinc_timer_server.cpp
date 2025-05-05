﻿#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <locale>

using boost::asio::ip::tcp;

int main() {
    setlocale(LC_ALL, "");

    try {
        boost::asio::io_context kontekst;

        tcp::acceptor priemnik(kontekst, tcp::endpoint(tcp::v4(), 12347));
        std::cout << "Сервер таймера запущен на порту 12347\n";

        tcp::socket soket(kontekst);
        priemnik.accept(soket);
        std::cout << "Клиент подключён\n";

        auto bufer = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(soket, *bufer, '\n',
            [&kontekst, &soket, bufer](const boost::system::error_code& oshibka, std::size_t) {
                if (!oshibka) {
                    std::istream potokVvoda(bufer.get());
                    std::string komand;
                    std::getline(potokVvoda, komand);

                    if (komand == "ping") {
                        std::cout << "Получена команда ping. Ответ через 3 секунды...\n";

                        auto timer = std::make_shared<boost::asio::steady_timer>(kontekst, std::chrono::seconds(3));

                        timer->async_wait([&soket, timer](const boost::system::error_code& oshibkaTimer) {
                            if (!oshibkaTimer) {
                                std::string otvet = "pong\n";
                                boost::asio::async_write(soket, boost::asio::buffer(otvet),
                                    [](const boost::system::error_code&, std::size_t) {
                                        std::cout << "Отправлен pong клиенту.\n";
                                    });
                            }
                            });
                    }
                    else {
                        std::cerr << "Неизвестная команда: " << komand << std::endl;
                    }
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
