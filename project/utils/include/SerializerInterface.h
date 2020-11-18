#pragma onсe

class SerializerInterface {
 private:
  virtual void serialize() = 0;
  virtual void deserialize() = 0;
};
