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
/*
 * Room.h
 *
 * A room in a school building
 */

#ifndef Room_H_
#define Room_H_

#include <nlohmann/json.hpp>
#include <string>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// A room in a school building
/// </summary>
class Room {
public:
  Room();
  virtual ~Room();

  void validate();

  /////////////////////////////////////////////
  /// Room members

  /// <summary>
  ///
  /// </summary>
  int64_t getId() const;
  void setId(int64_t const value);
  bool idIsSet() const;
  void unsetId();
  /// <summary>
  ///
  /// </summary>
  std::string getName() const;
  void setName(std::string const &value);
  bool nameIsSet() const;
  void unsetName();
  /// <summary>
  ///
  /// </summary>
  std::string getNumber() const;
  void setNumber(std::string const &value);
  bool numberIsSet() const;
  void unsetnumber();
  /// <summary>
  ///
  /// </summary>
  std::string getFloor() const;
  void setFloor(std::string const &value);
  bool floorIsSet() const;
  void unsetFloor();
  /// <summary>
  ///
  /// </summary>
  int32_t getCapacity() const;
  void setCapacity(int32_t const value);
  bool capacityIsSet() const;
  void unsetCapacity();
  /// <summary>
  ///
  /// </summary>
  int64_t getBuildingId() const;
  void setBuildingId(int64_t const value);
  bool buildingIdIsSet() const;
  void unsetBuildingId();

  friend void to_json(nlohmann::json &j, const Room &o);
  friend void from_json(const nlohmann::json &j, Room &o);

protected:
  int64_t m_Id;
  bool m_IdIsSet;
  std::string m_Name;
  bool m_NameIsSet;
  std::string m_number;
  bool m_numberIsSet;
  std::string m_Floor;
  bool m_FloorIsSet;
  int32_t m_Capacity;
  bool m_CapacityIsSet;
  int64_t m_BuildingId;
  bool m_BuildingIdIsSet;
};

} // namespace model
} // namespace server
} // namespace openapitools
} // namespace org

#endif /* Room_H_ */