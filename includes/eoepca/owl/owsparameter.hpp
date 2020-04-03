
#ifndef EOEPCAOWS_OWSPARAMETER_HPP
#define EOEPCAOWS_OWSPARAMETER_HPP

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

namespace EOEPCA::OWS {

class Descriptor {
 protected:
  std::string identifier{""};
  std::string title{""};
  std::string abstract{""};
  std::string version{""};

 public:
  Descriptor() = default;
  Descriptor(const Descriptor &other) noexcept(false)
      : identifier(other.identifier),
        title(other.title),
        version(other.version),
        abstract(other.abstract) {}

  Descriptor(Descriptor &&other) noexcept(false)
      : identifier(std::move(other.identifier)),
        title(std::move(other.title)),
        abstract(std::move(other.abstract)),
        version(std::move(other.version)) {}

  Descriptor &operator=(const Descriptor &other) {
    if (this == &other) return *this;

    identifier = other.identifier;
    title = other.title;
    abstract = other.abstract;
    version = other.version;

    return *this;
  }

  Descriptor &operator=(Descriptor &&other) noexcept(false) {
    if (this == &other) return *this;

    identifier = std::move(other.identifier);
    title = std::move(other.title);
    abstract = std::move(other.abstract);
    version = std::move(other.version);

    return *this;
  }

  virtual ~Descriptor() = default;

 public:
  void setVersion(std::string pVersion) {
    Descriptor::version = std::move(pVersion);
  }

  const std::string &getIdentifier() const { return identifier; }
  void setIdentifier(std::string pIdentifier) {
    Descriptor::identifier = std::move(pIdentifier);
  }
  const std::string &getTitle() const { return title; }
  void setTitle(const std::string &title) { Descriptor::title = title; }
  const std::string &getAbstract() const { return abstract; }
  void setAbstract(const std::string &abstract) {
    Descriptor::abstract = abstract;
  }
};

class Occurs {
 protected:
  int minOccurs{1};
  int maxOccurs{0};

 public:
  Occurs() = default;

  Occurs(const Occurs &) = default;
  Occurs(Occurs &&) = default;

  Occurs &operator=(const Occurs &other) {
    if (this == &other) return *this;
    minOccurs = other.minOccurs;
    maxOccurs = other.maxOccurs;
    return *this;
  }

  Occurs &operator=(Occurs &&other) noexcept(false) {
    if (this == &other) return *this;
    minOccurs = other.minOccurs;
    maxOccurs = other.maxOccurs;
    return *this;
  }

  virtual ~Occurs() = default;

 public:
  int getMinOccurs() const { return minOccurs; }
  int getMaxOccurs() const { return maxOccurs; }

  void setMinOccurs(std::string pMinOccurs) {
    if (pMinOccurs.empty()) {
      minOccurs = 0;
    } else {
      std::istringstream iss(pMinOccurs);
      iss >> minOccurs;
      if (iss.fail()) {
        // TODO: raise the exception
      }
    }
  }

  void setMaxOccurs(std::string pMaxOccurs) {
    if (pMaxOccurs.empty()) {
      maxOccurs = 0;
    } else {
      std::istringstream iss(pMaxOccurs);
      iss >> maxOccurs;
      if (iss.fail()) {
        // TODO: raise the exception
      }
    }
  }

  void setMinOccurs(int minOccurs) { Occurs::minOccurs = minOccurs; }
  void setMaxOccurs(int maxOccurs) { Occurs::maxOccurs = maxOccurs; }
};

class Param : public Descriptor, public Occurs {
 public:
  Param() : Descriptor(), Occurs(){};
  Param(const Param &other) noexcept(false)
      : Descriptor(other), Occurs(other) {}
  Param(Param &&other) noexcept(false)
      : Descriptor(std::move(other)), Occurs(std::move(other)) {}

  Param &operator=(const Param &other) {
    if (this == &other) return *this;

    Descriptor::operator=(other);
    Occurs::operator=(other);

    return *this;
  }

  Param &operator=(Param &&other) noexcept(false) {
    if (this == &other) return *this;

    Descriptor::operator=(std::move(other));
    Occurs::operator=(std::move(other));

    return *this;
  }

  Param &operator<<(const Descriptor &descriptor) {
    setIdentifier(descriptor.getIdentifier());
    setTitle(descriptor.getTitle());
    setAbstract(descriptor.getAbstract());

    return *this;
  }

  Param &operator<<(const Occurs &occurs) {
    setMinOccurs(occurs.getMinOccurs());
    setMaxOccurs(occurs.getMaxOccurs());
    return *this;
  }

  ~Param() override = default;

  virtual std::string getType() = 0;
};

class Supported {
  std::list<std::string> supported{};
  std::string defaultValue{""};

 public:
  Supported() = default;

  Supported(const Supported &other) : defaultValue(other.defaultValue) {
    for (auto &s : other.supported) {
      supported.emplace_back(s);
    }
  }

  Supported(Supported &&other) noexcept(false)
      : defaultValue(std::move(other.defaultValue)) {
    for (auto &s : other.supported) {
      supported.emplace_back(s);
    }
    other.supported.clear();
  }

  virtual ~Supported() = default;

 public:
  void addSupportedValues(std::string value) {
    supported.emplace_back(std::move(value));
  }

  void setDefault(std::string value) { defaultValue = std::move(value); }
};

class Values {
  std::list<std::string> allowedValues{};
  std::string defaultValue{""};

 public:
  Values() = default;

  Values(const Values &other) : defaultValue(other.defaultValue) {
    for (auto &s : other.allowedValues) {
      allowedValues.emplace_back(s);
    }
  }

  Values(Values &&other) noexcept(false)
      : defaultValue(std::move(other.defaultValue)) {
    for (auto &s : other.allowedValues) {
      allowedValues.emplace_back(s);
    }
    other.allowedValues.clear();
  }

  virtual ~Values() = default;

 public:
  void addAllowedValues(std::string value) {
    allowedValues.emplace_back(std::move(value));
  }

  void setDefault(std::string value) { defaultValue = std::move(value); }
};

class LiteralData final : public Param, public Values {
  std::string dataType{"string"};

 public:
  //  LiteralData():Param(),Values(){}

  LiteralData() = default;

  LiteralData(const LiteralData &) = delete;
  LiteralData(LiteralData &&) = delete;

  ~LiteralData() override = default;

 public:
  std::string getType() override { return std::string("LiteralData"); }
  void setDataType(std::string data) { dataType = std::move(data); }
};

class BoundingBoxData final : public Param, public Supported {
 public:
  BoundingBoxData() = default;

  BoundingBoxData(const BoundingBoxData &) = delete;
  BoundingBoxData(BoundingBoxData &&) = delete;

  ~BoundingBoxData() override = default;

 public:
  std::string getType() override { return std::string("BoundingBoxData"); }
};

class Format {
  std::string mimeType{""};
  std::string encoding{""};
  std::string schema{""};

 public:
  Format() = default;

  Format(const LiteralData &) = delete;
  Format(Format &&) = delete;

  ~Format() {}

  void setMimeType(std::string pMimeType) {
    Format::mimeType = std::move(pMimeType);
  }
  void setEncoding(std::string pEncoding) {
    Format::encoding = std::move(pEncoding);
  }
  void setSchema(std::string pSchema) { Format::schema = std::move(pSchema); }

  const std::string &getMimeType() const { return mimeType; }
  const std::string &getEncoding() const { return encoding; }
  const std::string &getSchema() const { return schema; }
};

class ComplexData final : public Param {
  std::list<std::unique_ptr<Format>> supported{};
  std::unique_ptr<Format> defaultSupported{nullptr};

  long maximumMegabytes = 0;

 public:
  ComplexData() = default;

  ComplexData(const LiteralData &) = delete;
  ComplexData(ComplexData &&) = delete;

  ~ComplexData() override = default;

 public:
  void setMaximumMegabytes(long pValue) { maximumMegabytes = pValue; }
  void setMaximumMegabytes(std::string pValue) {
    if (pValue.empty()) {
      maximumMegabytes = 0;
    } else {
      std::istringstream iss(pValue);
      iss >> maximumMegabytes;
      if (iss.fail()) {
        // TODO: raise the exception
      }
    }
  }

  std::string getType() override { return std::string("ComplexData"); }

  void moveDefaultSupported(std::unique_ptr<Format> &format) {
    if (format) {
      defaultSupported.reset(format.release());
    }
  }

  void moveAddSupported(std::unique_ptr<Format> &format) {
    if (format) {
      supported.emplace_back(format.release());
    }
  }

  const std::list<std::unique_ptr<Format>> &getSupported() const {
    return supported;
  }

  const std::unique_ptr<Format> &getDefaultSupported() const {
    return defaultSupported;
  }
};

struct Content {
  std::string href{""};
  std::string code{""};
  std::string tag{""};

  Content(std::string pCode, std::string pHref)
      : code(std::move(pCode)), href(std::move(pHref)) {}
};

class OWSParameter final : public Descriptor {
  std::string packageIdentifier{""};

  std::list<std::unique_ptr<Param>> inputs;
  std::list<std::unique_ptr<Param>> outputs;

  std::list<Content> contents{};

 public:
  OWSParameter() = default;
  OWSParameter(const OWSParameter &) = delete;
  OWSParameter(OWSParameter &&) = delete;

  ~OWSParameter() override = default;

 public:
  void setPackageIdentifier(std::string pPackageIdentifier) {
    OWSParameter::packageIdentifier = std::move(pPackageIdentifier);
  }

  const std::string &getPackageIdentifier() const { return packageIdentifier; }

  const std::list<Content> &getContents() const { return contents; }

  void addInput(Param *param) {
    if (param) {
      inputs.emplace_back(param);
    }
  }

  void addOutput(Param *param) {
    if (param) {
      outputs.emplace_back(param);
    }
  }

  void addContent(std::string code, std::string href) {
    if (!code.empty()) {
      contents.emplace_back(code, href);
    }
  }

  const std::list<std::unique_ptr<Param>> &getInputs() const { return inputs; }
  const std::list<std::unique_ptr<Param>> &getOutputs() const {
    return outputs;
  }
};

class Ows {};

}  // namespace EOEPCA::OWS
#endif  // EOEPCAOWS_OWSPARAMETER_HPP
