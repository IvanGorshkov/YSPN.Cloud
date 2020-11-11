#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

template<typename T>
class Serializer {
 public:
  explicit Serializer(boost::property_tree::ptree const &jsonT);
  explicit Serializer(const T &structT);
  T &GetStructT();
  void PutStructT(const T &structT);
  boost::property_tree::ptree &GetJsonT(boost::property_tree::ptree const &jsonT);
  void PutJsonT();

 private:
  void serialize();
  void deserialize();

 private:
  std::vector<T> structT{};
  boost::property_tree::ptree jsonT;
};
