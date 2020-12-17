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

#include "Person.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

Person::Person() {
  m_Id = 0L;
  m_IdIsSet = false;
  m_FirstName = "";
  m_FirstNameIsSet = false;
  m_LastName = "";
  m_LastNameIsSet = false;
  m_Birthdate = 0;
  m_BirthdateIsSet = false;
  m_FaceSignature = 0L;
  m_FaceSignatureIsSet = false;
  m_Role = "";
  m_RoleIsSet = false;
  m_StudentId = 0L;
  m_StudentIdIsSet = false;
  m_ParentId = 0L;
  m_ParentIdIsSet = false;
  m_StaffId = 0L;
  m_StaffIdIsSet = false;
  m_RelationshipToStudent = "";
  m_RelationshipToStudentIsSet = false;
  m_HiredDate = 0;
  m_HiredDateIsSet = false;
}

Person::~Person() {}

void Person::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json &j, const Person &o) {
  j = nlohmann::json();
  if (o.idIsSet())
    j["id"] = o.m_Id;
  if (o.firstNameIsSet())
    j["firstName"] = o.m_FirstName;
  if (o.lastNameIsSet())
    j["lastName"] = o.m_LastName;
  if (o.birthdateIsSet())
    j["birthdate"] = o.m_Birthdate;
  if (o.faceSignatureIsSet())
    j["faceSignature"] = o.m_FaceSignature;
  if (o.roleIsSet())
    j["role"] = o.m_Role;
  if (o.studentIdIsSet())
    j["studentId"] = o.m_StudentId;
  if (o.parentIdIsSet())
    j["parentId"] = o.m_ParentId;
  if (o.staffIdIsSet())
    j["staffId"] = o.m_StaffId;
  if (o.relationshipToStudentIsSet())
    j["relationshipToStudent"] = o.m_RelationshipToStudent;
  if (o.hiredDateIsSet())
    j["hiredDate"] = o.m_HiredDate;
}

void from_json(const nlohmann::json &j, Person &o) {
  if (j.find("id") != j.end()) {
    j.at("id").get_to(o.m_Id);
    o.m_IdIsSet = true;
  }
  if (j.find("firstName") != j.end()) {
    j.at("firstName").get_to(o.m_FirstName);
    o.m_FirstNameIsSet = true;
  }
  if (j.find("lastName") != j.end()) {
    j.at("lastName").get_to(o.m_LastName);
    o.m_LastNameIsSet = true;
  }
  if (j.find("birthdate") != j.end()) {
    j.at("birthdate").get_to(o.m_Birthdate);
    o.m_BirthdateIsSet = true;
  }
  if (j.find("faceSignature") != j.end()) {
    j.at("faceSignature").get_to(o.m_FaceSignature);
    o.m_FaceSignatureIsSet = true;
  }
  if (j.find("role") != j.end()) {
    j.at("role").get_to(o.m_Role);
    o.m_RoleIsSet = true;
  }
  if (j.find("studentId") != j.end()) {
    j.at("studentId").get_to(o.m_StudentId);
    o.m_StudentIdIsSet = true;
  }
  if (j.find("parentId") != j.end()) {
    j.at("parentId").get_to(o.m_ParentId);
    o.m_ParentIdIsSet = true;
  }
  if (j.find("staffId") != j.end()) {
    j.at("staffId").get_to(o.m_StaffId);
    o.m_StaffIdIsSet = true;
  }
  if (j.find("relationshipToStudent") != j.end()) {
    j.at("relationshipToStudent").get_to(o.m_RelationshipToStudent);
    o.m_RelationshipToStudentIsSet = true;
  }
  if (j.find("hiredDate") != j.end()) {
    j.at("hiredDate").get_to(o.m_HiredDate);
    o.m_HiredDateIsSet = true;
  }
}

int64_t Person::getId() const { return m_Id; }
void Person::setId(int64_t const value) {
  m_Id = value;
  m_IdIsSet = true;
}
bool Person::idIsSet() const { return m_IdIsSet; }
void Person::unsetId() { m_IdIsSet = false; }
std::string Person::getFirstName() const { return m_FirstName; }
void Person::setFirstName(std::string const &value) {
  m_FirstName = value;
  m_FirstNameIsSet = true;
}
bool Person::firstNameIsSet() const { return m_FirstNameIsSet; }
void Person::unsetFirstName() { m_FirstNameIsSet = false; }
std::string Person::getLastName() const { return m_LastName; }
void Person::setLastName(std::string const &value) {
  m_LastName = value;
  m_LastNameIsSet = true;
}
bool Person::lastNameIsSet() const { return m_LastNameIsSet; }
void Person::unsetLastName() { m_LastNameIsSet = false; }
int32_t Person::getBirthdate() const { return m_Birthdate; }
void Person::setBirthdate(int32_t const value) {
  m_Birthdate = value;
  m_BirthdateIsSet = true;
}
bool Person::birthdateIsSet() const { return m_BirthdateIsSet; }
void Person::unsetBirthdate() { m_BirthdateIsSet = false; }
int64_t Person::getFaceSignature() const { return m_FaceSignature; }
void Person::setFaceSignature(int64_t const value) {
  m_FaceSignature = value;
  m_FaceSignatureIsSet = true;
}
bool Person::faceSignatureIsSet() const { return m_FaceSignatureIsSet; }
void Person::unsetFaceSignature() { m_FaceSignatureIsSet = false; }
std::string Person::getRole() const { return m_Role; }
void Person::setRole(std::string const &value) {
  m_Role = value;
  m_RoleIsSet = true;
}
bool Person::roleIsSet() const { return m_RoleIsSet; }
void Person::unsetRole() { m_RoleIsSet = false; }
int64_t Person::getStudentId() const { return m_StudentId; }
void Person::setStudentId(int64_t const value) {
  m_StudentId = value;
  m_StudentIdIsSet = true;
}
bool Person::studentIdIsSet() const { return m_StudentIdIsSet; }
void Person::unsetStudentId() { m_StudentIdIsSet = false; }
int64_t Person::getParentId() const { return m_ParentId; }
void Person::setParentId(int64_t const value) {
  m_ParentId = value;
  m_ParentIdIsSet = true;
}
bool Person::parentIdIsSet() const { return m_ParentIdIsSet; }
void Person::unsetParentId() { m_ParentIdIsSet = false; }
int64_t Person::getStaffId() const { return m_StaffId; }
void Person::setStaffId(int64_t const value) {
  m_StaffId = value;
  m_StaffIdIsSet = true;
}
bool Person::staffIdIsSet() const { return m_StaffIdIsSet; }
void Person::unsetStaffId() { m_StaffIdIsSet = false; }
std::string Person::getRelationshipToStudent() const {
  return m_RelationshipToStudent;
}
void Person::setRelationshipToStudent(std::string const &value) {
  m_RelationshipToStudent = value;
  m_RelationshipToStudentIsSet = true;
}
bool Person::relationshipToStudentIsSet() const {
  return m_RelationshipToStudentIsSet;
}
void Person::unsetRelationshipToStudent() {
  m_RelationshipToStudentIsSet = false;
}
int32_t Person::getHiredDate() const { return m_HiredDate; }
void Person::setHiredDate(int32_t const value) {
  m_HiredDate = value;
  m_HiredDateIsSet = true;
}
bool Person::hiredDateIsSet() const { return m_HiredDateIsSet; }
void Person::unsetHiredDate() { m_HiredDateIsSet = false; }

} // namespace model
} // namespace server
} // namespace openapitools
} // namespace org