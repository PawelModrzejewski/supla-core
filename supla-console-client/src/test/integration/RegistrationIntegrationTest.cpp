/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "RegistrationIntegrationTest.h"

namespace testing {

RegistrationIntegrationTest::RegistrationIntegrationTest() {
  expectedRegistrationErrorCode = 0;
  sslEnabled = true;
  fillArrayWithOrdinalNumbers(GUID, SUPLA_GUID_SIZE, 0);
  fillArrayWithOrdinalNumbers(AuthKey, SUPLA_AUTHKEY_SIZE, 0);

  snprintf(Email, SUPLA_EMAIL_MAXSIZE, "test@supla.org");
  AccessID = 0;
  memset(AccessIDpwd, 0, SUPLA_ACCESSID_PWD_MAXSIZE);
}

RegistrationIntegrationTest::~RegistrationIntegrationTest() {}

void RegistrationIntegrationTest::beforeClientInit(TSuplaClientCfg *scc) {
  scc->ssl_enabled = sslEnabled;
  memcpy(scc->clientGUID, GUID, SUPLA_GUID_SIZE);
  memcpy(scc->AuthKey, AuthKey, SUPLA_AUTHKEY_SIZE);
  memcpy(scc->Email, Email, SUPLA_EMAIL_MAXSIZE);
  scc->AccessID = AccessID;
  memcpy(scc->AccessIDpwd, AccessIDpwd, SUPLA_ACCESSID_PWD_MAXSIZE);
}

void RegistrationIntegrationTest::onRegistered(
    TSC_SuplaRegisterClientResult_B *result) {
  IntegrationTest::onRegistered(result);
  ASSERT_EQ(expectedRegistrationErrorCode, 0);
  cancelIteration();
}

void RegistrationIntegrationTest::onRegistrationError(int code) {
  ASSERT_EQ(code, expectedRegistrationErrorCode);
  cancelIteration();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithBadGUID) {
  memset(GUID, 0, SUPLA_GUID_SIZE);
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_GUID_ERROR;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithBadGUID_NoSSL) {
  memset(GUID, 0, SUPLA_GUID_SIZE);
  sslEnabled = false;
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_GUID_ERROR;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithBadAuthKey) {
  memset(AuthKey, 0, SUPLA_AUTHKEY_SIZE);
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_AUTHKEY_ERROR;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithModifiedAuthKey) {
  AuthKey[0] = 50;
  initTestDatabase();
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_BAD_CREDENTIALS;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithBadCredentials_Email) {
  snprintf(Email, SUPLA_EMAIL_MAXSIZE, "noneistent@supla.org");
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_BAD_CREDENTIALS;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWithBadCredentials_AccessID) {
  Email[0] = 0;
  AccessID = 1;
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_BAD_CREDENTIALS;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest,
       RegistrationWhenRegistrationIsDisabledAndThereIsNoAnyClientsRegistered) {
  initTestDatabase();
  runSqlScript("RemoveTestClient.sql");
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_REGISTRATION_DISABLED;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest,
       RegistrationWhenRegistrationIsEnabledAndThereIsNoAnyClientsRegistered) {
  initTestDatabase();
  runSqlScript("RemoveTestClient.sql");
  runSqlScript("EnableClientRegistrationForTestUser.sql");
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_REGISTRATION_DISABLED;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest,
       RegistrationAsSecondClientWhenRegistrationIsEnabled) {
  GUID[0] = 10;
  initTestDatabase();
  runSqlScript("EnableClientRegistrationForTestUser.sql");
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_ACCESSID_NOT_ASSIGNED;
  iterateUntilDefaultTimeout();
}

TEST_F(RegistrationIntegrationTest, RegistrationWhenClientIsDisabled) {
  initTestDatabase();
  runSqlScript("DisableTestClient.sql");
  expectedRegistrationErrorCode = SUPLA_RESULTCODE_CLIENT_DISABLED;
  iterateUntilDefaultTimeout();
}

} /* namespace testing */
