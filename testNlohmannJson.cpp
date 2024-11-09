#include <iostream>
#include <string>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

class address {
public:
  std::string street;
  int housenumber;
  int postcode;

public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(address, street, housenumber, postcode)
  address(const std::string &street, int housenumber, int postcode)
      : street(street), housenumber(housenumber), postcode(postcode) {}
  address(){};
};

int testNlohmannJson() {
  std::string json = R"(
        {
        "pi": 3.141,
        "happy": true,
        "name": "Niels",
        "nothing": null,
        "answer": {
            "everything": 42
        },
        "list": [1, 0, 2],
        "object": {
            "currency": "USD",
            "value": 42.99
        }
        }
    )";

  try {
    auto j = nlohmann::json::parse(json);
    fmt::print("pi: {}\n", j["pi"].get<double>());
    fmt::print("pi: {}\n", j["pi"].get<int>());
    fmt::print("happy: {}\n", j["happy"].get<bool>());
    fmt::print("name: {}\n", j["name"].get<std::string>());
    fmt::print("nothing: {}\n", j["nothing"].is_null());
    fmt::print("answer: {}\n", j["answer"]["everything"].get<int>());
    fmt::print("list size: {}\n", j["list"].size());
    fmt::print("object currency: {}\n",
               j["object"]["currency"].get<std::string>());

    if (j.contains("answer") && j["answer"].contains("everything")) {
      fmt::println("answer everything: {}",
                   j["answer"]["everything"].get<int>());
    } else {
      fmt::println("answer everything does not exist.");
    }

    if (j.contains("answer") && j["answer"].contains("nothing")) {
      fmt::println("answer nothing: {}", j["answer"]["nothing"].get<int>());
    } else {
      fmt::println("answer nothing does not exist.");
    }

    auto nothing = j["/answer/nothing"_json_pointer];
    if (nothing.is_null()) {
      fmt::println("answer nothing is null.");
    } else {
      fmt::println("answer nothing is not null.");
    }

    auto everything = j["/answer/everything"_json_pointer];
    if (everything.is_null()) {
      fmt::println("answer everything is null.");
    } else {
      fmt::println("answer everything is not null.");
      fmt::println("answer nothing: {}", j["answer"]["everything"].get<int>());
    }

    // serialize
    address a{"Elmstreet", 13, 12345};
    auto j2 = nlohmann::json::object();
    to_json(j2, a);
    fmt::print("{}\n", j2.dump(4));

    auto j3 = R"(
            {
                "street": "Elmstreet",
                "housenumber": 13,
                "postcode": 12345
            }
        )"_json;

    address b;
    from_json(j3, b);

    fmt::print("street: {}\n", b.street);
    fmt::print("housenumber: {}\n", b.housenumber);
    fmt::print("postcode: {}\n", b.postcode);

  } catch (const std::exception &e) {
    fmt::print("parse json error: {}\n", e.what());
    return 1;
  }

  return 0;
}

void parse_json() {
  std::string json = R"(
        {
        "asymkey_index":14,
        "machine_name":"HSM-G25-5",
        "machine_config_path":"/opt/config/EtCfgJsonNginx.ini",
        "password":"eetrust123"
        }
    )";

  try {
    auto j = nlohmann::json::parse(json);
    fmt::print("asymkey_index: {}\n", j["asymkey_index"].get<int>());
    fmt::print("machine_name: {}\n", j["machine_name"].get<std::string>());
    fmt::print("machine_config_path: {}\n",
               j["machine_config_path"].get<std::string>());
    fmt::print("password: {}\n", j["password"].get<std::string>());

    std::string machine_config_path = j["machine_config_path"];
    fmt::print("machine_config_path: {}\n", machine_config_path.c_str());

  } catch (const std::exception &e) {
    fmt::print("parse json error: {}\n", e.what());
  }
}

void parse_json2()
{
    std::string json = R"({
    "DeviceFactorys":[
        {
            "FactoryName":"SDYX",
            "DeviceLibPath":"/opt/EETRUST/cosign/so/",
            "DeviceTypes":[
                {
                    "DeviceType":"HSM-G25",
                    "SubDevices":[
                        {
                            "DeviceName":"HSM-G25-5",
                            "ConfigPath":"/opt/EETRUST/cosign/so/HSM_G25-1.ini"
                        }
                    ]
                }
            ]
        }
    ]
}
)";

  try
  {
    auto j = nlohmann::json::parse(json);
    fmt::print("DeviceFactorys size: {}\n", j["DeviceFactorys"].size());
    fmt::print("DeviceFactorys is array ? {}", j["DeviceFactorys"].is_array());

  }
  catch (const std::exception& e)
  {
    fmt::print("parse json error: {}\n", e.what());
  }
}

int main() {
  int ret = -530575361;
  return parse_json2();
  // return testNlohmannJson();
}