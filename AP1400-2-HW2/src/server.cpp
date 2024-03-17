#include "server.h"

#include <crypto.h>

#include <random>

#include "client.h"
Server::Server() {}

std::vector<std::string> pending_trxs;

std::shared_ptr<Client> Server::add_client(const std::string& id) {
  auto now_id = id;
  for (const auto& client : this->clients) {
    if (client.first->get_id() == id) {
      // now_id += rand() % 9000 + 1000;
      auto rand_number = std::to_string(rand() % 9000 + 1000);
      now_id += rand_number;
      break;
    }
  }
  auto client_ptr = std::make_shared<Client>(now_id, *this);
  this->clients[client_ptr] = 5;
  return client_ptr;
}

std::shared_ptr<Client> Server::get_client(const std::string& id) const {
  for (const auto& client : this->clients) {
    if (client.first->get_id() == id) {
      return client.first;
    }
  }
  return nullptr;
}

double Server::get_wallet(const std::string& id) const {
  for (const auto& client : this->clients) {
    if (client.first->get_id() == id) {
      return client.second;
    }
  }
  return 0;
}

bool Server::parse_trx(const std::string& trx, std::string& sender,
                       std::string& receiver, double& value) {
  auto first_pos = trx.find_first_of('-');
  if (first_pos == trx.npos) {
    throw std::runtime_error{""};
  }
  auto second_pos = trx.find_first_of('-', first_pos + 1);
  if (second_pos == trx.npos) {
    throw std::runtime_error{""};
  }
  sender = std::string(trx.begin(), trx.begin() + first_pos);
  receiver = std::string(trx.begin() + first_pos + 1, trx.begin() + second_pos);
  value = atof(std::string(trx.begin() + second_pos + 1, trx.end()).c_str());
  return true;
}

bool Server::add_pending_trx(const std::string& trx,
                             const std::string& signature) const {
  std::string sender, receiver;
  double value;
  if (!this->parse_trx(trx, sender, receiver, value)) {
    return false;
  }
  auto sender_ptr = this->get_client(sender);
  auto receiver_ptr = this->get_client(receiver);
  if (sender_ptr == nullptr || receiver_ptr == nullptr) {
    return false;
  }
  if (this->clients.at(sender_ptr) < value) {
    return false;
  }
  if (!crypto::verifySignature(sender_ptr->get_publickey(), trx, signature)) {
    return false;
  }
  pending_trxs.push_back(trx);
  return true;
}

size_t Server::mine() {
  std::string mem_pool;
  for (const auto& str : pending_trxs) {
    mem_pool += str;
  }
  while (true) {
    for (auto& client : this->clients) {
      auto nonce = client.first->generate_nonce();
      auto str = crypto::sha256(mem_pool + std::to_string(nonce));
      std::cout << str << "\n";
      if (str.substr(0, 10).find("000") != str.npos) {
        std::cout << "mine success !!!!!!!!!\n";
        client.second += 6.25;
        for (auto& trx : pending_trxs) {
          std::string sender, receiver;
          double value;
          this->parse_trx(trx, sender, receiver, value);
          for (auto& c : this->clients) {
            if (c.first->get_id() == sender) {
              c.second -= value;
            }
            if (c.first->get_id() == receiver) {
              c.second += value;
            }
          }
        }
        pending_trxs.clear();
        return nonce;
      }
    }
  }
}

void show_wallets(const Server& server) {
  for (auto& client : server.clients) {
    std::cout << "client : " << client.first->get_id()
              << " wallet : " << client.second << "\n";
  }
  std::cout << "\n";
}