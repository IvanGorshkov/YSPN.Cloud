#include "Serializer.h"

template<typename T>
Serializer<T>::Serializer(const T &structT) {

}
template<typename T>
T &Serializer<T>::GetStructT() {
}

template<typename T>
void Serializer<T>::PutStructT(const T &structT) {
}

template<typename T>
void Serializer<T>::PutJsonT() {
}

template<typename T>
void Serializer<T>::serialize() {
}

template<typename T>
void Serializer<T>::deserialize() {
}

template<typename T>
Serializer<T>::Serializer(const boost::property_tree::ptree &jsonT) {
}

template<typename T>
boost::property_tree::ptree &Serializer<T>::GetJsonT(const boost::property_tree::ptree &jsonT) {
}
