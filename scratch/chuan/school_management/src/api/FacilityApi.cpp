/**
 * Gaia School
 * School management system REST API
 *
 * The version of the OpenAPI document: 1.0.0
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "FacilityApi.h"
#include "Helpers.h"

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::helpers;
using namespace org::openapitools::server::model;

FacilityApi::FacilityApi(std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void FacilityApi::init() { setupRoutes(); }

void FacilityApi::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Post(*router, base + "/building",
               Routes::bind(&FacilityApi::add_building_handler, this));
  Routes::Post(*router, base + "/facescan",
               Routes::bind(&FacilityApi::add_facescan_handler, this));
  Routes::Post(*router, base + "/room",
               Routes::bind(&FacilityApi::add_room_handler, this));
  Routes::Delete(*router, base + "/building/:buildingId",
                 Routes::bind(&FacilityApi::delete_building_handler, this));
  Routes::Delete(*router, base + "/room/:roomId",
                 Routes::bind(&FacilityApi::delete_room_handler, this));
  Routes::Get(*router, base + "/building/:buildingId",
              Routes::bind(&FacilityApi::get_building_by_id_handler, this));
  Routes::Get(*router, base + "/room/:roomId",
              Routes::bind(&FacilityApi::get_room_by_id_handler, this));
  Routes::Get(*router, base + "/buildings",
              Routes::bind(&FacilityApi::list_buildings_handler, this));
  Routes::Get(
      *router, base + "/building/:buildingId/facescan",
      Routes::bind(&FacilityApi::list_facescan_at_building_handler, this));
  Routes::Get(*router, base + "/rooms",
              Routes::bind(&FacilityApi::list_rooms_handler, this));
  Routes::Put(*router, base + "/building",
              Routes::bind(&FacilityApi::update_building_handler, this));
  Routes::Put(*router, base + "/room",
              Routes::bind(&FacilityApi::update_room_handler, this));

  // Default handler, called when a route is not found
  router->addCustomHandler(
      Routes::bind(&FacilityApi::facility_api_default_handler, this));
}

void FacilityApi::add_building_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {

  // Getting the body param

  Building building;

  try {
    nlohmann::json::parse(request.body()).get_to(building);
    this->add_building(building, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::add_facescan_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {

  // Getting the body param

  Facescan facescan;

  try {
    nlohmann::json::parse(request.body()).get_to(facescan);
    this->add_facescan(facescan, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::add_room_handler(const Pistache::Rest::Request &request,
                                   Pistache::Http::ResponseWriter response) {

  // Getting the body param

  Room room;

  try {
    nlohmann::json::parse(request.body()).get_to(room);
    this->add_room(room, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::delete_building_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto buildingId = request.param(":buildingId").as<int64_t>();

  try {
    this->delete_building(buildingId, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::delete_room_handler(const Pistache::Rest::Request &request,
                                      Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto roomId = request.param(":roomId").as<int64_t>();

  try {
    this->delete_room(roomId, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::get_building_by_id_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto buildingId = request.param(":buildingId").as<int64_t>();

  try {
    this->get_building_by_id(buildingId, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::get_room_by_id_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto roomId = request.param(":roomId").as<int64_t>();

  try {
    this->get_room_by_id(roomId, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::list_buildings_handler(
    const Pistache::Rest::Request &, Pistache::Http::ResponseWriter response) {

  try {
    this->list_buildings(response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::list_facescan_at_building_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {
  // Getting the path params
  auto buildingId = request.param(":buildingId").as<int64_t>();

  try {
    this->list_facescan_at_building(buildingId, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::list_rooms_handler(const Pistache::Rest::Request &,
                                     Pistache::Http::ResponseWriter response) {

  try {
    this->list_rooms(response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::update_building_handler(
    const Pistache::Rest::Request &request,
    Pistache::Http::ResponseWriter response) {

  // Getting the body param

  Building building;

  try {
    nlohmann::json::parse(request.body()).get_to(building);
    this->update_building(building, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}
void FacilityApi::update_room_handler(const Pistache::Rest::Request &request,
                                      Pistache::Http::ResponseWriter response) {

  // Getting the body param

  Room room;

  try {
    nlohmann::json::parse(request.body()).get_to(room);
    this->update_room(room, response);
  } catch (nlohmann::detail::exception &e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError &e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception &e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}

void FacilityApi::facility_api_default_handler(
    const Pistache::Rest::Request &, Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::Not_Found,
                "The requested method does not exist");
}

} // namespace api
} // namespace server
} // namespace openapitools
} // namespace org