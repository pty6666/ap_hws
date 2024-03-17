#include "client.h"
#include <server.h>
#include <crypto.h>
#include <random>

Client::Client(const std::string& id, const Server& server) : id{id}, server{&server} {
  crypto::generate_key(this->public_key, this->private_key);
}

std::string Client::get_id() const {
  return this->id;
}

std::string Client::get_publickey() const {
  return this->public_key;
}

double Client::get_wallet() const {
  return this->server->get_wallet(this->id);
}

std::string Client::sign(const std::string& txt) const {
  return crypto::signMessage(this->private_key, txt);
}

bool Client::transfer_money(const std::string& receiver, double value) const {
  auto trx = this->id + '-' + receiver + '-' + std::to_string(value);
  auto signature = this->sign(trx);
  return this->server->add_pending_trx(trx, signature);
}

size_t Client::generate_nonce() const {
  return rand();
}